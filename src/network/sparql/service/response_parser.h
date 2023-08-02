#pragma once

#include <chrono>
#include <map>
#include <set>
#include <unordered_map>
#include <variant>

#include <boost/property_tree/ptree.hpp>
#include <boost/tokenizer.hpp>

#include "query/executor/binding.h"
#include "graph_models/object_id.h"

// The response format
enum class Format {json, xml, tsv, csv};

class ResponseParser {
public:
    ResponseParser(
        std::string&&                    query,
        std::string&&                    prefixes,
        std::variant<VarId, std::string> var_or_iri,
        const std::set<VarId>&           scope_vars,
        const std::set<VarId>&           fixed_vars
    );

    // Methods which implement the operator interface
    void begin(Binding& parent_binding);
    bool next();
    void reset();

    // Variables used by the Service operator
    std::string query;
    std::string prefixes;
    std::variant<VarId, std::string> var_or_iri;
    // Vars that are in scope after evaluating service, these have to be copied into the parent binding.
    const std::set<VarId>& scope_vars;
    // Vars that are fixed from the outside, these have to be passed to the service using VALUES.
    const std::set<VarId>& fixed_vars;
    // std::map<std::string, VarId> varname2varid; // Vars within SERVICE query and not in assigned vars
    std::chrono::duration<float, std::milli> request_duration;
    std::chrono::duration<float, std::milli> parse_duration;

private:
    // Method to build the parts of future request
    bool parse_iri_and_body(std::string& host, std::string& target, std::string& body);

    // Methods to extract objects from property tree
    bool extract_object_json();
    bool extract_object_xml();

    // Automata
    bool header_automata();
    bool response_automata();

    // Methods to build object id
    ObjectId get_object_id(const std::string& attr_type, const std::string& attr_value, const std::string& extra_data);
    uint8_t  get_prefix_id(std::string& value);
    uint64_t get_datatype_id(const std::string& value);
    uint64_t get_language_id(const std::string& value);
    uint64_t get_bnode_id(const std::string& value);

    // Variables used in all formats
    Format format;
    Binding* parent_binding;
    std::string current_iri; // The current iri consumed
    std::unordered_map<std::string, std::unordered_map<std::string, uint64_t>> bnode_maps; // Iri bnode scope

    // Variables used in json and xml formats
    boost::property_tree::ptree results;
    boost::property_tree::ptree::iterator result;

    // Variables used in csv and tsv formats
    size_t line_end;
    std::string response;
    std::vector<std::string> header;
};
