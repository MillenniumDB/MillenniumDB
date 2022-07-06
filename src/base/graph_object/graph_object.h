#pragma once

#include <memory>
#include <ostream>
#include <type_traits>

#include "base/graph_object/anonymous_node.h"
#include "base/graph_object/edge.h"
#include "base/graph_object/named_node_external.h"
#include "base/graph_object/named_node_inlined.h"
#include "base/graph_object/named_node_tmp.h"
#include "base/graph_object/not_found_object.h"
#include "base/graph_object/null_graph_object.h"
#include "base/graph_object/path.h"
#include "base/graph_object/string_external.h"
#include "base/graph_object/string_inlined.h"
#include "base/graph_object/string_tmp.h"

enum class GraphObjectType {
    NULL_OBJ,
    NOT_FOUND,
    NAMED_INLINED,
    NAMED_EXTERNAL,
    NAMED_TMP,
    EDGE,
    ANON,
    PATH,
    STR_INLINED,
    STR_EXTERNAL,
    STR_TMP,
    BOOL,
    INT,
    FLOAT,
};

union GraphObjectUnion {

    GraphObjectUnion() { }
    GraphObjectUnion(NamedNodeExternal n) : named_external(n) { }
    GraphObjectUnion(NamedNodeInlined n) : named_inlined(n) { }
    GraphObjectUnion(NamedNodeTmp n) : named_tmp(n) { }
    GraphObjectUnion(Edge n) : edge(n) { }
    GraphObjectUnion(AnonymousNode n) : anon(n) { }
    GraphObjectUnion(Path n) : path(n) { }
    GraphObjectUnion(StringInlined n) : str_inlined(n) { }
    GraphObjectUnion(StringExternal n) : str_external(n) { }
    GraphObjectUnion(StringTmp n) : str_tmp(n) { }
    GraphObjectUnion(bool n) : b(n) { }
    GraphObjectUnion(int64_t n) : i(n) { }
    GraphObjectUnion(float n) : f(n) { }

    NamedNodeInlined  named_inlined;
    NamedNodeExternal named_external;
    NamedNodeTmp      named_tmp;
    Edge              edge;
    AnonymousNode     anon;
    Path              path;
    StringInlined     str_inlined;
    StringExternal    str_external;
    StringTmp         str_tmp;
    bool              b;
    int64_t           i;
    float             f;
};

class GraphObject {
public:
    GraphObjectUnion value;

    GraphObjectType type;

    GraphObject() : type (GraphObjectType::NULL_OBJ) { }

    static GraphObject make_named_node_external(uint64_t external_id) {
        return GraphObject(GraphObjectUnion(NamedNodeExternal(external_id)), GraphObjectType::NAMED_EXTERNAL);
    }

    static GraphObject make_named_node_inlined(const char c[8]) {
        return GraphObject(GraphObjectUnion(NamedNodeInlined(c)), GraphObjectType::NAMED_INLINED);
    }

    static GraphObject make_named_node_tmp(const std::string& tmp) {
        return GraphObject(GraphObjectUnion(NamedNodeTmp(tmp)), GraphObjectType::NAMED_TMP);
    }

    static GraphObject make_anonymous(uint64_t id) {
        return GraphObject(GraphObjectUnion(AnonymousNode(id)), GraphObjectType::ANON);
    }

    static GraphObject make_edge(uint64_t id) {
        return GraphObject(GraphObjectUnion(Edge(id)), GraphObjectType::EDGE);
    }

    static GraphObject make_int(int64_t i) {
        return GraphObject(GraphObjectUnion(i), GraphObjectType::INT);
    }

    static GraphObject make_float(float f) {
        return GraphObject(GraphObjectUnion(f), GraphObjectType::FLOAT);
    }

    static GraphObject make_bool(bool b) {
        return GraphObject(GraphObjectUnion(b), GraphObjectType::BOOL);
    }

    static GraphObject make_null() {
        return GraphObject();
    }

    static GraphObject make_not_found() {
        return GraphObject(GraphObjectUnion(), GraphObjectType::NOT_FOUND);
    }

