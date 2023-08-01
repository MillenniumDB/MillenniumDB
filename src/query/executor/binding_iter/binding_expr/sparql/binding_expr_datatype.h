#pragma once

#include <memory>
#include <sstream>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "storage/unified_storage.h"


namespace SPARQL {
class BindingExprDatatype : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprDatatype(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.get_sub_type()) {
        case ObjectId::MASK_STRING_DATATYPE: {
            auto datatype_id = (expr_oid.id & ObjectId::MASK_LITERAL_TAG) >> ObjectId::STR_DT_INLINE_BYTES * 8;
            std::stringstream ss;
            UnifiedStorage::print_datatype(ss, datatype_id);
            return Conversions::pack_iri(ss.str());
        }
        case ObjectId::MASK_STRING_SIMPLE:
        case ObjectId::MASK_STRING_XSD:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#string");
        case ObjectId::MASK_STRING_LANG:
            return Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#langString");
        case ObjectId::MASK_INT:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#integer");
        case ObjectId::MASK_DECIMAL:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#decimal");
        case ObjectId::MASK_FLOAT:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#float");
        case ObjectId::MASK_DOUBLE:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#double");
        case ObjectId::MASK_DT_DATE:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#date");
        case ObjectId::MASK_DT_DATETIME:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#dateTime");
        case ObjectId::MASK_DT_TIME:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#time");
        case ObjectId::MASK_DT_DATETIMESTAMP:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#dateTimeStamp");
        case ObjectId::MASK_BOOL:
            return Conversions::pack_iri("http://www.w3.org/2001/XMLSchema#boolean");
        default: {
            return ObjectId::get_null();
        }
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "DATATYPE(" << *expr << ")";
        return os;
    }
};
} // namespace SPARQL
