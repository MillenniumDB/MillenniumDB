#include "update_visitor.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "misc/is_name_valid_for_path.h"
#include "misc/transliterator.h"
#include "misc/unicode_escape.h"
#include "query/parser/op/sparql/ops.h"
#include "storage/index/hnsw/hnsw_metric.h"

using namespace SPARQL;
using antlrcpp::Any;
using SUP = SparqlUpdateParser;

UpdateVisitor::UpdateVisitor()
{
    op_update = std::make_unique<OpUpdate>();
}

std::string UpdateVisitor::iriCtxToString(SUP::IriContext* ctx)
{
    std::string iri;
    if (ctx->IRIREF()) {
        iri = ctx->IRIREF()->getText();
        iri = iri.substr(1, iri.size() - 2);
    } else {
        std::string prefixedName = ctx->prefixedName()->getText();
        auto pos = prefixedName.find(':');
        auto prefix = prefixedName.substr(0, pos);
        auto suffix = prefixedName.substr(pos + 1);

        if (iri_prefix_map.find(prefix) == iri_prefix_map.end()) {
            throw QuerySemanticException("The prefix '" + prefix + "' is not defined");
        }
        iri = iri_prefix_map[prefix] + suffix;
    }
    // Check if the IRI is absolute or not
    // If it is not absolute, it needs to be expanded with the base IRI
    auto pos = iri.find(":");
    if (pos == std::string::npos) {
        if (base_iri.empty()) {
            throw QuerySemanticException(
                "The IRI '" + iri + "' is not absolute and the base IRI is not defined"
            );
        }
        iri = base_iri + iri;
    }
    return iri;
}

std::string UpdateVisitor::stringCtxToString(SUP::StringContext* ctx)
{
    std::string str = ctx->getText();
    if (ctx->STRING_LITERAL1() || ctx->STRING_LITERAL2()) {
        // One quote per side
        str = str.substr(1, str.size() - 2);
    } else {
        // Three quotes per side
        str = str.substr(3, str.size() - 6);
    }
    return UnicodeEscape::normalize_string(str);
}

ObjectId UpdateVisitor::handleIntegerString(const std::string& str, const std::string& iri)
{
    try {
        size_t pos;
        int64_t n = std::stoll(str, &pos);
        // Check if the whole string was parsed
        if (pos != str.size())
            return Conversions::pack_string_datatype(iri, str);
        return Conversions::pack_int(n);
    } catch (std::out_of_range& e) {
        // The integer is too big, we use a Decimal
        bool error;
        Decimal dec(str, &error);
        if (error) {
            return Conversions::pack_string_datatype(iri, str);
        }
        return Conversions::pack_decimal(dec);
    } catch (std::invalid_argument& e) {
        // The string is not a valid integer
        return Conversions::pack_string_datatype(iri, str);
    }
}

ObjectId UpdateVisitor::get_new_blank_node()
{
    auto new_blank_id = rdf_model.catalog.get_new_blank_node();

    return Conversions::pack_blank_inline(new_blank_id);
}

Any UpdateVisitor::visitQuery(SUP::QueryContext* ctx)
{
    visitChildren(ctx);
    if (op_update == nullptr) {
        throw QueryParsingException("Empty query", 0, 0);
    }
    return 0;
}

Any UpdateVisitor::visitPrologue(SUP::PrologueContext* ctx)
{
    visitChildren(ctx);
    for (auto&& [alias, iri_prefix] : rdf_model.default_query_prefixes) {
        if (iri_prefix_map.find(alias) == iri_prefix_map.end()) {
            iri_prefix_map.insert({ alias, iri_prefix });
        }
    }
    return 0;
}

Any UpdateVisitor::visitBaseDecl(SUP::BaseDeclContext* ctx)
{
    std::string base_iri = ctx->IRIREF()->getText();
    base_iri = base_iri.substr(1, base_iri.size() - 2); // remove '<' ... '>'
    return 0;
}

