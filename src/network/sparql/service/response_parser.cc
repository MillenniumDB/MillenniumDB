#include "response_parser.h"

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "graph_models/rdf_model/datatypes/decimal.h"
#include "graph_models/rdf_model/datatypes/decimal_inlined.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "network/sparql/service/consume_api.h"
#include "query/executor/query_executor/sparql/ttl_writer.h"
#include "storage/string_manager.h"
#include "storage/tmp_manager.h"

using namespace std;

// Method to calculate an ObjectID when id fits inline (for any variable type).
// Returns the inline ID.
// The size parameter varies depending on the variable type.
inline uint64_t get_inline_id(const string& value, uint8_t size) {
    uint64_t inline_id  = 0;
    int shift_size = 8 * (size - 1);
    for (uint8_t byte : value) {
        uint64_t byte64 = static_cast<uint64_t>(byte);
        inline_id  |= byte64 << shift_size;
        shift_size -= 8;
    }
    return inline_id;
}


ResponseParser::ResponseParser(
    std::string&&                    query,
    std::string&&                    prefixes,
    std::variant<VarId, std::string> var_or_iri,
    const std::set<VarId>&           scope_vars,
    const std::set<VarId>&           fixed_vars
) :
    query           (query),
    prefixes        (prefixes),
    var_or_iri      (var_or_iri),
    scope_vars      (scope_vars),
    fixed_vars      (fixed_vars)
{
    request_duration = chrono::seconds(0);
    parse_duration   = chrono::seconds(0);
    format           = Format::json; // default response format, consume overrides it with response format
}


// Implements the operator begin method.
// Builds the request parts, send the request and depending on the format it parse the response.
// Any error within this method is thrown.
void ResponseParser::begin(Binding& parent_binding) {
    this->parent_binding = &parent_binding;

    bool   https;
    string host, target, body;
    https = parse_iri_and_body(host, target, body); // build the request parts

    int    response_status;
    string response;
    string port = https ? "443" : "80";
    auto   start_request = chrono::system_clock::now();

    response_status  = consume(https, host, port, target, body, response, format); // send request
    request_duration += chrono::system_clock::now() - start_request;
    if (response.empty()) {
        throw runtime_error("Empty response");
    }
    if (response_status != 200) {
        throw runtime_error("Bad Request to " + host + " with HTTP response status code: " + to_string(response_status));
    }

    auto start_parse = chrono::system_clock::now();
    switch (format) {
        case Format::json: {
            boost::property_tree::ptree pt;
            stringstream ss { response };
            try {
                boost::property_tree::read_json(ss, pt); // parse response
                results = pt.get_child("results.bindings");
            }
            catch (exception const& e) {
                throw runtime_error(
                "Wrong Content-Type header or response format is inconsistent with W3C JSON specification");
            }
            result = results.begin(); // store first result
            break;
        }
        case Format::xml: {
            boost::property_tree::ptree pt;
            stringstream ss { response };
            try {
                boost::property_tree::read_xml(ss, pt); // parse response
                results = pt.get_child("sparql.results");
            }
            catch (exception const& e) {
                throw runtime_error(
                "Wrong Content-Type header or response format is inconsistent with W3C XML specification");
            }
            result = results.begin(); // store first result
            break;
        }
        case Format::tsv: {
            this->response = response; // need to save the response
            if (!header_automata()) {
                throw runtime_error("Wrong TSV response format, the variable name found is not a query variable");
            }
            line_end = response.find('\n');
            break;
        }
        case Format::csv: {
            throw runtime_error("CSV Content-Type header is not supported");
        }
    }
    parse_duration += chrono::system_clock::now() - start_parse; // parsing duration is underestimated
}


// Implements the operator next method.
// Returns true if there was a next binding and false otherwise.
// Try to extract the binding from the response structure. Throws an error if fails.
bool ResponseParser::next() {
    for (auto var_id : scope_vars) {
        parent_binding->add(var_id, ObjectId::get_null());
    }
    switch (format) {
        case Format::json: {
            return extract_object_json();
        }
        case Format::xml: {
            return extract_object_xml();
        }
        case Format::tsv: {
            return response_automata(); // line_end gets updated with next result
        }
        case Format::csv: {
            throw runtime_error("CSV Content-Type header is not supported");
        }
    }
    return false;
}


