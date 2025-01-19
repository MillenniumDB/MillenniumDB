
// Generated from SparqlUpdateParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "SparqlUpdateParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by SparqlUpdateParser.
 */
class  SparqlUpdateParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by SparqlUpdateParser.
   */
    virtual std::any visitQuery(SparqlUpdateParser::QueryContext *context) = 0;

    virtual std::any visitPrologue(SparqlUpdateParser::PrologueContext *context) = 0;

    virtual std::any visitBaseDecl(SparqlUpdateParser::BaseDeclContext *context) = 0;

    virtual std::any visitPrefixDecl(SparqlUpdateParser::PrefixDeclContext *context) = 0;

    virtual std::any visitUpdate(SparqlUpdateParser::UpdateContext *context) = 0;

    virtual std::any visitInsertData(SparqlUpdateParser::InsertDataContext *context) = 0;

    virtual std::any visitDeleteData(SparqlUpdateParser::DeleteDataContext *context) = 0;

    virtual std::any visitCreateTextIndex(SparqlUpdateParser::CreateTextIndexContext *context) = 0;

    virtual std::any visitNormalizeTextIndex(SparqlUpdateParser::NormalizeTextIndexContext *context) = 0;

    virtual std::any visitNormalizeType(SparqlUpdateParser::NormalizeTypeContext *context) = 0;

    virtual std::any visitTokenizeTextIndex(SparqlUpdateParser::TokenizeTextIndexContext *context) = 0;

    virtual std::any visitTokenizeType(SparqlUpdateParser::TokenizeTypeContext *context) = 0;

    virtual std::any visitQuadData(SparqlUpdateParser::QuadDataContext *context) = 0;

    virtual std::any visitTriplesTemplate(SparqlUpdateParser::TriplesTemplateContext *context) = 0;

    virtual std::any visitConstructTemplate(SparqlUpdateParser::ConstructTemplateContext *context) = 0;

    virtual std::any visitConstructTriples(SparqlUpdateParser::ConstructTriplesContext *context) = 0;

    virtual std::any visitTriplesSameSubject(SparqlUpdateParser::TriplesSameSubjectContext *context) = 0;

    virtual std::any visitPropertyList(SparqlUpdateParser::PropertyListContext *context) = 0;

    virtual std::any visitPropertyListNotEmpty(SparqlUpdateParser::PropertyListNotEmptyContext *context) = 0;

    virtual std::any visitVerb(SparqlUpdateParser::VerbContext *context) = 0;

    virtual std::any visitObjectList(SparqlUpdateParser::ObjectListContext *context) = 0;

    virtual std::any visitObject(SparqlUpdateParser::ObjectContext *context) = 0;

    virtual std::any visitPropertyListPathNotEmpty(SparqlUpdateParser::PropertyListPathNotEmptyContext *context) = 0;

    virtual std::any visitPropertyListPathNotEmptyList(SparqlUpdateParser::PropertyListPathNotEmptyListContext *context) = 0;

    virtual std::any visitObjectListPath(SparqlUpdateParser::ObjectListPathContext *context) = 0;

    virtual std::any visitObjectPath(SparqlUpdateParser::ObjectPathContext *context) = 0;

    virtual std::any visitInteger(SparqlUpdateParser::IntegerContext *context) = 0;

    virtual std::any visitTriplesNode(SparqlUpdateParser::TriplesNodeContext *context) = 0;

    virtual std::any visitBlankNodePropertyList(SparqlUpdateParser::BlankNodePropertyListContext *context) = 0;

    virtual std::any visitTriplesNodePath(SparqlUpdateParser::TriplesNodePathContext *context) = 0;

    virtual std::any visitBlankNodePropertyListPath(SparqlUpdateParser::BlankNodePropertyListPathContext *context) = 0;

    virtual std::any visitCollection(SparqlUpdateParser::CollectionContext *context) = 0;

    virtual std::any visitCollectionPath(SparqlUpdateParser::CollectionPathContext *context) = 0;

    virtual std::any visitGraphNode(SparqlUpdateParser::GraphNodeContext *context) = 0;

    virtual std::any visitGraphNodePath(SparqlUpdateParser::GraphNodePathContext *context) = 0;

    virtual std::any visitGraphTerm(SparqlUpdateParser::GraphTermContext *context) = 0;

    virtual std::any visitNil(SparqlUpdateParser::NilContext *context) = 0;

    virtual std::any visitRdfLiteral(SparqlUpdateParser::RdfLiteralContext *context) = 0;

    virtual std::any visitNumericLiteral(SparqlUpdateParser::NumericLiteralContext *context) = 0;

    virtual std::any visitNumericLiteralUnsigned(SparqlUpdateParser::NumericLiteralUnsignedContext *context) = 0;

    virtual std::any visitNumericLiteralPositive(SparqlUpdateParser::NumericLiteralPositiveContext *context) = 0;

    virtual std::any visitNumericLiteralNegative(SparqlUpdateParser::NumericLiteralNegativeContext *context) = 0;

    virtual std::any visitBooleanLiteral(SparqlUpdateParser::BooleanLiteralContext *context) = 0;

    virtual std::any visitString(SparqlUpdateParser::StringContext *context) = 0;

    virtual std::any visitIri(SparqlUpdateParser::IriContext *context) = 0;

    virtual std::any visitPrefixedName(SparqlUpdateParser::PrefixedNameContext *context) = 0;

    virtual std::any visitBlankNode(SparqlUpdateParser::BlankNodeContext *context) = 0;

    virtual std::any visitAnon(SparqlUpdateParser::AnonContext *context) = 0;


};

