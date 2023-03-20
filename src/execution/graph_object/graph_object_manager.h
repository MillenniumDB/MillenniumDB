#pragma once

#include <ostream>

#include "base/exceptions.h"
#include "base/graph_object/anonymous_node.h"
#include "base/graph_object/edge.h"
#include "base/graph_object/graph_object.h"
#include "base/graph_object/not_found_object.h"
#include "base/graph_object/null_graph_object.h"
#include "base/graph_object/path.h"
#include "base/graph_object/string_external.h"
#include "base/graph_object/string_inlined.h"
#include "base/graph_object/string_tmp.h"
#include "base/graph_object/iri_external.h"
#include "base/graph_object/iri_inlined.h"
#include "base/graph_object/iri_tmp.h"
#include "base/graph_object/literal_datatype_inlined.h"
#include "base/graph_object/literal_datatype_external.h"
#include "base/graph_object/literal_datatype_tmp.h"
#include "base/graph_object/literal_language_inlined.h"
#include "base/graph_object/literal_language_external.h"
#include "base/graph_object/literal_language_tmp.h"
#include "base/graph_object/datetime.h"
#include "base/graph_object/decimal_inlined.h"
#include "base/graph_object/decimal_external.h"
#include "base/graph_object/decimal_tmp.h"
#include "execution/graph_object/char_iter.h"
#include "execution/graph_object/graph_object_factory.h"
#include "execution/graph_object/graph_object_types.h"
#include "storage/string_manager.h"
#include "query_optimizer/rdf_model/rdf_model.h"

struct GraphObjectManager {
    static std::ostream& print(std::ostream& os, const GraphObject& graph_obj) {
        switch (graph_obj.type) {
        case GraphObjectType::NAMED_INLINED:
            return os << GraphObjectInterpreter::get<StringInlined>(graph_obj).id;
        case GraphObjectType::NAMED_EXTERNAL:
            string_manager.print(os,GraphObjectInterpreter::get<StringExternal>(graph_obj).external_id);
            return os;
        case GraphObjectType::NAMED_TMP:
            return os << *GraphObjectInterpreter::get<StringTmp>(graph_obj).str;
        case GraphObjectType::STR_INLINED:
            return os << '"' << GraphObjectInterpreter::get<StringInlined>(graph_obj).id << '"';
        case GraphObjectType::STR_EXTERNAL:
            os << '"';
            string_manager.print(os, GraphObjectInterpreter::get<StringExternal>(graph_obj).external_id);
            return os << '"';
        case GraphObjectType::STR_TMP:
            return os << '"' << *GraphObjectInterpreter::get<StringTmp>(graph_obj).str << '"';
        case GraphObjectType::EDGE:
            return os << "_e" << GraphObjectInterpreter::get<Edge>(graph_obj).id;
        case GraphObjectType::ANON:
            return os << "_a" << GraphObjectInterpreter::get<AnonymousNode>(graph_obj).id;
        case GraphObjectType::NULL_OBJ:
            return os << "null";
        case GraphObjectType::NOT_FOUND:
            return os << "NotFoundObj";
        case GraphObjectType::INT:
            return os << GraphObjectInterpreter::get<int64_t>(graph_obj);
        case GraphObjectType::FLOAT:
            return os << GraphObjectInterpreter::get<float>(graph_obj);
        case GraphObjectType::BOOL:
            return os << (GraphObjectInterpreter::get<bool>(graph_obj) ? "true" : "false");
        case GraphObjectType::PATH: {
            auto path = GraphObjectInterpreter::get<Path>(graph_obj);
            path.path_printer->print(os, path.path_id);
            return os;
        }
        default:
            throw LogicException("Unmanaged case");
        }
    }