// Implements the operator reset method.
// The formats which implement the same logic are grouped.
// Does not consume the API again.
void ResponseParser::reset() {
    switch (format) {
        case Format::json:
        case Format::xml: {
            result = results.begin();
            break;
        }
        case Format::tsv: {
            line_end = response.find('\n');
            break;
        }
        case Format::csv: {
            throw runtime_error("CSV Content-Type header is not supported");
        }
    }
}


// Method to build the parts of the request that is going to be send.
// The only schemes allowed are http and https.
// The method overrides its parameters and returns the scheme used, true means https.
bool ResponseParser::parse_iri_and_body(string& host, string& target, string& body) {
    string sub_iri;
    bool https;
    if (std::holds_alternative<std::string>(var_or_iri)) {
        current_iri = std::get<std::string>(var_or_iri);
    } else {
        auto oid = (*parent_binding)[std::get<VarId>(var_or_iri)];
        if (oid.get_generic_type() != ObjectId::MASK_IRI) {
            throw runtime_error("SERVICE VAR is not a SPARQL endpoint");
        }

        current_iri = SPARQL::Conversions::unpack_iri(oid);
    }

    if (bnode_maps.find(current_iri) == bnode_maps.end()) {
        bnode_maps.insert({current_iri, unordered_map<string, uint64_t>()});
    }

    if (current_iri.compare(0, 7, "http://") == 0) {
        https   = false;
        sub_iri = current_iri.substr(7);
    } else if (current_iri.compare(0, 8, "https://") == 0) {
        https   = true;
        sub_iri = current_iri.substr(8);
    } else {
        throw runtime_error("Iri scheme should be 'http://' or 'https://'");
    }
    auto slash = sub_iri.find("/");
    if (slash != string::npos) {
        host   = sub_iri.substr(0, slash);
        target = sub_iri.substr(slash);
    } else {
        host   = sub_iri;
        target = "";
    }
    string values_header = "VALUES ("; // If query has a VALUES for a join var, 2 VALUES means the intersection (the expected result)
    stringstream values_body;
    values_body << ("{(");
    for (auto var_id : fixed_vars) {
        auto oid = (*parent_binding)[var_id];
        if (not oid.is_null()) {
            auto var_name = get_query_ctx().get_var_name(var_id);
            values_header += " ?" + var_name;

            values_body << " ";
            write_and_escape_ttl(values_body, oid);
        }
    }
    values_header += " ) ";
    values_body << " )}\n";

    query.pop_back();
    body = prefixes + "SELECT * WHERE " + query + values_header + values_body.str() + '}';
    query.push_back('}');
    return https;
}


// Method to extract the next binding from a property tree built from JSON.
// Returns true if there was a next binding and false otherwise.
// The method will throw an error if the response structure does not follow the W3 JSON specification.
bool ResponseParser::extract_object_json() {
    if (result == results.end())
        return false;
    for (const auto& [var_name, values] : result->second.get_child("")) {
        bool found;
        auto var_id = get_query_ctx().get_var(var_name, &found);
        if (found && scope_vars.find(var_id) != scope_vars.end()) {
            // process var info and its specific type
            string attr_type  = values.get<string>("type");
            string attr_value = values.get<string>("value");
            string extra_data = "";
            if (attr_type == "literal" && values.count("datatype")) { // checks if there is a datatype child
                extra_data = values.get<string>("datatype");
                attr_type  = "datatype";
            } else if (attr_type == "literal" && values.count("xml:lang")) { // checks if there is a language child
                extra_data = values.get<string>("xml:lang");
                attr_type  = "lang";
            }
            // set ObjectId in respective var of binding
            ObjectId object_id;
            object_id   = get_object_id(attr_type, attr_value, extra_data);
            parent_binding->add(var_id, object_id); // update parent binding
        }
    }
    ++result;
    return true;
}


