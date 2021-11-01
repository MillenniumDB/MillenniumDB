#ifndef BASE__OBJECT_TYPE_H_
#define BASE__OBJECT_TYPE_H_

#include <ostream>
#include <memory>
#include <string>
#include <type_traits>
#include <variant>

#include "base/graph/anonymous_node.h"
#include "base/graph/identifiable_inlined.h"
#include "base/graph/identifiable_external.h"
#include "base/graph/edge.h"
#include "base/graph/string_inlined.h"
#include "base/graph/string_external.h"
#include "base/graph/path.h"

struct NullGraphObject {
    inline bool operator==(const NullGraphObject&) const noexcept {
        return true;
    }

    inline bool operator!=(const NullGraphObject&) const noexcept {
        return false;
    }

    inline bool operator<=(const NullGraphObject&) const noexcept {
        return true;
    }

    inline bool operator>=(const NullGraphObject&) const noexcept {
        return true;
    }

    inline bool operator<(const NullGraphObject&) const noexcept {
        return false;
    }

    inline bool operator>(const NullGraphObject&) const noexcept {
        return false;
    }
};

struct NotFoundObject {
    inline bool operator==(const NotFoundObject&) const noexcept {
        return false;
    }

    inline bool operator!=(const NotFoundObject&) const noexcept {
        return false;
    }

    inline bool operator<=(const NotFoundObject&) const noexcept {
        return false;
    }

    inline bool operator>=(const NotFoundObject&) const noexcept {
        return false;
    }

    inline bool operator<(const NotFoundObject&) const noexcept {
        return false;
    }

    inline bool operator>(const NotFoundObject&) const noexcept {
        return false;
    }
};


// int64_t and float needs to be contigous
// StringInlined and StringExternal needs to be contigous
// IdentifiableInlined and IdentifiableExternal needs to be contigous
using GraphObjectVariant = std::variant<
        NullGraphObject,
        NotFoundObject,
        IdentifiableInlined,
        IdentifiableExternal,
        Edge,
        AnonymousNode,
        Path,
        bool,
        StringInlined,
        StringExternal,
        int64_t,
        float>;

struct GraphObjectOstreamVisitor {
    std::ostream& os;

    GraphObjectOstreamVisitor(std::ostream& os) :
        os (os) { }

    void operator()(const IdentifiableInlined& i)   const { os << i.id; }
    void operator()(const IdentifiableExternal& i)  const { os << i.id; }
    void operator()(const Edge& e)                  const { os << "_c" << e.id; }
    void operator()(const AnonymousNode& a)         const { os << "_a" << a.id; }
    void operator()(const StringInlined& s)         const { os << '"' << s.id << '"'; }
    void operator()(const StringExternal& s)        const { os << '"' << s.id << '"'; }
    void operator()(const NullGraphObject&)         const { os << "null"; }
    void operator()(const NotFoundObject&)          const { os << "NotFoundObj"; }
    void operator()(const int64_t n)                const { os << n; }
    void operator()(const float f)                  const { os << f; }

    void operator()(const bool b) const {
        if (b) os << "true";
        else os << "false";
    }

    void operator()(const Path& p) const {
        p.path_printer->print(os, p.path_id);
    }
};


class GraphObject {
public:
    GraphObjectVariant value;

    GraphObject() :
        value (NullGraphObject()) { }

    // GraphObject(const GraphObject& graph_object) :
    //     value (graph_object.value) { }

    static GraphObject make_identifiable_external(const char* str) {
        IdentifiableExternal string_external{ str };
        return GraphObject(string_external);
    }

    static GraphObject make_identifiable_inlined(const char c[8]) {
        IdentifiableInlined idetifiable_inlined{ c };
        return GraphObject(idetifiable_inlined);
    }

    static GraphObject make_anonymous(const uint64_t id) {
        return GraphObject(AnonymousNode(id));
    }

    static GraphObject make_edge(const uint64_t id) {
        return GraphObject(Edge(id));
    }

    static GraphObject make_int(const int64_t i) {
        return GraphObject(i);
    }

    static GraphObject make_float(const float f) {
        return GraphObject(f);
    }

    static GraphObject make_bool(const bool b) {
        return GraphObject(b);
    }

    static GraphObject make_null() {
        return GraphObject(NullGraphObject());
    }