Any UpdateVisitor::visitPrefixDecl(SUP::PrefixDeclContext* ctx)
{
    std::string alias = ctx->PNAME_NS()->getText();
    alias = alias.substr(0, alias.size() - 1); // remove ':'

    std::string iri_prefix = ctx->IRIREF()->getText();
    iri_prefix = iri_prefix.substr(1, iri_prefix.size() - 2); // remove '<' ... '>'

    iri_prefix_map[alias] = iri_prefix;
    return 0;
}

Any UpdateVisitor::visitInsertData(SparqlUpdateParser::InsertDataContext* ctx)
{
    visitChildren(ctx);
    op_update->updates.push_back(std::make_unique<OpInsertData>(std::move(current_triples)));
    return 0;
}

Any UpdateVisitor::visitDeleteData(SparqlUpdateParser::DeleteDataContext* ctx)
{
    visitChildren(ctx);
    op_update->updates.push_back(std::make_unique<OpDeleteData>(std::move(current_triples)));
    return 0;
}

Any UpdateVisitor::visitTriplesTemplate(SUP::TriplesTemplateContext* ctx)
{
    for (auto& triples_same_subject : ctx->triplesSameSubject()) {
        visit(triples_same_subject);
    }
    return 0;
}

Any UpdateVisitor::visitTriplesSameSubject(SUP::TriplesSameSubjectContext* ctx)
{
    if (ctx->graphTerm()) { // graphTerm propertyListNotEmpty
        // 1. Visit the subject
        visit(ctx->graphTerm());
        subject_stack.push(current_sparql_element);
        // 2. Visit the predicate object list
        visit(ctx->propertyListNotEmpty());
        subject_stack.pop();
    } else { // triplesNode propertyList
        // Create new blank node
        subject_stack.push(get_new_blank_node());
        // Visit the inner predicates/object lists
        visit(ctx->triplesNode());
        // Visit the outer predicate/object list
        visit(ctx->propertyList());
        subject_stack.pop();
    }
    return 0;
}

Any UpdateVisitor::visitPropertyListNotEmpty(SUP::PropertyListNotEmptyContext* ctx)
{
    for (size_t i = 0; i < ctx->verb().size(); i++) {
        // Visit the predicate
        visit(ctx->verb(i));

        predicate_stack.push(current_sparql_element);
        for (auto& object : ctx->objectList(i)->object()) {
            visit(object);
        }
        predicate_stack.pop();
    }
    return 0;
}

Any UpdateVisitor::visitPropertyListPathNotEmpty(SUP::PropertyListPathNotEmptyContext* ctx)
{
    // 1. Visit the predicate
    visit(ctx->verb());

    predicate_stack.push(current_sparql_element);
    // 2. Visit the object list
    auto olp = ctx->objectListPath();
    auto op = olp->objectPath();
    for (auto& op_item : op) {
        visit(op_item);
    }
    predicate_stack.pop();

    for (auto& property_list_path_not_empty_list : ctx->propertyListPathNotEmptyList()) {
        // 1. Visit the predicate
        visit(property_list_path_not_empty_list->verb());
        predicate_stack.push(current_sparql_element);

        // 2. Visit the object list
        for (auto& object : property_list_path_not_empty_list->objectList()->object()) {
            visit(object);
        }
        predicate_stack.pop();
    }
    return 0;
}

Any UpdateVisitor::visitObject(SUP::ObjectContext* ctx)
{
    auto gn = ctx->graphNode();
    if (gn->graphTerm()) {
        visit(gn->graphTerm());

        current_triples.emplace_back(subject_stack.top(), predicate_stack.top(), current_sparql_element);
    } else {
        auto new_blank_node = get_new_blank_node();

        subject_stack.push(new_blank_node);
        visit(gn->triplesNode());
        subject_stack.pop();

        current_triples.emplace_back(subject_stack.top(), predicate_stack.top(), new_blank_node);
    }
    return 0;
}

