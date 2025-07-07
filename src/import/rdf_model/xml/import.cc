#include "import.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/iri_compression.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "graph_models/rdf_model/rdf_object_id.h"
#include "import/import_helper.h"
#include "misc/fatal_error.h"
#include "misc/unicode_escape.h"
#include "query/parser/grammar/sparql/mdb_extensions.h"

using namespace Import::Rdf::XML;

// https://cplusplus.com/forum/general/263696/
// This is not necessary. It is meant to print these enums in an easier way
std::ostream& operator<<(std::ostream& strm, Token t)
{
    const std::string name_token[] = { "END_OF_FILE",      "XML_METADATA", "OPENING_TAG", "CLOSING_TAG",
                                       "SELF_CLOSING_TAG", "TAG_CONTENT",  "COMMENT",     "WHITESPACE",
                                       "ENDLINE",          "UNRECOGNIZED", "TOTAL_TOKENS" };
    return strm << name_token[t];
}

std::ostream& operator<<(std::ostream& strm, State s)
{
    const std::string name_token[] = { "ERROR", "START", "TOTAL_STATES" };
    return strm << name_token[s];
}

RDFTerm& operator++(RDFTerm& a)
{
    if (a == RDFTerm::OBJECT) {
        a = RDFTerm::SUBJECT;
    } else {
        a = static_cast<RDFTerm>(static_cast<int>(a) + 1);
    }
    return a;
}

RDFTerm& operator--(RDFTerm& a)
{
    if (a == RDFTerm::SUBJECT) {
        a = RDFTerm::OBJECT;
    } else {
        a = static_cast<RDFTerm>(static_cast<int>(a) - 1);
    }
    return a;
}

OnDiskImport::OnDiskImport(
    const std::string& db_folder,
    uint64_t strings_buffer_size,
    uint64_t tensors_buffer_size,
    size_t index_permutations
) :
    index_permutations(index_permutations),
    db_folder(db_folder),
    strings_buffer_size(strings_buffer_size),
    tensors_buffer_size(tensors_buffer_size),
    catalog(RdfCatalog("catalog.dat", index_permutations)),
    triples(db_folder + "/tmp_triples"),
    equal_spo(db_folder + "/tmp_equal_spo"),
    equal_sp(db_folder + "/tmp_equal_sp"),
    equal_so(db_folder + "/tmp_equal_so"),
    equal_po(db_folder + "/tmp_equal_po")
{
    state_transitions = new int[Token::TOTAL_TOKENS * State::TOTAL_STATES];
    buffer_iri = new char[StringManager::MAX_STRING_SIZE];
    create_automata();
}

OnDiskImport::~OnDiskImport()
{
    delete[] state_transitions;
    delete[] buffer_iri;
}

void OnDiskImport::print_error()
{
    parsing_errors++;
    std::cout << "ERROR on line " << current_line << std::endl;
}

void OnDiskImport::print_error_msg(const std::string& msg)
{
    parsing_errors++;
    std::cout << "ERROR on line " << current_line << ": " << msg << std::endl;
}

void OnDiskImport::print_fatal_error_msg(const std::string& msg)
{
    std::string error = "FATAL ERROR on line ";
    error += std::to_string(current_line);
    error += ": " + msg;
    FATAL_ERROR(error);
}

std::vector<std::string> OnDiskImport::split(const std::string& input, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
        end = input.find(delimiter, start);
    }

    // Add the last substring
    tokens.push_back(input.substr(start));

    return tokens;
}

std::vector<std::string> OnDiskImport::split_xml_param(const std::string& input)
{
    std::string delimiter = "=";
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = input.find(delimiter);

    if (end != std::string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
        end = input.find(delimiter, start);
    }

    // Add the last substring
    tokens.push_back(input.substr(start));

    return tokens;
}