    static GraphObject make_string(const std::string& str) {
        if (str.size() < 8) {
            char c[8];
            std::size_t i = 0;
            for (; i < str.size(); ++i) {
                c[i] = str[i];
            }
            for (; i < 8; ++i) {
                c[i] = '\0';
            }
            return GraphObject::make_string_inlined(c);
        } else {
            return GraphObject::make_string_tmp(str);
        }
    }

    static GraphObject make_named_node(const std::string& str) {
        if (str.size() < 8) {
            char c[8];
            std::size_t i = 0;
            for (; i < str.size(); ++i) {
                c[i] = str[i];
            }
            for (; i < 8; ++i) {
                c[i] = '\0';
            }
            return GraphObject::make_named_node_inlined(c);
        } else {
            return GraphObject::make_named_node_tmp(str);
        }
    }

    static GraphObject make_string_external(uint64_t external_id) {
        return GraphObject(GraphObjectUnion(StringExternal(external_id)), GraphObjectType::STR_EXTERNAL);
    }

    // must receive an array of size 8, terminating in '\0'
    static GraphObject make_string_inlined(const char* str) {
        return GraphObject(GraphObjectUnion(StringInlined(str)), GraphObjectType::STR_INLINED);
    }

    static GraphObject make_string_tmp(const std::string& str) {
        return GraphObject(GraphObjectUnion(StringTmp(str)), GraphObjectType::STR_TMP);
    }

    static GraphObject make_path(uint64_t path_id) {
        return GraphObject(GraphObjectUnion(Path(path_id)), GraphObjectType::PATH);
    }

    friend std::ostream& operator<<(std::ostream& os, const GraphObject& graph_obj) {
        switch (graph_obj.type) {
            case GraphObjectType::NAMED_INLINED:
                os << graph_obj.value.named_inlined.id;
                break;
            case GraphObjectType::NAMED_EXTERNAL:
                graph_obj.value.named_external.print(os);
                break;
            case GraphObjectType::NAMED_TMP:
                os << *graph_obj.value.named_tmp.name;
                break;
            case GraphObjectType::STR_INLINED:
                os << '"';
                os << graph_obj.value.str_inlined.id;
                os << '"';
                break;
            case GraphObjectType::STR_EXTERNAL:
                os << '"';
                graph_obj.value.str_external.print(os);
                os << '"';
                break;
            case GraphObjectType::STR_TMP:
                os << '"';
                os << *graph_obj.value.str_tmp.str;
                os << '"';
                break;
            case GraphObjectType::EDGE:
                os << "_e" << graph_obj.value.edge.id;
                break;
            case GraphObjectType::ANON:
                os << "_a" << graph_obj.value.anon.id;
                break;
            case GraphObjectType::NULL_OBJ:
                os << "null";
                break;
            case GraphObjectType::NOT_FOUND:
                os << "NotFoundObj";
                break;
            case GraphObjectType::INT:
                os << graph_obj.value.i;
                break;
            case GraphObjectType::FLOAT:
                os << graph_obj.value.f;
                break;
            case GraphObjectType::BOOL:
                os << (graph_obj.value.b ? "true" : "false");
                break;
            case GraphObjectType::PATH:
                graph_obj.value.path.path_printer->print(os, graph_obj.value.path.path_id);
                break;
        }
        return os;
    }

    inline bool operator==(const GraphObject& rhs) const noexcept {
        if (this->type != rhs.type) {
            return false;
        }
        switch (this->type) {
            case GraphObjectType::NAMED_INLINED:
                return this->value.named_inlined == rhs.value.named_inlined;
            case GraphObjectType::NAMED_EXTERNAL:
                return this->value.named_external == rhs.value.named_external;
            case GraphObjectType::NAMED_TMP:
                return this->value.named_tmp == rhs.value.named_tmp;
            case GraphObjectType::STR_INLINED:
                return this->value.str_inlined == rhs.value.str_inlined;
            case GraphObjectType::STR_EXTERNAL:
                return this->value.str_external == rhs.value.str_external;
            case GraphObjectType::STR_TMP:
                return this->value.str_tmp == rhs.value.str_tmp;
            case GraphObjectType::EDGE:
                return this->value.edge == rhs.value.edge;
            case GraphObjectType::ANON:
                return this->value.anon == rhs.value.anon;
            case GraphObjectType::NULL_OBJ:
                return true;
            case GraphObjectType::NOT_FOUND:
                return true;
            case GraphObjectType::INT:
                return this->value.i == rhs.value.i;
            case GraphObjectType::FLOAT:
                return this->value.f == rhs.value.f;
            case GraphObjectType::BOOL:
                return this->value.b == rhs.value.b;
            case GraphObjectType::PATH:
                return this->value.path == rhs.value.path;
        }
        // unreachable
        return false;
    }