Any UpdateVisitor::visitObjectPath(SUP::ObjectPathContext* ctx)
{
    auto gnp = ctx->graphNodePath();
    if (gnp->graphTerm()) {
        visit(gnp->graphTerm());

        current_triples.emplace_back(subject_stack.top(), predicate_stack.top(), current_sparql_element);
    } else {
        auto new_blank_node = get_new_blank_node();

        subject_stack.push(new_blank_node);
        visit(gnp->triplesNodePath());
        subject_stack.pop();

        current_triples.emplace_back(subject_stack.top(), predicate_stack.top(), new_blank_node);
    }
    return 0;
}

Any UpdateVisitor::visitCollection(SUP::CollectionContext* ctx)
{
    Id representative_bnode = subject_stack.top();
    // I assume this prev_bnode is the blank node that will be added, the
    // reason for this is because in the ObjectPath visitation the blank node
    // is added into the subject_stack, not because it is a subject though.
    Id prev_bnode = subject_stack.top();
    subject_stack.push(prev_bnode);
    for (size_t i = 0; i < ctx->graphNode().size(); ++i) {
        subject_stack.push(get_new_blank_node());
        auto previous_amount_of_triples = current_triples.size();
        visit(ctx->graphNode(i));
        // Case 1. Created a new triple
        if (previous_amount_of_triples < current_triples.size()) {
            current_sparql_element = current_triples[previous_amount_of_triples].subject;
        }
        // Case 2. A sparql element was created, default case:
        Id rdf_node = current_sparql_element;

        Id predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#first");
        current_triples.emplace_back(prev_bnode, predicate, rdf_node);

        if (i < ctx->graphNode().size() - 1) {
            Id next_bnode = subject_stack.top();

            predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#rest");

            current_triples.emplace_back(prev_bnode, predicate, next_bnode);
            prev_bnode = std::move(next_bnode);
        } else {
            predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#rest");
            current_triples.emplace_back(
                prev_bnode,
                predicate,
                Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#nil")
            );
        }
        subject_stack.pop();
    }
    subject_stack.pop();
    current_sparql_element = std::move(representative_bnode);

    return 0;
}

Any UpdateVisitor::visitCollectionPath(SUP::CollectionPathContext* ctx)
{
    Id representative_bnode = subject_stack.top();

    // prev_bnode will be the subject of the first triple that will be created.
    Id prev_bnode = subject_stack.top();
    for (size_t i = 0; i < ctx->graphNodePath().size(); ++i) {
        subject_stack.push(get_new_blank_node());
        auto previous_amount_of_triples = current_triples.size();
        visit(ctx->graphNodePath(i));
        // Case 1. Created a new triple
        if (previous_amount_of_triples < current_triples.size()) {
            current_sparql_element = current_triples[previous_amount_of_triples].subject;
        }
        // Case 2. A sparql element was created, default case:
        auto rdf_node = current_sparql_element;

        Id predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#first");
        current_triples.emplace_back(prev_bnode, predicate, rdf_node);

        if (i < ctx->graphNodePath().size() - 1) {
            Id next_bnode = subject_stack.top();

            predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#rest");

            current_triples.emplace_back(prev_bnode, predicate, next_bnode);
            prev_bnode = std::move(next_bnode);
        } else {
            predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#rest");
            current_triples.emplace_back(
                prev_bnode,
                predicate,
                Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#nil")
            );
        }
        subject_stack.pop();
    }
    current_sparql_element = std::move(representative_bnode);

    return 0;
}

Any UpdateVisitor::visitVerb(SUP::VerbContext* ctx)
{
    if (ctx->A()) {
        current_sparql_element = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#type");
    } else {
        visit(ctx->iri());
    }
    return 0;
}

Any UpdateVisitor::visitIri(SUP::IriContext* ctx)
{
    current_sparql_element = Conversions::pack_iri(iriCtxToString(ctx));
    return 0;
}

Any UpdateVisitor::visitRdfLiteral(SUP::RdfLiteralContext* ctx)
{
    std::string str = stringCtxToString(ctx->string());
    if (ctx->iri()) {
        std::string iri = iriCtxToString(ctx->iri());
        current_sparql_element = Conversions::try_pack_string_datatype(iri, str);
    } else if (ctx->LANGTAG()) {
        current_sparql_element = Conversions::pack_string_lang(ctx->LANGTAG()->getText().substr(1), str);
    } else {
        current_sparql_element = Conversions::pack_string_simple(str);
    }
    return 0;
}

