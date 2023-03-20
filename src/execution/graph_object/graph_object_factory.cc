#include "graph_object_factory.h"

#include "base/graph_object/anonymous_node.h"
#include "base/graph_object/datetime.h"
#include "base/graph_object/decimal_inlined.h"
#include "base/graph_object/decimal_external.h"
#include "base/graph_object/decimal_tmp.h"
#include "base/graph_object/edge.h"
#include "base/graph_object/graph_object.h"
#include "base/graph_object/iri_external.h"
#include "base/graph_object/iri_inlined.h"
#include "base/graph_object/iri_tmp.h"
#include "base/graph_object/literal_datatype_external.h"
#include "base/graph_object/literal_datatype_inlined.h"
#include "base/graph_object/literal_datatype_tmp.h"
#include "base/graph_object/literal_language_external.h"
#include "base/graph_object/literal_language_inlined.h"
#include "base/graph_object/literal_language_tmp.h"
#include "base/graph_object/not_found_object.h"
#include "base/graph_object/null_graph_object.h"
#include "base/graph_object/path.h"
#include "base/graph_object/string_external.h"
#include "base/graph_object/string_inlined.h"
#include "base/graph_object/string_tmp.h"
#include "execution/graph_object/graph_object_types.h"

union GraphObjectUnion {
    friend class GraphObjectFactory;
    GraphObjectUnion(int64_t n) : i(n) { }

    Edge                    edge;
    AnonymousNode           anon;
    Path                    path;
    StringInlined           str_inlined;
    StringExternal          str_external;
    StringTmp               str_tmp;
    bool                    b;
    int64_t                 i;
    float                   f;
    IriInlined              iri_inlined;
    IriExternal             iri_external;
    IriTmp                  iri_tmp;
    LiteralDatatypeInlined  ld_inlined;
    LiteralDatatypeExternal ld_external;
    LiteralDatatypeTmp      ld_tmp;
    LiteralLanguageInlined  ll_inlined;
    LiteralLanguageExternal ll_external;
    LiteralLanguageTmp      ll_tmp;
    DateTime                datetime;
    DecimalInlined          decimal_inlined;
    DecimalExternal         decimal_external;
    DecimalTmp              decimal_tmp;

private:
    GraphObjectUnion(Edge n) : edge(n) { }
    GraphObjectUnion(AnonymousNode n) : anon(n) { }
    GraphObjectUnion(Path n) : path(n) { }
    GraphObjectUnion(StringInlined n) : str_inlined(n) { }
    GraphObjectUnion(StringExternal n) : str_external(n) { }
    GraphObjectUnion(StringTmp n) : str_tmp(n) { }
    GraphObjectUnion(bool n) : b(n) { }
    GraphObjectUnion(float n) : f(n) { }
    GraphObjectUnion(IriInlined n) : iri_inlined(n) { }
    GraphObjectUnion(IriExternal n) : iri_external(n) { }
    GraphObjectUnion(IriTmp n) : iri_tmp(n) { }
    GraphObjectUnion(LiteralDatatypeInlined n) : ld_inlined(n) { }
    GraphObjectUnion(LiteralDatatypeExternal n) : ld_external(n) { }
    GraphObjectUnion(LiteralDatatypeTmp n) : ld_tmp(n) { }
    GraphObjectUnion(LiteralLanguageInlined n) : ll_inlined(n) { }
    GraphObjectUnion(LiteralLanguageExternal n) : ll_external(n) { }
    GraphObjectUnion(LiteralLanguageTmp n) : ll_tmp(n) { }
    GraphObjectUnion(DateTime n) : datetime(n) { }
    GraphObjectUnion(DecimalInlined n) : decimal_inlined(n) { }
    GraphObjectUnion(DecimalExternal n) : decimal_external(n) { }
    GraphObjectUnion(DecimalTmp n) : decimal_tmp(n) { }
    // TODO: implement what to do with sparql paths?? they shouldn't be transformed to GraphObject
};

static_assert(sizeof(int64_t) == sizeof(GraphObjectUnion));

GraphObject GraphObjectFactory::make_null() {
    return GraphObject();
}

GraphObject GraphObjectFactory::make_not_found() {
    return GraphObject(0, GraphObjectType::NOT_FOUND);
}

GraphObject GraphObjectFactory::make_named_node_external(uint64_t external_id) {
    return GraphObject(GraphObjectUnion(StringExternal(external_id)).i, GraphObjectType::NAMED_EXTERNAL);
}

GraphObject GraphObjectFactory::make_named_node_inlined(const char c[8]) {
    return GraphObject(GraphObjectUnion(StringInlined(c)).i, GraphObjectType::NAMED_INLINED);
}

GraphObject GraphObjectFactory::make_named_node_tmp(const std::string& tmp) {
    return GraphObject(GraphObjectUnion(StringTmp(tmp)).i, GraphObjectType::NAMED_TMP);
}

GraphObject GraphObjectFactory::make_anonymous(uint64_t id) {
    return GraphObject(GraphObjectUnion(AnonymousNode(id)).i, GraphObjectType::ANON);
}

GraphObject GraphObjectFactory::make_edge(uint64_t id) {
    return GraphObject(GraphObjectUnion(Edge(id)).i, GraphObjectType::EDGE);
}

GraphObject GraphObjectFactory::make_int(int64_t i) {
    return GraphObject(i, GraphObjectType::INT);
}

GraphObject GraphObjectFactory::make_float(float f) {
    return GraphObject(GraphObjectUnion(f).i, GraphObjectType::FLOAT);
}

GraphObject GraphObjectFactory::make_bool(bool b) {
    return GraphObject(GraphObjectUnion(b).i, GraphObjectType::BOOL);
}

