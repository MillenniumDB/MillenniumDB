#include "update_visitor.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;
using antlrcpp::Any;
using SUP = SparqlUpdateParser;

UpdateVisitor::UpdateVisitor() {
    op_update = std::make_unique<OpUpdate>();
}


std::string UpdateVisitor::iriCtxToString(SUP::IriContext* ctx) {
    std::string iri;
    if (ctx->IRIREF()) {
        iri = ctx->IRIREF()->getText();
        iri = iri.substr(1, iri.size() - 2);
    }
    else {
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
    auto pos = iri.find("://");
    if (pos == std::string::npos) {
        if (base_iri.empty()) {
            throw QuerySemanticException("The IRI '" + iri + "' is not absolute and the base IRI is not defined");
        }
        iri = base_iri + iri;
    }
    return iri;
}


std::string UpdateVisitor::stringCtxToString(SUP::StringContext* ctx) {
    std::string str = ctx->getText();
    if (ctx->STRING_LITERAL1() || ctx->STRING_LITERAL2()) {
        // One quote per side
        str = str.substr(1, str.size() - 2);
    } else {
        // Three quotes per side
        str = str.substr(3, str.size() - 6);
    }
    std::string res;
    res.reserve(str.size());

    auto chars = str.data();
    auto end = chars + str.size();

    while (chars < end - 1) {
        if (*chars != '\\') {
            res += *chars;
            chars++;
        } else {
            switch (*(chars + 1)) {
            case 't':  { res += '\t'; break; }
            case 'b':  { res += '\b'; break; }
            case 'n':  { res += '\n'; break; }
            case 'r':  { res += '\r'; break; }
            case 'f':  { res += '\f'; break; }
            case '\\': { res += '\\'; break; }
            case '"':  { res += '"';  break; }
            case '\'': { res += '\''; break; }
            default: {
                std::stringstream ss;
                ss << "Invalid escape sequence: \"\\" << *(chars+1) << '"';
                throw QuerySemanticException(ss.str());
            }
            }
            chars += 2;
        }
    }

    if (chars < end) {
        res += *chars;
    }

    return res;
}


ObjectId UpdateVisitor::handleIntegerString(const std::string& str,
                                            const std::string& iri)
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


ObjectId UpdateVisitor::get_new_blank_node() {
    auto new_blank_id = rdf_model.catalog.get_new_blank_node();

    return Conversions::pack_blank_inline(new_blank_id);
}


Any UpdateVisitor::visitQuery(SUP::QueryContext* ctx) {
    visitChildren(ctx);
    if (op_update == nullptr) {
        throw QueryParsingException("Empty query", 0, 0);
    }
    return 0;
}


Any UpdateVisitor::visitPrologue(SUP::PrologueContext* ctx) {
    visitChildren(ctx);
    for (auto&& [alias, iri_prefix] : rdf_model.default_query_prefixes) {
        if (iri_prefix_map.find(alias) == iri_prefix_map.end()) {
            iri_prefix_map.insert({alias, iri_prefix});
        }
    }
    return 0;
}


Any UpdateVisitor::visitBaseDecl(SUP::BaseDeclContext* ctx) {
    std::string base_iri = ctx->IRIREF()->getText();
    base_iri = base_iri.substr(1, base_iri.size() - 2); // remove '<' ... '>'
    return 0;
}


Any UpdateVisitor::visitPrefixDecl(SUP::PrefixDeclContext* ctx) {
    std::string alias = ctx->PNAME_NS()->getText();
    alias = alias.substr(0, alias.size() - 1); // remove ':'

    std::string iri_prefix = ctx->IRIREF()->getText();
    iri_prefix = iri_prefix.substr(1, iri_prefix.size() - 2); // remove '<' ... '>'

    iri_prefix_map[alias] = iri_prefix;
    return 0;
}


Any UpdateVisitor::visitInsertData(SparqlUpdateParser::InsertDataContext* ctx) {
    visitChildren(ctx);
    op_update->updates.push_back(
        std::make_unique<OpInsertData>(std::move(current_triples))
    );
    return 0;
}


Any UpdateVisitor::visitDeleteData(SparqlUpdateParser::DeleteDataContext* ctx) {
    visitChildren(ctx);
    op_update->updates.push_back(
        std::make_unique<OpDeleteData>(std::move(current_triples))
    );
    return 0;
}


Any UpdateVisitor::visitTriplesTemplate(SUP::TriplesTemplateContext* ctx) {
    for (auto& triples_same_subject : ctx->triplesSameSubject()) {
        visit(triples_same_subject);
    }
    return 0;
}


Any UpdateVisitor::visitTriplesSameSubject(SUP::TriplesSameSubjectContext* ctx) {
    if (ctx->graphTerm()) { // graphTerm propertyListNotEmpty
        // 1. Visit the subject
        visit(ctx->graphTerm());
        subject_stack.push(current_sparql_element);
        // 2. Visit the predicate object list
        visit(ctx->propertyListNotEmpty());
        subject_stack.pop();
    }
    else { // triplesNode propertyList
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


Any UpdateVisitor::visitPropertyListNotEmpty(SUP::PropertyListNotEmptyContext* ctx) {
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


Any UpdateVisitor::visitPropertyListPathNotEmpty(SUP::PropertyListPathNotEmptyContext* ctx) {
    // 1. Visit the predicate
    visit(ctx->verb());

    predicate_stack.push(current_sparql_element);
    // 2. Visit the object list
    auto olp = ctx->objectListPath();
    auto op  = olp->objectPath();
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


Any UpdateVisitor::visitObject(SUP::ObjectContext* ctx) {
    auto gn = ctx->graphNode();
    if (gn->graphTerm()) {
        visit(gn->graphTerm());

        current_triples.emplace_back(
            subject_stack.top(),
            predicate_stack.top(),
            current_sparql_element
        );
    }
    else {
        auto new_blank_node = get_new_blank_node();

        subject_stack.push(new_blank_node);
        visit(gn->triplesNode());
        subject_stack.pop();

        current_triples.emplace_back(
            subject_stack.top(),
            predicate_stack.top(),
            new_blank_node
        );
    }
    return 0;
}


Any UpdateVisitor::visitObjectPath(SUP::ObjectPathContext* ctx) {
    auto gnp = ctx->graphNodePath();
    if (gnp->graphTerm()) {
        visit(gnp->graphTerm());

        current_triples.emplace_back(
            subject_stack.top(),
            predicate_stack.top(),
            current_sparql_element
        );
    }
    else {
        auto new_blank_node = get_new_blank_node();

        subject_stack.push(new_blank_node);
        visit(gnp->triplesNodePath());
        subject_stack.pop();

        current_triples.emplace_back(
            subject_stack.top(),
            predicate_stack.top(),
            new_blank_node
        );
    }
    return 0;
}


Any UpdateVisitor::visitCollection(SUP::CollectionContext* ctx) {
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
        current_triples.emplace_back(
            prev_bnode,
            predicate,
            rdf_node
        );

        if (i < ctx->graphNode().size() - 1) {
            Id next_bnode = subject_stack.top();

            predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#rest");

            current_triples.emplace_back(
                prev_bnode,
                predicate,
                next_bnode
            );
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


Any UpdateVisitor::visitCollectionPath(SUP::CollectionPathContext* ctx) {
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
        current_triples.emplace_back(
            prev_bnode,
            predicate,
            rdf_node
        );

        if (i < ctx->graphNodePath().size() - 1) {
            Id next_bnode = subject_stack.top();

            predicate = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#rest");

            current_triples.emplace_back(
                prev_bnode,
                predicate,
                next_bnode
            );
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


Any UpdateVisitor::visitVerb(SUP::VerbContext* ctx) {
    if (ctx->A()) {
        current_sparql_element = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#type");
    } else {
        visit(ctx->iri());
    }
    return 0;
}


Any UpdateVisitor::visitIri(SUP::IriContext* ctx) {
    current_sparql_element = Conversions::pack_iri(iriCtxToString(ctx));
    return 0;
}


Any UpdateVisitor::visitRdfLiteral(SUP::RdfLiteralContext* ctx) {
    std::string str = stringCtxToString(ctx->string());
    if (ctx->iri()) {
        std::string iri = iriCtxToString(ctx->iri());
        current_sparql_element = Conversions::try_pack_string_datatype(iri, str);
    }
    else if (ctx->LANGTAG()) {
        current_sparql_element = Conversions::pack_string_lang(ctx->LANGTAG()->getText().substr(1), str);
    } else {
        current_sparql_element = Conversions::pack_string_simple(str);
    }
    return 0;
}

Any UpdateVisitor::visitNumericLiteralUnsigned(SUP::NumericLiteralUnsignedContext* ctx) {
    if (ctx->INTEGER()) {
        current_sparql_element = handleIntegerString(ctx->getText(), "http://www.w3.org/2001/XMLSchema#integer");
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
                ctx->getText());
        } catch (const std::invalid_argument& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText());
        }
    }
    return 0;
}


Any UpdateVisitor::visitNumericLiteralPositive(SUP::NumericLiteralPositiveContext* ctx) {
    if (ctx->INTEGER_POSITIVE()) {
        current_sparql_element = handleIntegerString(ctx->getText(), "http://www.w3.org/2001/XMLSchema#positiveInteger");
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
                ctx->getText());
        } catch (const std::invalid_argument& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText());
        }
    }
    return 0;
}


Any UpdateVisitor::visitNumericLiteralNegative(SUP::NumericLiteralNegativeContext* ctx) {
    if (ctx->INTEGER_NEGATIVE()) {
        current_sparql_element = handleIntegerString(ctx->getText(), "http://www.w3.org/2001/XMLSchema#negativeInteger");
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
                ctx->getText());
        } catch (const std::invalid_argument& e) {
            current_sparql_element = Conversions::pack_string_datatype(
                "http://www.w3.org/2001/XMLSchema#double",
                ctx->getText());
        }
    }
    return 0;
}


Any UpdateVisitor::visitBooleanLiteral(SUP::BooleanLiteralContext* ctx) {
    current_sparql_element = Conversions::pack_bool(ctx->TRUE() != nullptr);
    return 0;
}


Any UpdateVisitor::visitBlankNode(SUP::BlankNodeContext* ctx) {
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


Any UpdateVisitor::visitNil(SUP::NilContext*) {
    current_sparql_element = Conversions::pack_iri("http://www.w3.org/1999/02/22-rdf-syntax-ns#nil");
    return 0;
}