Any UpdateVisitor::visitNumericLiteralUnsigned(SUP::NumericLiteralUnsignedContext* ctx)
{
    if (ctx->INTEGER()) {
        current_sparql_element = handleIntegerString(
            ctx->getText(),
            "http://www.w3.org/2001/XMLSchema#integer"
        );
    } else if (ctx->DECIMAL()) {
        bool error;
        Decimal dec(ctx->getText(), &error);
        if (error) {
            throw QueryException("Invalid decimal value: " + ctx->getText());
        }
        current_sparql_element = Conversions::pack_decimal(dec);
    } else {
        // Double
        try {
            current_sparql_element = Conversions::pack_double(std::stod(ctx->getText()));
        } catch (const std::out_of_range& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText()
            );
        } catch (const std::invalid_argument& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText()
            );
        }
    }
    return 0;
}

Any UpdateVisitor::visitNumericLiteralPositive(SUP::NumericLiteralPositiveContext* ctx)
{
    if (ctx->INTEGER_POSITIVE()) {
        current_sparql_element = handleIntegerString(
            ctx->getText(),
            "http://www.w3.org/2001/XMLSchema#positiveInteger"
        );
    } else if (ctx->DECIMAL_POSITIVE()) {
        bool error;
        Decimal dec(ctx->getText(), &error);
        if (error) {
            throw QueryException("Invalid decimal value: " + ctx->getText());
        }
        current_sparql_element = Conversions::pack_decimal(dec);
    } else {
        // Double
        try {
            current_sparql_element = Conversions::pack_double(std::stod(ctx->getText()));
        } catch (const std::out_of_range& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText()
            );
        } catch (const std::invalid_argument& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText()
            );
        }
    }
    return 0;
}

Any UpdateVisitor::visitNumericLiteralNegative(SUP::NumericLiteralNegativeContext* ctx)
{
    if (ctx->INTEGER_NEGATIVE()) {
        current_sparql_element = handleIntegerString(
            ctx->getText(),
            "http://www.w3.org/2001/XMLSchema#negativeInteger"
        );
    } else if (ctx->DECIMAL_NEGATIVE()) {
        bool error;
        Decimal dec(ctx->getText(), &error);
        if (error) {
            throw QueryException("Invalid decimal value: " + ctx->getText());
        }
        current_sparql_element = Conversions::pack_decimal(dec);
    } else {
        // Double
        try {
            current_sparql_element = Conversions::pack_double(std::stod(ctx->getText()));
        } catch (const std::out_of_range& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText()
            );
        } catch (const std::invalid_argument& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText()
            );
        }
    }
    return 0;
}

Any UpdateVisitor::visitBooleanLiteral(SUP::BooleanLiteralContext* ctx)
{
    current_sparql_element = Conversions::pack_bool(ctx->K_TRUE() != nullptr);
    return 0;
}

Any UpdateVisitor::visitBlankNode(SUP::BlankNodeContext* ctx)
{
    if (ctx->BLANK_NODE_LABEL()) {
        auto var_name = ctx->getText();
        // TODO: what is the correct scope for the blank node names? for now is the entire update
        auto search = bnode_map.find(var_name);
        if (search != bnode_map.end()) {
            current_sparql_element = search->second;
        } else {
            auto new_blank = get_new_blank_node();
            bnode_map.insert({ var_name, new_blank });
            current_sparql_element = new_blank;
        }
    } else {
        current_sparql_element = get_new_blank_node();
    }
    return 0;
}

Any UpdateVisitor::visitNil(SUP::NilContext*)
{
    current_sparql_element = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#nil");
    return 0;
}