    static GraphObject make_not_found() {
        return GraphObject(NotFoundObject());
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
            return GraphObject::make_string_external(const_cast<char*>(str.c_str()));
        }
    }


    static GraphObject make_string(const char* str) {
        std::size_t size = strlen(str);
        if (size < 8) {
            char c[8];
            std::size_t i = 0;
            for (; i < size; ++i) {
                c[i] = str[i];
            }
            for (; i < 8; ++i) {
                c[i] = '\0';
            }
            return GraphObject::make_string_inlined(c);
        } else {
            return GraphObject::make_string_external(const_cast<char*>(str));
        }
    }

    static GraphObject make_identifiable(const std::string& str) {
        if (str.size() < 8) {
            char c[8];
            std::size_t i = 0;
            for (; i < str.size(); ++i) {
                c[i] = str[i];
            }
            for (; i < 8; ++i) {
                c[i] = '\0';
            }
            return GraphObject::make_identifiable_inlined(c);
        } else {
            return GraphObject::make_identifiable_external(const_cast<char*>(str.c_str()));
        }
    }

    static GraphObject make_string_external(const char* str) {
        StringExternal string_external{ str };
        return GraphObject(string_external);
    }

    // must receive an array of size 8, terminating in '\0'
    static GraphObject make_string_inlined(const char* str) {
        StringInlined string_inlined{ str };
        return GraphObject(string_inlined);
    }

    static GraphObject make_path(uint64_t path_id) {
        Path p{ path_id };
        return GraphObject(p);
    }

    friend std::ostream& operator<<(std::ostream& os, const GraphObject& graph_obj) {
        std::visit(GraphObjectOstreamVisitor{os}, graph_obj.value);
        return os;
    }

    // inline void operator=(const GraphObject& other) noexcept {
    //     this->value = other.value;
    // }

    inline bool operator==(const GraphObject& rhs) const noexcept {
        return this->value == rhs.value;
    }

    inline bool operator!=(const GraphObject& rhs) const noexcept {
        return this->value != rhs.value;
    }

    inline bool operator<=(const GraphObject& rhs) const noexcept {
        if (std::holds_alternative<IdentifiableExternal>(this->value)) {
            if (std::holds_alternative<IdentifiableInlined>(rhs.value)) {
                return strcmp(
                    std::get<IdentifiableExternal>(this->value).id,
                    std::get<IdentifiableInlined>(rhs.value).id
                ) <= 0;
            }
        }

        else if (std::holds_alternative<IdentifiableInlined>(this->value)) {
            if (std::holds_alternative<IdentifiableExternal>(rhs.value)) {
                return strcmp(
                    std::get<IdentifiableInlined>(this->value).id,
                    std::get<IdentifiableExternal>(rhs.value).id
                ) <= 0;
            }
        }

        else if (std::holds_alternative<StringExternal>(this->value)) {
            if (std::holds_alternative<StringInlined>(rhs.value)) {
                return strcmp(
                    std::get<StringExternal>(this->value).id,
                    std::get<StringInlined>(rhs.value).id
                ) <= 0;
            }
        }

        else if (std::holds_alternative<StringInlined>(this->value)) {
            if (std::holds_alternative<StringExternal>(rhs.value)) {
                return strcmp(
                    std::get<StringInlined>(this->value).id,
                    std::get<StringExternal>(rhs.value).id
                ) <= 0;
            }
        }

        else if (std::holds_alternative<int64_t>(this->value)) {
            if (std::holds_alternative<float>(rhs.value)) {
                return std::get<int64_t>(this->value) <= std::get<float>(rhs.value);
            }
        }

        else if (std::holds_alternative<float>(this->value)) {
            if (std::holds_alternative<int64_t>(rhs.value)) {
                return std::get<float>(this->value) <= std::get<int64_t>(rhs.value);
            }
        }
        return this->value <= rhs.value;
    }

    inline bool operator>=(const GraphObject& rhs) const noexcept {
        if (std::holds_alternative<IdentifiableExternal>(this->value)) {
            if (std::holds_alternative<IdentifiableInlined>(rhs.value)) {
                return strcmp(
                    std::get<IdentifiableExternal>(this->value).id,
                    std::get<IdentifiableInlined>(rhs.value).id
                ) >= 0;
            }
        }

        else if (std::holds_alternative<IdentifiableInlined>(this->value)) {
            if (std::holds_alternative<IdentifiableExternal>(rhs.value)) {
                return strcmp(
                    std::get<IdentifiableInlined>(this->value).id,
                    std::get<IdentifiableExternal>(rhs.value).id
                ) >= 0;
            }
        }

        else if (std::holds_alternative<StringExternal>(this->value)) {
            if (std::holds_alternative<StringInlined>(rhs.value)) {
                return strcmp(
                    std::get<StringExternal>(this->value).id,
                    std::get<StringInlined>(rhs.value).id
                ) >= 0;
            }
        }

        else if (std::holds_alternative<StringInlined>(this->value)) {
            if (std::holds_alternative<StringExternal>(rhs.value)) {
                return strcmp(
                    std::get<StringInlined>(this->value).id,
                    std::get<StringExternal>(rhs.value).id
                ) >= 0;
            }
        }

        else if (std::holds_alternative<int64_t>(this->value)) {
            if (std::holds_alternative<float>(rhs.value)) {
                return std::get<int64_t>(this->value) >= std::get<float>(rhs.value);
            }
        }

        else if (std::holds_alternative<float>(this->value)) {
            if (std::holds_alternative<int64_t>(rhs.value)) {
                return std::get<float>(this->value) >= std::get<int64_t>(rhs.value);
            }
        }
        return this->value >= rhs.value;
    }

    inline bool operator<(const GraphObject& rhs)  const noexcept {
        if (std::holds_alternative<IdentifiableExternal>(this->value)) {
            if (std::holds_alternative<IdentifiableInlined>(rhs.value)) {
                return strcmp(
                    std::get<IdentifiableExternal>(this->value).id,
                    std::get<IdentifiableInlined>(rhs.value).id
                ) < 0;
            }
        }

        else if (std::holds_alternative<IdentifiableInlined>(this->value)) {
            if (std::holds_alternative<IdentifiableExternal>(rhs.value)) {
                return strcmp(
                    std::get<IdentifiableInlined>(this->value).id,
                    std::get<IdentifiableExternal>(rhs.value).id
                ) < 0;
            }
        }

        else if (std::holds_alternative<StringExternal>(this->value)) {
            if (std::holds_alternative<StringInlined>(rhs.value)) {
                return strcmp(
                    std::get<StringExternal>(this->value).id,
                    std::get<StringInlined>(rhs.value).id
                ) < 0;
            }
        }

        else if (std::holds_alternative<StringInlined>(this->value)) {
            if (std::holds_alternative<StringExternal>(rhs.value)) {
                return strcmp(
                    std::get<StringInlined>(this->value).id,
                    std::get<StringExternal>(rhs.value).id
                ) < 0;
            }
        }

        else if (std::holds_alternative<int64_t>(this->value)) {
            if (std::holds_alternative<float>(rhs.value)) {
                return std::get<int64_t>(this->value) < std::get<float>(rhs.value);
            }
        }

        else if (std::holds_alternative<float>(this->value)) {
            if (std::holds_alternative<int64_t>(rhs.value)) {
                return std::get<float>(this->value) < std::get<int64_t>(rhs.value);
            }
        }
        return this->value < rhs.value;
    }

    inline bool operator>(const GraphObject& rhs)  const noexcept {
        if (std::holds_alternative<IdentifiableExternal>(this->value)) {
            if (std::holds_alternative<IdentifiableInlined>(rhs.value)) {
                return strcmp(
                    std::get<IdentifiableExternal>(this->value).id,
                    std::get<IdentifiableInlined>(rhs.value).id
                ) > 0;
            }
        }

        else if (std::holds_alternative<IdentifiableInlined>(this->value)) {
            if (std::holds_alternative<IdentifiableExternal>(rhs.value)) {
                return strcmp(
                    std::get<IdentifiableInlined>(this->value).id,
                    std::get<IdentifiableExternal>(rhs.value).id
                ) > 0;
            }
        }

        else if (std::holds_alternative<StringExternal>(this->value)) {
            if (std::holds_alternative<StringInlined>(rhs.value)) {
                return strcmp(
                    std::get<StringExternal>(this->value).id,
                    std::get<StringInlined>(rhs.value).id
                ) > 0;
            }
        }

        else if (std::holds_alternative<StringInlined>(this->value)) {
            if (std::holds_alternative<StringExternal>(rhs.value)) {
                return strcmp(
                    std::get<StringInlined>(this->value).id,
                    std::get<StringExternal>(rhs.value).id
                ) > 0;
            }
        }

        else if (std::holds_alternative<int64_t>(this->value)) {
            if (std::holds_alternative<float>(rhs.value)) {
                return std::get<int64_t>(this->value) > std::get<float>(rhs.value);
            }
        }

        else if (std::holds_alternative<float>(this->value)) {
            if (std::holds_alternative<int64_t>(rhs.value)) {
                return std::get<float>(this->value) > std::get<int64_t>(rhs.value);
            }
        }
        return this->value > rhs.value;
    }

private:
    GraphObject(GraphObjectVariant value) :
        value (value) { }
};

static_assert(std::is_trivially_copyable<GraphObject>::value);
static_assert(sizeof(GraphObject) <= 16, "GraphObject size should be small, if it needs to increase the size avoid copies");

#endif // BASE__OBJECT_TYPE_H_