    static std::ostream& print_rdf(std::ostream& os, const GraphObject& graph_obj) {
        switch (graph_obj.type) {
        case GraphObjectType::STR_INLINED:
            return os << '"' << GraphObjectInterpreter::get<StringInlined>(graph_obj).id << '"';
        case GraphObjectType::STR_EXTERNAL:
            os << '"';
            string_manager.print(os, GraphObjectInterpreter::get<StringExternal>(graph_obj).external_id);
            return os << '"';
        case GraphObjectType::STR_TMP:
            return os << '"' << *GraphObjectInterpreter::get<StringTmp>(graph_obj).str << '"';
        case GraphObjectType::ANON:
            return os << "_:b" << GraphObjectInterpreter::get<AnonymousNode>(graph_obj).id;
        case GraphObjectType::NULL_OBJ:
            return os;
        case GraphObjectType::NOT_FOUND:
            return os << "NotFoundObj";
        case GraphObjectType::BOOL:
            return os << '"' << (GraphObjectInterpreter::get<bool>(graph_obj) ? "true" : "false") << "\"^^<http://www.w3.org/2001/XMLSchema#boolean>";
        case GraphObjectType::IRI_INLINED: {
            auto iri_inl = GraphObjectInterpreter::get<IriInlined>(graph_obj);

            return os << '<'
                      << rdf_model.catalog().prefixes[iri_inl.prefix_id]
                      << iri_inl.id 
                      << '>';
        }
        case GraphObjectType::IRI_EXTERNAL: {
            auto iri_ext = GraphObjectInterpreter::get<IriExternal>(graph_obj);

            uint8_t prefix_id = (iri_ext.external_id & 0x00FF'0000'0000'0000UL) >> 48;
            uint64_t iri_id = iri_ext.external_id & 0x0000'FFFF'FFFF'FFFFUL;

            os << '<'
               << rdf_model.catalog().prefixes[prefix_id];
            string_manager.print(os, iri_id);
            return os << '>';
        }
        case GraphObjectType::IRI_TMP:
            return os << '<'
                      << *GraphObjectInterpreter::get<IriTmp>(graph_obj).str
                      << '>';
        case GraphObjectType::LITERAL_DATATYPE_INLINED: {
            auto ld_inl = GraphObjectInterpreter::get<LiteralDatatypeInlined>(graph_obj);

            return os << '"' 
                      << ld_inl.id
                      << "\"^^<"
                      << rdf_model.catalog().datatypes[ld_inl.datatype_id]
                      << '>';
        }
        case GraphObjectType::LITERAL_DATATYPE_EXTERNAL: {
            auto ld_ext = GraphObjectInterpreter::get<LiteralDatatypeExternal>(graph_obj);

            uint16_t datatype_id = (ld_ext.external_id & 0x00FF'FF00'0000'0000UL) >> 40;
            uint64_t str_id      = ld_ext.external_id & 0x0000'00FF'FFFF'FFFFUL;

            os << '"';
            string_manager.print(os, str_id);
            return os << "\"^^<"
                      << rdf_model.catalog().datatypes[datatype_id]
                      << '>';
        }
        case GraphObjectType::LITERAL_DATATYPE_TMP: {
            auto ld_tmp = GraphObjectInterpreter::get<LiteralDatatypeTmp>(graph_obj);

            return os << '"' 
                      << ld_tmp.ld->str
                      << "\"^^<"
                      << ld_tmp.ld->datatype
                      << '>';
        }
        case GraphObjectType::LITERAL_LANGUAGE_INLINED: {
            auto ll_inl = GraphObjectInterpreter::get<LiteralLanguageInlined>(graph_obj);

            return os << '"' 
                      << ll_inl.id
                      << "\"@"
                      << rdf_model.catalog().languages[ll_inl.language_id];
        }
        case GraphObjectType::LITERAL_LANGUAGE_EXTERNAL: {
            auto ll_ext = GraphObjectInterpreter::get<LiteralLanguageExternal>(graph_obj);

            uint16_t language_id = (ll_ext.external_id & 0x00FF'FF00'0000'0000UL) >> 40;
            uint64_t str_id      = ll_ext.external_id & 0x0000'00FF'FFFF'FFFFUL;

            os << '"';
            string_manager.print(os, str_id);
            return os << "\"@"
                      << rdf_model.catalog().languages[language_id];
        }
        case GraphObjectType::LITERAL_LANGUAGE_TMP: {
            auto ll_tmp = GraphObjectInterpreter::get<LiteralLanguageTmp>(graph_obj);

            return os << '"' 
                      << ll_tmp.ll->str
                      << "\"@"
                      << ll_tmp.ll->language;
        }
        case GraphObjectType::DATETIME: {
            return os << '"'
                      << GraphObjectInterpreter::get<DateTime>(graph_obj).get_value_string()
                      << "\"^^<http://www.w3.org/2001/XMLSchema#dateTime>";            
        }
        case GraphObjectType::DECIMAL_INLINED: {
            return os << '"'
                      << GraphObjectInterpreter::get<DecimalInlined>(graph_obj).get_value_string()
                      << "\"^^<http://www.w3.org/2001/XMLSchema#decimal>";
        }
        case GraphObjectType::DECIMAL_EXTERNAL: {
            os << '"';
            string_manager.print(os, GraphObjectInterpreter::get<DecimalExternal>(graph_obj).external_id);
            return os << "\"^^<http://www.w3.org/2001/XMLSchema#decimal>";
        }
        case GraphObjectType::DECIMAL_TMP: {
            return os << '"'
                      << *GraphObjectInterpreter::get<DecimalTmp>(graph_obj).str
                      << "\"^^<http://www.w3.org/2001/XMLSchema#decimal>";
        }
        default:
            throw LogicException("Unmanaged case");
        }
    }


    static bool equal(const GraphObject& lhs, const GraphObject& rhs) {
        return lhs.type == rhs.type && lhs.encoded_value == rhs.encoded_value;
    }

    // returns negative number if lhs < rhs,
    // returns positive number if lhs > rhs
    // returns 0 if lhs == rhs
    static int compare(const GraphObject& lhs, const GraphObject& rhs) {
        if (lhs.type == rhs.type && lhs.encoded_value == rhs.encoded_value)
            return 0;

        bool lhs_is_string = false;
        bool rhs_is_string = false;

        bool lhs_is_numeric = false;
        bool rhs_is_numeric = false;

        std::unique_ptr<CharIter> lhs_string_iter;
        std::unique_ptr<CharIter> rhs_string_iter;

        double lhs_numeric;
        double rhs_numeric;

        switch (lhs.type) {
        case GraphObjectType::STR_INLINED:
        case GraphObjectType::NAMED_INLINED:
            lhs_string_iter = std::make_unique<StringInlineIter>(lhs.encoded_value);
            lhs_is_string   = true;
            break;
        case GraphObjectType::STR_EXTERNAL:
        case GraphObjectType::NAMED_EXTERNAL:
            lhs_string_iter = string_manager.get_char_iter(GraphObjectInterpreter::get<StringExternal>(lhs).external_id);
            lhs_is_string = true;
            break;
        case GraphObjectType::NAMED_TMP:
        case GraphObjectType::STR_TMP:
            lhs_string_iter = std::make_unique<StringTmpIter>(*GraphObjectInterpreter::get<StringTmp>(lhs).str);
            lhs_is_string   = true;
            break;
        case GraphObjectType::INT:
            lhs_is_numeric = true;
            lhs_numeric    = GraphObjectInterpreter::get<int64_t>(lhs);
            break;
        case GraphObjectType::FLOAT:
            lhs_is_numeric = true;
            lhs_numeric    = GraphObjectInterpreter::get<float>(lhs);
            break;
        // case GraphObjectType::EDGE:
        // case GraphObjectType::ANON:
        // case GraphObjectType::NULL_OBJ:
        // case GraphObjectType::NOT_FOUND:
        // case GraphObjectType::BOOL:
        // case GraphObjectType::PATH:
        default:
            break;
        }

        switch (rhs.type) {
        case GraphObjectType::STR_INLINED:
        case GraphObjectType::NAMED_INLINED:
            rhs_string_iter = std::make_unique<StringInlineIter>(rhs.encoded_value);
            rhs_is_string   = true;
            break;
        case GraphObjectType::STR_EXTERNAL:
        case GraphObjectType::NAMED_EXTERNAL:
            rhs_string_iter = string_manager.get_char_iter(GraphObjectInterpreter::get<StringExternal>(rhs).external_id);
            rhs_is_string = true;
            break;
        case GraphObjectType::NAMED_TMP:
        case GraphObjectType::STR_TMP:
            rhs_string_iter = std::make_unique<StringTmpIter>(*GraphObjectInterpreter::get<StringTmp>(rhs).str);
            rhs_is_string   = true;
            break;
        case GraphObjectType::INT:
            rhs_is_numeric = true;
            rhs_numeric    = GraphObjectInterpreter::get<int64_t>(rhs);
            break;
        case GraphObjectType::FLOAT:
            rhs_is_numeric = true;
            rhs_numeric    = GraphObjectInterpreter::get<float>(rhs);
            break;
        // case GraphObjectType::EDGE:
        // case GraphObjectType::ANON:
        // case GraphObjectType::NULL_OBJ:
        // case GraphObjectType::NOT_FOUND:
        // case GraphObjectType::BOOL:
        // case GraphObjectType::PATH:
        default:
            break;
        }

        if (lhs_is_string && rhs_is_string) {
            return StringManager::compare(*lhs_string_iter, *rhs_string_iter);
        } else if (lhs_is_numeric && rhs_is_numeric) {
            if (lhs_numeric < rhs_numeric)
                return -1;
            else if (lhs_numeric > rhs_numeric)
                return 1;
            else
                return 0;
        } else if (rhs.type == lhs.type) {
            switch (lhs.type) {
            // case GraphObjectType::NAMED_INLINED:
            // case GraphObjectType::NAMED_EXTERNAL:
            // case GraphObjectType::NAMED_TMP:
            // case GraphObjectType::STR_INLINED:
            // case GraphObjectType::STR_EXTERNAL:
            // case GraphObjectType::STR_TMP:
            // case GraphObjectType::INT:
            // case GraphObjectType::FLOAT:
            case GraphObjectType::NULL_OBJ:
                return 0;
            case GraphObjectType::NOT_FOUND:
                return 0;
            case GraphObjectType::EDGE:
                return GraphObjectInterpreter::get<Edge>(lhs).id - GraphObjectInterpreter::get<Edge>(lhs).id;
            case GraphObjectType::ANON:
                return GraphObjectInterpreter::get<AnonymousNode>(lhs).id
                     - GraphObjectInterpreter::get<AnonymousNode>(rhs).id;
            case GraphObjectType::BOOL:
                return GraphObjectInterpreter::get<bool>(lhs) - GraphObjectInterpreter::get<bool>(rhs);
            case GraphObjectType::PATH:
                return GraphObjectInterpreter::get<Path>(lhs).path_id - GraphObjectInterpreter::get<Path>(rhs).path_id;
            default:
                throw LogicException("Unmanaged case");
            }
        } else {
            return (lhs.type < rhs.type) ? -1 : 1;
        }
    }

    // returns negative number if lhs < rhs,
    // returns positive number if lhs > rhs
    // returns 0 if lhs == rhs
    static int compare_rdf(const GraphObject& lhs, const GraphObject& rhs) {
        if (lhs.type == rhs.type && lhs.encoded_value == rhs.encoded_value)
            return 0;
        switch(lhs.type) {
            // case GraphObjectType::NULL_OBJ:
            // case GraphObjectType::NOT_FOUND:
            // case GraphObjectType::IRI_INLINED:
            // case GraphObjectType::IRI_EXTERNAL:
            // case GraphObjectType::IRI_TMP:
            // case GraphObjectType::STRING_INLINED:
            // case GraphObjectType::STRING_EXTERNAL:
            // case GraphObjectType::STRING_TMP:
            // case GraphObjectType::LITERAL_DATATYPE_INLINED:
            // case GraphObjectType::LITERAL_DATATYPE_EXTERNAL:
            // case GraphObjectType::LITERAL_DATATYPE_TMP:
            // case GraphObjectType::LITERAL_LANGUAGE_INLINED:
            // case GraphObjectType::LITERAL_LANGUAGE_EXTERNAL:
            // case GraphObjectType::LITERAL_LANGUAGE_TMP:
            // case GraphObjectType::DATETIME:
            // case GraphObjectType::DECIMAL_INLINED:
            // case GraphObjectType::DECIMAL_EXTERNAL:
            // case GraphObjectType::DECIMAL_TMP:
            // case GraphObjectType::BOOL:
            // case GraphObjectType::PATH:
            default:
                throw LogicException("Unmanaged case");
        }
        switch(rhs.type) {
            // case GraphObjectType::NULL_OBJ:
            // case GraphObjectType::NOT_FOUND:
            // case GraphObjectType::IRI_INLINED:
            // case GraphObjectType::IRI_EXTERNAL:
            // case GraphObjectType::IRI_TMP:
            // case GraphObjectType::STRING_INLINED:
            // case GraphObjectType::STRING_EXTERNAL:
            // case GraphObjectType::STRING_TMP:
            // case GraphObjectType::LITERAL_DATATYPE_INLINED:
            // case GraphObjectType::LITERAL_DATATYPE_EXTERNAL:
            // case GraphObjectType::LITERAL_DATATYPE_TMP:
            // case GraphObjectType::LITERAL_LANGUAGE_INLINED:
            // case GraphObjectType::LITERAL_LANGUAGE_EXTERNAL:
            // case GraphObjectType::LITERAL_LANGUAGE_TMP:
            // case GraphObjectType::DATETIME:
            // case GraphObjectType::DECIMAL_INLINED:
            // case GraphObjectType::DECIMAL_EXTERNAL:
            // case GraphObjectType::DECIMAL_TMP:
            // case GraphObjectType::BOOL:
            // case GraphObjectType::PATH:
            default:
                throw LogicException("Unmanaged case");
        }
        if (lhs.type == lhs.type) {
            switch (lhs.type) {
            // case GraphObjectType::IRI_INLINED:
            // case GraphObjectType::IRI_EXTERNAL:
            // case GraphObjectType::IRI_TMP:
            // case GraphObjectType::STRING_INLINED:
            // case GraphObjectType::STRING_EXTERNAL:
            // case GraphObjectType::STRING_TMP:
            // case GraphObjectType::LITERAL_DATATYPE_INLINED:
            // case GraphObjectType::LITERAL_DATATYPE_EXTERNAL:
            // case GraphObjectType::LITERAL_DATATYPE_TMP:
            // case GraphObjectType::LITERAL_LANGUAGE_INLINED:
            // case GraphObjectType::LITERAL_LANGUAGE_EXTERNAL:
            // case GraphObjectType::LITERAL_LANGUAGE_TMP:
            // case GraphObjectType::DECIMAL_INLINED:
            // case GraphObjectType::DECIMAL_EXTERNAL:
            // case GraphObjectType::DECIMAL_TMP:
            // case GraphObjectType::PATH:
            case GraphObjectType::NULL_OBJ:
                return 0;
            case GraphObjectType::NOT_FOUND:
                return 0;
            case GraphObjectType::ANON:
                return GraphObjectInterpreter::get<AnonymousNode>(lhs).id
                     - GraphObjectInterpreter::get<AnonymousNode>(rhs).id;
            case GraphObjectType::BOOL:
                return GraphObjectInterpreter::get<bool>(lhs) - GraphObjectInterpreter::get<bool>(rhs);
            case GraphObjectType::DATETIME: {
                const auto& lhs_id = GraphObjectInterpreter::get<DateTime>(lhs).id;
                const auto& rhs_id = GraphObjectInterpreter::get<DateTime>(rhs).id;

                // Check sign bit
                uint64_t sign_mask = 1ULL << 55;
                int64_t  sign_diff = (lhs_id & sign_mask) - (rhs_id & sign_mask);
                if (sign_diff != 0) {
                    return sign_diff > 0 ? -1 : 1;
                }
                // From now on, both numbers have the same sign
                int64_t diff = 0;
                // Check precision bit
                uint64_t precision_mask = 1ULL << 54;
                int64_t  precision_diff = (lhs_id & precision_mask) - (rhs_id & precision_mask);
                if (precision_diff != 0) {
                    diff = precision_diff > 0 ? 1 : -1;
                }
                // Handle same precision
                else {
                    uint64_t datetime_mask = 0x00'3F'FFFF'FFFF'FFFF;
                    int64_t  datetime_diff = (lhs_id & datetime_mask) - (rhs_id & datetime_mask);
                    diff = datetime_diff > 0 ? 1 : -1;
                }
                // Flip if the result if both signs were negative
                if (lhs_id & sign_mask) {
                    return -diff;
                } else {
                    return diff;
                }
            }
            default:
                throw LogicException("Unmanaged case");
            }
        }
        else {
            return (lhs.type < rhs.type) ? -1 : 1;
        }
    }

    static GraphObject sum(const GraphObject& lhs, const GraphObject& rhs) {
        if (lhs.type == GraphObjectType::INT) {
            if (rhs.type == GraphObjectType::INT) {
                return GraphObjectFactory::make_int(GraphObjectInterpreter::get<int64_t>(lhs)
                                                    + GraphObjectInterpreter::get<int64_t>(rhs));
            } else if (rhs.type == GraphObjectType::FLOAT) {
                return GraphObjectFactory::make_float(GraphObjectInterpreter::get<int64_t>(lhs)
                                                      + GraphObjectInterpreter::get<float>(rhs));
            }
        } else if (lhs.type == GraphObjectType::FLOAT) {
            if (rhs.type == GraphObjectType::INT) {
                return GraphObjectFactory::make_float(GraphObjectInterpreter::get<float>(lhs)
                                                      + GraphObjectInterpreter::get<int64_t>(rhs));
            } else if (rhs.type == GraphObjectType::FLOAT) {
                return GraphObjectFactory::make_float(GraphObjectInterpreter::get<float>(lhs)
                                                      + GraphObjectInterpreter::get<float>(rhs));
            }
        }
        return GraphObjectFactory::make_null();
    }

    static GraphObject minus(const GraphObject& lhs, const GraphObject& rhs) {
        if (lhs.type == GraphObjectType::INT) {
            if (rhs.type == GraphObjectType::INT) {
                return GraphObjectFactory::make_int(GraphObjectInterpreter::get<int64_t>(lhs)
                                                    - GraphObjectInterpreter::get<int64_t>(rhs));
            } else if (rhs.type == GraphObjectType::FLOAT) {
                return GraphObjectFactory::make_float(GraphObjectInterpreter::get<int64_t>(lhs)
                                                      - GraphObjectInterpreter::get<float>(rhs));
            }
        } else if (lhs.type == GraphObjectType::FLOAT) {
            if (rhs.type == GraphObjectType::INT) {
                return GraphObjectFactory::make_float(GraphObjectInterpreter::get<float>(lhs)
                                                      - GraphObjectInterpreter::get<int64_t>(rhs));
            } else if (rhs.type == GraphObjectType::FLOAT) {
                return GraphObjectFactory::make_float(GraphObjectInterpreter::get<float>(lhs)
                                                      - GraphObjectInterpreter::get<float>(rhs));
            }
        }
        return GraphObjectFactory::make_null();
    }

    static GraphObject multiply(const GraphObject& lhs, const GraphObject& rhs) {
        if (lhs.type == GraphObjectType::INT) {
            if (rhs.type == GraphObjectType::INT) {
                return GraphObjectFactory::make_int(GraphObjectInterpreter::get<int64_t>(lhs)
                                                    * GraphObjectInterpreter::get<int64_t>(rhs));
            } else if (rhs.type == GraphObjectType::FLOAT) {
                return GraphObjectFactory::make_float(GraphObjectInterpreter::get<int64_t>(lhs)
                                                      * GraphObjectInterpreter::get<float>(rhs));
            }
        } else if (lhs.type == GraphObjectType::FLOAT) {
            if (rhs.type == GraphObjectType::INT) {
                return GraphObjectFactory::make_float(GraphObjectInterpreter::get<float>(lhs)
                                                      * GraphObjectInterpreter::get<int64_t>(rhs));
            } else if (rhs.type == GraphObjectType::FLOAT) {
                return GraphObjectFactory::make_float(GraphObjectInterpreter::get<float>(lhs)
                                                      * GraphObjectInterpreter::get<float>(rhs));
            }
        }
        return GraphObjectFactory::make_null();
    }

    static GraphObject divide(const GraphObject& lhs, const GraphObject& rhs) {
        if (rhs.type == GraphObjectType::INT) {
            // avoid division by 0
            if (GraphObjectInterpreter::get<int64_t>(rhs) == 0)
                return GraphObjectFactory::make_null();
            if (lhs.type == GraphObjectType::INT) {
                return GraphObjectFactory::make_int(GraphObjectInterpreter::get<int64_t>(lhs)
                                                    / GraphObjectInterpreter::get<int64_t>(rhs));
            } else if (lhs.type == GraphObjectType::FLOAT) {
                return GraphObjectFactory::make_float(GraphObjectInterpreter::get<float>(lhs)
                                                      / GraphObjectInterpreter::get<int64_t>(rhs));
            }
        } else if (rhs.type == GraphObjectType::FLOAT) {
            // avoid division by 0
            if (GraphObjectInterpreter::get<float>(rhs) == 0.0f)
                return GraphObjectFactory::make_null();
            if (lhs.type == GraphObjectType::INT) {
                return GraphObjectFactory::make_float(GraphObjectInterpreter::get<int64_t>(lhs)
                                                      / GraphObjectInterpreter::get<float>(rhs));
            } else if (lhs.type == GraphObjectType::FLOAT) {
                return GraphObjectFactory::make_float(GraphObjectInterpreter::get<float>(lhs)
                                                      / GraphObjectInterpreter::get<float>(rhs));
            }
        }
        return GraphObjectFactory::make_null();
    }

    static GraphObject modulo(const GraphObject& lhs, const GraphObject& rhs) {
        if (lhs.type == GraphObjectType::INT && rhs.type == GraphObjectType::INT
            && GraphObjectInterpreter::get<int64_t>(rhs) != 0)
        {
            return GraphObjectFactory::make_int(GraphObjectInterpreter::get<int64_t>(lhs)
                                                % GraphObjectInterpreter::get<int64_t>(rhs));
        }
        return GraphObjectFactory::make_null();
    }
};
