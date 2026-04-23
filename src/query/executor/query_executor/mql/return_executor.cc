#include "return_executor.h"

#include "graph_models/inliner.h"
#include "graph_models/quad_model/conversions.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/query_executor/csv_ostream_escape.h"
#include "query/executor/query_executor/tsv_ostream_escape.h"
#include "system/path_manager.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"

using namespace MQL;

template<ReturnType ret>
void ReturnExecutor<ret>::print(std::ostream& os, ObjectId oid)
{
    print(os, os, oid);
}

template<ReturnType ret>
void ReturnExecutor<ret>::print_handle_escape(std::ostream& os, std::ostream& escaped_os, ObjectId oid)
{
    switch (oid.generic_type()) {
    case ObjectGenType::Dict:
    case ObjectGenType::List:
        os << '"';
        print(os, escaped_os, oid);
        os << '"';
        break;
    default:
        print(os, escaped_os, oid);
    }
}

template<ReturnType ret>
void ReturnExecutor<ret>::print_path_node(std::ostream& os, ObjectId node_id)
{
    os << "(";
    ReturnExecutor::print(os, os, node_id);
    os << ")";
}

template<ReturnType ret>
void ReturnExecutor<ret>::print_path_edge(std::ostream& os, ObjectId edge_id, bool inverse)
{
    if (inverse) {
        os << "<-[:";
        ReturnExecutor::print(os, os, edge_id);
        os << "]-";
    } else {
        os << "-[:";
        ReturnExecutor::print(os, os, edge_id);
        os << "]->";
    }
}

template<ReturnType ret>
void ReturnExecutor<ret>::print_list(std::ostream& os, std::ostream& escaped_os, ObjectId oid)
{
    std::vector<ObjectId> out;
    Conversions::unpack_list(oid, out);
    os << "[";
    for (auto it = out.begin(); it != out.end(); ++it) {
        if (it != out.begin()) {
            os << ",";
        }
        print(escaped_os, escaped_os, *it);
    }
    os << "]";
}

template<ReturnType ret>
void ReturnExecutor<ret>::print(std::ostream& os, std::ostream& escaped_os, ObjectId oid)
{
    const auto type = oid.type();
    const auto unmasked_id = oid.id & ObjectId::VALUE_MASK;
    switch (type) {
    case ObjectType::Null: {
        os << "null";
        break;
    }
    case ObjectType::AnonInl: {
        os << "_a" << unmasked_id;
        break;
    }
    case ObjectType::AnonTmp: {
        os << "_t" << unmasked_id;
        break;
    }
    case ObjectType::NamedNodeInl: {
        Inliner::print_string_inlined<7>(os, unmasked_id);
        break;
    }
    case ObjectType::NamedNodeExt: {
        string_manager.print(os, unmasked_id);
        break;
    }
    case ObjectType::NamedNodeTmp: {
        tmp_manager.print_str(os, unmasked_id);
        break;
    }
    case ObjectType::StringInl: {
        os << '"';
        Inliner::print_string_inlined<7>(escaped_os, unmasked_id);
        os << '"';
        break;
    }
    case ObjectType::StringExt: {
        os << '"';
        string_manager.print(escaped_os, unmasked_id);
        os << '"';
        break;
    }
    case ObjectType::StringTmp: {
        os << '"';
        tmp_manager.print_str(escaped_os, unmasked_id);
        os << '"';
        break;
    }
    case ObjectType::NegativeInt56:
    case ObjectType::PositiveInt56: {
        int64_t i = Conversions::unpack_int(oid);
        os << i;
        break;
    }
    case ObjectType::Float: {
        float f = Conversions::unpack_float(oid);
        os << f;
        break;
    }
    case ObjectType::Bool: {
        os << (unmasked_id == 0 ? "false" : "true");
        break;
    }
    case ObjectType::DirectedEdge: {
        os << "_e" << unmasked_id;
        break;
    }
    case ObjectType::Path: {
        using namespace std::placeholders;
        os << '[';
        path_manager.for_each(
            unmasked_id,
            [&](ObjectId oid) { print_path_node(os, oid); },
            [&](ObjectId oid, bool reverse) { print_path_edge(os, oid, reverse); }
        );
        os << ']';
        break;
    }
    case ObjectType::Date: {
        DateTime datetime = SPARQL::Conversions::unpack_date(oid);
        os << "date(\"" << datetime.get_value_string() << "\")";
        break;
    }
    case ObjectType::Datetime: {
        DateTime datetime = SPARQL::Conversions::unpack_date(oid);
        os << "dateTime(\"" << datetime.get_value_string() << "\")";
        break;
    }
    case ObjectType::Datetimestamp: {
        DateTime datetime = SPARQL::Conversions::unpack_date(oid);
        os << "dateTimeStamp(\"" << datetime.get_value_string() << "\")";
        break;
    }
    case ObjectType::Time: {
        DateTime datetime = SPARQL::Conversions::unpack_date(oid);
        os << "time(\"" << datetime.get_value_string() << "\")";
        break;
    }
    case ObjectType::TensorFloatExt:
    case ObjectType::TensorFloatTmp: {
        const auto tensor = Conversions::unpack_tensor<float>(oid);
        os << "tensorFloat(\"" << tensor.to_string() << "\")";
        break;
    }
    case ObjectType::TensorDoubleExt:
    case ObjectType::TensorDoubleTmp: {
        const auto tensor = Conversions::unpack_tensor<double>(oid);
        os << "tensorDouble(\"" << tensor.to_string() << "\")";
        break;
    }
    case ObjectType::ListExt:
    case ObjectType::ListTmp: {
        print_list(os, escaped_os, oid);
        break;
    }
    case ObjectType::DictionaryExt:
    case ObjectType::DictionaryTmp: {
        std::unique_ptr<Dictionary> dict = Common::Conversions::unpack_dictionary(oid);
        dict->to_string(escaped_os);
        break;
    }
    default:
        throw std::logic_error("Unmanaged mask in ReturnExecutor print: " + to_string(type));
    }
}

