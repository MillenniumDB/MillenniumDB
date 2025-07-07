#pragma once

#include <string>

namespace SPARQL::MDBExtensions {

static std::string MDB_PREFIX_IRI = "https://mdb.imfd.cl/";

namespace Function {
/**
 * Functions are additional expressions that we provide
 *
 */
static std::string FUNCTION_PREFIX_IRI = MDB_PREFIX_IRI + "function#";

static std::string SQRT_SUFFIX_IRI = "sqrt";
static std::string SQRT_IRI = FUNCTION_PREFIX_IRI + SQRT_SUFFIX_IRI;

static std::string POW_SUFFIX_IRI = "pow";
static std::string POW_IRI = FUNCTION_PREFIX_IRI + POW_SUFFIX_IRI;

static std::string DOT_SUFFIX_IRI = "dot";
static std::string DOT_IRI = FUNCTION_PREFIX_IRI + DOT_SUFFIX_IRI;

static std::string SUM_SUFFIX_IRI = "sum";
static std::string SUM_IRI = FUNCTION_PREFIX_IRI + SUM_SUFFIX_IRI;

static std::string COSINE_SIMILARITY_SUFFIX_IRI = "cosineSimilarity";
static std::string COSINE_SIMILARITY_IRI = FUNCTION_PREFIX_IRI + COSINE_SIMILARITY_SUFFIX_IRI;

static std::string COSINE_DISTANCE_SUFFIX_IRI = "cosineDistance";
static std::string COSINE_DISTANCE_IRI = FUNCTION_PREFIX_IRI + COSINE_SIMILARITY_SUFFIX_IRI;

static std::string EUCLIDEAN_DISTANCE_SUFFIX_IRI = "euclideanDistance";
static std::string EUCLIDEAN_DISTANCE_IRI = FUNCTION_PREFIX_IRI + EUCLIDEAN_DISTANCE_SUFFIX_IRI;

static std::string MANHATTAN_DISTANCE_SUFFIX_IRI = "manhattanDistance";
static std::string MANHATTAN_DISTANCE_IRI = FUNCTION_PREFIX_IRI + MANHATTAN_DISTANCE_SUFFIX_IRI;
} // namespace Function

namespace Type {
/**
 * Types are additional literals with datatype that we define and handle specially
 *
 */
static std::string TYPE_PREFIX_IRI = MDB_PREFIX_IRI + "type#";

static std::string TENSOR_FLOAT_SUFFIX_IRI = "tensorFloat";
static std::string TENSOR_FLOAT_IRI = TYPE_PREFIX_IRI + TENSOR_FLOAT_SUFFIX_IRI;

static std::string TENSOR_DOUBLE_SUFFIX_IRI = "tensorDouble";
static std::string TENSOR_DOUBLE_IRI = TYPE_PREFIX_IRI + TENSOR_DOUBLE_SUFFIX_IRI;
} // namespace Type

} // namespace SPARQL::MDBExtensions