void OnDiskImport::start_import(MDBIstream& in, const std::string& prefixes_filename)
{
    auto start = std::chrono::system_clock::now();
    auto importer_start = start;

    pending_triples = std::make_unique<DiskVector<3>>(db_folder + "/" + PENDING_TRIPLES_FILENAME_PREFIX);

    // Initialize external helper
    external_helper = std::make_unique<ExternalHelper>(db_folder, strings_buffer_size, tensors_buffer_size);

    { // Process IRI prefixes
        // We look for prefixes in the following order:
        //   1. prefix file
        //   2. default prefixes
        // Until exhausting or reaching 256 prefixes.

        // std::set<std::string> prefix_set = { "" }; // empty prefix
        prefix_set = { "" }; // empty prefix

        if (!prefixes_filename.empty()) {
            std::ifstream prefixes_file(prefixes_filename);
            if (prefixes_file.fail()) {
                throw std::runtime_error("Could not open file " + prefixes_filename);
            }

            std::string line;

            while (prefix_set.size() < 256 && std::getline(prefixes_file, line)) {
                auto start_prefix = line.find_first_not_of(" \t");
                if (start_prefix == std::string::npos) {
                    // Whitespace only
                    continue;
                }

                auto end_prefix = line.find_last_not_of(" \t") + 1;

                auto invalid_prefix = false;
                for (size_t i = start_prefix; i < end_prefix; i++) {
                    unsigned char c = line[i];
                    if (c < IRI_PREFIX_CHAR_MIN || c > IRI_PREFIX_CHAR_MAX) {
                        invalid_prefix = true;
                        break;
                    }
                }

                if (invalid_prefix) {
                    WARN(
                        "Invalid prefix: ",
                        line,
                        "\nOnly chars in range [",
                        IRI_PREFIX_CHAR_MIN,
                        ", ",
                        IRI_PREFIX_CHAR_MAX,
                        "] are allowed"
                    );
                    continue;
                }

                auto prefix = line.substr(start_prefix, end_prefix - start_prefix);

                if (!prefix_set.insert(prefix).second) {
                    WARN("Duplicated prefix: ", prefix);
                }
            }
            prefixes_file.close();
        }

        for (auto& prefix : RdfModel::default_compression_prefixes) {
            if (prefix_set.size() < 256) {
                prefix_set.insert(prefix);
            } else {
                break;
            }
        }
    }

    lexer.begin(in);

    while (auto token = lexer.get_token()) {
        current_state = get_transition(current_state, token);
        // State state = static_cast<State>(current_state); // Only for debugging. Remove when not in use
        // std::cout << "Content: " << lexer.str << "\tToken: " << token << "\tState: " << state << std::endl; //"\n";
    }
    // std::cout << std::endl;

    print_duration("Parsing", start);

    // initial flush
    external_helper->flush_to_disk();

    int i = 0;
    pending_triples->finish_appends();

    while (pending_triples->get_total_tuples() > 0) {
        std::cout << "pending triples: " << pending_triples->get_total_tuples() << std::endl;
        auto old_pending_triples = std::move(pending_triples);
        pending_triples = std::make_unique<DiskVector<3>>(
            db_folder + "/" + PENDING_TRIPLES_FILENAME_PREFIX + std::to_string(i)
        );
        ++i;

        // advance pending variables for current iteration
        external_helper->advance_pending();
        external_helper->clear_sets();

        old_pending_triples->begin_tuple_iter();
        while (old_pending_triples->has_next_tuple()) {
            const auto& pending_triple = old_pending_triples->next_tuple();

            // resolve each id
            subject_id.id = external_helper->resolve_id(pending_triple[0]);
            predicate_id.id = external_helper->resolve_id(pending_triple[1]);
            object_id.id = external_helper->resolve_id(pending_triple[2]);

            save_triple();
        }

        // write out new data
        external_helper->flush_to_disk();
        // close and delete the old pending files
        external_helper->clean_up_old();

        // close and delete old_pending_triples file
        pending_triples->finish_appends();
        old_pending_triples->skip_indexing(); // will close and remove file
    }

    // delete pending triples and pending strings
    pending_triples->skip_indexing(); // will close and remove file

    // delete all unnecessary files and free-up memory
    external_helper->clean_up();

    print_duration("Processing strings and tensors", start);

    // Destroy blank_ids_map and replacing it with an empty map
    {
        boost::unordered_flat_map<std::string, uint64_t> tmp;
        blank_ids_map.swap(tmp);
    }

    external_helper->build_disk_hash();

    print_duration("Write strings and tensors hashes", start);

    // we reuse the buffer for external strings in the B+trees creation
    char* const buffer = external_helper->buffer;
    const auto buffer_size = external_helper->buffer_size;

    // Save lasts blocks to disk
    triples.finish_appends();
    equal_sp.finish_appends();
    equal_so.finish_appends();
    equal_po.finish_appends();
    equal_spo.finish_appends();

    triples.start_indexing(buffer, buffer_size, { 0, 1, 2 });
    equal_sp.start_indexing(buffer, buffer_size, { 0, 1 });
    equal_so.start_indexing(buffer, buffer_size, { 0, 1 });
    equal_po.start_indexing(buffer, buffer_size, { 0, 1 });
    equal_spo.start_indexing(buffer, buffer_size, { 0 });

    { // B+tree creation for triple
        size_t COL_SUBJ = 0, COL_PRED = 1, COL_OBJ = 2;

        Import::PredicateStat pred_stat;
        Import::NoStat<3> no_stat;

        triples.create_bpt(db_folder + "/spo", { COL_SUBJ, COL_PRED, COL_OBJ }, no_stat);

        triples.create_bpt(db_folder + "/pos", { COL_PRED, COL_OBJ, COL_SUBJ }, pred_stat);
        pred_stat.end();
        catalog.set_triples_count(pred_stat.all_count);
        catalog.set_predicate_stats(std::move(pred_stat.map_predicate_count));

        triples.create_bpt(db_folder + "/osp", { COL_OBJ, COL_SUBJ, COL_PRED }, no_stat);

        if (index_permutations >= 4) {
            triples.create_bpt(db_folder + "/pso", { COL_PRED, COL_SUBJ, COL_OBJ }, no_stat);
        }

        if (index_permutations == 6) {
            triples.create_bpt(db_folder + "/sop", { COL_SUBJ, COL_OBJ, COL_PRED }, no_stat);
            triples.create_bpt(db_folder + "/ops", { COL_OBJ, COL_PRED, COL_SUBJ }, no_stat);
        }
    }

    print_duration("Write triples indexes", start);

    { // B+tree creation SUBJECT=PREDICATE OBJECT
        size_t COL_SUBJ_PRED = 0, COL_OBJ = 1;

        Import::AllStat<2> all_stat;
        equal_sp.create_bpt(db_folder + "/equal_sp", { COL_SUBJ_PRED, COL_OBJ }, all_stat);
        catalog.set_equal_sp_count(all_stat.all);

        Import::NoStat<2> no_stat_inverted;
        equal_sp.create_bpt(db_folder + "/equal_sp_inverted", { COL_OBJ, COL_SUBJ_PRED }, no_stat_inverted);
    }

    { // B+tree creation SUBJECT=OBJECT PREDICATE
        size_t COL_SUBJ_OBJ = 0, COL_PRED = 1;

        Import::AllStat<2> all_stat;
        equal_so.create_bpt(db_folder + "/equal_so", { COL_SUBJ_OBJ, COL_PRED }, all_stat);
        catalog.set_equal_so_count(all_stat.all);
        Import::NoStat<2> no_stat_inverted;
        equal_so.create_bpt(db_folder + "/equal_so_inverted", { COL_PRED, COL_SUBJ_OBJ }, no_stat_inverted);
    }

    { // B+tree creation PREDICATE=OBJECT SUBJECT
        size_t COL_PRED_OBJ = 0, COL_SUBJ = 1;

        Import::AllStat<2> all_stat;
        equal_po.create_bpt(db_folder + "/equal_po", { COL_PRED_OBJ, COL_SUBJ }, all_stat);
        catalog.set_equal_po_count(all_stat.all);

        Import::NoStat<2> no_stat_inverted;
        equal_po.create_bpt(db_folder + "/equal_po_inverted", { COL_SUBJ, COL_PRED_OBJ }, no_stat_inverted);
    }

    { // B+tree creation SUBJECT=PREDICATE=OBJECT
        size_t COL_SUBJ_PRED_OBJ = 0;

        Import::AllStat<1> all_stat;
        equal_spo.create_bpt(db_folder + "/equal_spo", { COL_SUBJ_PRED_OBJ }, all_stat);
        catalog.set_equal_spo_count(all_stat.all);
    }

    // calling finish_indexing() closes and removes the file.
    triples.finish_indexing();
    equal_sp.finish_indexing();
    equal_so.finish_indexing();
    equal_po.finish_indexing();
    equal_spo.finish_indexing();

    print_duration("Write special cases B+tree index", start);

    // Store IRI aliases, prefixes and literal datatypes/languages into catalog
    catalog.prefixes = std::move(prefixes);
    catalog.datatypes.resize(datatype_ids_map.size());
    for (auto&& [datatype, id] : datatype_ids_map) {
        catalog.datatypes[id] = datatype;
    }
    catalog.languages.resize(language_ids_map.size());
    for (auto&& [language, id] : language_ids_map) {
        catalog.languages[id] = language;
    }

    catalog.print(std::cout);

    print_duration("Total Import", importer_start);
}

