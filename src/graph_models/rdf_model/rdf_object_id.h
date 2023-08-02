#pragma once

#include <string>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "graph_models/rdf_model/datatypes/decimal.h"

struct RDFObjectId {
    static ObjectId get_Iri(const std::string&);
    static ObjectId get_Literal(const std::string&);
    static ObjectId get_LiteralXSDString(const std::string&);
    static ObjectId get_LiteralDatatype(const std::string& literal, const std::string& datatype);
    static ObjectId get_LiteralLang(const std::string& literal, const std::string& lang);
    static ObjectId get(DateTime);
    static ObjectId get(Decimal);
    static ObjectId get(bool);
    static ObjectId get(int64_t);
    static ObjectId get(float);
    static ObjectId get(double);

private:
    static ObjectId get_string_id(const std::string&, uint64_t subtype_mask);
};
