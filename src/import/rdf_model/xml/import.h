#pragma once

#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_flat_set.hpp>
#include <cstdint>
#include <string>

#include "graph_models/rdf_model/rdf_catalog.h"
#include "import/disk_vector.h"
#include "import/external_helper.h"
#include "import/rdf_model/xml/lexer/tokenizer.h"
#include "lexer/state.h"
#include "misc/istream.h"

namespace Import { namespace Rdf { namespace XML {


/**TODO: The following characteristics of RDF XML are not supported
         - XML Literals (rdf:parseType="Literal"): https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-XML-literals
         - Omitting Blank Nodes (rdf:parseType="Resource"): https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-parsetype-resource
         - Omitting Nodes: Property Attributes on an empty Property Element: https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-property-attributes-on-property-element
         - Typed Node Elements: https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-typed-nodes
         - Container Membership Property Elements (rdf:li and rdf:_n): https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-list-elements
         - Collections (rdf:parseType="Collection"): https://www.w3.org/TR/rdf-syntax-grammar/#section-Syntax-parsetype-Collection
         - owl:Ontology: https://stackoverflow.com/questions/27727845/what-does-owlontology-rdfabout-xmlbase-in-an-ontology-mean
*/

enum RDFTerm {
    SUBJECT = 0,
    PREDICATE = 1,
    OBJECT = 2
};

enum class RDFDatatype {
    UNSPECIFIED = 0,
    UNRECOGNIZED,
    DATETIME,
    DATE,
    TIME,
    DATETIMESTAMP,
    STRING,
    DECIMAL,
    FLOAT,
    DOUBLE,
    INTEGER,
    BOOLEAN,
    TENSOR_FLOAT,
    TENSOR_DOUBLE
};

struct XMLTag {
    std::string name;
    std::string iri;
    boost::unordered_flat_map<std::string, std::string> params;
    std::string value = "";
    std::string lang;
    std::string node_id;
    ObjectId object_id = ObjectId::get_null();
    RDFTerm rdf_term;
    RDFDatatype datatype = RDFDatatype::UNSPECIFIED;
};

class OnDiskImport {
public:
    static constexpr char PENDING_TRIPLES_FILENAME_PREFIX[] = "tmp_pending_triples";

    OnDiskImport(const std::string& db_folder, uint64_t strings_buffer_size, uint64_t tensors_buffer_size, size_t index_permutations = 3);
    ~OnDiskImport();

    size_t index_permutations;

    void start_import(MDBIstream& in, const std::string& prefixes_filename);
    void print_error();
    void print_error_msg(const std::string& msg);
    void print_fatal_error_msg(const std::string& msg);
    std::vector<std::string> split(const std::string& input, const std::string& delimiter);
    std::vector<std::string> split_xml_param(const std::string& input);

private:
    std::string db_folder;
    uint64_t strings_buffer_size;
    uint64_t tensors_buffer_size;

    XMLTokenizer lexer;
    int current_line = 1;
    int parsing_errors = 0;
    int current_state = State::START;

    RdfCatalog catalog;

    ObjectId subject_id;
    ObjectId predicate_id;
    ObjectId object_id;

    uint64_t subject_idx;
    uint64_t predicate_idx;
    uint64_t object_idx;

    Import::DiskVector<3> triples;
    Import::DiskVector<1> equal_spo;
    Import::DiskVector<2> equal_sp;
    Import::DiskVector<2> equal_so;
    Import::DiskVector<2> equal_po;

    // manager writing bytes to disk in a buffered manner
    std::unique_ptr<ExternalHelper> external_helper;

    void normalize_string_literal();

    std::unique_ptr<Import::DiskVector<3>> pending_triples;
    char* buffer_iri;

    std::function<void()> state_funcs[Token::TOTAL_TOKENS * State::TOTAL_STATES];
    int* state_transitions;
    void create_automata();
    void set_transition(int state, int token, int value, std::function<void()> func);
    int get_transition(int state, int token);

    // Blank nodes
    uint64_t blank_node_count = 0;
    boost::unordered_flat_map<std::string, uint64_t> blank_ids_map;

    IriPrefixes prefixes;
    std::set<std::string> prefix_set;

    boost::unordered_flat_map<std::string, uint64_t> datatype_ids_map;
    boost::unordered_flat_map<std::string, uint64_t> language_ids_map;

    boost::unordered_flat_map<std::string, std::string> rdf_xml_prefixes;
    boost::unordered_flat_map<std::string, ObjectId> node_id_oid;
    std::string last_xml_prefix;
    std::string xml_global_namespace;
    std::string xml_base;
    bool using_global_namespace = false;
    bool initialized_iri_prefixes = false;
    std::vector<XMLTag> xml_branch;
    std::string last_param;
    XMLTag last_removed_tag;
    RDFTerm current_rdf_term = RDFTerm::OBJECT;

    std::string get_url_from_tag_declaration(std::string& string);
    bool has_ending(const std::string& full_string, const std::string& ending);
    std::string remove_endline_and_whitespace(const std::string& in);
    std::vector<std::string> parse_xml_params(const std::string& in);
    ObjectId get_iri_id(const char* str, size_t str_len);
    ObjectId get_blank_id(const char* str);
    uint64_t get_datatype_id(const char* datatype);
    uint64_t get_lang_id(const char* lang);
    ObjectId handle_integer_string(const std::string& str, bool* error);
    ObjectId save_ill_typed(unsigned line, const char* value, const char* datatype);
    void save_object_id_literal_lang(XMLTag& object);
    void save_object_id_literal_datatype(XMLTag& predicate, XMLTag& object);
    void try_save_object_id_mdbtype(const char* str, uint64_t str_size, const char* dt);
    template<typename T>
    void try_save_tensor(const char* str, const char* dt);
    void handle_object_value(XMLTag& predicate, XMLTag& object);
    void triple_preparation(XMLTag& subject, XMLTag& predicate, XMLTag& object);
    void save_triple();
    void initialize_iri_prefixes();
    void verify_triple();
    void verify_triple_from_subject(XMLTag& tag);
    void verify_triple_from_predicate(XMLTag& new_xml_tag);
    void determine_tag_datatype(XMLTag& tag, std::vector<std::string>& param);

    void do_nothing() { };
    void open_tag();
    void add_tag_value();
    void close_tag(bool self_closing_tag=false);
    void move_to_next_line() { current_line++; }
};

}}}