std::string OnDiskImport::get_url_from_tag_declaration(std::string& string)
{
    std::vector<std::string> separated = split(string, ":");

    std::string to_return;
    auto it = rdf_xml_prefixes.find(separated[0]);
    if (it != rdf_xml_prefixes.end()) {
        to_return += it->second;
        to_return += separated.back();
    } else if (using_global_namespace) {
        to_return += xml_global_namespace;
        to_return += separated[0];
    } else {
        print_fatal_error_msg("Prefix " + separated[0] + " was not linked");
    }

    return to_return;
}

bool OnDiskImport::has_ending(const std::string& full_string, const std::string& ending)
{
    if (full_string.length() >= ending.length()) {
        return (0 == full_string.compare(full_string.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

std::string OnDiskImport::remove_endline_and_whitespace(const std::string& in)
{
    std::string result;
    for (char ch : in) {
        if (ch != '\n' && ch != '\t') {
            result += ch;
        }
    }
    return result;
}

std::vector<std::string> OnDiskImport::parse_xml_params(const std::string& in)
{
    std::vector<std::string> parameters;
    std::stringstream ss(in);
    std::string token;
    bool inQuotes = false;
    char quoteChar = '\0';
    std::string currentToken;

    for (size_t i = 0; i < in.size(); ++i) {
        char ch = in[i];

        if (inQuotes) {
            currentToken += ch;
            if (ch == quoteChar) {
                inQuotes = false;
                parameters.push_back(currentToken);
                currentToken.clear();
            }
        } else {
            if (ch == ' ' || ch == '\0') {
                if (!currentToken.empty()) {
                    parameters.push_back(currentToken);
                    currentToken.clear();
                }
            } else if (ch == '"' || ch == '\'') {
                inQuotes = true;
                quoteChar = ch;
                currentToken += ch;
            } else {
                currentToken += ch;
            }
        }
    }

    if (!currentToken.empty()) {
        parameters.push_back(currentToken);
    }

    return parameters;
}

ObjectId OnDiskImport::get_iri_id(const char* str, size_t str_len)
{
    auto [prefix_id, prefix_size] = prefixes.get_prefix_id(str, str_len);
    uint64_t prefix_id_shifted = static_cast<uint64_t>(prefix_id) << 48;

    str += prefix_size;
    str_len -= prefix_size;

    if (UUIDCompression::compress_lower(str, str_len, buffer_iri)) {
        str_len = str_len - 20;
        return ObjectId(
            external_helper->get_or_create_external_string_id(buffer_iri, str_len)
            | (ObjectId::MASK_IRI_UUID_LOWER & (~ObjectId::MOD_MASK)) | prefix_id_shifted
        );
    } else if (UUIDCompression::compress_upper(str, str_len, buffer_iri)) {
        str_len = str_len - 20;
        return ObjectId(
            external_helper->get_or_create_external_string_id(buffer_iri, str_len)
            | (ObjectId::MASK_IRI_UUID_UPPER & (~ObjectId::MOD_MASK)) | prefix_id_shifted
        );
    }

    if (str_len >= HexCompression::MIN_LEN_TO_COMPRESS) {
        auto lower_hex_length = HexCompression::get_lower_hex_length(str, str_len);
        auto upper_hex_length = HexCompression::get_upper_hex_length(str, str_len);

        // Compress lowercase hex characters
        if (lower_hex_length >= upper_hex_length
            && lower_hex_length > HexCompression::MIN_HEX_LEN_TO_COMPRESS)
        {
            str_len = HexCompression::compress(str, str_len, lower_hex_length, buffer_iri);
            return ObjectId(
                external_helper->get_or_create_external_string_id(buffer_iri, str_len)
                | (ObjectId::MASK_IRI_HEX_LOWER & (~ObjectId::MOD_MASK)) | prefix_id_shifted
            );

            // Compress uppercase hex characters
        } else if (upper_hex_length > HexCompression::MIN_HEX_LEN_TO_COMPRESS) {
            str_len = HexCompression::compress(str, str_len, upper_hex_length, buffer_iri);
            return ObjectId(
                external_helper->get_or_create_external_string_id(buffer_iri, str_len)
                | (ObjectId::MASK_IRI_HEX_UPPER & (~ObjectId::MOD_MASK)) | prefix_id_shifted
            );
        }
    }

    if (str_len <= RDF_OID::MAX_INLINE_LEN_IRI) {
        return SPARQL::Conversions::pack_iri_inline(str, prefix_id);
    } else {
        return ObjectId(
            external_helper->get_or_create_external_string_id(str, str_len) | ObjectId::MASK_IRI
            | prefix_id_shifted
        );
    }
}

ObjectId OnDiskImport::get_blank_id(const char* str)
{
    auto it = blank_ids_map.find(str);
    if (it == blank_ids_map.end()) {
        auto new_blank_id = blank_node_count;
        blank_ids_map.insert({ str, new_blank_id });
        blank_node_count++;
        catalog.set_blank_node_count(blank_node_count);
        return SPARQL::Conversions::pack_blank_inline(new_blank_id);
    } else {
        return SPARQL::Conversions::pack_blank_inline(it->second);
    }
}

uint64_t OnDiskImport::get_datatype_id(const char* datatype)
{
    auto it = datatype_ids_map.find(datatype);
    if (it == datatype_ids_map.end()) {
        uint64_t new_id = datatype_ids_map.size();
        datatype_ids_map.insert({ datatype, new_id });
        return new_id;
    } else {
        return it->second;
    }
}

uint64_t OnDiskImport::get_lang_id(const char* lang)
{
    auto it = language_ids_map.find(lang);
    if (it == language_ids_map.end()) {
        // Lang not seen before
        uint64_t new_id = language_ids_map.size();
        language_ids_map.insert({ lang, new_id });
        return new_id;
    } else {
        // Lang found
        return it->second;
    }
}

ObjectId OnDiskImport::handle_integer_string(const std::string& str, bool* error)
{
    *error = false;
    try {
        size_t pos;
        int64_t i = std::stoll(str, &pos);
        // Check if the whole string was parsed
        if (pos != str.size()) {
            *error = true;
            return ObjectId::get_null();
        }
        // If the integer uses more than 56 bits, it must be converted into Decimal Extern (overflow)
        else if (i > SPARQL::Conversions::INTEGER_MAX || i < -SPARQL::Conversions::INTEGER_MAX)
        {
            Decimal dec(str, error);
            if (*error) {
                return ObjectId::get_null();
            }

            char dec_buffer[Decimal::EXTERN_BUFFER_SIZE];
            dec.serialize_extern(dec_buffer);
            return ObjectId(
                external_helper->get_or_create_external_string_id(dec_buffer, Decimal::EXTERN_BUFFER_SIZE)
                | ObjectId::MASK_DECIMAL
            );
        } else {
            return SPARQL::Conversions::pack_int(i);
        }
    } catch (const std::out_of_range& e) {
        Decimal dec(str, error);
        if (*error) {
            return ObjectId::get_null();
        }

        char dec_buffer[Decimal::EXTERN_BUFFER_SIZE];
        dec.serialize_extern(dec_buffer);
        return ObjectId(
            external_helper->get_or_create_external_string_id(dec_buffer, Decimal::EXTERN_BUFFER_SIZE)
            | ObjectId::MASK_DECIMAL
        );
    } catch (const std::invalid_argument& e) {
        *error = true;
        return ObjectId::get_null();
    }
}

ObjectId OnDiskImport::save_ill_typed(unsigned line, const char* value, const char* datatype)
{
    auto size = strlen(value);

    WARN("line ", line, " invalid: ", datatype, ": ", value);

    uint64_t datatype_id = get_datatype_id(datatype);
    if (size <= ObjectId::STR_DT_INLINE_BYTES) {
        return SPARQL::Conversions::pack_string_datatype_inline(datatype_id, value);
    } else {
        return ObjectId(
            external_helper->get_or_create_external_string_id(value, size) | ObjectId::MASK_STRING_DATATYPE
            | (datatype_id << SPARQL::Conversions::TMP_SHIFT)
        );
    }
}

void OnDiskImport::save_object_id_literal_lang(XMLTag& object)
{
    uint64_t lang_id = get_lang_id(object.lang.c_str());
    if (object.value.size() <= RDF_OID::MAX_INLINE_LEN_STRING_LANG) {
        object_id = SPARQL::Conversions::pack_string_lang_inline(lang_id, object.value.c_str());
    } else {
        object_id.id = external_helper->get_or_create_external_string_id(
                           object.value.c_str(),
                           object.value.size()
                       )
                     | ObjectId::MASK_STRING_LANG | (lang_id << SPARQL::Conversions::TMP_SHIFT);
    }
}

template<typename T>
void OnDiskImport::try_save_tensor(const char* str, const char* dt)
{
    bool error;
    const auto tensor = tensor::Tensor<T>::from_literal(str, &error);
    if (error) {
        object_id = save_ill_typed(current_line, str, dt);
        return;
    }

    if (tensor.empty()) {
        object_id.id = tensor.get_inline_mask();
        return;
    }

    const auto bytes = reinterpret_cast<const char*>(tensor.data());
    const auto num_bytes = sizeof(T) * tensor.size();

    object_id.id = tensor::Tensor<T>::get_subtype()
                 | external_helper->get_or_create_external_tensor_id(bytes, num_bytes);
}

void OnDiskImport::save_object_id_literal_datatype(XMLTag& predicate, XMLTag& object)
{
    std::string datatype_name;
    auto it = predicate.params.find("rdf:datatype");
    if (it != predicate.params.end()) {
        datatype_name = it->second;
    }

    switch (object.datatype) {
    case RDFDatatype::UNSPECIFIED: {
        if (object.value.size() <= RDF_OID::MAX_INLINE_LEN_STRING) {
            object_id = SPARQL::Conversions::pack_string_simple_inline(object.value.c_str());
        } else {
            object_id.id = external_helper->get_or_create_external_string_id(
                               object.value.c_str(),
                               object.value.size()
                           )
                         | ObjectId::MASK_STRING_SIMPLE;
        }
        break;
    }
    case RDFDatatype::UNRECOGNIZED: {
        // Create a new datatype
        uint64_t datatype_id = get_datatype_id(datatype_name.c_str());
        if (object.value.size() <= ObjectId::STR_DT_INLINE_BYTES) {
            object_id = SPARQL::Conversions::pack_string_datatype_inline(datatype_id, object.value.c_str());
        } else {
            object_id.id = external_helper->get_or_create_external_string_id(
                               object.value.c_str(),
                               object.value.size()
                           )
                         | ObjectId::MASK_STRING_DATATYPE | (datatype_id << SPARQL::Conversions::TMP_SHIFT);
        }
        break;
    }
    case RDFDatatype::DATETIME: {
        object_id.id = DateTime::from_dateTime(object.value.c_str());
        if (object_id.is_null()) {
            object_id = save_ill_typed(current_line, object.value.c_str(), datatype_name.c_str());
        }
        break;
    }
    case RDFDatatype::DATE: {
        object_id.id = DateTime::from_date(object.value.c_str());
        if (object_id.is_null()) {
            object_id = save_ill_typed(current_line, object.value.c_str(), datatype_name.c_str());
        }
        break;
    }
    case RDFDatatype::TIME: {
        object_id.id = DateTime::from_time(object.value.c_str());
        if (object_id.is_null()) {
            object_id = save_ill_typed(current_line, object.value.c_str(), datatype_name.c_str());
        }
        break;
    }
    case RDFDatatype::DATETIMESTAMP: {
        object_id.id = DateTime::from_dateTimeStamp(object.value.c_str());
        if (object_id.is_null()) {
            object_id = save_ill_typed(current_line, object.value.c_str(), datatype_name.c_str());
        }
        break;
    }
    case RDFDatatype::STRING: {
        if (object.value.size() <= RDF_OID::MAX_INLINE_LEN_STRING) {
            object_id = SPARQL::Conversions::pack_string_xsd_inline(object.value.c_str());
        } else {
            object_id.id = external_helper->get_or_create_external_string_id(
                               object.value.c_str(),
                               object.value.size()
                           )
                         | ObjectId::MASK_STRING_XSD;
        }
        break;
    }
    case RDFDatatype::DECIMAL: {
        bool error;
        Decimal dec(object.value, &error);

        if (error) {
            object_id = save_ill_typed(current_line, object.value.c_str(), datatype_name.c_str());
        } else {
            if (dec.can_inline()) {
                object_id.id = dec.serialize_inlined() | ObjectId::MASK_DECIMAL_INLINED;
            } else {
                char dec_buffer[Decimal::EXTERN_BUFFER_SIZE];
                dec.serialize_extern(dec_buffer);
                object_id.id = external_helper->get_or_create_external_string_id(
                                   dec_buffer,
                                   Decimal::EXTERN_BUFFER_SIZE
                               )
                             | ObjectId::MASK_DECIMAL;
            }
        }
        break;
    }
    case RDFDatatype::FLOAT: {
        try {
            float f = std::stof(object.value);
            object_id = SPARQL::Conversions::pack_float(f);
        } catch (const std::out_of_range& e) {
            object_id = save_ill_typed(current_line, object.value.c_str(), datatype_name.c_str());
        } catch (const std::invalid_argument& e) {
            object_id = save_ill_typed(current_line, object.value.c_str(), datatype_name.c_str());
        }
        break;
    }
    case RDFDatatype::DOUBLE: {
        try {
            double d = std::stod(object.value);
            const char* chars = reinterpret_cast<const char*>(&d);
            object_id.id = external_helper->get_or_create_external_string_id(chars, sizeof(d))
                         | ObjectId::MASK_DOUBLE;
        } catch (const std::out_of_range& e) {
            object_id = save_ill_typed(current_line, object.value.c_str(), datatype_name.c_str());
        } catch (const std::invalid_argument& e) {
            object_id = save_ill_typed(current_line, object.value.c_str(), datatype_name.c_str());
        }
        break;
    }
    case RDFDatatype::INTEGER: {
        bool int_parser_error;
        object_id = handle_integer_string(object.value, &int_parser_error);
        if (int_parser_error) {
            object_id = save_ill_typed(current_line, object.value.c_str(), datatype_name.c_str());
        }
        break;
    }
    case RDFDatatype::BOOLEAN: {
        if (object.value == "true" || object.value == "1") {
            object_id = SPARQL::Conversions::pack_bool(true);
        } else if (object.value == "false" || object.value == "0") {
            object_id = SPARQL::Conversions::pack_bool(false);
        } else {
            object_id = save_ill_typed(current_line, object.value.c_str(), datatype_name.c_str());
        }
        break;
    }
    case RDFDatatype::TENSOR_FLOAT: {
        try_save_tensor<float>(object.value.c_str(), datatype_name.c_str());
        break;
    }
    case RDFDatatype::TENSOR_DOUBLE: {
        try_save_tensor<double>(object.value.c_str(), datatype_name.c_str());
        break;
    }
    default:
        break;
    }
}
void OnDiskImport::handle_object_value(XMLTag& predicate, XMLTag& object)
{
    if (object.lang != "") {
        save_object_id_literal_lang(object);
    } else {
        save_object_id_literal_datatype(predicate, object);
    }
}

void OnDiskImport::triple_preparation(XMLTag& subject, XMLTag& predicate, XMLTag& object)
{
    if (subject.object_id.id != ObjectId::NULL_ID) {
        subject_id = subject.object_id;
    } else if (subject.iri != "") {
        subject_id = get_iri_id(subject.iri.c_str(), subject.iri.size());
        subject.object_id = subject_id;
    } else {
        std::string node_name = "b";
        node_name += std::to_string(blank_node_count);
        subject_id = get_blank_id(node_name.c_str());
        node_id_oid[subject.node_id] = subject_id;
        subject.object_id = subject_id;
    }

    if (predicate.object_id.id != ObjectId::NULL_ID) {
        predicate_id = predicate.object_id;
    } else if (predicate.iri != "") {
        predicate_id = get_iri_id(predicate.iri.c_str(), predicate.iri.size());
        predicate.object_id = predicate_id;
    } else {
        print_fatal_error_msg("Predicate does not have an IRI specified, but all predicates must have an IRI"
        );
    }

    if (object.object_id.id != ObjectId::NULL_ID) {
        object_id = object.object_id;
    }
    if (object.iri != "") {
        object_id = get_iri_id(object.iri.c_str(), object.iri.size());
        object.object_id = object_id;
    } else if (object.value != "") {
        handle_object_value(predicate, object);
        object.object_id = object_id;
    } else {
        std::string node_name = "b";
        node_name += std::to_string(blank_node_count);
        object_id = get_blank_id(node_name.c_str());
        node_id_oid[object.node_id] = object_id;
        object.object_id = object_id;
    }

    save_triple();
}

void OnDiskImport::save_triple()
{
    if ((subject_id.id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
        || (predicate_id.id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
        || (object_id.id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP)
    {
        pending_triples->push_back({ subject_id.id, predicate_id.id, object_id.id });
    } else {
        if (subject_id == predicate_id) {
            equal_sp.push_back({ subject_id.id, object_id.id });
            if (subject_id == object_id) {
                equal_spo.push_back({ subject_id.id });
            }
        }
        if (subject_id == object_id) {
            equal_so.push_back({ subject_id.id, predicate_id.id });
        }
        if (predicate_id == object_id) {
            equal_po.push_back({ predicate_id.id, subject_id.id });
        }
        triples.push_back({ subject_id.id, predicate_id.id, object_id.id });
    }
}

void OnDiskImport::initialize_iri_prefixes()
{
    for (auto& prefix_pair : rdf_xml_prefixes) {
        prefix_set.insert(prefix_pair.second);
    }

    prefixes.init(std::move(prefix_set));
}

void OnDiskImport::verify_triple()
{
    if (current_rdf_term == RDFTerm::OBJECT) {
        XMLTag *subject, *predicate, *object;
        subject = &xml_branch[subject_idx];
        predicate = &xml_branch[predicate_idx];
        object = &xml_branch[object_idx];

        triple_preparation(*subject, *predicate, *object);

        ++current_rdf_term;
        subject_idx = object_idx;
    }
}

void OnDiskImport::verify_triple_from_subject(XMLTag& tag)
{
    bool subject_defines_triple = false;
    std::string param_name, param_value;
    for (auto param : tag.params) {
        if (param.first != "rdf:about" && param.first != "rdf:resource" && param.first != "rdf:ID"
            && param.first != "xml:lang")
        {
            param_name = param.first;
            param_value = param.second;
            subject_defines_triple = true;
            break;
        }
    }
    if (subject_defines_triple) {
        XMLTag tmp_predicate, tmp_object;
        tmp_predicate.name = param_name;
        tmp_predicate.iri = get_url_from_tag_declaration(param_name);
        tmp_predicate.lang = tag.lang;
        tmp_predicate.rdf_term = RDFTerm::PREDICATE;
        tmp_object.name = "rdf:Description";
        tmp_object.iri = param_value;
        tmp_object.lang = tag.lang;
        tmp_object.rdf_term = RDFTerm::OBJECT;
        triple_preparation(tag, tmp_predicate, tmp_object);
    }
}

void OnDiskImport::verify_triple_from_predicate(XMLTag& new_xml_tag)
{
    auto it = new_xml_tag.params.find("rdf:resource");
    if (it != new_xml_tag.params.end()) {
        XMLTag tmp_object;
        tmp_object.name = "rdf:Description";
        tmp_object.iri = it->second;
        tmp_object.lang = new_xml_tag.lang;
        tmp_object.rdf_term = RDFTerm::OBJECT;
        triple_preparation(xml_branch[xml_branch.size() - 1], new_xml_tag, tmp_object);
    } else if (!new_xml_tag.node_id.empty()) {
        XMLTag tmp_object;
        tmp_object.name = "rdf:Description";
        tmp_object.lang = new_xml_tag.lang;
        tmp_object.rdf_term = RDFTerm::OBJECT;
        tmp_object.node_id = new_xml_tag.node_id;
        triple_preparation(xml_branch[xml_branch.size() - 1], new_xml_tag, tmp_object);
    }
}

void OnDiskImport::determine_tag_datatype(XMLTag& new_xml_tag, std::vector<std::string>& param)
{
    namespace MDBType = SPARQL::MDBExtensions::Type;

    const auto& dt = param[1];

    if (dt.size() > MDBType::TYPE_PREFIX_IRI.size()
        && std::memcmp(dt.c_str(), MDBType::TYPE_PREFIX_IRI.c_str(), MDBType::TYPE_PREFIX_IRI.size()) == 0)
    {
        const auto mdbtype_suffix = dt.c_str() + MDBType::TYPE_PREFIX_IRI.size();
        // Supported mdb datatypes
        // mdbtype:tensorFloat
        if (strcmp(mdbtype_suffix, MDBType::TENSOR_FLOAT_SUFFIX_IRI.c_str()) == 0) {
            new_xml_tag.datatype = RDFDatatype::TENSOR_FLOAT;
        }
        // mdbtype:tensorDouble
        else if (strcmp(mdbtype_suffix, MDBType::TENSOR_DOUBLE_SUFFIX_IRI.c_str()) == 0)
        {
            new_xml_tag.datatype = RDFDatatype::TENSOR_DOUBLE;
        }
        // Unsupported datatypes are stored as literals with datatype
        else
        {
            new_xml_tag.datatype = RDFDatatype::UNRECOGNIZED;
        }

        return;
    }

    constexpr std::string_view XML_SCHEMA = "http://www.w3.org/2001/XMLSchema#";
    if (dt.size() > XML_SCHEMA.size() && std::memcmp(dt.c_str(), XML_SCHEMA.data(), XML_SCHEMA.size()) == 0) {
        const auto xsd_suffix = param[1].c_str() + XML_SCHEMA.size();
        if (strcmp(xsd_suffix, "dateTime") == 0) {
            new_xml_tag.datatype = RDFDatatype::DATETIME;
        } else if (strcmp(xsd_suffix, "date") == 0) {
            new_xml_tag.datatype = RDFDatatype::DATE;
        } else if (strcmp(xsd_suffix, "time") == 0) {
            new_xml_tag.datatype = RDFDatatype::TIME;
        } else if (strcmp(xsd_suffix, "dateTimeStamp") == 0) {
            new_xml_tag.datatype = RDFDatatype::DATETIMESTAMP;
        } else if (strcmp(xsd_suffix, "string") == 0) {
            new_xml_tag.datatype = RDFDatatype::STRING;
        } else if (strcmp(xsd_suffix, "decimal") == 0) {
            new_xml_tag.datatype = RDFDatatype::DECIMAL;
        } else if (strcmp(xsd_suffix, "float") == 0) {
            new_xml_tag.datatype = RDFDatatype::FLOAT;
        } else if (strcmp(xsd_suffix, "double") == 0) {
            new_xml_tag.datatype = RDFDatatype::DOUBLE;
        } else if (strcmp(xsd_suffix, "integer") == 0 || strcmp(xsd_suffix, "long") == 0
                   || strcmp(xsd_suffix, "int") == 0 || strcmp(xsd_suffix, "short") == 0
                   || strcmp(xsd_suffix, "byte") == 0 || strcmp(xsd_suffix, "nonPositiveInteger") == 0
                   || strcmp(xsd_suffix, "negativeInteger") == 0 || strcmp(xsd_suffix, "positiveInteger") == 0
                   || strcmp(xsd_suffix, "nonNegativeInteger") == 0 || strcmp(xsd_suffix, "unsignedLong") == 0
                   || strcmp(xsd_suffix, "unsignedInt") == 0 || strcmp(xsd_suffix, "unsignedShort") == 0
                   || strcmp(xsd_suffix, "unsignedByte") == 0)
        {
            new_xml_tag.datatype = RDFDatatype::INTEGER;
        } else if (strcmp(xsd_suffix, "boolean") == 0) {
            new_xml_tag.datatype = RDFDatatype::BOOLEAN;
        } else {
            new_xml_tag.datatype = RDFDatatype::UNRECOGNIZED;
        }

        return;
    }

    new_xml_tag.datatype = RDFDatatype::UNRECOGNIZED;
}

void OnDiskImport::open_tag()
{
    bool self_closing_tag = false;
    std::vector<std::string> params;
    {
        std::string sanitized_tag_text = remove_endline_and_whitespace(lexer.str);
        sanitized_tag_text.erase(0, 1); // Remove '<'
        sanitized_tag_text.pop_back(); // Remove '>'
        params = parse_xml_params(sanitized_tag_text);
    }

    if (has_ending(params.back(), "/")) {
        self_closing_tag = true;
        params.back().pop_back();
    }

    XMLTag new_xml_tag;
    new_xml_tag.name = params[0];

    if (new_xml_tag.name != "rdf:RDF") {
        new_xml_tag.rdf_term = ++current_rdf_term;
    } else {
        new_xml_tag.rdf_term = RDFTerm::OBJECT;
    }

    if (current_rdf_term == RDFTerm::PREDICATE) {
        new_xml_tag.iri = get_url_from_tag_declaration(new_xml_tag.name);
    }

    for (size_t i = 1; i < params.size(); i++) {
        if (params[i] == "" || params[i] == "/") {
            continue;
        }

        std::vector<std::string> param = split_xml_param(params[i]);
        param[1].pop_back(); // Remove last quotation mark
        param[1].erase(0, 1); // Remove first quotation mark

        // If the tag corresponds to rdf:RDF, all params correspond to assigning
        // prefixes and namespaces
        if (new_xml_tag.name == "rdf:RDF") {
            std::vector<std::string> key = split(param[0], ":");
            if (key.size() <= 1) {
                using_global_namespace = true;
                xml_global_namespace = param[1];
            } else {
                if (key[0] == "xml" && key[1] == "base") {
                    xml_base = param[1];
                }
                rdf_xml_prefixes.insert({ key[1], param[1] });
            }
            continue;
        }

        bool writing_iri = (param[0] == "rdf:about" || param[0] == "rdf:resource" || param[0] == "rdf:ID");

        if (new_xml_tag.rdf_term != RDFTerm::PREDICATE && writing_iri) {
            auto found_http = strstr(new_xml_tag.iri.c_str(), "http://");
            auto found_https = strstr(new_xml_tag.iri.c_str(), "https://");
            if (xml_base != "" && !(found_http || found_https)) {
                new_xml_tag.iri += xml_base;
            }

            if (param[0] == "rdf:about" || param[0] == "rdf:resource") {
                new_xml_tag.iri += param[1];
            } else if (param[0] == "rdf:ID") {
                new_xml_tag.iri += '#';
                new_xml_tag.iri += param[1];
            }
        }

        if (param[0] == "rdf:nodeID") {
            auto it = node_id_oid.find(param[1]);
            if (it != node_id_oid.end()) {
                new_xml_tag.object_id = it->second;
            } else {
                node_id_oid.insert({ param[1], ObjectId::get_null() });
            }
            new_xml_tag.node_id = param[1];
        }

        if (param[0] == "rdf:datatype") {
            determine_tag_datatype(new_xml_tag, param);
        }

        if (param[0] == "xml:lang") {
            new_xml_tag.lang = param[1];
        }

        new_xml_tag.params.insert({ param[0], param[1] });
    }

    // Propagate the xml:lang property
    if (!xml_branch.empty() && xml_branch.back().lang != "" && new_xml_tag.lang == "") {
        new_xml_tag.lang = xml_branch.back().lang;
    }

    // Initialize IRI prefixes
    if (!initialized_iri_prefixes && new_xml_tag.name == "rdf:RDF") {
        initialized_iri_prefixes = true;
        initialize_iri_prefixes();
    }

    // Some of the parameters may trigger a triple save
    switch (new_xml_tag.rdf_term) {
    case RDFTerm::SUBJECT: {
        verify_triple_from_subject(new_xml_tag);
        break;
    }
    case RDFTerm::PREDICATE: {
        verify_triple_from_predicate(new_xml_tag);
        break;
    }
    case RDFTerm::OBJECT: {
        // Do nothing. This never happens
        break;
    }
    }

    xml_branch.push_back(std::move(new_xml_tag));

    switch (current_rdf_term) {
    case RDFTerm::SUBJECT:
        subject_idx = xml_branch.size() - 1;
        break;
    case RDFTerm::PREDICATE:
        predicate_idx = xml_branch.size() - 1;
        break;
    case RDFTerm::OBJECT:
        object_idx = xml_branch.size() - 1;
        break;
    }

    if (new_xml_tag.rdf_term == RDFTerm::OBJECT && xml_branch.back().name != "rdf:RDF") {
        verify_triple();
    }

    if (self_closing_tag) {
        close_tag(true);
    }
}

void OnDiskImport::add_tag_value()
{
    if (current_rdf_term != RDFTerm::PREDICATE) {
        print_fatal_error_msg("Only the predicate can have a written value");
    }
    xml_branch.back().value += lexer.str;

    // This value means that a triple must be saved. The value corresponds to the object
    XMLTag tmp_object;
    tmp_object.name = "rdf:Description";
    tmp_object.datatype = xml_branch.back().datatype;
    tmp_object.value = xml_branch.back().value;
    tmp_object.lang = xml_branch.back().lang;
    tmp_object.rdf_term = RDFTerm::OBJECT;
    triple_preparation(xml_branch[subject_idx], xml_branch.back(), tmp_object);
}

void OnDiskImport::close_tag(bool self_closing_tag)
{
    if (xml_branch.empty()) {
        WARN("Attempting to close tags when none are empty");
        return;
    }

    if (!self_closing_tag) {
        std::string tag_closing = lexer.str;
        tag_closing = tag_closing.substr(2, tag_closing.size() - 3);

        if (xml_branch.back().name != tag_closing) {
            auto warning = "Incorrect tag closure. Expected " + xml_branch.back().name + " but instead got "
                         + tag_closing + "Unexpected behaviour may occur.";
            WARN(warning);
        }
    }

    xml_branch.pop_back();

    if (xml_branch.size() != 0) {
        current_rdf_term = xml_branch.back().rdf_term;
        switch (current_rdf_term) {
        case RDFTerm::SUBJECT:
            subject_idx = xml_branch.size() - 1;
            break;
        case RDFTerm::PREDICATE:
            predicate_idx = xml_branch.size() - 1;
            if (xml_branch.size() > 1 && xml_branch[xml_branch.size() - 2].rdf_term == RDFTerm::SUBJECT) {
                subject_idx = xml_branch.size() - 2;
            }
            break;
        case RDFTerm::OBJECT:
            object_idx = xml_branch.size() - 1;
            break;
        }
    }
}

void OnDiskImport::normalize_string_literal()
{
    char* write_ptr = lexer.str;
    char* read_ptr = write_ptr + 1; // skip first character: '"'

    lexer.str_len -= 2;
    char* end = lexer.str + lexer.str_len + 1;

    UnicodeEscape::normalize_string(read_ptr, write_ptr, end, lexer.str_len);
}