GraphObject GraphObjectFactory::make_string_external(uint64_t external_id) {
    return GraphObject(GraphObjectUnion(StringExternal(external_id)).i, GraphObjectType::STR_EXTERNAL);
}

// must receive an array of size 8, terminating in '\0'
GraphObject GraphObjectFactory::make_string_inlined(const char* str) {
    return GraphObject(GraphObjectUnion(StringInlined(str)).i, GraphObjectType::STR_INLINED);
}

GraphObject GraphObjectFactory::make_string_tmp(const std::string& str) {
    return GraphObject(GraphObjectUnion(StringTmp(str)).i, GraphObjectType::STR_TMP);
}

GraphObject GraphObjectFactory::make_path(uint64_t path_id) {
    return GraphObject(GraphObjectUnion(Path(path_id)).i, GraphObjectType::PATH);
}

GraphObject GraphObjectFactory::make_iri_external(uint64_t external_id) {
    return GraphObject(GraphObjectUnion(IriExternal(external_id)).i, GraphObjectType::IRI_EXTERNAL);
}

// must receive an array of size 7, terminating in '\0' and an 1 byte prefix_id
GraphObject GraphObjectFactory::make_iri_inlined(const char* str, uint8_t prefix_id) {
    return GraphObject(GraphObjectUnion(IriInlined(str, prefix_id)).i, GraphObjectType::IRI_INLINED);
}

GraphObject GraphObjectFactory::make_literal_datatype_external(uint64_t external_id) {
    return GraphObject(GraphObjectUnion(LiteralDatatypeExternal(external_id)).i, GraphObjectType::LITERAL_DATATYPE_EXTERNAL);
}

GraphObject GraphObjectFactory::make_literal_datatype_inlined(const char* str, uint16_t datatype_id) {
    return GraphObject(GraphObjectUnion(LiteralDatatypeInlined(str, datatype_id)).i, GraphObjectType::LITERAL_DATATYPE_INLINED);
}

GraphObject GraphObjectFactory::make_literal_language_external(uint64_t external_id) {
    return GraphObject(GraphObjectUnion(LiteralLanguageExternal(external_id)).i, GraphObjectType::LITERAL_LANGUAGE_EXTERNAL);
}

GraphObject GraphObjectFactory::make_literal_language_inlined(const char* str, uint16_t language_id) {
    return GraphObject(GraphObjectUnion(LiteralLanguageInlined(str, language_id)).i, GraphObjectType::LITERAL_LANGUAGE_INLINED);
}

GraphObject GraphObjectFactory::make_datetime(uint64_t datetime_id) {
    return GraphObject(GraphObjectUnion(DateTime(datetime_id)).i, GraphObjectType::DATETIME);
}

GraphObject GraphObjectFactory::make_decimal_external(uint64_t external_id) {
    return GraphObject(GraphObjectUnion(DecimalExternal(external_id)).i, GraphObjectType::DECIMAL_EXTERNAL);
}

GraphObject GraphObjectFactory::make_decimal_inlined(uint64_t decimal_id) {
    return GraphObject(GraphObjectUnion(DecimalInlined(decimal_id)).i, GraphObjectType::DECIMAL_INLINED);
}

template<>
Edge GraphObjectInterpreter::get<Edge>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).edge;
}

template<>
AnonymousNode GraphObjectInterpreter::get<AnonymousNode>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).anon;
}

template<>
Path GraphObjectInterpreter::get<Path>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).path;
}

template<>
StringInlined GraphObjectInterpreter::get<StringInlined>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).str_inlined;
}

template<>
StringExternal GraphObjectInterpreter::get<StringExternal>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).str_external;
}

template<>
StringTmp GraphObjectInterpreter::get<StringTmp>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).str_tmp;
}

template<>
bool GraphObjectInterpreter::get<bool>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).b;
}

template<>
int64_t GraphObjectInterpreter::get<int64_t>(const GraphObject graph_object) {
    return graph_object.encoded_value;
}

template<>
float GraphObjectInterpreter::get<float>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).f;
}

template<>
IriInlined GraphObjectInterpreter::get<IriInlined>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).iri_inlined;
}

template<>
IriExternal GraphObjectInterpreter::get<IriExternal>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).iri_external;
}

template<>
IriTmp GraphObjectInterpreter::get<IriTmp>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).iri_tmp;
}

template<>
LiteralDatatypeInlined GraphObjectInterpreter::get<LiteralDatatypeInlined>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).ld_inlined;
}

template<>
LiteralDatatypeExternal GraphObjectInterpreter::get<LiteralDatatypeExternal>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).ld_external;
}

template<>
LiteralDatatypeTmp GraphObjectInterpreter::get<LiteralDatatypeTmp>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).ld_tmp;
}

template<>
LiteralLanguageInlined GraphObjectInterpreter::get<LiteralLanguageInlined>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).ll_inlined;
}

template<>
LiteralLanguageExternal GraphObjectInterpreter::get<LiteralLanguageExternal>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).ll_external;
}

template<>
LiteralLanguageTmp GraphObjectInterpreter::get<LiteralLanguageTmp>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).ll_tmp;
}

template<>
DateTime GraphObjectInterpreter::get<DateTime>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).datetime;
}

template<>
DecimalInlined GraphObjectInterpreter::get<DecimalInlined>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).decimal_inlined;
}

template<>
DecimalExternal GraphObjectInterpreter::get<DecimalExternal>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).decimal_external;
}

template<>
DecimalTmp GraphObjectInterpreter::get<DecimalTmp>(const GraphObject graph_object) {
    return GraphObjectUnion(int64_t(graph_object.encoded_value)).decimal_tmp;
}