// Method to extract the next binding from a property tree built from XML.
// Returns true if there was a next binding and false otherwise.
// The method will throw an error if the response structure does not follow the W3 XML specification.
bool ResponseParser::extract_object_xml() {
    if (result == results.end()) return false;
    if (result->first == "<xmlattr>") ++result; // sometimes results XML variable has a XML attribute
    if (result == results.end()) return false;

    for (auto& attr : result->second.get_child("")) {
        if (attr.first == "<xmlattr>") continue;
        // process variable info and its specific type
        string attr_name = attr.second.get<string>("<xmlattr>.name");
        bool found;
        auto var_id = get_query_ctx().get_var(attr_name, &found);
        if (found and scope_vars.find(var_id) != scope_vars.end()) {
            string attr_type, attr_value, extra_data = "";
            if (attr.second.count("literal")) { // checks if there is a literal child before extracting it
                attr_type = "literal";
                auto var  = attr.second.get_child("literal");
                if (var.count("<xmlattr>") && var.get_child("<xmlattr>").count("datatype")) {
                    extra_data = var.get<string>("<xmlattr>.datatype");
                    attr_type  = "datatype";
                } else if (var.count("<xmlattr>") && var.get_child("<xmlattr>").count("xml:lang")) {
                    extra_data = var.get<string>("<xmlattr>.xml:lang");
                    attr_type  = "lang";
                }
                attr_value = var.get<string>("");
            } else if (attr.second.count("uri")) { // checks if there is an uri child before extracting it
                attr_type  = "uri";
                attr_value = attr.second.get<string>("uri");
            } else if (attr.second.count("bnode")) { // checks if there is a blank node child before extracting it
                attr_type  = "bnode";
                attr_value = attr.second.get<string>("bnode");
            }
            // set ObjectId in respective var of binding
            ObjectId object_id;
            object_id   = get_object_id(attr_type, attr_value, extra_data);
            parent_binding->add(var_id, object_id); // update parent binding
        }
    }
    ++result;
    return true;
}


// Method to automata parse the header of a TSV.
// Returns the status of the parsing (true is ended successfully).
// After calling this method the ResponseParser header variable is ready to use.
// Valid TSV header: '?var1\t?var2'.
bool ResponseParser::header_automata() {
    header.clear();
    auto pos = response.find('\n'); // pos is also the size we want to read
    if (pos == string::npos) pos = response.length() - 1; // response is not empty
    if (pos == 0) return false;
    if (response[pos - 1] == '\r') pos -= 1;
    if (pos == 0) return false;

    string header_token;
    for (char c : response.substr(0, pos)) {
        if (c == '\t') {
            bool found;
            auto var_id = get_query_ctx().get_var(header_token, &found);
            if (scope_vars.find(var_id) != scope_vars.end()) {
                header.push_back(header_token); // store header variable names
            } else {
                return false;
            }
            header_token.clear();
        } else if (c != '?' && c != '\"') { // (c != '\"') shouldn't be necessary, gives some more robustness
            header_token += c;
        }
    }
    bool found;
    auto var_id = get_query_ctx().get_var(header_token, &found);
    if (scope_vars.find(var_id) != scope_vars.end()) {
        header.push_back(header_token); // store header variable names
    } else {
        return false;
    }
    return true;
}