Any UpdateVisitor::visitCreateIndexQuery(SUP::CreateIndexQueryContext* ctx)
{
    std::string index_name = stringCtxToString(ctx->string());

    if (!misc::is_name_valid_for_path(index_name)) {
        throw QueryException("Invalid index name: \"" + index_name + "\"");
    }

    const std::string index_type = ctx->ALPHANUMERIC_IDENTIFIER()->getText();
    const std::string index_type_lowercased = Transliterator::lowercase(index_type);

    auto graph_term_to_str = [&](SUP::GraphTermContext* ctx, const std::string& key) -> std::string {
        visit(ctx);
        if (current_sparql_element.is_OID()) {
            const auto oid = current_sparql_element.get_OID();
            const auto gen_sub_t = RDF_OID::get_generic_sub_type(oid);
            switch (gen_sub_t) {
            case RDF_OID::GenericSubType::STRING_SIMPLE:
            case RDF_OID::GenericSubType::STRING_XSD:
                return SPARQL::Conversions::unpack_string(oid);
            default:
                break;
            }
        }

        throw QueryException("index option \"" + key + "\" is expected to be a string");
    };

    auto graph_term_to_iri = [&](SUP::GraphTermContext* ctx, const std::string& key) -> std::string {
        visit(ctx);
        if (current_sparql_element.is_OID()) {
            const auto oid = current_sparql_element.get_OID();
            const auto gen_sub_t = RDF_OID::get_generic_sub_type(oid);
            switch (gen_sub_t) {
            case RDF_OID::GenericSubType::IRI:
                return SPARQL::Conversions::unpack_iri(oid);
            default:
                break;
            }
        }

        throw QueryException("index option \"" + key + "\" is expected to be an IRI");
    };

    auto graph_term_to_uint64 = [&](SUP::GraphTermContext* ctx, const std::string& key) -> uint64_t {
        visit(ctx);
        if (current_sparql_element.is_OID()) {
            const auto oid = current_sparql_element.get_OID();
            const auto gen_sub_t = RDF_OID::get_generic_sub_type(oid);
            switch (gen_sub_t) {
            case RDF_OID::GenericSubType::INTEGER: {
                const auto i = SPARQL::Conversions::unpack_int(oid);
                if (i >= 0) {
                    return static_cast<uint64_t>(i);
                }
                break;
            }
            default:
                break;
            }
        }
        throw QueryException("index option \"" + key + "\" is expected to be an unsigned integer");
    };

    if (index_type_lowercased == "text") {
        // Check if text index existed before
        if (rdf_model.catalog.text_index_manager.get_text_index(index_name) != nullptr) {
            throw QueryException("Text index \"" + index_name + "\" already exists");
        }

        TextIndexOptions text_index_opts;

        parse_index_options(
            ctx->createIndexOptions(),
            text_index_opts,
            [&](const std::string& key, SUP::GraphTermContext* graphTermCtx) {
                if (key == "predicate") {
                    text_index_opts.predicate = graph_term_to_iri(graphTermCtx, key);
                } else if (key == "normalization") {
                    text_index_opts.normalization = graph_term_to_str(graphTermCtx, key);
                } else if (key == "tokenization") {
                    text_index_opts.tokenization = graph_term_to_str(graphTermCtx, key);
                } else {
                    assert(false && "unhandled text index option");
                }
            }
        );

        TextSearch::NORMALIZE_TYPE normalize_type;
        if (text_index_opts.normalization == "nfkdCasefold") {
            normalize_type = TextSearch::NORMALIZE_TYPE::NFKD_CASEFOLD;
        } else {
            assert(text_index_opts.normalization == "identity");
            normalize_type = TextSearch::NORMALIZE_TYPE::IDENTITY;
        }

        TextSearch::TOKENIZE_TYPE tokenize_type;
        if (text_index_opts.tokenization == "wsSplitPunc") {
            tokenize_type = TextSearch::TOKENIZE_TYPE::WHITESPACE_SPLIT_PUNCTUATION;
            // identity, wsSplitPunc, wsRmPunc, wsKeepPunc
        } else if (text_index_opts.tokenization == "wsRmPunc") {
            tokenize_type = TextSearch::TOKENIZE_TYPE::WHITESPACE_REMOVE_PUNCTUATION;
        } else if (text_index_opts.tokenization == "wsKeepPunc") {
            tokenize_type = TextSearch::TOKENIZE_TYPE::WHITESPACE_KEEP_PUNCTUATION;
        } else {
            assert(text_index_opts.tokenization == "identity");
            tokenize_type = TextSearch::TOKENIZE_TYPE::IDENTITY;
        }

        op_update->updates.emplace_back(std::make_unique<OpCreateTextIndex>(
            std::move(index_name),
            std::move(text_index_opts.predicate),
            normalize_type,
            tokenize_type
        ));
    } else if (index_type_lowercased == "hnsw") {
        // Check if hnsw index existed before
        if (rdf_model.catalog.hnsw_index_manager.get_hnsw_index(index_name) != nullptr) {
            throw QueryException("HNSW index \"" + index_name + "\" already exists");
        }

        HNSWIndexOptions hnsw_index_opts;

        parse_index_options(
            ctx->createIndexOptions(),
            hnsw_index_opts,
            [&](const std::string& key, SUP::GraphTermContext* graphTermCtx) {
                if (key == "predicate") {
                    hnsw_index_opts.predicate = graph_term_to_iri(graphTermCtx, key);
                } else if (key == "metric") {
                    hnsw_index_opts.metric = graph_term_to_str(graphTermCtx, key);
                } else if (key == "dimension") {
                    hnsw_index_opts.dimension = graph_term_to_uint64(graphTermCtx, key);
                } else if (key == "maxEdges") {
                    hnsw_index_opts.max_edges = graph_term_to_uint64(graphTermCtx, key);
                } else if (key == "maxCandidates") {
                    hnsw_index_opts.max_candidates = graph_term_to_uint64(graphTermCtx, key);
                } else {
                    assert(false && "unhandled hnsw index option");
                }
            }
        );

        HNSW::MetricType metric_type;
        if (hnsw_index_opts.metric == "cosineDistance") {
            metric_type = HNSW::MetricType::COSINE_DISTANCE;
        } else if (hnsw_index_opts.metric == "manhattanDistance") {
            metric_type = HNSW::MetricType::MANHATTAN_DISTANCE;
        } else {
            assert(hnsw_index_opts.metric == "euclideanDistance");
            metric_type = HNSW::MetricType::EUCLIDEAN_DISTANCE;
        }

        op_update->updates.emplace_back(std::make_unique<OpCreateHNSWIndex>(
            std::move(index_name),
            std::move(hnsw_index_opts.predicate),
            hnsw_index_opts.dimension,
            hnsw_index_opts.max_edges,
            hnsw_index_opts.max_candidates,
            metric_type
        ));
    } else {
        throw QueryException("Invalid index type \"" + index_type + "\"");
    }

    return 0;
}

template<typename IndexOptions, typename OptionHandlerFunc>
void UpdateVisitor::parse_index_options(
    SparqlUpdateParser::CreateIndexOptionsContext* ctx,
    IndexOptions& index_opts,
    OptionHandlerFunc option_handler_func
)
{
    assert(ctx != nullptr);
    for (const auto& opt : ctx->createIndexOption()) {
        const std::string key = stringCtxToString(opt->string());

        if (!index_opts.opt2seen.contains(key)) {
            const auto valid_options = index_opts.valid_options();
            std::string valid_options_str;
            valid_options_str += valid_options[0];
            for (std::size_t i = 1; i < valid_options.size(); ++i) {
                valid_options_str += ", " + valid_options[i];
            }
            throw QueryException(
                "Unexpected index option: \"" + key + "\". Expected one of the following: { "
                + valid_options_str + " }"
            );
        }

        if (index_opts.opt2seen[key] == true) {
            throw QueryException("Duplicate index option: \"" + key + "\"");
        }

        option_handler_func(key, opt->graphTerm());

        index_opts.opt2seen[key] = true;
    }

    index_opts.validate();
}