    inline bool operator!=(const GraphObject& rhs) const noexcept {
        return !this->operator==(rhs);
    }

    inline bool operator<=(const GraphObject& rhs) const noexcept {
        if (this->type != rhs.type) {
            switch (this->type) { // TODO: support comparison with temporal strings?
                case GraphObjectType::NAMED_INLINED:
                    if (rhs.type == GraphObjectType::NAMED_EXTERNAL) {
                        return strcmp(this->value.named_inlined.id,
                                      rhs.value.named_external.to_string().c_str()) // TODO: ineficient
                               <= 0;
                    }
                    break;
                case GraphObjectType::NAMED_EXTERNAL:
                    if (rhs.type == GraphObjectType::NAMED_INLINED) {
                        return strcmp(this->value.named_external.to_string().c_str(),
                                      rhs.value.named_inlined.id) // TODO: ineficient
                               <= 0;
                    }
                    break;
                case GraphObjectType::STR_INLINED:
                    if (rhs.type == GraphObjectType::STR_EXTERNAL) {
                        return strcmp(this->value.str_inlined.id,
                                      rhs.value.str_external.to_string().c_str()) // TODO: ineficient
                               <= 0;
                    }
                    break;
                case GraphObjectType::STR_EXTERNAL:
                    if (rhs.type == GraphObjectType::STR_INLINED) {
                        return strcmp(this->value.str_external.to_string().c_str(),
                                      rhs.value.str_inlined.id) // TODO: ineficient
                               <= 0;
                    }
                    break;
                case GraphObjectType::INT:
                    if (rhs.type == GraphObjectType::FLOAT) {
                        return this->value.i <= rhs.value.f;
                    }
                    break;
                case GraphObjectType::FLOAT:
                    if (rhs.type == GraphObjectType::INT) {
                        return this->value.f <= rhs.value.i;
                    }
                    break;

                case GraphObjectType::NAMED_TMP:
                case GraphObjectType::STR_TMP:
                case GraphObjectType::EDGE:
                case GraphObjectType::ANON:
                case GraphObjectType::NULL_OBJ:
                case GraphObjectType::NOT_FOUND:
                case GraphObjectType::BOOL:
                case GraphObjectType::PATH:
                // default:
                    break;
            }
            return this->type < rhs.type;
        }
        switch (this->type) {
            case GraphObjectType::NAMED_INLINED:
                return this->value.named_inlined <= rhs.value.named_inlined;
            case GraphObjectType::NAMED_EXTERNAL:
                return this->value.named_external <= rhs.value.named_external;
            case GraphObjectType::NAMED_TMP:
                return this->value.named_tmp <= rhs.value.named_tmp;
            case GraphObjectType::STR_INLINED:
                return this->value.str_inlined <= rhs.value.str_inlined;
            case GraphObjectType::STR_EXTERNAL:
                return this->value.str_external <= rhs.value.str_external;
            case GraphObjectType::STR_TMP:
                return this->value.str_tmp <= rhs.value.str_tmp;
            case GraphObjectType::EDGE:
                return this->value.edge <= rhs.value.edge;
            case GraphObjectType::ANON:
                return this->value.anon <= rhs.value.anon;
            case GraphObjectType::NULL_OBJ:
                return true;
            case GraphObjectType::NOT_FOUND:
                return true;
            case GraphObjectType::INT:
                return this->value.i <= rhs.value.i;
            case GraphObjectType::FLOAT:
                return this->value.f <= rhs.value.f;
            case GraphObjectType::BOOL:
                return this->value.b <= rhs.value.b;
            case GraphObjectType::PATH:
                return this->value.path <= rhs.value.path;
        }
        // unreachable
        return false;
    }