// Method to automata parse the response for TSV.
// Returns true if there was a next binding and false otherwise.
// The quotes escaped within strings are skipped.
// If parser finds '\\' + 't' writes '\t' in the string (t|b|f|n).
bool ResponseParser::response_automata() {
    if (line_end >= response.length() - 1) return false;

    // States
    bool in_simple_string = false, in_double_string = false;
    bool in_datatype      = false, in_lang          = false;
    bool in_integer       = false, in_decimal       = false;
    bool in_bnode         = false;
    bool in_uri           = false;
    bool escaped          = false;
    bool first_char       = true;
    string attr_value, attr_type, extra_data;
    size_t header_index = 0;
    char c;
    size_t pos;

    for (pos = line_end + 1; pos < response.length(); pos++) {
        c = response.at(pos);

        // Token parsing finished
        if (c == '\t' || c == '\n' || (pos == response.length() - 1)) {
            // not \n because that can be the last char
            if (c != '\n' && (pos == response.length() - 1) && (in_datatype || in_lang)) extra_data += c;
            if (c != '\n' && (pos == response.length() - 1)) attr_value += c;

            if (header_index >= header.size()) {
                throw runtime_error("Response format is inconsistent with W3C CSV/TSV specification");
            }

            ObjectId object_id;
            if (attr_value.empty()) {
                object_id = ObjectId::get_null();
            } else {
                if (in_bnode) {
                    in_bnode  = false;
                    attr_type = "bnode";

                } else if (in_uri) {
                    in_uri    = false;
                    attr_type = "uri";
                    attr_value.pop_back(); // erase '>' in uri

                } else if (in_double_string || in_simple_string) {
                    in_double_string = false;
                    in_simple_string = false;
                    attr_type        = "literal";
                    attr_value.pop_back(); // erase '\'' or '\"' in literal

                } else if (in_lang) {
                    in_lang   = false;
                    attr_type = "lang";

                } else if (in_datatype) {
                    in_datatype = false;
                    attr_type   = "datatype";
                    extra_data.pop_back(); // erase '>' in datatype

                } else if (in_integer) {
                    in_integer = false;
                    attr_type  = "datatype";
                    extra_data = "http://www.w3.org/2001/XMLSchema#integer";

                } else if (in_decimal) {
                    in_decimal = false;
                    attr_type  = "datatype";
                    extra_data = "http://www.w3.org/2001/XMLSchema#decimal";

                } else {
                    attr_type = "literal";
                }

                object_id = get_object_id(attr_type, attr_value, extra_data);
            }
            bool found;
            auto var_id = get_query_ctx().get_var(header[header_index], &found);
            if (found and scope_vars.find(var_id) != scope_vars.end()) {
                parent_binding->add(var_id, object_id); // update parent binding
            }
            attr_value.clear();
            extra_data.clear();
            first_char = true;
            ++header_index;
            if (c == '\n' || (pos == response.length() - 1)) { // binding parsing is finished
                if (header_index != header.size()) {
                    throw runtime_error("Response format is inconsistent with W3C CSV/TSV specification");
                }
                line_end = pos;
                return true;
            }

            // Process the first char and set the variable type
        } else if (first_char) {
            switch (c) {
                case '<': {
                    in_uri = true;
                    break;
                }
                case '\"': {
                    in_double_string = true;
                    break;
                }
                case '\'': {
                    in_simple_string = true;
                    break;
                }
                case '_': {
                    if ((pos <= response.length() - 2) && (response.at(pos + 1) == ':')) { // 1-lookahead
                        in_bnode = true;
                        ++pos;
                    } else {
                        attr_value += c;
                    }
                    break;
                }
                default: {
                    in_integer = true;
                    if (c != '\r') attr_value += c;
                    break;
                }
            }
            first_char = false;

            // Process the next char of a escape
        } else if (escaped) { // assumes that languages and datatypes does not have escaped chars
            switch (c) {
                case 't': {
                    attr_value += '\t';
                    break;
                }
                case 'b': {
                    attr_value += '\b';
                    break;
                }
                case 'n': {
                    attr_value += '\n';
                    break;
                }
                case 'f': {
                    attr_value += '\f';
                    break;
                }
                case '\"': {
                    attr_value += '\"';
                    break;
                }
                case '\'': {
                    attr_value += '\'';
                    break;
                }
                case '\r': {
                    attr_value += '\\';
                    break;
                }
                default: {
                    attr_value += '\\';
                    attr_value += c;
                    break;
                }
            }

            // Build extra data
        } else if (in_lang || in_datatype) {
            extra_data += c;

            // Check if literal is a datatype variable, the quotation marks are not escaped
        } else if (((in_simple_string && c == '\'') || (in_double_string && c == '\"'))
                   && (pos <= response.length() - 4) && (response.compare(pos + 1, 3, "^^<") == 0)) { // 3-lookahead
            in_simple_string = false;
            in_double_string = false;
            in_datatype      = true;
            pos += 3;

            // Check if literal is a language variable, the quotation marks are not escaped
        } else if (((in_simple_string && c == '\'') || (in_double_string && c == '\"'))
                   && (pos <= response.length() - 2) && (response.at(pos + 1) == '@')) { // 1-lookahead
            in_simple_string = false;
            in_double_string = false;
            in_lang          = true;
            pos += 1;

        } else if (c == '\\') {
            escaped = true;

        } else if (in_integer && (c == ',' || c == '.')) {
            in_integer = false;
            in_decimal = true;
            attr_value += c;

        } else {
            attr_value += c;
        }
    }
    line_end = pos; // this line should not execute, just a security measure
    return true;    // avoid warning
}


