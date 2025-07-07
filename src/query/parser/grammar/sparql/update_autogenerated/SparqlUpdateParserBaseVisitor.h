
// Generated from SparqlUpdateParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "SparqlUpdateParserVisitor.h"


/**
 * This class provides an empty implementation of SparqlUpdateParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  SparqlUpdateParserBaseVisitor : public SparqlUpdateParserVisitor {
public:

  virtual std::any visitQuery(SparqlUpdateParser::QueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrologue(SparqlUpdateParser::PrologueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBaseDecl(SparqlUpdateParser::BaseDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrefixDecl(SparqlUpdateParser::PrefixDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUpdate(SparqlUpdateParser::UpdateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCreateIndexQuery(SparqlUpdateParser::CreateIndexQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCreateIndexOptions(SparqlUpdateParser::CreateIndexOptionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCreateIndexOption(SparqlUpdateParser::CreateIndexOptionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInsertData(SparqlUpdateParser::InsertDataContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeleteData(SparqlUpdateParser::DeleteDataContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQuadData(SparqlUpdateParser::QuadDataContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTriplesTemplate(SparqlUpdateParser::TriplesTemplateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstructTemplate(SparqlUpdateParser::ConstructTemplateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstructTriples(SparqlUpdateParser::ConstructTriplesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTriplesSameSubject(SparqlUpdateParser::TriplesSameSubjectContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyList(SparqlUpdateParser::PropertyListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyListNotEmpty(SparqlUpdateParser::PropertyListNotEmptyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVerb(SparqlUpdateParser::VerbContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitObjectList(SparqlUpdateParser::ObjectListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitObject(SparqlUpdateParser::ObjectContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyListPathNotEmpty(SparqlUpdateParser::PropertyListPathNotEmptyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPropertyListPathNotEmptyList(SparqlUpdateParser::PropertyListPathNotEmptyListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitObjectListPath(SparqlUpdateParser::ObjectListPathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitObjectPath(SparqlUpdateParser::ObjectPathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInteger(SparqlUpdateParser::IntegerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTriplesNode(SparqlUpdateParser::TriplesNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlankNodePropertyList(SparqlUpdateParser::BlankNodePropertyListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTriplesNodePath(SparqlUpdateParser::TriplesNodePathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlankNodePropertyListPath(SparqlUpdateParser::BlankNodePropertyListPathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCollection(SparqlUpdateParser::CollectionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCollectionPath(SparqlUpdateParser::CollectionPathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGraphNode(SparqlUpdateParser::GraphNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGraphNodePath(SparqlUpdateParser::GraphNodePathContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGraphTerm(SparqlUpdateParser::GraphTermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNil(SparqlUpdateParser::NilContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRdfLiteral(SparqlUpdateParser::RdfLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumericLiteral(SparqlUpdateParser::NumericLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumericLiteralUnsigned(SparqlUpdateParser::NumericLiteralUnsignedContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumericLiteralPositive(SparqlUpdateParser::NumericLiteralPositiveContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumericLiteralNegative(SparqlUpdateParser::NumericLiteralNegativeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBooleanLiteral(SparqlUpdateParser::BooleanLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitString(SparqlUpdateParser::StringContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIri(SparqlUpdateParser::IriContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrefixedName(SparqlUpdateParser::PrefixedNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlankNode(SparqlUpdateParser::BlankNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAnon(SparqlUpdateParser::AnonContext *ctx) override {
    return visitChildren(ctx);
  }


};