    inline bool operator<(const GraphObject& rhs) const noexcept {
        if (this->type != rhs.type) {
            switch (this->type) { // TODO: support comparison with temporal strings?
                case GraphObjectType::NAMED_INLINED:
                    if (rhs.type == GraphObjectType::NAMED_EXTERNAL) {
                        return strcmp(this->value.named_inlined.id,
                                      rhs.value.named_external.to_string().c_str()) // TODO: ineficient
                               < 0;
                    }
                    break;
                case GraphObjectType::NAMED_EXTERNAL:
                    if (rhs.type == GraphObjectType::NAMED_INLINED) {
                        return strcmp(this->value.named_external.to_string().c_str(),
                                      rhs.value.named_inlined.id) // TODO: ineficient
                               < 0;
                    }
                    break;
                case GraphObjectType::STR_INLINED:
                    if (rhs.type == GraphObjectType::STR_EXTERNAL) {
                        return strcmp(this->value.str_inlined.id,
                                      rhs.value.str_external.to_string().c_str()) // TODO: ineficient
                               < 0;
                    }
                    break;
                case GraphObjectType::STR_EXTERNAL:
                    if (rhs.type == GraphObjectType::STR_INLINED) {
                        return strcmp(this->value.str_external.to_string().c_str(),
                                      rhs.value.str_inlined.id) // TODO: ineficient
                               < 0;
                    }
                    break;
                case GraphObjectType::INT:
                    if (rhs.type == GraphObjectType::FLOAT) {
                        return this->value.i < rhs.value.f;
                    }
                    break;
                case GraphObjectType::FLOAT:
                    if (rhs.type == GraphObjectType::INT) {
                        return this->value.f < rhs.value.i;
                    }
                    break;

                case GraphObjectType::NAMED_TMP:
                case GraphObjectType::STR_TMP:
                case GraphObjectType::EDGE:
                case GraphObjectType::ANON:
                case GraphObjectType::NULL_OBJ:
                case GraphObjectType::NOT_FOUND:
                case GraphObjectType::BOOL:
                case GraphObjectType::PATH:
                // default:
                    break;
            }
            return this->type < rhs.type;
        }
        switch (this->type) {
            case GraphObjectType::NAMED_INLINED:
                return this->value.named_inlined < rhs.value.named_inlined;
            case GraphObjectType::NAMED_EXTERNAL:
                return this->value.named_external < rhs.value.named_external;
            case GraphObjectType::NAMED_TMP:
                return this->value.named_tmp < rhs.value.named_tmp;
            case GraphObjectType::STR_INLINED:
                return this->value.str_inlined < rhs.value.str_inlined;
            case GraphObjectType::STR_EXTERNAL:
                return this->value.str_external < rhs.value.str_external;
            case GraphObjectType::STR_TMP:
                return this->value.str_tmp < rhs.value.str_tmp;
            case GraphObjectType::EDGE:
                return this->value.edge < rhs.value.edge;
            case GraphObjectType::ANON:
                return this->value.anon < rhs.value.anon;
            case GraphObjectType::NULL_OBJ:
                return false;
            case GraphObjectType::NOT_FOUND:
                return false;
            case GraphObjectType::INT:
                return this->value.i < rhs.value.i;
            case GraphObjectType::FLOAT:
                return this->value.f < rhs.value.f;
            case GraphObjectType::BOOL:
                return this->value.b < rhs.value.b;
            case GraphObjectType::PATH:
                return this->value.path < rhs.value.path;
        }
        // unreachable
        return false;
    }

    inline bool operator>=(const GraphObject& rhs) const noexcept {
        return !this->operator<(rhs);
    }

    inline bool operator>(const GraphObject& rhs) const noexcept {
        return !this->operator<=(rhs);
    }