// Method to get an ObjectId given the type of the attribute.
// Returns the built ObjectId.
// All variable types from the specification can be built.
ObjectId ResponseParser::get_object_id(const string& attr_type,
                                       const string& attr_value,
                                       const string& extra_data) {
    string new_type = "";
    if (attr_type == "datatype") {
        const std::string xml_schema = "http://www.w3.org/2001/XMLSchema#";

        bool is_xml_schema = false;
        if (extra_data.size() > xml_schema.size()) {
            is_xml_schema = extra_data.substr(0, xml_schema.size()) == xml_schema;
        }

        if (!is_xml_schema) {
            uint64_t datatype_id = get_datatype_id(extra_data);
            if (attr_value.size() <= ObjectId::STR_DT_INLINE_BYTES) {
                uint64_t inline_id = get_inline_id(attr_value, ObjectId::STR_DT_INLINE_BYTES);
                return ObjectId(ObjectId::MASK_STRING_DATATYPE_INLINED | datatype_id | inline_id);
            } else {
                uint64_t external_id = string_manager.get_str_id(attr_value, false);
                if (external_id != ObjectId::MASK_NOT_FOUND) {
                    return ObjectId(ObjectId::MASK_STRING_DATATYPE_EXTERN | datatype_id | external_id);
                } else {
                    uint64_t temporal_id = tmp_manager.get_str_id(attr_value);
                    return ObjectId(ObjectId::MASK_STRING_DATATYPE_TMP | datatype_id | temporal_id);
                }
            }
        }

        auto xsd_suffix = extra_data.substr(xml_schema.size());

        if (xsd_suffix == "boolean") {
            if (attr_value == "true" || attr_value == "1") {
                return ObjectId(ObjectId::MASK_BOOL | 0x01);
            } else if (attr_value == "false" || attr_value == "0") {
                return ObjectId(ObjectId::MASK_BOOL | 0x00);
            } else {
                throw runtime_error("Unsupported boolean value: " + attr_value);
            }

        } else if (xsd_suffix == "date") {
            uint64_t datetime_id = DateTime::from_date(attr_value.c_str());
            if (datetime_id == ObjectId::NULL_ID) {
                throw runtime_error("Invalid date value: " + attr_value);
            }
            return ObjectId(datetime_id);

        } else if (xsd_suffix == "time") {
            uint64_t datetime_id = DateTime::from_time(attr_value.c_str());
            if (datetime_id == ObjectId::NULL_ID) {
                throw runtime_error("Invalid time value: " + attr_value);
            }
            return ObjectId(datetime_id);

        } else if (xsd_suffix == "dateTime") {
            uint64_t datetime_id = DateTime::from_dateTime(attr_value.c_str());
            if (datetime_id == ObjectId::NULL_ID) {
                throw runtime_error("Invalid dateTime value: " + attr_value);
            }
            return ObjectId(datetime_id);

        } else if (xsd_suffix == "dateTimeStamp") {
            uint64_t datetime_id = DateTime::from_dateTime(attr_value.c_str());
            if (datetime_id == ObjectId::NULL_ID) {
                throw runtime_error("Invalid dateTimeStamp value: " + attr_value);
            }
            return ObjectId(datetime_id);

        } else if (xsd_suffix == "float") {
            auto f = stof(attr_value);
            static_assert(sizeof(f) == 4);
            unsigned char bytes[sizeof(f)];
            memcpy(bytes, &f, sizeof(f));
            uint64_t inline_id  = 0;
            int shift_size      = 0;
            for (uint64_t byte : bytes) {
                inline_id |= byte << shift_size;
                shift_size += 8;
            }
            return ObjectId(ObjectId::MASK_FLOAT | inline_id);

        } else if (xsd_suffix == "double") {
            try {
                double d = std::stod(attr_value);
                static_assert(sizeof(d) == 8);
                return SPARQL::Conversions::pack_double(d);
            } catch (const std::out_of_range& e) {
                throw runtime_error("Invalid decimal value: " + attr_value);
            } catch (const std::invalid_argument& e) {
                throw runtime_error("Invalid decimal value: " + attr_value);
            }

        } else if (xsd_suffix == "decimal") {
            uint64_t decimal_id = DecimalInlined::get_decimal_id(attr_value.c_str());
            if (decimal_id != DecimalInlined::INVALID_ID) {
                return ObjectId(ObjectId::MASK_DECIMAL_INLINED | decimal_id);
            } else {
                bool error;
                Decimal decimal(attr_value, &error);
                if (error) {
                     throw runtime_error("Invalid decimal value: " + attr_value);
                }
                auto normalized_decimal = decimal.to_external();
                uint64_t external_id    = string_manager.get_str_id(normalized_decimal, false);
                if (external_id != ObjectId::MASK_NOT_FOUND) {
                    return ObjectId(ObjectId::MASK_DECIMAL_EXTERN | external_id);
                } else {
                    uint64_t temporal_id = tmp_manager.get_str_id(normalized_decimal);
                    return ObjectId(ObjectId::MASK_DECIMAL_TMP | temporal_id);
                }
            }

        } else if (xsd_suffix == "integer"
                || xsd_suffix == "long"
                || xsd_suffix == "int"
                || xsd_suffix == "short"
                || xsd_suffix == "byte"
                || xsd_suffix == "nonPositiveInteger"
                || xsd_suffix == "negativeInteger"
                || xsd_suffix == "nonNegativeInteger"
                || xsd_suffix == "unsignedLong"
                || xsd_suffix == "unsignedInt"
                || xsd_suffix == "unsignedShort") {
            auto i = stoll(attr_value);
            // If the integer uses more than 56 bits, it must be converted into Decimal Extern or Temporal (overflow)
            if (i < -0x00FF'FFFF'FFFF'FFFF || i > 0x00FF'FFFF'FFFF'FFFF) { // TODO: rewrite the conditions
                bool error;
                Decimal dec(attr_value, &error);
                if (error) {
                    throw runtime_error("Invalid integer value: " + attr_value);
                }
                return SPARQL::Conversions::pack_decimal(dec);
            } else if (i < 0) {
                i *= -1;
                i = (~i) & ObjectId::VALUE_MASK;
                return ObjectId(ObjectId::MASK_NEGATIVE_INT | i);
            } else {
                return ObjectId(ObjectId::MASK_POSITIVE_INT | i);
            }

        } else if (xsd_suffix == "string") { // datatype not added in any format
            new_type = "literal";

        } else {
            uint64_t datatype_id = get_datatype_id(extra_data);
            if (attr_value.size() <= ObjectId::STR_DT_INLINE_BYTES) {
                uint64_t inline_id = get_inline_id(attr_value, ObjectId::STR_DT_INLINE_BYTES);
                return ObjectId(ObjectId::MASK_STRING_DATATYPE_INLINED | datatype_id | inline_id);
            } else {
                uint64_t external_id = string_manager.get_str_id(attr_value, false);
                if (external_id != ObjectId::MASK_NOT_FOUND) {
                    return ObjectId(ObjectId::MASK_STRING_DATATYPE_EXTERN | datatype_id | external_id);
                } else {
                    uint64_t temporal_id = tmp_manager.get_str_id(attr_value);
                    return ObjectId(ObjectId::MASK_STRING_DATATYPE_TMP | datatype_id | temporal_id);
                }
            }
        }
    }

    if (attr_type == "uri") {
        string sliced_attr = attr_value;
        uint8_t prefix_id  = get_prefix_id(sliced_attr); // sliced_attr gets sliced
        uint64_t shifted_prefix_id = static_cast<uint64_t>(prefix_id) << 48;
        if (sliced_attr.size() <= ObjectId::IRI_INLINE_BYTES) {
            uint64_t inline_id = get_inline_id(sliced_attr, ObjectId::IRI_INLINE_BYTES);
            return ObjectId(ObjectId::MASK_IRI_INLINED | shifted_prefix_id | inline_id);
        } else {
            uint64_t external_id = string_manager.get_str_id(sliced_attr, false);
            if (external_id != ObjectId::MASK_NOT_FOUND) {
                return ObjectId(ObjectId::MASK_IRI_EXTERN | shifted_prefix_id | external_id);
            } else {
                uint64_t temporal_id = tmp_manager.get_str_id(sliced_attr);
                return ObjectId(ObjectId::MASK_IRI_TMP | shifted_prefix_id | temporal_id);
            }
        }

    } else if (new_type == "literal" || attr_type == "literal") {
        if (attr_value.size() <= ObjectId::STR_INLINE_BYTES) {
            uint64_t inline_id = get_inline_id(attr_value, ObjectId::STR_INLINE_BYTES);
            return ObjectId(ObjectId::MASK_STRING_SIMPLE_INLINED | inline_id);
        } else {
            uint64_t external_id = string_manager.get_str_id(attr_value, false);
            if (external_id != ObjectId::MASK_NOT_FOUND) {
                return ObjectId(ObjectId::MASK_STRING_SIMPLE_EXTERN | external_id);
            } else {
                uint64_t temporal_id = tmp_manager.get_str_id(attr_value);
                return ObjectId(ObjectId::MASK_STRING_SIMPLE_TMP | temporal_id);
            }
        }

    } else if (attr_type == "lang") {
        uint64_t language_id = get_language_id(extra_data);
        if (attr_value.size() <= ObjectId::STR_LANG_INLINE_BYTES) {
            uint64_t inline_id = get_inline_id(attr_value, ObjectId::STR_LANG_INLINE_BYTES);
            return ObjectId(ObjectId::MASK_STRING_LANG_INLINED | language_id | inline_id);
        } else {
            uint64_t external_id = string_manager.get_str_id(attr_value, false);
            if (external_id != ObjectId::MASK_NOT_FOUND) {
                return ObjectId(ObjectId::MASK_STRING_LANG_EXTERN | language_id | external_id);
            } else {
                uint64_t temporal_id = tmp_manager.get_str_id(attr_value);
                return ObjectId(ObjectId::MASK_STRING_LANG_TMP | language_id | temporal_id);
            }
        }

    } else /*if (attr_type == "bnode")*/ {
        uint64_t bnode_id = get_bnode_id(attr_value);
        return ObjectId(ObjectId::MASK_ANON_TMP | bnode_id);
    }
}