template<ReturnType ret>
uint64_t ReturnExecutor<ret>::execute(std::ostream& os)
{
    // print header
    auto it = projection_vars.cbegin();
    if (it != projection_vars.cend()) {
        std::string var_name = get_query_ctx().get_var_name(*it);
        if (var_name[0] == '.') {
            var_name.erase(0, 1);
        }
        os << var_name;
        ++it;
    }
    while (it != projection_vars.cend()) {
        if (ret == ReturnType::CSV) {
            os << ',';
        } else {
            os << '\t';
        }
        std::string var_name = get_query_ctx().get_var_name(*it);
        if (var_name[0] == '.') {
            var_name.erase(0, 1);
        }
        os << var_name;
        ++it;
    }
    os << "\n";

    uint64_t result_count = 0;
    Binding binding(get_query_ctx().get_var_size());

    iter->begin(binding);

    std::unique_ptr<std::ostream> escaped_os;
    std::unique_ptr<std::stringbuf> stream_escaper;
    if (ret == ReturnType::CSV) {
        stream_escaper = std::make_unique<CSVOstreamEscape>(os);
    } else {
        stream_escaper = std::make_unique<TSVOstreamEscape>(os);
    }
    escaped_os = std::make_unique<std::ostream>(stream_escaper.get());

    while (iter->next()) {
        result_count++;
        auto it = projection_vars.cbegin();

        if (it != projection_vars.cend()) {
            auto value = binding[*it];
            print_handle_escape(os, *escaped_os, value);
            ++it;
        }
        while (it != projection_vars.cend()) {
            auto value = binding[*it];
            if (ret == ReturnType::CSV) {
                os << ',';
            } else {
                os << '\t';
            }
            print_handle_escape(os, *escaped_os, value);
            ++it;
        }
        os << "\n";
    }

    return result_count;
}

template<ReturnType ret>
void ReturnExecutor<ret>::analyze(std::ostream& os, bool print_stats, int indent) const
{
    iter->print(os, indent, print_stats);
}

template class MQL::ReturnExecutor<MQL::ReturnType::CSV>;
template class MQL::ReturnExecutor<MQL::ReturnType::TSV>;