    inline GraphObject operator+(const GraphObject& rhs) const noexcept {
        if (this->type == GraphObjectType::INT) {
            if (rhs.type == GraphObjectType::INT) {
                return GraphObject::make_int(this->value.i + rhs.value.i);
            } else if (rhs.type == GraphObjectType::FLOAT) {
                return GraphObject::make_float((float)this->value.i + rhs.value.f);
            }
        } else if (this->type == GraphObjectType::FLOAT) {
            if (rhs.type == GraphObjectType::INT) {
                return GraphObject::make_float(this->value.f + (float)rhs.value.i);
            } else if (rhs.type == GraphObjectType::FLOAT) {
                return GraphObject::make_float(this->value.f + rhs.value.f);
            }
        }
        return GraphObject::make_null();
    }

    inline GraphObject operator-(const GraphObject& rhs) const noexcept {
        if (this->type == GraphObjectType::INT) {
            if (rhs.type == GraphObjectType::INT) {
                return GraphObject::make_int(this->value.i - rhs.value.i);
            } else if (rhs.type == GraphObjectType::FLOAT) {
                return GraphObject::make_float((float)this->value.i - rhs.value.f);
            }
        } else if (this->type == GraphObjectType::FLOAT) {
            if (rhs.type == GraphObjectType::INT) {
                return GraphObject::make_float(this->value.f - (float)rhs.value.i);
            } else if (rhs.type == GraphObjectType::FLOAT) {
                return GraphObject::make_float(this->value.f - rhs.value.f);
            }
        }
        return GraphObject::make_null();
    }

    inline GraphObject operator*(const GraphObject& rhs) const noexcept {
        if (this->type == GraphObjectType::INT) {
            if (rhs.type == GraphObjectType::INT) {
                return GraphObject::make_int(this->value.i * rhs.value.i);
            } else if (rhs.type == GraphObjectType::FLOAT) {
                return GraphObject::make_float((float)this->value.i * rhs.value.f);
            }
        } else if (this->type == GraphObjectType::FLOAT) {
            if (rhs.type == GraphObjectType::INT) {
                return GraphObject::make_float(this->value.f * (float)rhs.value.i);
            } else if (rhs.type == GraphObjectType::FLOAT) {
                return GraphObject::make_float(this->value.f * rhs.value.f);
            }
        }
        return GraphObject::make_null();
    }

    inline GraphObject operator/(const GraphObject& rhs) const noexcept {
        if (rhs.type == GraphObjectType::INT) {
            if (rhs.value.i == 0) {
                // avoid division by 0
                return GraphObject::make_null();
            }
            if (this->type == GraphObjectType::INT) {
                return GraphObject::make_float((float)this->value.i / (float)rhs.value.i);
            } else if (this->type == GraphObjectType::FLOAT) {
                return GraphObject::make_float(this->value.f / (float)rhs.value.i);
            }
        } else if (rhs.type == GraphObjectType::FLOAT) {
            if (rhs.value.f == 0) {
                // avoid division by 0
                return GraphObject::make_null();
            }
            if (this->type == GraphObjectType::INT) {
                return GraphObject::make_float((float)this->value.i / rhs.value.f);
            } else if (this->type == GraphObjectType::FLOAT) {
                return GraphObject::make_float(this->value.f / rhs.value.f);
            }
        }
        return GraphObject::make_null();
    }

    inline GraphObject operator%(const GraphObject& rhs) const noexcept {
        if (this->type == GraphObjectType::INT
            && rhs.type == GraphObjectType::INT
            && rhs.value.i != 0)
        {
            return GraphObject::make_float(this->value.i / rhs.value.i);
        }
        return GraphObject::make_null();
    }

private:
    GraphObject(GraphObjectUnion value, GraphObjectType type) : value(value), type(type) { }
};

static_assert(std::is_trivially_copyable<GraphObject>::value);

static_assert(std::is_trivially_move_constructible<GraphObject>::value);

static_assert(sizeof(GraphObject) <= 16,
              "GraphObject size should be small, if it needs to increase the size avoid copies");