// Method to get the catalog prefix_id for a given URI (value parameter).
// Returns the prefix ID.
// Value get its prefix sliced inside this method.
uint8_t ResponseParser::get_prefix_id(string& value) {
    uint8_t prefix_id = 0;
    for (auto& prefix : rdf_model.catalog().prefixes) {
        if (value.compare(0, prefix.size(), prefix) == 0) {
            value = value.substr(prefix.size(), value.size() - prefix.size());
            break;
        }
        ++prefix_id;
    }
    return prefix_id;
}


// Method to get the ID for a given datatype from the catalog or temporal manager.
// Returns the datatype ID shifted.
// The most significant bit indicates if it comes from the catalog (0) or temporal manager (1).
uint64_t ResponseParser::get_datatype_id(const string& value) {
    uint64_t datatype_id = 0;
    bool found = false;
    for (auto& datatype : rdf_model.catalog().datatypes) {
        if (value == datatype) {
            found = true;
            break;
        }
        ++datatype_id;
    }
    if (found) return datatype_id << 40;
    datatype_id = tmp_manager.get_dtt_id(value);
    return (ObjectId::MASK_TAG_MANAGER | datatype_id) << 40;
}


// Method to get the ID for a given language from the catalog or temporal manager.
// Returns the language ID shifted.
// The most significant bit indicates if it comes from the catalog (0) or temporal manager (1).
uint64_t ResponseParser::get_language_id(const string& value) {
    uint64_t language_id = 0;
    bool found = false;
    for (auto& language : rdf_model.catalog().languages) {
        if (value == language) {
            found = true;
            break;
        }
        ++language_id;
    }
    if (found) return language_id << 40;
    language_id = tmp_manager.get_lan_id(value);
    return (ObjectId::MASK_TAG_MANAGER | language_id) << 40;
}


// Method to get the ID for a given blank node.
// Returns the blank node ID.
// The bnode ID series starts after the series in the catalog and the scope its just within the service response.
uint64_t ResponseParser::get_bnode_id(const string& value) {
    auto it = bnode_maps[current_iri].find(value);
    if (it == bnode_maps[current_iri].end()) {
        auto res = get_query_ctx().get_new_blank_node();
        bnode_maps[current_iri].insert({ value, res });
        return res;
    } else {
        return it->second;
    }
}
