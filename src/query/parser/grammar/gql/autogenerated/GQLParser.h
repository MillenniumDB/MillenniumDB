
// Generated from GQLParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  GQLParser : public antlr4::Parser {
public:
  enum {
    AMPERSAND = 1, ASTERISK = 2, CIRCUMFLEX = 3, COLON = 4, COMMA = 5, DOLLAR_SIGN = 6, 
    EQUALS_OPERATOR = 7, EXCLAMATION_MARK = 8, RIGHT_ANGLE_BRACKET = 9, 
    LEFT_BRACE = 10, LEFT_BRACKET = 11, LEFT_PAREN = 12, LEFT_ANGLE_BRACKET = 13, 
    MINUS_SIGN = 14, PERCENT = 15, PERIOD = 16, PLUS_SIGN = 17, QUESTION_MARK = 18, 
    REVERSE_SOLIDUS = 19, RIGHT_BRACE = 20, RIGHT_BRACKET = 21, RIGHT_PAREN = 22, 
    SEMICOLON = 23, SOLIDUS = 24, TILDE = 25, UNDERSCORE = 26, VERTICAL_BAR = 27, 
    ABS = 28, ACOS = 29, ACYCLIC = 30, ALL = 31, ALL_DIFFERENT = 32, AND = 33, 
    ANY = 34, ARRAY = 35, AS = 36, ASC = 37, ASCENDING = 38, ASIN = 39, 
    AT = 40, ATAN = 41, AVG = 42, BIG = 43, BIGINT = 44, BINARY = 45, BINDING = 46, 
    BINDINGS = 47, BOOL = 48, BOOLEAN = 49, BOTH = 50, BTRIM = 51, BY = 52, 
    BYTE_LENGTH = 53, BYTES = 54, CALL = 55, CASE = 56, CAST = 57, CEIL = 58, 
    CEILING = 59, CHAR_LENGTH = 60, CHARACTER_LENGTH = 61, CHARACTERISTICS = 62, 
    CLOSE = 63, COALESCE = 64, COLLECT = 65, COMMIT = 66, CONNECTING = 67, 
    COPY = 68, COS = 69, COSH = 70, COT = 71, COUNT = 72, CREATE = 73, CURRENT_DATE = 74, 
    CURRENT_GRAPH = 75, CURRENT_PROPERTY_GRAPH = 76, CURRENT_SCHEMA = 77, 
    CURRENT_TIME = 78, CURRENT_TIMESTAMP = 79, CURRENT_USER = 80, DATE = 81, 
    DATETIME = 82, DEC = 83, DECIMAL = 84, DEGREES = 85, DELETE = 86, DESC = 87, 
    DESCENDING = 88, DESTINATION = 89, DETACH = 90, DIFFERENT = 91, DIRECTED = 92, 
    DISTINCT = 93, DOUBLE = 94, DROP = 95, DURATION = 96, DURATION_BETWEEN = 97, 
    ELEMENT = 98, ELEMENT_ID = 99, ELEMENTS = 100, ELSE = 101, END = 102, 
    EXCEPT = 103, EXISTS = 104, EXP = 105, K_FALSE = 106, FILTER = 107, 
    FINISH = 108, FIRST = 109, FLOAT = 110, FLOAT128 = 111, FLOAT16 = 112, 
    FLOAT256 = 113, FLOAT32 = 114, FLOAT64 = 115, FLOOR = 116, FOR = 117, 
    FROM = 118, GRAPH = 119, GROUP = 120, GROUPS = 121, HAVING = 122, HOME_GRAPH = 123, 
    HOME_PROPERTY_GRAPH = 124, HOME_SCHEMA = 125, I_DONT_KNOW_1 = 126, I_DONT_KNOW_2 = 127, 
    I_DONT_KNOW_3 = 128, I_DONT_KNOW_4 = 129, I_DONT_KNOW_5 = 130, IF = 131, 
    IN = 132, INSERT = 133, INT = 134, INT8 = 135, INT16 = 136, INT32 = 137, 
    INT64 = 138, INT128 = 139, INT256 = 140, INTEGER = 141, INTEGER8 = 142, 
    INTEGER16 = 143, INTEGER32 = 144, INTEGER64 = 145, INTEGER128 = 146, 
    INTEGER256 = 147, INTERSECT = 148, IS = 149, KEEP = 150, LABEL = 151, 
    LABELED = 152, LABELS = 153, LAST = 154, LEADING = 155, LEFT = 156, 
    LET = 157, LIKE = 158, LIMIT = 159, LIST = 160, LN = 161, LOCAL = 162, 
    LOCAL_DATETIME = 163, LOCAL_TIME = 164, LOCAL_TIMESTAMP = 165, LOG = 166, 
    LOG10 = 167, LOWER = 168, LTRIM = 169, MATCH = 170, MAX = 171, MIN = 172, 
    MOD = 173, NEXT = 174, NFC = 175, NFD = 176, NFKC = 177, NFKD = 178, 
    NO = 179, NODETACH = 180, NORMALIZE = 181, NORMALIZED = 182, NOT = 183, 
    NULL_ = 184, NULLIF = 185, NULLS = 186, OCTET_LENGTH = 187, OF = 188, 
    OFFSET = 189, SKIP_ = 190, ONLY = 191, OPEN = 192, OPTIONAL = 193, OR = 194, 
    ORDER = 195, ORDINALITY = 196, OTHERWISE = 197, PARAMETER = 198, PARAMETERS = 199, 
    PATH = 200, PATH_LENGTH = 201, PATHS = 202, PERCENTILE_CONT = 203, PERCENTILE_DISC = 204, 
    POWER = 205, PRECISION = 206, PROPERTY = 207, PROPERTY_EXISTS = 208, 
    RADIANS = 209, READ = 210, REAL = 211, RECORD = 212, REMOVE = 213, REPEATABLE = 214, 
    REPLACE = 215, RESET = 216, RETURN = 217, ROLLBACK = 218, RTRIM = 219, 
    RIGHT = 220, SAME = 221, SCHEMA = 222, SELECT = 223, SESSION = 224, 
    SET = 225, SHORTEST = 226, SIGNED = 227, SIMPLE = 228, SIN = 229, SINH = 230, 
    SMALL = 231, SMALLINT = 232, SOURCE = 233, SQRT = 234, START = 235, 
    STDDEV_POP = 236, STDDEV_SAMP = 237, STRING = 238, SUM = 239, TABLE = 240, 
    TAN = 241, TANH = 242, TEMP = 243, THEN = 244, TIME = 245, TIMESTAMP = 246, 
    TIMEZONE = 247, TO = 248, TRAIL = 249, TRAILING = 250, TRANSACTION = 251, 
    TRIM = 252, K_TRUE = 253, TYPE = 254, TYPED = 255, UBIGINT = 256, UINT = 257, 
    UINT128 = 258, UINT16 = 259, UINT256 = 260, UINT32 = 261, UINT64 = 262, 
    UINT8 = 263, UNDIRECTED = 264, UNION = 265, UNKNOWN = 266, UNSIGNED = 267, 
    UPPER = 268, USE = 269, USMALLINT = 270, VALUE = 271, VARBINARY = 272, 
    VARCHAR = 273, VARIABLE = 274, WALK = 275, WHEN = 276, WHERE = 277, 
    WITH = 278, WITHOUT = 279, WRITE = 280, XOR = 281, YIELD = 282, ZONE = 283, 
    ZONED = 284, ZONED_DATETIME = 285, ZONED_TIME = 286, INTERVAL_DAY = 287, 
    INTERVAL_WEEK = 288, INTERVAL_MONTH = 289, INTERVAL_YEAR = 290, EDGE_SYNONYM = 291, 
    EDGES_SYNONYM = 292, NODE_SYNONYM = 293, KEY_WORD = 294, RESERVED_WORD = 295, 
    PRE_RESERVED_WORD = 296, NON_RESERVED_WORD = 297, MULTISET_ALTERNATION_OPERATOR = 298, 
    CONCATENATION_OPERATOR = 299, DOUBLE_COLON = 300, DOUBLE_PERIOD = 301, 
    GREATER_THAN_OR_EQUALS_OPERATOR = 302, LEFT_MINUS_RIGHT = 303, LEFT_MINUS_SLASH = 304, 
    LEFT_TILDE_SLASH = 305, LESS_THAN_OR_EQUALS_OPERATOR = 306, MINUS_SLASH = 307, 
    NOT_EQUALS_OPERATOR = 308, RIGHT_ARROW = 309, SLASH_MINUS_RIGHT = 310, 
    SLASH_TILDE_RIGHT = 311, TILDE_RIGHT_ARROW = 312, TILDE_SLASH = 313, 
    DOUBLE_SOLIDUS = 314, ESCAPED_CHARACTER = 315, ESCAPED_REVERSE_SOLIDUS = 316, 
    ESCAPED_QUOTE = 317, ESCAPED_DOUBLE_QUOTE = 318, ESCAPED_GRAVE_ACCENT = 319, 
    ESCAPED_TAB = 320, ESCAPED_BACKSPACE = 321, ESCAPED_NEWLINE = 322, ESCAPED_CARRIAGE_RETURN = 323, 
    ESCAPED_FORM_FEED = 324, UNICODE_ESCAPE_VALUE = 325, UNICODE_4_DIGIT_ESCAPE_VALUE = 326, 
    UNICODE_6_DIGIT_ESCAPE_VALUE = 327, UNSIGNED_DECIMAL_INTEGER = 328, 
    UNSIGNED_HEXADECIMAL_INTEGER = 329, UNSIGNED_OCTAL_INTEGER = 330, UNSIGNED_BINARY_INTEGER = 331, 
    UNSIGNED_DECIMAL_IN_SCIENTIFIC_NOTATION = 332, UNSIGNED_DECIMAL_IN_COMMON_NOTATION = 333, 
    FOUR_DIGIT = 334, DOUBLE_DIGIT = 335, SINGLE_QUOTED_STRING_LITERAL = 336, 
    DOUBLE_QUOTED_STRING_LITERAL = 337, ACCENT_QUOTED_STRING_LITERAL = 338, 
    BYTE_STRING_LITERAL = 339, QUOTE = 340, DOUBLE_QUOTE = 341, GRAVE_ACCENT = 342, 
    REGULAR_IDENTIFIER = 343, IDENTIFIER_START = 344, IDENTIFIER_EXTEND = 345, 
    SP = 346, SEPARATOR = 347, WHITESPACE = 348, Comment = 349, BIDIRECTIONAL_CONTROL_CHARACTER = 350, 
    COMMENT = 351, SIMPLE_COMMENT = 352, SIMPLE_COMMENT_INTRODUCER = 353, 
    SIMPLE_COMMENT_CHARACTER = 354, BRACKETED_COMMENT = 355, BRACKETED_COMMENT_INTRODUCER = 356, 
    BRACKETED_COMMENT_TERMINATOR = 357, BRACKETED_COMMENT_CONTENTS = 358, 
    GQL_TERMINAL_CHARACTER = 359, GQL_LANGUAGE_CHARACTER = 360, SIMPLE_LATIN_LETTER = 361, 
    SIMPLE_LATIN_LOWER_CASE_LETTER = 362, SIMPLE_LATIN_UPPER_CASE_LETTER = 363, 
    OTHER_DIGIT = 364, GQL_SPECIAL_CHARACTER = 365, OTHER_LANGUAGE_CHARACTER = 366
  };

  enum {
    RuleRoot = 0, RuleGqlProgram = 1, RuleSessionActivity = 2, RuleSessionActivityCommand = 3, 
    RuleTransactionActivity = 4, RuleEndTransactionCommand = 5, RuleSessionSetCommand = 6, 
    RuleSessionSetSchemaClause = 7, RuleSessionSetGraphClause = 8, RuleSessionSetTimeZoneClause = 9, 
    RuleSetTimeZoneValue = 10, RuleSessionSetParameterClause = 11, RuleSessionSetGraphParameterClause = 12, 
    RuleSessionSetBindingTableParameterClause = 13, RuleSessionSetValueParameterClause = 14, 
    RuleSessionSetParameterName = 15, RuleSessionResetCommand = 16, RuleSessionResetArguments = 17, 
    RuleSessionCloseCommand = 18, RuleStartTransactionCommand = 19, RuleTransactionMode = 20, 
    RuleImplementationDefinedAccessMode = 21, RuleRollbackCommand = 22, 
    RuleCommitCommand = 23, RuleNestedProcedureSpecification = 24, RuleUnsignedNumericLiteral = 25, 
    RuleIntegerLiteral = 26, RuleFloatLiteral = 27, RuleUnbrokenSingleQuotedCharacterSequence = 28, 
    RuleUnbrokenDoubleQuotedCharacterSequence = 29, RuleUnbrokenAccentQuotedCharacterSequence = 30, 
    RuleSingleQuotedCharacterSequence = 31, RuleDoubleQuotedCharacterSequence = 32, 
    RuleAccentQuotedCharacterSequence = 33, RuleUnbrokenCharacterStringLiteral = 34, 
    RuleNullLiteral = 35, RuleTemporalLiteral = 36, RuleSqlDatetimeLiteral = 37, 
    RuleDateLiteral = 38, RuleTimeLiteral = 39, RuleDatetimeLiteral = 40, 
    RuleDurationLiteral = 41, RuleSqlIntervalLiteral = 42, RuleSqlIntervalType = 43, 
    RuleIdentifier = 44, RuleDelimitedIdentifier = 45, RuleObjectName = 46, 
    RuleObjectNameOrBindingVariable = 47, RuleDirectoryName = 48, RuleSchemaName = 49, 
    RuleGraphName = 50, RuleDelimitedGraphName = 51, RuleGraphTypeName = 52, 
    RuleElementTypeName = 53, RuleBindingTableName = 54, RuleDelimitedBindingTableName = 55, 
    RuleProcedureName = 56, RuleLabelName = 57, RuleFunctionName = 58, RulePropertyName = 59, 
    RuleFieldName = 60, RuleParameterName = 61, RuleVariable = 62, RuleGraphVariable = 63, 
    RuleGraphPatternVariable = 64, RulePathOrSubpathVariable = 65, RuleElementVariable = 66, 
    RulePathVariable = 67, RuleSubpathVariable = 68, RuleBindingTableVariable = 69, 
    RuleValueVariable = 70, RuleBindingVariable = 71, RulePredefinedTypeLiteral = 72, 
    RuleBooleanLiteral = 73, RuleCharacterStringLiteral = 74, RuleByteStringLiteral = 75, 
    RuleProcedureBody = 76, RuleBindingVariableDefinitionBlock = 77, RuleBindingVariableDefinition = 78, 
    RuleStatementBlock = 79, RuleStatement = 80, RuleNextStatement = 81, 
    RuleGraphVariableDefinition = 82, RuleOptTypedGraphInitializer = 83, 
    RuleGraphInitializer = 84, RuleBindingTableVariableDefinition = 85, 
    RuleOptTypedBindingTableInitializer = 86, RuleBindingTableInitializer = 87, 
    RuleValueVariableDefinition = 88, RuleOptTypedValueInitializer = 89, 
    RuleGraphExpression = 90, RuleCurrentGraph = 91, RuleBindingTableExpression = 92, 
    RuleObjectExpressionPrimary = 93, RuleLinearCatalogModifyingStatement = 94, 
    RuleSimpleCatalogModifyingStatement = 95, RulePrimitiveCatalogModifyingStatement = 96, 
    RuleCreateSchemaStatement = 97, RuleDropSchemaStatement = 98, RuleCreateGraphStatement = 99, 
    RuleOpenGraphType = 100, RuleOfGraphType = 101, RuleGraphTypeLikeGraph = 102, 
    RuleGraphSource = 103, RuleDropGraphStatement = 104, RuleCreateGraphTypeStatement = 105, 
    RuleGraphTypeSource = 106, RuleCopyOfGraphType = 107, RuleDropGraphTypeStatement = 108, 
    RuleLinearDataModifyingStatement = 109, RuleLinearDataModifyingStatementBody = 110, 
    RuleNestedDataModifyingProcedure = 111, RuleSimpleDataAccessingStatement = 112, 
    RulePrimitiveDataModifyingStatement = 113, RuleInsertStatement = 114, 
    RuleSetStatement = 115, RuleSetItemList = 116, RuleSetItem = 117, RuleSetPropertyItem = 118, 
    RuleSetAllPropertiesItem = 119, RuleSetLabelItem = 120, RuleLabelSetSpecification = 121, 
    RuleRemoveStatement = 122, RuleRemoveItemList = 123, RuleRemoveItem = 124, 
    RuleRemovePropertyItem = 125, RuleRemoveLabelItem = 126, RuleDeleteStatement = 127, 
    RuleDeleteItemList = 128, RuleDeleteItem = 129, RuleCallDataModifyingProcedureStatement = 130, 
    RuleCompositeQueryExpression = 131, RuleQueryConjunction = 132, RuleSetOperator = 133, 
    RuleCompositeQueryPrimary = 134, RuleLinearQueryStatement = 135, RuleFocusedLinearQueryStatement = 136, 
    RuleFocusedLinearQueryStatementPart = 137, RuleFocusedLinearQueryAndPrimitiveResultStatementPart = 138, 
    RuleFocusedPrimitiveResultStatement = 139, RuleFocusedNestedQuerySpecification = 140, 
    RuleAmbientLinearQueryStatement = 141, RuleSimpleLinearQueryStatement = 142, 
    RulePrimitiveQueryStatement = 143, RuleMatchStatement = 144, RuleSimpleMatchStatement = 145, 
    RuleOptionalMatchStatement = 146, RuleOptionalOperand = 147, RuleMatchStatementBlock = 148, 
    RuleCallQueryStatement = 149, RuleFilterStatement = 150, RuleLetStatement = 151, 
    RuleLetVariableDefinitionList = 152, RuleLetVariableDefinition = 153, 
    RuleForStatement = 154, RuleForItem = 155, RuleForItemAlias = 156, RuleForOrdinalityOrOffset = 157, 
    RuleOrderByAndPageStatement = 158, RulePrimitiveResultStatement = 159, 
    RuleReturnStatement = 160, RuleReturnStatementBody = 161, RuleReturnItemList = 162, 
    RuleReturnItem = 163, RuleReturnItemAlias = 164, RuleSelectStatement = 165, 
    RuleSelectItemList = 166, RuleSelectItem = 167, RuleSelectItemAlias = 168, 
    RuleHavingClause = 169, RuleSelectStatementBody = 170, RuleSelectGraphMatchList = 171, 
    RuleSelectGraphMatch = 172, RuleSelectQuerySpecification = 173, RuleCallProcedureStatement = 174, 
    RuleProcedureCall = 175, RuleInlineProcedureCall = 176, RuleVariableScopeClause = 177, 
    RuleBindingVariableReferenceList = 178, RuleNamedProcedureCall = 179, 
    RuleProcedureArgumentList = 180, RuleProcedureArgument = 181, RuleUseGraphClause = 182, 
    RuleAtSchemaClause = 183, RuleBindingVariableReference = 184, RuleElementVariableReference = 185, 
    RulePathVariableReference = 186, RuleParameter = 187, RuleGraphPatternYieldClause = 188, 
    RuleGraphPatternYieldItemList = 189, RuleGraphPatternYieldItem = 190, 
    RuleGraphPattern = 191, RuleMatchMode = 192, RuleRepeatableElementsMatchMode = 193, 
    RuleDifferentEdgesMatchMode = 194, RuleElementBindingsOrElements = 195, 
    RuleEdgeBindingsOrEdges = 196, RulePathPatternList = 197, RulePathPattern = 198, 
    RulePathVariableDeclaration = 199, RuleKeepClause = 200, RuleGraphPatternWhereClause = 201, 
    RulePathPatternPrefix = 202, RulePathModePrefix = 203, RulePathMode = 204, 
    RulePathSearchPrefix = 205, RuleAllPathSearch = 206, RulePathOrPaths = 207, 
    RuleAnyPathSearch = 208, RuleNumberOfPaths = 209, RuleShortestPathSearch = 210, 
    RuleAllShortestPathSearch = 211, RuleAnyShortestPathSearch = 212, RuleCountedShortestPathSearch = 213, 
    RuleCountedShortestGroupSearch = 214, RuleNumberOfGroups = 215, RulePathPatternExpression = 216, 
    RulePathMultisetAlternation = 217, RulePathPatternUnion = 218, RulePathTerm = 219, 
    RulePathFactor = 220, RuleQuantifiedPathPrimary = 221, RuleQuestionedPathPrimary = 222, 
    RulePathPrimary = 223, RuleElementPattern = 224, RuleNodePattern = 225, 
    RuleElementPatternFiller = 226, RuleElementVariableDeclaration = 227, 
    RuleIsLabelExpression = 228, RuleIsOrColon = 229, RuleElementPatternPredicate = 230, 
    RuleElementPatternWhereClause = 231, RuleElementPropertySpecification = 232, 
    RulePropertyKeyValuePairList = 233, RulePropertyKeyValuePair = 234, 
    RuleEdgePattern = 235, RuleFullEdgePattern = 236, RuleFullEdgePointingLeft = 237, 
    RuleFullEdgeUndirected = 238, RuleFullEdgePointingRight = 239, RuleFullEdgeLeftOrUndirected = 240, 
    RuleFullEdgeUndirectedOrRight = 241, RuleFullEdgeLeftOrRight = 242, 
    RuleFullEdgeAnyDirection = 243, RuleAbbreviatedEdgePattern = 244, RuleParenthesizedPathPatternExpression = 245, 
    RuleSubpathVariableDeclaration = 246, RuleParenthesizedPathPatternWhereClause = 247, 
    RuleInsertGraphPattern = 248, RuleInsertPathPatternList = 249, RuleInsertPathPattern = 250, 
    RuleInsertNodePattern = 251, RuleInsertEdgePattern = 252, RuleInsertEdgePointingLeft = 253, 
    RuleInsertEdgePointingRight = 254, RuleInsertEdgeUndirected = 255, RuleInsertElementPatternFiller = 256, 
    RuleLabelAndPropertySetSpecification = 257, RuleLabelExpression = 258, 
    RuleLabelTerm = 259, RuleLabelFactor = 260, RuleLabelPrimary = 261, 
    RuleWildcardLabel = 262, RuleParenthesizedLabelExpression = 263, RuleGraphPatternQuantifier = 264, 
    RuleFixedQuantifier = 265, RuleGeneralQuantifier = 266, RuleLowerBound = 267, 
    RuleUpperBound = 268, RuleSimplifiedPathPatternExpression = 269, RuleSimplifiedDefaultingLeft = 270, 
    RuleSimplifiedDefaultingUndirected = 271, RuleSimplifiedDefaultingRight = 272, 
    RuleSimplifiedDefaultingLeftOrUndirected = 273, RuleSimplifiedDefaultingUndirectedOrRight = 274, 
    RuleSimplifiedDefaultingLeftOrRight = 275, RuleSimplifiedDefaultingAnyDirection = 276, 
    RuleSimplifiedContents = 277, RuleSimplifiedPathUnion = 278, RuleSimplifiedMultisetAlternation = 279, 
    RuleSimplifiedTerm = 280, RuleSimplifiedFactorLow = 281, RuleSimplifiedFactorHigh = 282, 
    RuleSimplifiedQuantified = 283, RuleSimplifiedQuestioned = 284, RuleSimplifiedTertiary = 285, 
    RuleSimplifiedDirectionOverride = 286, RuleSimplifiedOverrideLeft = 287, 
    RuleSimplifiedOverrideUndirected = 288, RuleSimplifiedOverrideRight = 289, 
    RuleSimplifiedOverrideLeftOrUndirected = 290, RuleSimplifiedOverrideUndirectedOrRight = 291, 
    RuleSimplifiedOverrideLeftOrRight = 292, RuleSimplifiedOverrideAnyDirection = 293, 
    RuleSimplifiedSecondary = 294, RuleSimplifiedNegation = 295, RuleSimplifiedPrimary = 296, 
    RuleWhereClause = 297, RuleYieldClause = 298, RuleYieldItemList = 299, 
    RuleYieldItem = 300, RuleYieldItemName = 301, RuleYieldItemAlias = 302, 
    RuleGroupByClause = 303, RuleGroupingElementList = 304, RuleGroupingElement = 305, 
    RuleEmptyGroupingSet = 306, RuleOrderByClause = 307, RuleAggregateFunction = 308, 
    RuleGeneralSetFunctionType = 309, RuleSetQuantifier = 310, RuleBinarySetFunctionType = 311, 
    RuleSortSpecificationList = 312, RuleSortSpecification = 313, RuleSortKey = 314, 
    RuleOrderingSpecification = 315, RuleNullOrdering = 316, RuleLimitClause = 317, 
    RuleOffsetClause = 318, RuleOffsetSynonym = 319, RuleNestedGraphTypeSpecification = 320, 
    RuleGraphTypeSpecificationBody = 321, RuleElementTypeDefinitionList = 322, 
    RuleElementTypeDefinition = 323, RuleNodeTypeDefinition = 324, RuleNodeTypePattern = 325, 
    RuleNodeTypePhrase = 326, RuleNodeTypeName = 327, RuleNodeTypeFiller = 328, 
    RuleNodeTypeLabelSetDefinition = 329, RuleNodeTypePropertyTypeSetDefinition = 330, 
    RuleEdgeTypeDefinition = 331, RuleEdgeTypePattern = 332, RuleEdgeTypePhrase = 333, 
    RuleEdgeTypeName = 334, RuleEdgeTypeFiller = 335, RuleEdgeTypeLabelSetDefinition = 336, 
    RuleEdgeTypePropertyTypeSetDefinition = 337, RuleFullEdgeTypePattern = 338, 
    RuleFullEdgeTypePatternPointingRight = 339, RuleFullEdgeTypePatternPointingLeft = 340, 
    RuleFullEdgeTypePatternUndirected = 341, RuleArcTypePointingRight = 342, 
    RuleArcTypePointingLeft = 343, RuleArcTypeUndirected = 344, RuleArcTypeFiller = 345, 
    RuleAbbreviatedEdgeTypePattern = 346, RuleAbbreviatedEdgeTypePatternPointingRight = 347, 
    RuleAbbreviatedEdgeTypePatternPointingLeft = 348, RuleAbbreviatedEdgeTypePatternUndirected = 349, 
    RuleNodeTypeReference = 350, RuleSourceNodeTypeReference = 351, RuleDestinationNodeTypeReference = 352, 
    RuleEdgeKind = 353, RuleEndpointDefinition = 354, RuleEndpointPairDefinition = 355, 
    RuleEndpointPairDefinitionPointingRight = 356, RuleEndpointPairDefinitionPointingLeft = 357, 
    RuleEndpointPairDefinitionUndirected = 358, RuleConnectorPointingRight = 359, 
    RuleConnectorUndirected = 360, RuleSourceNodeTypeName = 361, RuleDestinationNodeTypeName = 362, 
    RuleLabelSetDefinition = 363, RulePropertyTypeSetDefinition = 364, RulePropertyTypeDefinitionList = 365, 
    RulePropertyTypeDefinition = 366, RulePropertyValueType = 367, RuleBindingTableType = 368, 
    RuleValueType = 369, RuleTyped = 370, RulePredefinedType = 371, RuleBooleanType = 372, 
    RuleCharacterStringType = 373, RuleByteStringType = 374, RuleMinLength = 375, 
    RuleMaxLength = 376, RuleFixedLength = 377, RuleNumericType = 378, RuleExactNumericType = 379, 
    RuleBinaryExactNumericType = 380, RuleSignedBinaryExactNumericType = 381, 
    RuleUnsignedBinaryExactNumericType = 382, RuleVerboseBinaryExactNumericType = 383, 
    RuleDecimalExactNumericType = 384, RulePrecision = 385, RuleScale = 386, 
    RuleApproximateNumericType = 387, RuleTemporalType = 388, RuleTemporalInstantType = 389, 
    RuleTemporalDurationType = 390, RuleDatetimeType = 391, RuleLocaldatetimeType = 392, 
    RuleDateType = 393, RuleTimeType = 394, RuleLocaltimeType = 395, RuleDurationType = 396, 
    RuleReferenceValueType = 397, RuleGraphReferenceValueType = 398, RuleClosedGraphReferenceValueType = 399, 
    RuleOpenGraphReferenceValueType = 400, RuleBindingTableReferenceValueType = 401, 
    RuleNodeReferenceValueType = 402, RuleClosedNodeReferenceValueType = 403, 
    RuleOpenNodeReferenceValueType = 404, RuleEdgeReferenceValueType = 405, 
    RuleClosedEdgeReferenceValueType = 406, RuleOpenEdgeReferenceValueType = 407, 
    RuleListValueTypeName = 408, RuleListValueTypeNameSynonym = 409, RuleFieldTypesSpecification = 410, 
    RuleFieldTypeList = 411, RulePathValueType = 412, RuleNotNull = 413, 
    RuleFieldType = 414, RuleSchemaReference = 415, RuleAbsoluteCatalogSchemaReference = 416, 
    RuleCatalogSchemaParentAndName = 417, RuleRelativeCatalogSchemaReference = 418, 
    RulePredefinedSchemaReference = 419, RuleAbsoluteDirectoryPath = 420, 
    RuleRelativeDirectoryPath = 421, RuleSimpleDirectoryPath = 422, RuleGraphReference = 423, 
    RuleCatalogGraphParentAndName = 424, RuleHomeGraph = 425, RuleGraphTypeReference = 426, 
    RuleCatalogGraphTypeParentAndName = 427, RuleBindingTableReference = 428, 
    RuleCatalogBindingTableParentAndName = 429, RuleProcedureReference = 430, 
    RuleCatalogProcedureParentAndName = 431, RuleCatalogObjectParentReference = 432, 
    RuleReferenceParameter = 433, RuleExternalObjectReference = 434, RuleComparisonPredicateCond = 435, 
    RuleCompOp = 436, RuleNullPredicateCond = 437, RuleNormalizedPredicateCond = 438, 
    RuleDirectedPredicateCond = 439, RuleLabeledPredicateCond = 440, RuleSourceDestinationPredicateCond = 441, 
    RuleUnsignedValueSpecification = 442, RuleUnsignedIntegerSpecification = 443, 
    RuleParameterValueSpecification = 444, RulePredefinedParameter = 445, 
    RuleExpression = 446, RuleExpressionPredicate = 447, RuleExpressionAtom = 448, 
    RuleTruthValue = 449, RuleUnaryOperator = 450, RuleFunctionCall = 451, 
    RuleNumericFunction = 452, RuleFunctionParameter = 453, RulePropertyReference = 454, 
    RuleOneArgNumericFunctionName = 455, RuleTwoArgNumericFunctionName = 456, 
    RuleStringFunction = 457, RuleListFunction = 458, RuleCaseFunction = 459, 
    RuleLabelsFunction = 460, RuleSimpleWhenClause = 461, RuleSearchedWhenClause = 462, 
    RuleElseClause = 463, RuleWhenOperand = 464, RuleCastFunction = 465, 
    RuleElementIdFunction = 466, RuleDatetimeValueFunction = 467, RuleDateFunction = 468, 
    RuleTimeFunction = 469, RuleLocalTimeFunction = 470, RuleDatetimeFunction = 471, 
    RuleLocalDatetimeFunction = 472, RuleDateFunctionParameters = 473, RuleTimeFunctionParameters = 474, 
    RuleDatetimeFunctionParameters = 475, RuleDateString = 476, RuleTimeString = 477, 
    RuleDatetimeString = 478, RuleDurationFunction = 479, RuleDurationFunctionParameters = 480, 
    RuleDurationString = 481, RuleGeneralFunction = 482, RuleCollectionValueConstructor = 483, 
    RuleTrimSpecification = 484, RuleNormalForm = 485, RuleListValueConstructor = 486, 
    RuleRecordValueConstructor = 487, RuleField = 488, RulePathValueConstructor = 489, 
    RuleUnsignedLiteral = 490, RuleGeneralLiteral = 491, RuleListLiteral = 492, 
    RuleRecordLiteral = 493, RuleRecordFieldLiteral = 494, RuleKeyword = 495
  };

  explicit GQLParser(antlr4::TokenStream *input);

  GQLParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~GQLParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class RootContext;
  class GqlProgramContext;
  class SessionActivityContext;
  class SessionActivityCommandContext;
  class TransactionActivityContext;
  class EndTransactionCommandContext;
  class SessionSetCommandContext;
  class SessionSetSchemaClauseContext;
  class SessionSetGraphClauseContext;
  class SessionSetTimeZoneClauseContext;
  class SetTimeZoneValueContext;
  class SessionSetParameterClauseContext;
  class SessionSetGraphParameterClauseContext;
  class SessionSetBindingTableParameterClauseContext;
  class SessionSetValueParameterClauseContext;
  class SessionSetParameterNameContext;
  class SessionResetCommandContext;
  class SessionResetArgumentsContext;
  class SessionCloseCommandContext;
  class StartTransactionCommandContext;
  class TransactionModeContext;
  class ImplementationDefinedAccessModeContext;
  class RollbackCommandContext;
  class CommitCommandContext;
  class NestedProcedureSpecificationContext;
  class UnsignedNumericLiteralContext;
  class IntegerLiteralContext;
  class FloatLiteralContext;
  class UnbrokenSingleQuotedCharacterSequenceContext;
  class UnbrokenDoubleQuotedCharacterSequenceContext;
  class UnbrokenAccentQuotedCharacterSequenceContext;
  class SingleQuotedCharacterSequenceContext;
  class DoubleQuotedCharacterSequenceContext;
  class AccentQuotedCharacterSequenceContext;
  class UnbrokenCharacterStringLiteralContext;
  class NullLiteralContext;
  class TemporalLiteralContext;
  class SqlDatetimeLiteralContext;
  class DateLiteralContext;
  class TimeLiteralContext;
  class DatetimeLiteralContext;
  class DurationLiteralContext;
  class SqlIntervalLiteralContext;
  class SqlIntervalTypeContext;
  class IdentifierContext;
  class DelimitedIdentifierContext;
  class ObjectNameContext;
  class ObjectNameOrBindingVariableContext;
  class DirectoryNameContext;
  class SchemaNameContext;
  class GraphNameContext;
  class DelimitedGraphNameContext;
  class GraphTypeNameContext;
  class ElementTypeNameContext;
  class BindingTableNameContext;
  class DelimitedBindingTableNameContext;
  class ProcedureNameContext;
  class LabelNameContext;
  class FunctionNameContext;
  class PropertyNameContext;
  class FieldNameContext;
  class ParameterNameContext;
  class VariableContext;
  class GraphVariableContext;
  class GraphPatternVariableContext;
  class PathOrSubpathVariableContext;
  class ElementVariableContext;
  class PathVariableContext;
  class SubpathVariableContext;
  class BindingTableVariableContext;
  class ValueVariableContext;
  class BindingVariableContext;
  class PredefinedTypeLiteralContext;
  class BooleanLiteralContext;
  class CharacterStringLiteralContext;
  class ByteStringLiteralContext;
  class ProcedureBodyContext;
  class BindingVariableDefinitionBlockContext;
  class BindingVariableDefinitionContext;
  class StatementBlockContext;
  class StatementContext;
  class NextStatementContext;
  class GraphVariableDefinitionContext;
  class OptTypedGraphInitializerContext;
  class GraphInitializerContext;
  class BindingTableVariableDefinitionContext;
  class OptTypedBindingTableInitializerContext;
  class BindingTableInitializerContext;
  class ValueVariableDefinitionContext;
  class OptTypedValueInitializerContext;
  class GraphExpressionContext;
  class CurrentGraphContext;
  class BindingTableExpressionContext;
  class ObjectExpressionPrimaryContext;
  class LinearCatalogModifyingStatementContext;
  class SimpleCatalogModifyingStatementContext;
  class PrimitiveCatalogModifyingStatementContext;
  class CreateSchemaStatementContext;
  class DropSchemaStatementContext;
  class CreateGraphStatementContext;
  class OpenGraphTypeContext;
  class OfGraphTypeContext;
  class GraphTypeLikeGraphContext;
  class GraphSourceContext;
  class DropGraphStatementContext;
  class CreateGraphTypeStatementContext;
  class GraphTypeSourceContext;
  class CopyOfGraphTypeContext;
  class DropGraphTypeStatementContext;
  class LinearDataModifyingStatementContext;
  class LinearDataModifyingStatementBodyContext;
  class NestedDataModifyingProcedureContext;
  class SimpleDataAccessingStatementContext;
  class PrimitiveDataModifyingStatementContext;
  class InsertStatementContext;
  class SetStatementContext;
  class SetItemListContext;
  class SetItemContext;
  class SetPropertyItemContext;
  class SetAllPropertiesItemContext;
  class SetLabelItemContext;
  class LabelSetSpecificationContext;
  class RemoveStatementContext;
  class RemoveItemListContext;
  class RemoveItemContext;
  class RemovePropertyItemContext;
  class RemoveLabelItemContext;
  class DeleteStatementContext;
  class DeleteItemListContext;
  class DeleteItemContext;
  class CallDataModifyingProcedureStatementContext;
  class CompositeQueryExpressionContext;
  class QueryConjunctionContext;
  class SetOperatorContext;
  class CompositeQueryPrimaryContext;
  class LinearQueryStatementContext;
  class FocusedLinearQueryStatementContext;
  class FocusedLinearQueryStatementPartContext;
  class FocusedLinearQueryAndPrimitiveResultStatementPartContext;
  class FocusedPrimitiveResultStatementContext;
  class FocusedNestedQuerySpecificationContext;
  class AmbientLinearQueryStatementContext;
  class SimpleLinearQueryStatementContext;
  class PrimitiveQueryStatementContext;
  class MatchStatementContext;
  class SimpleMatchStatementContext;
  class OptionalMatchStatementContext;
  class OptionalOperandContext;
  class MatchStatementBlockContext;
  class CallQueryStatementContext;
  class FilterStatementContext;
  class LetStatementContext;
  class LetVariableDefinitionListContext;
  class LetVariableDefinitionContext;
  class ForStatementContext;
  class ForItemContext;
  class ForItemAliasContext;
  class ForOrdinalityOrOffsetContext;
  class OrderByAndPageStatementContext;
  class PrimitiveResultStatementContext;
  class ReturnStatementContext;
  class ReturnStatementBodyContext;
  class ReturnItemListContext;
  class ReturnItemContext;
  class ReturnItemAliasContext;
  class SelectStatementContext;
  class SelectItemListContext;
  class SelectItemContext;
  class SelectItemAliasContext;
  class HavingClauseContext;
  class SelectStatementBodyContext;
  class SelectGraphMatchListContext;
  class SelectGraphMatchContext;
  class SelectQuerySpecificationContext;
  class CallProcedureStatementContext;
  class ProcedureCallContext;
  class InlineProcedureCallContext;
  class VariableScopeClauseContext;
  class BindingVariableReferenceListContext;
  class NamedProcedureCallContext;
  class ProcedureArgumentListContext;
  class ProcedureArgumentContext;
  class UseGraphClauseContext;
  class AtSchemaClauseContext;
  class BindingVariableReferenceContext;
  class ElementVariableReferenceContext;
  class PathVariableReferenceContext;
  class ParameterContext;
  class GraphPatternYieldClauseContext;
  class GraphPatternYieldItemListContext;
  class GraphPatternYieldItemContext;
  class GraphPatternContext;
  class MatchModeContext;
  class RepeatableElementsMatchModeContext;
  class DifferentEdgesMatchModeContext;
  class ElementBindingsOrElementsContext;
  class EdgeBindingsOrEdgesContext;
  class PathPatternListContext;
  class PathPatternContext;
  class PathVariableDeclarationContext;
  class KeepClauseContext;
  class GraphPatternWhereClauseContext;
  class PathPatternPrefixContext;
  class PathModePrefixContext;
  class PathModeContext;
  class PathSearchPrefixContext;
  class AllPathSearchContext;
  class PathOrPathsContext;
  class AnyPathSearchContext;
  class NumberOfPathsContext;
  class ShortestPathSearchContext;
  class AllShortestPathSearchContext;
  class AnyShortestPathSearchContext;
  class CountedShortestPathSearchContext;
  class CountedShortestGroupSearchContext;
  class NumberOfGroupsContext;
  class PathPatternExpressionContext;
  class PathMultisetAlternationContext;
  class PathPatternUnionContext;
  class PathTermContext;
  class PathFactorContext;
  class QuantifiedPathPrimaryContext;
  class QuestionedPathPrimaryContext;
  class PathPrimaryContext;
  class ElementPatternContext;
  class NodePatternContext;
  class ElementPatternFillerContext;
  class ElementVariableDeclarationContext;
  class IsLabelExpressionContext;
  class IsOrColonContext;
  class ElementPatternPredicateContext;
  class ElementPatternWhereClauseContext;
  class ElementPropertySpecificationContext;
  class PropertyKeyValuePairListContext;
  class PropertyKeyValuePairContext;
  class EdgePatternContext;
  class FullEdgePatternContext;
  class FullEdgePointingLeftContext;
  class FullEdgeUndirectedContext;
  class FullEdgePointingRightContext;
  class FullEdgeLeftOrUndirectedContext;
  class FullEdgeUndirectedOrRightContext;
  class FullEdgeLeftOrRightContext;
  class FullEdgeAnyDirectionContext;
  class AbbreviatedEdgePatternContext;
  class ParenthesizedPathPatternExpressionContext;
  class SubpathVariableDeclarationContext;
  class ParenthesizedPathPatternWhereClauseContext;
  class InsertGraphPatternContext;
  class InsertPathPatternListContext;
  class InsertPathPatternContext;
  class InsertNodePatternContext;
  class InsertEdgePatternContext;
  class InsertEdgePointingLeftContext;
  class InsertEdgePointingRightContext;
  class InsertEdgeUndirectedContext;
  class InsertElementPatternFillerContext;
  class LabelAndPropertySetSpecificationContext;
  class LabelExpressionContext;
  class LabelTermContext;
  class LabelFactorContext;
  class LabelPrimaryContext;
  class WildcardLabelContext;
  class ParenthesizedLabelExpressionContext;
  class GraphPatternQuantifierContext;
  class FixedQuantifierContext;
  class GeneralQuantifierContext;
  class LowerBoundContext;
  class UpperBoundContext;
  class SimplifiedPathPatternExpressionContext;
  class SimplifiedDefaultingLeftContext;
  class SimplifiedDefaultingUndirectedContext;
  class SimplifiedDefaultingRightContext;
  class SimplifiedDefaultingLeftOrUndirectedContext;
  class SimplifiedDefaultingUndirectedOrRightContext;
  class SimplifiedDefaultingLeftOrRightContext;
  class SimplifiedDefaultingAnyDirectionContext;
  class SimplifiedContentsContext;
  class SimplifiedPathUnionContext;
  class SimplifiedMultisetAlternationContext;
  class SimplifiedTermContext;
  class SimplifiedFactorLowContext;
  class SimplifiedFactorHighContext;
  class SimplifiedQuantifiedContext;
  class SimplifiedQuestionedContext;
  class SimplifiedTertiaryContext;
  class SimplifiedDirectionOverrideContext;
  class SimplifiedOverrideLeftContext;
  class SimplifiedOverrideUndirectedContext;
  class SimplifiedOverrideRightContext;
  class SimplifiedOverrideLeftOrUndirectedContext;
  class SimplifiedOverrideUndirectedOrRightContext;
  class SimplifiedOverrideLeftOrRightContext;
  class SimplifiedOverrideAnyDirectionContext;
  class SimplifiedSecondaryContext;
  class SimplifiedNegationContext;
  class SimplifiedPrimaryContext;
  class WhereClauseContext;
  class YieldClauseContext;
  class YieldItemListContext;
  class YieldItemContext;
  class YieldItemNameContext;
  class YieldItemAliasContext;
  class GroupByClauseContext;
  class GroupingElementListContext;
  class GroupingElementContext;
  class EmptyGroupingSetContext;
  class OrderByClauseContext;
  class AggregateFunctionContext;
  class GeneralSetFunctionTypeContext;
  class SetQuantifierContext;
  class BinarySetFunctionTypeContext;
  class SortSpecificationListContext;
  class SortSpecificationContext;
  class SortKeyContext;
  class OrderingSpecificationContext;
  class NullOrderingContext;
  class LimitClauseContext;
  class OffsetClauseContext;
  class OffsetSynonymContext;
  class NestedGraphTypeSpecificationContext;
  class GraphTypeSpecificationBodyContext;
  class ElementTypeDefinitionListContext;
  class ElementTypeDefinitionContext;
  class NodeTypeDefinitionContext;
  class NodeTypePatternContext;
  class NodeTypePhraseContext;
  class NodeTypeNameContext;
  class NodeTypeFillerContext;
  class NodeTypeLabelSetDefinitionContext;
  class NodeTypePropertyTypeSetDefinitionContext;
  class EdgeTypeDefinitionContext;
  class EdgeTypePatternContext;
  class EdgeTypePhraseContext;
  class EdgeTypeNameContext;
  class EdgeTypeFillerContext;
  class EdgeTypeLabelSetDefinitionContext;
  class EdgeTypePropertyTypeSetDefinitionContext;
  class FullEdgeTypePatternContext;
  class FullEdgeTypePatternPointingRightContext;
  class FullEdgeTypePatternPointingLeftContext;
  class FullEdgeTypePatternUndirectedContext;
  class ArcTypePointingRightContext;
  class ArcTypePointingLeftContext;
  class ArcTypeUndirectedContext;
  class ArcTypeFillerContext;
  class AbbreviatedEdgeTypePatternContext;
  class AbbreviatedEdgeTypePatternPointingRightContext;
  class AbbreviatedEdgeTypePatternPointingLeftContext;
  class AbbreviatedEdgeTypePatternUndirectedContext;
  class NodeTypeReferenceContext;
  class SourceNodeTypeReferenceContext;
  class DestinationNodeTypeReferenceContext;
  class EdgeKindContext;
  class EndpointDefinitionContext;
  class EndpointPairDefinitionContext;
  class EndpointPairDefinitionPointingRightContext;
  class EndpointPairDefinitionPointingLeftContext;
  class EndpointPairDefinitionUndirectedContext;
  class ConnectorPointingRightContext;
  class ConnectorUndirectedContext;
  class SourceNodeTypeNameContext;
  class DestinationNodeTypeNameContext;
  class LabelSetDefinitionContext;
  class PropertyTypeSetDefinitionContext;
  class PropertyTypeDefinitionListContext;
  class PropertyTypeDefinitionContext;
  class PropertyValueTypeContext;
  class BindingTableTypeContext;
  class ValueTypeContext;
  class TypedContext;
  class PredefinedTypeContext;
  class BooleanTypeContext;
  class CharacterStringTypeContext;
  class ByteStringTypeContext;
  class MinLengthContext;
  class MaxLengthContext;
  class FixedLengthContext;
  class NumericTypeContext;
  class ExactNumericTypeContext;
  class BinaryExactNumericTypeContext;
  class SignedBinaryExactNumericTypeContext;
  class UnsignedBinaryExactNumericTypeContext;
  class VerboseBinaryExactNumericTypeContext;
  class DecimalExactNumericTypeContext;
  class PrecisionContext;
  class ScaleContext;
  class ApproximateNumericTypeContext;
  class TemporalTypeContext;
  class TemporalInstantTypeContext;
  class TemporalDurationTypeContext;
  class DatetimeTypeContext;
  class LocaldatetimeTypeContext;
  class DateTypeContext;
  class TimeTypeContext;
  class LocaltimeTypeContext;
  class DurationTypeContext;
  class ReferenceValueTypeContext;
  class GraphReferenceValueTypeContext;
  class ClosedGraphReferenceValueTypeContext;
  class OpenGraphReferenceValueTypeContext;
  class BindingTableReferenceValueTypeContext;
  class NodeReferenceValueTypeContext;
  class ClosedNodeReferenceValueTypeContext;
  class OpenNodeReferenceValueTypeContext;
  class EdgeReferenceValueTypeContext;
  class ClosedEdgeReferenceValueTypeContext;
  class OpenEdgeReferenceValueTypeContext;
  class ListValueTypeNameContext;
  class ListValueTypeNameSynonymContext;
  class FieldTypesSpecificationContext;
  class FieldTypeListContext;
  class PathValueTypeContext;
  class NotNullContext;
  class FieldTypeContext;
  class SchemaReferenceContext;
  class AbsoluteCatalogSchemaReferenceContext;
  class CatalogSchemaParentAndNameContext;
  class RelativeCatalogSchemaReferenceContext;
  class PredefinedSchemaReferenceContext;
  class AbsoluteDirectoryPathContext;
  class RelativeDirectoryPathContext;
  class SimpleDirectoryPathContext;
  class GraphReferenceContext;
  class CatalogGraphParentAndNameContext;
  class HomeGraphContext;
  class GraphTypeReferenceContext;
  class CatalogGraphTypeParentAndNameContext;
  class BindingTableReferenceContext;
  class CatalogBindingTableParentAndNameContext;
  class ProcedureReferenceContext;
  class CatalogProcedureParentAndNameContext;
  class CatalogObjectParentReferenceContext;
  class ReferenceParameterContext;
  class ExternalObjectReferenceContext;
  class ComparisonPredicateCondContext;
  class CompOpContext;
  class NullPredicateCondContext;
  class NormalizedPredicateCondContext;
  class DirectedPredicateCondContext;
  class LabeledPredicateCondContext;
  class SourceDestinationPredicateCondContext;
  class UnsignedValueSpecificationContext;
  class UnsignedIntegerSpecificationContext;
  class ParameterValueSpecificationContext;
  class PredefinedParameterContext;
  class ExpressionContext;
  class ExpressionPredicateContext;
  class ExpressionAtomContext;
  class TruthValueContext;
  class UnaryOperatorContext;
  class FunctionCallContext;
  class NumericFunctionContext;
  class FunctionParameterContext;
  class PropertyReferenceContext;
  class OneArgNumericFunctionNameContext;
  class TwoArgNumericFunctionNameContext;
  class StringFunctionContext;
  class ListFunctionContext;
  class CaseFunctionContext;
  class LabelsFunctionContext;
  class SimpleWhenClauseContext;
  class SearchedWhenClauseContext;
  class ElseClauseContext;
  class WhenOperandContext;
  class CastFunctionContext;
  class ElementIdFunctionContext;
  class DatetimeValueFunctionContext;
  class DateFunctionContext;
  class TimeFunctionContext;
  class LocalTimeFunctionContext;
  class DatetimeFunctionContext;
  class LocalDatetimeFunctionContext;
  class DateFunctionParametersContext;
  class TimeFunctionParametersContext;
  class DatetimeFunctionParametersContext;
  class DateStringContext;
  class TimeStringContext;
  class DatetimeStringContext;
  class DurationFunctionContext;
  class DurationFunctionParametersContext;
  class DurationStringContext;
  class GeneralFunctionContext;
  class CollectionValueConstructorContext;
  class TrimSpecificationContext;
  class NormalFormContext;
  class ListValueConstructorContext;
  class RecordValueConstructorContext;
  class FieldContext;
  class PathValueConstructorContext;
  class UnsignedLiteralContext;
  class GeneralLiteralContext;
  class ListLiteralContext;
  class RecordLiteralContext;
  class RecordFieldLiteralContext;
  class KeywordContext; 

  class  RootContext : public antlr4::ParserRuleContext {
  public:
    RootContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GqlProgramContext *gqlProgram();
    antlr4::tree::TerminalNode *EOF();
    antlr4::tree::TerminalNode *SEMICOLON();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootContext* root();

  class  GqlProgramContext : public antlr4::ParserRuleContext {
  public:
    GqlProgramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SessionActivityContext *sessionActivity();
    TransactionActivityContext *transactionActivity();
    SessionCloseCommandContext *sessionCloseCommand();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GqlProgramContext* gqlProgram();

  class  SessionActivityContext : public antlr4::ParserRuleContext {
  public:
    SessionActivityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SessionActivityCommandContext *> sessionActivityCommand();
    SessionActivityCommandContext* sessionActivityCommand(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionActivityContext* sessionActivity();

  class  SessionActivityCommandContext : public antlr4::ParserRuleContext {
  public:
    SessionActivityCommandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SessionSetCommandContext *sessionSetCommand();
    SessionResetCommandContext *sessionResetCommand();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionActivityCommandContext* sessionActivityCommand();

  class  TransactionActivityContext : public antlr4::ParserRuleContext {
  public:
    TransactionActivityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StartTransactionCommandContext *startTransactionCommand();
    ProcedureBodyContext *procedureBody();
    EndTransactionCommandContext *endTransactionCommand();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TransactionActivityContext* transactionActivity();

  class  EndTransactionCommandContext : public antlr4::ParserRuleContext {
  public:
    EndTransactionCommandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RollbackCommandContext *rollbackCommand();
    CommitCommandContext *commitCommand();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EndTransactionCommandContext* endTransactionCommand();

  class  SessionSetCommandContext : public antlr4::ParserRuleContext {
  public:
    SessionSetCommandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SESSION();
    antlr4::tree::TerminalNode *SET();
    SessionSetSchemaClauseContext *sessionSetSchemaClause();
    SessionSetGraphClauseContext *sessionSetGraphClause();
    SessionSetTimeZoneClauseContext *sessionSetTimeZoneClause();
    SessionSetParameterClauseContext *sessionSetParameterClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionSetCommandContext* sessionSetCommand();

  class  SessionSetSchemaClauseContext : public antlr4::ParserRuleContext {
  public:
    SessionSetSchemaClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SCHEMA();
    SchemaReferenceContext *schemaReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionSetSchemaClauseContext* sessionSetSchemaClause();

  class  SessionSetGraphClauseContext : public antlr4::ParserRuleContext {
  public:
    SessionSetGraphClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GRAPH();
    GraphExpressionContext *graphExpression();
    antlr4::tree::TerminalNode *PROPERTY();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionSetGraphClauseContext* sessionSetGraphClause();

  class  SessionSetTimeZoneClauseContext : public antlr4::ParserRuleContext {
  public:
    SessionSetTimeZoneClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *ZONE();
    SetTimeZoneValueContext *setTimeZoneValue();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionSetTimeZoneClauseContext* sessionSetTimeZoneClause();

  class  SetTimeZoneValueContext : public antlr4::ParserRuleContext {
  public:
    SetTimeZoneValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionAtomContext *expressionAtom();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetTimeZoneValueContext* setTimeZoneValue();

  class  SessionSetParameterClauseContext : public antlr4::ParserRuleContext {
  public:
    SessionSetParameterClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SessionSetGraphParameterClauseContext *sessionSetGraphParameterClause();
    SessionSetBindingTableParameterClauseContext *sessionSetBindingTableParameterClause();
    SessionSetValueParameterClauseContext *sessionSetValueParameterClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionSetParameterClauseContext* sessionSetParameterClause();

  class  SessionSetGraphParameterClauseContext : public antlr4::ParserRuleContext {
  public:
    SessionSetGraphParameterClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GRAPH();
    SessionSetParameterNameContext *sessionSetParameterName();
    OptTypedGraphInitializerContext *optTypedGraphInitializer();
    antlr4::tree::TerminalNode *PROPERTY();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionSetGraphParameterClauseContext* sessionSetGraphParameterClause();

  class  SessionSetBindingTableParameterClauseContext : public antlr4::ParserRuleContext {
  public:
    SessionSetBindingTableParameterClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TABLE();
    SessionSetParameterNameContext *sessionSetParameterName();
    OptTypedBindingTableInitializerContext *optTypedBindingTableInitializer();
    antlr4::tree::TerminalNode *BINDING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionSetBindingTableParameterClauseContext* sessionSetBindingTableParameterClause();

  class  SessionSetValueParameterClauseContext : public antlr4::ParserRuleContext {
  public:
    SessionSetValueParameterClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VALUE();
    SessionSetParameterNameContext *sessionSetParameterName();
    OptTypedValueInitializerContext *optTypedValueInitializer();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionSetValueParameterClauseContext* sessionSetValueParameterClause();

  class  SessionSetParameterNameContext : public antlr4::ParserRuleContext {
  public:
    SessionSetParameterNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ParameterNameContext *parameterName();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *EXISTS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionSetParameterNameContext* sessionSetParameterName();

  class  SessionResetCommandContext : public antlr4::ParserRuleContext {
  public:
    SessionResetCommandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SESSION();
    antlr4::tree::TerminalNode *RESET();
    SessionResetArgumentsContext *sessionResetArguments();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionResetCommandContext* sessionResetCommand();

  class  SessionResetArgumentsContext : public antlr4::ParserRuleContext {
  public:
    SessionResetArgumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PARAMETERS();
    antlr4::tree::TerminalNode *CHARACTERISTICS();
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *SCHEMA();
    antlr4::tree::TerminalNode *GRAPH();
    antlr4::tree::TerminalNode *PROPERTY();
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *ZONE();
    ParameterNameContext *parameterName();
    antlr4::tree::TerminalNode *PARAMETER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionResetArgumentsContext* sessionResetArguments();

  class  SessionCloseCommandContext : public antlr4::ParserRuleContext {
  public:
    SessionCloseCommandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SESSION();
    antlr4::tree::TerminalNode *CLOSE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SessionCloseCommandContext* sessionCloseCommand();

  class  StartTransactionCommandContext : public antlr4::ParserRuleContext {
  public:
    StartTransactionCommandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *START();
    antlr4::tree::TerminalNode *TRANSACTION();
    TransactionModeContext *transactionMode();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StartTransactionCommandContext* startTransactionCommand();

  class  TransactionModeContext : public antlr4::ParserRuleContext {
  public:
    TransactionModeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *READ();
    antlr4::tree::TerminalNode *ONLY();
    antlr4::tree::TerminalNode *WRITE();
    ImplementationDefinedAccessModeContext *implementationDefinedAccessMode();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TransactionModeContext* transactionMode();

  class  ImplementationDefinedAccessModeContext : public antlr4::ParserRuleContext {
  public:
    ImplementationDefinedAccessModeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *I_DONT_KNOW_1();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ImplementationDefinedAccessModeContext* implementationDefinedAccessMode();

  class  RollbackCommandContext : public antlr4::ParserRuleContext {
  public:
    RollbackCommandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ROLLBACK();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RollbackCommandContext* rollbackCommand();

  class  CommitCommandContext : public antlr4::ParserRuleContext {
  public:
    CommitCommandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMIT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CommitCommandContext* commitCommand();

  class  NestedProcedureSpecificationContext : public antlr4::ParserRuleContext {
  public:
    NestedProcedureSpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_BRACE();
    ProcedureBodyContext *procedureBody();
    antlr4::tree::TerminalNode *RIGHT_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NestedProcedureSpecificationContext* nestedProcedureSpecification();

  class  UnsignedNumericLiteralContext : public antlr4::ParserRuleContext {
  public:
    UnsignedNumericLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerLiteralContext *integerLiteral();
    FloatLiteralContext *floatLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnsignedNumericLiteralContext* unsignedNumericLiteral();

  class  IntegerLiteralContext : public antlr4::ParserRuleContext {
  public:
    IntegerLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_DECIMAL_INTEGER();
    antlr4::tree::TerminalNode *UNSIGNED_OCTAL_INTEGER();
    antlr4::tree::TerminalNode *UNSIGNED_HEXADECIMAL_INTEGER();
    antlr4::tree::TerminalNode *UNSIGNED_BINARY_INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerLiteralContext* integerLiteral();

  class  FloatLiteralContext : public antlr4::ParserRuleContext {
  public:
    FloatLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_DECIMAL_IN_COMMON_NOTATION();
    antlr4::tree::TerminalNode *UNSIGNED_DECIMAL_IN_SCIENTIFIC_NOTATION();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FloatLiteralContext* floatLiteral();

  class  UnbrokenSingleQuotedCharacterSequenceContext : public antlr4::ParserRuleContext {
  public:
    UnbrokenSingleQuotedCharacterSequenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SINGLE_QUOTED_STRING_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnbrokenSingleQuotedCharacterSequenceContext* unbrokenSingleQuotedCharacterSequence();

  class  UnbrokenDoubleQuotedCharacterSequenceContext : public antlr4::ParserRuleContext {
  public:
    UnbrokenDoubleQuotedCharacterSequenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOUBLE_QUOTED_STRING_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnbrokenDoubleQuotedCharacterSequenceContext* unbrokenDoubleQuotedCharacterSequence();

  class  UnbrokenAccentQuotedCharacterSequenceContext : public antlr4::ParserRuleContext {
  public:
    UnbrokenAccentQuotedCharacterSequenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ACCENT_QUOTED_STRING_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnbrokenAccentQuotedCharacterSequenceContext* unbrokenAccentQuotedCharacterSequence();

  class  SingleQuotedCharacterSequenceContext : public antlr4::ParserRuleContext {
  public:
    SingleQuotedCharacterSequenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<UnbrokenSingleQuotedCharacterSequenceContext *> unbrokenSingleQuotedCharacterSequence();
    UnbrokenSingleQuotedCharacterSequenceContext* unbrokenSingleQuotedCharacterSequence(size_t i);
    std::vector<antlr4::tree::TerminalNode *> VERTICAL_BAR();
    antlr4::tree::TerminalNode* VERTICAL_BAR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SingleQuotedCharacterSequenceContext* singleQuotedCharacterSequence();

  class  DoubleQuotedCharacterSequenceContext : public antlr4::ParserRuleContext {
  public:
    DoubleQuotedCharacterSequenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<UnbrokenDoubleQuotedCharacterSequenceContext *> unbrokenDoubleQuotedCharacterSequence();
    UnbrokenDoubleQuotedCharacterSequenceContext* unbrokenDoubleQuotedCharacterSequence(size_t i);
    std::vector<antlr4::tree::TerminalNode *> VERTICAL_BAR();
    antlr4::tree::TerminalNode* VERTICAL_BAR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DoubleQuotedCharacterSequenceContext* doubleQuotedCharacterSequence();

  class  AccentQuotedCharacterSequenceContext : public antlr4::ParserRuleContext {
  public:
    AccentQuotedCharacterSequenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<UnbrokenAccentQuotedCharacterSequenceContext *> unbrokenAccentQuotedCharacterSequence();
    UnbrokenAccentQuotedCharacterSequenceContext* unbrokenAccentQuotedCharacterSequence(size_t i);
    std::vector<antlr4::tree::TerminalNode *> VERTICAL_BAR();
    antlr4::tree::TerminalNode* VERTICAL_BAR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AccentQuotedCharacterSequenceContext* accentQuotedCharacterSequence();

  class  UnbrokenCharacterStringLiteralContext : public antlr4::ParserRuleContext {
  public:
    UnbrokenCharacterStringLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnbrokenSingleQuotedCharacterSequenceContext *unbrokenSingleQuotedCharacterSequence();
    UnbrokenDoubleQuotedCharacterSequenceContext *unbrokenDoubleQuotedCharacterSequence();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnbrokenCharacterStringLiteralContext* unbrokenCharacterStringLiteral();

  class  NullLiteralContext : public antlr4::ParserRuleContext {
  public:
    NullLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NULL_();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NullLiteralContext* nullLiteral();

  class  TemporalLiteralContext : public antlr4::ParserRuleContext {
  public:
    TemporalLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DateLiteralContext *dateLiteral();
    TimeLiteralContext *timeLiteral();
    DatetimeLiteralContext *datetimeLiteral();
    SqlDatetimeLiteralContext *sqlDatetimeLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TemporalLiteralContext* temporalLiteral();

  class  SqlDatetimeLiteralContext : public antlr4::ParserRuleContext {
  public:
    SqlDatetimeLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DATE();
    std::vector<antlr4::tree::TerminalNode *> QUOTE();
    antlr4::tree::TerminalNode* QUOTE(size_t i);
    antlr4::tree::TerminalNode *FOUR_DIGIT();
    std::vector<antlr4::tree::TerminalNode *> MINUS_SIGN();
    antlr4::tree::TerminalNode* MINUS_SIGN(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOUBLE_DIGIT();
    antlr4::tree::TerminalNode* DOUBLE_DIGIT(size_t i);
    antlr4::tree::TerminalNode *TIME();
    std::vector<antlr4::tree::TerminalNode *> COLON();
    antlr4::tree::TerminalNode* COLON(size_t i);
    antlr4::tree::TerminalNode *TIMESTAMP();
    antlr4::tree::TerminalNode *DATETIME();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SqlDatetimeLiteralContext* sqlDatetimeLiteral();

  class  DateLiteralContext : public antlr4::ParserRuleContext {
  public:
    DateLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DATE();
    UnbrokenCharacterStringLiteralContext *unbrokenCharacterStringLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DateLiteralContext* dateLiteral();

  class  TimeLiteralContext : public antlr4::ParserRuleContext {
  public:
    TimeLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TIME();
    UnbrokenCharacterStringLiteralContext *unbrokenCharacterStringLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TimeLiteralContext* timeLiteral();

  class  DatetimeLiteralContext : public antlr4::ParserRuleContext {
  public:
    DatetimeLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnbrokenCharacterStringLiteralContext *unbrokenCharacterStringLiteral();
    antlr4::tree::TerminalNode *DATETIME();
    antlr4::tree::TerminalNode *TIMESTAMP();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DatetimeLiteralContext* datetimeLiteral();

  class  DurationLiteralContext : public antlr4::ParserRuleContext {
  public:
    DurationLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DURATION();
    UnbrokenCharacterStringLiteralContext *unbrokenCharacterStringLiteral();
    SqlIntervalLiteralContext *sqlIntervalLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DurationLiteralContext* durationLiteral();

  class  SqlIntervalLiteralContext : public antlr4::ParserRuleContext {
  public:
    SqlIntervalLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_DECIMAL_INTEGER();
    SqlIntervalTypeContext *sqlIntervalType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SqlIntervalLiteralContext* sqlIntervalLiteral();

  class  SqlIntervalTypeContext : public antlr4::ParserRuleContext {
  public:
    SqlIntervalTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTERVAL_DAY();
    antlr4::tree::TerminalNode *INTERVAL_WEEK();
    antlr4::tree::TerminalNode *INTERVAL_MONTH();
    antlr4::tree::TerminalNode *INTERVAL_YEAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SqlIntervalTypeContext* sqlIntervalType();

  class  IdentifierContext : public antlr4::ParserRuleContext {
  public:
    IdentifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGULAR_IDENTIFIER();
    DelimitedIdentifierContext *delimitedIdentifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdentifierContext* identifier();

  class  DelimitedIdentifierContext : public antlr4::ParserRuleContext {
  public:
    DelimitedIdentifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DoubleQuotedCharacterSequenceContext *doubleQuotedCharacterSequence();
    AccentQuotedCharacterSequenceContext *accentQuotedCharacterSequence();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DelimitedIdentifierContext* delimitedIdentifier();

  class  ObjectNameContext : public antlr4::ParserRuleContext {
  public:
    ObjectNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectNameContext* objectName();

  class  ObjectNameOrBindingVariableContext : public antlr4::ParserRuleContext {
  public:
    ObjectNameOrBindingVariableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGULAR_IDENTIFIER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectNameOrBindingVariableContext* objectNameOrBindingVariable();

  class  DirectoryNameContext : public antlr4::ParserRuleContext {
  public:
    DirectoryNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DirectoryNameContext* directoryName();

  class  SchemaNameContext : public antlr4::ParserRuleContext {
  public:
    SchemaNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SchemaNameContext* schemaName();

  class  GraphNameContext : public antlr4::ParserRuleContext {
  public:
    GraphNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGULAR_IDENTIFIER();
    DelimitedGraphNameContext *delimitedGraphName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphNameContext* graphName();

  class  DelimitedGraphNameContext : public antlr4::ParserRuleContext {
  public:
    DelimitedGraphNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DelimitedIdentifierContext *delimitedIdentifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DelimitedGraphNameContext* delimitedGraphName();

  class  GraphTypeNameContext : public antlr4::ParserRuleContext {
  public:
    GraphTypeNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphTypeNameContext* graphTypeName();

  class  ElementTypeNameContext : public antlr4::ParserRuleContext {
  public:
    ElementTypeNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementTypeNameContext* elementTypeName();

  class  BindingTableNameContext : public antlr4::ParserRuleContext {
  public:
    BindingTableNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGULAR_IDENTIFIER();
    DelimitedBindingTableNameContext *delimitedBindingTableName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingTableNameContext* bindingTableName();

  class  DelimitedBindingTableNameContext : public antlr4::ParserRuleContext {
  public:
    DelimitedBindingTableNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DelimitedIdentifierContext *delimitedIdentifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DelimitedBindingTableNameContext* delimitedBindingTableName();

  class  ProcedureNameContext : public antlr4::ParserRuleContext {
  public:
    ProcedureNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProcedureNameContext* procedureName();

  class  LabelNameContext : public antlr4::ParserRuleContext {
  public:
    LabelNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    KeywordContext *keyword();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabelNameContext* labelName();

  class  FunctionNameContext : public antlr4::ParserRuleContext {
  public:
    FunctionNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionNameContext* functionName();

  class  PropertyNameContext : public antlr4::ParserRuleContext {
  public:
    PropertyNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    KeywordContext *keyword();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyNameContext* propertyName();

  class  FieldNameContext : public antlr4::ParserRuleContext {
  public:
    FieldNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    KeywordContext *keyword();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FieldNameContext* fieldName();

  class  ParameterNameContext : public antlr4::ParserRuleContext {
  public:
    ParameterNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOLLAR_SIGN();
    UnsignedNumericLiteralContext *unsignedNumericLiteral();
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterNameContext* parameterName();

  class  VariableContext : public antlr4::ParserRuleContext {
  public:
    VariableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableContext *bindingVariable();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VariableContext* variable();

  class  GraphVariableContext : public antlr4::ParserRuleContext {
  public:
    GraphVariableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableContext *bindingVariable();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphVariableContext* graphVariable();

  class  GraphPatternVariableContext : public antlr4::ParserRuleContext {
  public:
    GraphPatternVariableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementVariableContext *elementVariable();
    PathOrSubpathVariableContext *pathOrSubpathVariable();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphPatternVariableContext* graphPatternVariable();

  class  PathOrSubpathVariableContext : public antlr4::ParserRuleContext {
  public:
    PathOrSubpathVariableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathVariableContext *pathVariable();
    SubpathVariableContext *subpathVariable();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathOrSubpathVariableContext* pathOrSubpathVariable();

  class  ElementVariableContext : public antlr4::ParserRuleContext {
  public:
    ElementVariableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableContext *bindingVariable();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementVariableContext* elementVariable();

  class  PathVariableContext : public antlr4::ParserRuleContext {
  public:
    PathVariableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableContext *bindingVariable();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathVariableContext* pathVariable();

  class  SubpathVariableContext : public antlr4::ParserRuleContext {
  public:
    SubpathVariableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SubpathVariableContext* subpathVariable();

  class  BindingTableVariableContext : public antlr4::ParserRuleContext {
  public:
    BindingTableVariableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableContext *bindingVariable();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingTableVariableContext* bindingTableVariable();

  class  ValueVariableContext : public antlr4::ParserRuleContext {
  public:
    ValueVariableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableContext *bindingVariable();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueVariableContext* valueVariable();

  class  BindingVariableContext : public antlr4::ParserRuleContext {
  public:
    BindingVariableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    KeywordContext *keyword();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingVariableContext* bindingVariable();

  class  PredefinedTypeLiteralContext : public antlr4::ParserRuleContext {
  public:
    PredefinedTypeLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BooleanLiteralContext *booleanLiteral();
    CharacterStringLiteralContext *characterStringLiteral();
    ByteStringLiteralContext *byteStringLiteral();
    TemporalLiteralContext *temporalLiteral();
    DurationLiteralContext *durationLiteral();
    NullLiteralContext *nullLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PredefinedTypeLiteralContext* predefinedTypeLiteral();

  class  BooleanLiteralContext : public antlr4::ParserRuleContext {
  public:
    BooleanLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_TRUE();
    antlr4::tree::TerminalNode *K_FALSE();
    antlr4::tree::TerminalNode *UNKNOWN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BooleanLiteralContext* booleanLiteral();

  class  CharacterStringLiteralContext : public antlr4::ParserRuleContext {
  public:
    CharacterStringLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SingleQuotedCharacterSequenceContext *singleQuotedCharacterSequence();
    DoubleQuotedCharacterSequenceContext *doubleQuotedCharacterSequence();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CharacterStringLiteralContext* characterStringLiteral();

  class  ByteStringLiteralContext : public antlr4::ParserRuleContext {
  public:
    ByteStringLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BYTE_STRING_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ByteStringLiteralContext* byteStringLiteral();

  class  ProcedureBodyContext : public antlr4::ParserRuleContext {
  public:
    ProcedureBodyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StatementBlockContext *statementBlock();
    AtSchemaClauseContext *atSchemaClause();
    BindingVariableDefinitionBlockContext *bindingVariableDefinitionBlock();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProcedureBodyContext* procedureBody();

  class  BindingVariableDefinitionBlockContext : public antlr4::ParserRuleContext {
  public:
    BindingVariableDefinitionBlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<BindingVariableDefinitionContext *> bindingVariableDefinition();
    BindingVariableDefinitionContext* bindingVariableDefinition(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingVariableDefinitionBlockContext* bindingVariableDefinitionBlock();

  class  BindingVariableDefinitionContext : public antlr4::ParserRuleContext {
  public:
    BindingVariableDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphVariableDefinitionContext *graphVariableDefinition();
    BindingTableVariableDefinitionContext *bindingTableVariableDefinition();
    ValueVariableDefinitionContext *valueVariableDefinition();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingVariableDefinitionContext* bindingVariableDefinition();

  class  StatementBlockContext : public antlr4::ParserRuleContext {
  public:
    StatementBlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StatementContext *statement();
    std::vector<NextStatementContext *> nextStatement();
    NextStatementContext* nextStatement(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementBlockContext* statementBlock();

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LinearCatalogModifyingStatementContext *linearCatalogModifyingStatement();
    LinearDataModifyingStatementContext *linearDataModifyingStatement();
    CompositeQueryExpressionContext *compositeQueryExpression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementContext* statement();

  class  NextStatementContext : public antlr4::ParserRuleContext {
  public:
    NextStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NEXT();
    StatementContext *statement();
    YieldClauseContext *yieldClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NextStatementContext* nextStatement();

  class  GraphVariableDefinitionContext : public antlr4::ParserRuleContext {
  public:
    GraphVariableDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GRAPH();
    GraphVariableContext *graphVariable();
    OptTypedGraphInitializerContext *optTypedGraphInitializer();
    antlr4::tree::TerminalNode *PROPERTY();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphVariableDefinitionContext* graphVariableDefinition();

  class  OptTypedGraphInitializerContext : public antlr4::ParserRuleContext {
  public:
    OptTypedGraphInitializerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphInitializerContext *graphInitializer();
    GraphReferenceValueTypeContext *graphReferenceValueType();
    TypedContext *typed();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptTypedGraphInitializerContext* optTypedGraphInitializer();

  class  GraphInitializerContext : public antlr4::ParserRuleContext {
  public:
    GraphInitializerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EQUALS_OPERATOR();
    GraphExpressionContext *graphExpression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphInitializerContext* graphInitializer();

  class  BindingTableVariableDefinitionContext : public antlr4::ParserRuleContext {
  public:
    BindingTableVariableDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TABLE();
    BindingTableVariableContext *bindingTableVariable();
    OptTypedBindingTableInitializerContext *optTypedBindingTableInitializer();
    antlr4::tree::TerminalNode *BINDING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingTableVariableDefinitionContext* bindingTableVariableDefinition();

  class  OptTypedBindingTableInitializerContext : public antlr4::ParserRuleContext {
  public:
    OptTypedBindingTableInitializerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingTableInitializerContext *bindingTableInitializer();
    BindingTableReferenceValueTypeContext *bindingTableReferenceValueType();
    TypedContext *typed();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptTypedBindingTableInitializerContext* optTypedBindingTableInitializer();

  class  BindingTableInitializerContext : public antlr4::ParserRuleContext {
  public:
    BindingTableInitializerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EQUALS_OPERATOR();
    BindingTableExpressionContext *bindingTableExpression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingTableInitializerContext* bindingTableInitializer();

  class  ValueVariableDefinitionContext : public antlr4::ParserRuleContext {
  public:
    ValueVariableDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VALUE();
    ValueVariableContext *valueVariable();
    OptTypedValueInitializerContext *optTypedValueInitializer();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueVariableDefinitionContext* valueVariableDefinition();

  class  OptTypedValueInitializerContext : public antlr4::ParserRuleContext {
  public:
    OptTypedValueInitializerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EQUALS_OPERATOR();
    ExpressionContext *expression();
    ValueTypeContext *valueType();
    TypedContext *typed();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptTypedValueInitializerContext* optTypedValueInitializer();

  class  GraphExpressionContext : public antlr4::ParserRuleContext {
  public:
    GraphExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NestedProcedureSpecificationContext *nestedProcedureSpecification();
    ObjectExpressionPrimaryContext *objectExpressionPrimary();
    GraphReferenceContext *graphReference();
    ObjectNameOrBindingVariableContext *objectNameOrBindingVariable();
    CurrentGraphContext *currentGraph();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphExpressionContext* graphExpression();

  class  CurrentGraphContext : public antlr4::ParserRuleContext {
  public:
    CurrentGraphContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CURRENT_PROPERTY_GRAPH();
    antlr4::tree::TerminalNode *CURRENT_GRAPH();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CurrentGraphContext* currentGraph();

  class  BindingTableExpressionContext : public antlr4::ParserRuleContext {
  public:
    BindingTableExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NestedProcedureSpecificationContext *nestedProcedureSpecification();
    ObjectExpressionPrimaryContext *objectExpressionPrimary();
    BindingTableReferenceContext *bindingTableReference();
    ObjectNameOrBindingVariableContext *objectNameOrBindingVariable();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingTableExpressionContext* bindingTableExpression();

  class  ObjectExpressionPrimaryContext : public antlr4::ParserRuleContext {
  public:
    ObjectExpressionPrimaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    ExpressionAtomContext *expressionAtom();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RIGHT_PAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectExpressionPrimaryContext* objectExpressionPrimary();

  class  LinearCatalogModifyingStatementContext : public antlr4::ParserRuleContext {
  public:
    LinearCatalogModifyingStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SimpleCatalogModifyingStatementContext *> simpleCatalogModifyingStatement();
    SimpleCatalogModifyingStatementContext* simpleCatalogModifyingStatement(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LinearCatalogModifyingStatementContext* linearCatalogModifyingStatement();

  class  SimpleCatalogModifyingStatementContext : public antlr4::ParserRuleContext {
  public:
    SimpleCatalogModifyingStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PrimitiveCatalogModifyingStatementContext *primitiveCatalogModifyingStatement();
    CallProcedureStatementContext *callProcedureStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleCatalogModifyingStatementContext* simpleCatalogModifyingStatement();

  class  PrimitiveCatalogModifyingStatementContext : public antlr4::ParserRuleContext {
  public:
    PrimitiveCatalogModifyingStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CreateSchemaStatementContext *createSchemaStatement();
    CreateGraphStatementContext *createGraphStatement();
    CreateGraphTypeStatementContext *createGraphTypeStatement();
    DropSchemaStatementContext *dropSchemaStatement();
    DropGraphStatementContext *dropGraphStatement();
    DropGraphTypeStatementContext *dropGraphTypeStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimitiveCatalogModifyingStatementContext* primitiveCatalogModifyingStatement();

  class  CreateSchemaStatementContext : public antlr4::ParserRuleContext {
  public:
    CreateSchemaStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *SCHEMA();
    CatalogSchemaParentAndNameContext *catalogSchemaParentAndName();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *EXISTS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateSchemaStatementContext* createSchemaStatement();

  class  DropSchemaStatementContext : public antlr4::ParserRuleContext {
  public:
    DropSchemaStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *SCHEMA();
    CatalogSchemaParentAndNameContext *catalogSchemaParentAndName();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DropSchemaStatementContext* dropSchemaStatement();

  class  CreateGraphStatementContext : public antlr4::ParserRuleContext {
  public:
    CreateGraphStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    CatalogGraphParentAndNameContext *catalogGraphParentAndName();
    antlr4::tree::TerminalNode *GRAPH();
    antlr4::tree::TerminalNode *OR();
    antlr4::tree::TerminalNode *REPLACE();
    OpenGraphTypeContext *openGraphType();
    OfGraphTypeContext *ofGraphType();
    GraphSourceContext *graphSource();
    antlr4::tree::TerminalNode *PROPERTY();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *EXISTS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateGraphStatementContext* createGraphStatement();

  class  OpenGraphTypeContext : public antlr4::ParserRuleContext {
  public:
    OpenGraphTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ANY();
    TypedContext *typed();
    antlr4::tree::TerminalNode *GRAPH();
    antlr4::tree::TerminalNode *PROPERTY();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OpenGraphTypeContext* openGraphType();

  class  OfGraphTypeContext : public antlr4::ParserRuleContext {
  public:
    OfGraphTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphTypeLikeGraphContext *graphTypeLikeGraph();
    GraphTypeReferenceContext *graphTypeReference();
    TypedContext *typed();
    NestedGraphTypeSpecificationContext *nestedGraphTypeSpecification();
    antlr4::tree::TerminalNode *GRAPH();
    antlr4::tree::TerminalNode *PROPERTY();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OfGraphTypeContext* ofGraphType();

  class  GraphTypeLikeGraphContext : public antlr4::ParserRuleContext {
  public:
    GraphTypeLikeGraphContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LIKE();
    GraphExpressionContext *graphExpression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphTypeLikeGraphContext* graphTypeLikeGraph();

  class  GraphSourceContext : public antlr4::ParserRuleContext {
  public:
    GraphSourceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *COPY();
    antlr4::tree::TerminalNode *OF();
    GraphExpressionContext *graphExpression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphSourceContext* graphSource();

  class  DropGraphStatementContext : public antlr4::ParserRuleContext {
  public:
    DropGraphStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *GRAPH();
    CatalogGraphParentAndNameContext *catalogGraphParentAndName();
    antlr4::tree::TerminalNode *PROPERTY();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DropGraphStatementContext* dropGraphStatement();

  class  CreateGraphTypeStatementContext : public antlr4::ParserRuleContext {
  public:
    CreateGraphTypeStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    CatalogGraphTypeParentAndNameContext *catalogGraphTypeParentAndName();
    GraphTypeSourceContext *graphTypeSource();
    antlr4::tree::TerminalNode *GRAPH();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *OR();
    antlr4::tree::TerminalNode *REPLACE();
    antlr4::tree::TerminalNode *PROPERTY();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *EXISTS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateGraphTypeStatementContext* createGraphTypeStatement();

  class  GraphTypeSourceContext : public antlr4::ParserRuleContext {
  public:
    GraphTypeSourceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CopyOfGraphTypeContext *copyOfGraphType();
    antlr4::tree::TerminalNode *AS();
    GraphTypeLikeGraphContext *graphTypeLikeGraph();
    NestedGraphTypeSpecificationContext *nestedGraphTypeSpecification();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphTypeSourceContext* graphTypeSource();

  class  CopyOfGraphTypeContext : public antlr4::ParserRuleContext {
  public:
    CopyOfGraphTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COPY();
    antlr4::tree::TerminalNode *OF();
    GraphTypeReferenceContext *graphTypeReference();
    ExternalObjectReferenceContext *externalObjectReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CopyOfGraphTypeContext* copyOfGraphType();

  class  DropGraphTypeStatementContext : public antlr4::ParserRuleContext {
  public:
    DropGraphTypeStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *GRAPH();
    antlr4::tree::TerminalNode *TYPE();
    CatalogGraphTypeParentAndNameContext *catalogGraphTypeParentAndName();
    antlr4::tree::TerminalNode *PROPERTY();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DropGraphTypeStatementContext* dropGraphTypeStatement();

  class  LinearDataModifyingStatementContext : public antlr4::ParserRuleContext {
  public:
    LinearDataModifyingStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LinearDataModifyingStatementBodyContext *linearDataModifyingStatementBody();
    NestedDataModifyingProcedureContext *nestedDataModifyingProcedure();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LinearDataModifyingStatementContext* linearDataModifyingStatement();

  class  LinearDataModifyingStatementBodyContext : public antlr4::ParserRuleContext {
  public:
    LinearDataModifyingStatementBodyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UseGraphClauseContext *useGraphClause();
    std::vector<SimpleDataAccessingStatementContext *> simpleDataAccessingStatement();
    SimpleDataAccessingStatementContext* simpleDataAccessingStatement(size_t i);
    PrimitiveResultStatementContext *primitiveResultStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LinearDataModifyingStatementBodyContext* linearDataModifyingStatementBody();

  class  NestedDataModifyingProcedureContext : public antlr4::ParserRuleContext {
  public:
    NestedDataModifyingProcedureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NestedProcedureSpecificationContext *nestedProcedureSpecification();
    UseGraphClauseContext *useGraphClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NestedDataModifyingProcedureContext* nestedDataModifyingProcedure();

  class  SimpleDataAccessingStatementContext : public antlr4::ParserRuleContext {
  public:
    SimpleDataAccessingStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PrimitiveQueryStatementContext *primitiveQueryStatement();
    PrimitiveDataModifyingStatementContext *primitiveDataModifyingStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleDataAccessingStatementContext* simpleDataAccessingStatement();

  class  PrimitiveDataModifyingStatementContext : public antlr4::ParserRuleContext {
  public:
    PrimitiveDataModifyingStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    InsertStatementContext *insertStatement();
    SetStatementContext *setStatement();
    RemoveStatementContext *removeStatement();
    DeleteStatementContext *deleteStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimitiveDataModifyingStatementContext* primitiveDataModifyingStatement();

  class  InsertStatementContext : public antlr4::ParserRuleContext {
  public:
    InsertStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INSERT();
    InsertGraphPatternContext *insertGraphPattern();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertStatementContext* insertStatement();

  class  SetStatementContext : public antlr4::ParserRuleContext {
  public:
    SetStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SET();
    SetItemListContext *setItemList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetStatementContext* setStatement();

  class  SetItemListContext : public antlr4::ParserRuleContext {
  public:
    SetItemListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SetItemContext *> setItem();
    SetItemContext* setItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetItemListContext* setItemList();

  class  SetItemContext : public antlr4::ParserRuleContext {
  public:
    SetItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SetPropertyItemContext *setPropertyItem();
    SetAllPropertiesItemContext *setAllPropertiesItem();
    SetLabelItemContext *setLabelItem();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetItemContext* setItem();

  class  SetPropertyItemContext : public antlr4::ParserRuleContext {
  public:
    SetPropertyItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableReferenceContext *bindingVariableReference();
    antlr4::tree::TerminalNode *PERIOD();
    PropertyNameContext *propertyName();
    antlr4::tree::TerminalNode *EQUALS_OPERATOR();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetPropertyItemContext* setPropertyItem();

  class  SetAllPropertiesItemContext : public antlr4::ParserRuleContext {
  public:
    SetAllPropertiesItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableReferenceContext *bindingVariableReference();
    antlr4::tree::TerminalNode *EQUALS_OPERATOR();
    antlr4::tree::TerminalNode *LEFT_BRACE();
    antlr4::tree::TerminalNode *RIGHT_BRACE();
    PropertyKeyValuePairListContext *propertyKeyValuePairList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetAllPropertiesItemContext* setAllPropertiesItem();

  class  SetLabelItemContext : public antlr4::ParserRuleContext {
  public:
    SetLabelItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableReferenceContext *bindingVariableReference();
    IsOrColonContext *isOrColon();
    LabelNameContext *labelName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetLabelItemContext* setLabelItem();

  class  LabelSetSpecificationContext : public antlr4::ParserRuleContext {
  public:
    LabelSetSpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LabelNameContext *> labelName();
    LabelNameContext* labelName(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AMPERSAND();
    antlr4::tree::TerminalNode* AMPERSAND(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabelSetSpecificationContext* labelSetSpecification();

  class  RemoveStatementContext : public antlr4::ParserRuleContext {
  public:
    RemoveStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REMOVE();
    RemoveItemListContext *removeItemList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RemoveStatementContext* removeStatement();

  class  RemoveItemListContext : public antlr4::ParserRuleContext {
  public:
    RemoveItemListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<RemoveItemContext *> removeItem();
    RemoveItemContext* removeItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RemoveItemListContext* removeItemList();

  class  RemoveItemContext : public antlr4::ParserRuleContext {
  public:
    RemoveItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RemovePropertyItemContext *removePropertyItem();
    RemoveLabelItemContext *removeLabelItem();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RemoveItemContext* removeItem();

  class  RemovePropertyItemContext : public antlr4::ParserRuleContext {
  public:
    RemovePropertyItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableReferenceContext *bindingVariableReference();
    antlr4::tree::TerminalNode *PERIOD();
    PropertyNameContext *propertyName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RemovePropertyItemContext* removePropertyItem();

  class  RemoveLabelItemContext : public antlr4::ParserRuleContext {
  public:
    RemoveLabelItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableReferenceContext *bindingVariableReference();
    IsOrColonContext *isOrColon();
    LabelNameContext *labelName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RemoveLabelItemContext* removeLabelItem();

  class  DeleteStatementContext : public antlr4::ParserRuleContext {
  public:
    DeleteStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DELETE();
    DeleteItemListContext *deleteItemList();
    antlr4::tree::TerminalNode *DETACH();
    antlr4::tree::TerminalNode *NODETACH();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeleteStatementContext* deleteStatement();

  class  DeleteItemListContext : public antlr4::ParserRuleContext {
  public:
    DeleteItemListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DeleteItemContext *> deleteItem();
    DeleteItemContext* deleteItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeleteItemListContext* deleteItemList();

  class  DeleteItemContext : public antlr4::ParserRuleContext {
  public:
    DeleteItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeleteItemContext* deleteItem();

  class  CallDataModifyingProcedureStatementContext : public antlr4::ParserRuleContext {
  public:
    CallDataModifyingProcedureStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CallProcedureStatementContext *callProcedureStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CallDataModifyingProcedureStatementContext* callDataModifyingProcedureStatement();

  class  CompositeQueryExpressionContext : public antlr4::ParserRuleContext {
  public:
    CompositeQueryExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CompositeQueryPrimaryContext *compositeQueryPrimary();
    CompositeQueryExpressionContext *compositeQueryExpression();
    QueryConjunctionContext *queryConjunction();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CompositeQueryExpressionContext* compositeQueryExpression();
  CompositeQueryExpressionContext* compositeQueryExpression(int precedence);
  class  QueryConjunctionContext : public antlr4::ParserRuleContext {
  public:
    QueryConjunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SetOperatorContext *setOperator();
    antlr4::tree::TerminalNode *OTHERWISE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QueryConjunctionContext* queryConjunction();

  class  SetOperatorContext : public antlr4::ParserRuleContext {
  public:
    SetOperatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNION();
    SetQuantifierContext *setQuantifier();
    antlr4::tree::TerminalNode *EXCEPT();
    antlr4::tree::TerminalNode *INTERSECT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetOperatorContext* setOperator();

  class  CompositeQueryPrimaryContext : public antlr4::ParserRuleContext {
  public:
    CompositeQueryPrimaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LinearQueryStatementContext *linearQueryStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CompositeQueryPrimaryContext* compositeQueryPrimary();

  class  LinearQueryStatementContext : public antlr4::ParserRuleContext {
  public:
    LinearQueryStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FocusedLinearQueryStatementContext *focusedLinearQueryStatement();
    AmbientLinearQueryStatementContext *ambientLinearQueryStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LinearQueryStatementContext* linearQueryStatement();

  class  FocusedLinearQueryStatementContext : public antlr4::ParserRuleContext {
  public:
    FocusedLinearQueryStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FocusedLinearQueryAndPrimitiveResultStatementPartContext *focusedLinearQueryAndPrimitiveResultStatementPart();
    std::vector<FocusedLinearQueryStatementPartContext *> focusedLinearQueryStatementPart();
    FocusedLinearQueryStatementPartContext* focusedLinearQueryStatementPart(size_t i);
    FocusedPrimitiveResultStatementContext *focusedPrimitiveResultStatement();
    FocusedNestedQuerySpecificationContext *focusedNestedQuerySpecification();
    SelectStatementContext *selectStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FocusedLinearQueryStatementContext* focusedLinearQueryStatement();

  class  FocusedLinearQueryStatementPartContext : public antlr4::ParserRuleContext {
  public:
    FocusedLinearQueryStatementPartContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UseGraphClauseContext *useGraphClause();
    SimpleLinearQueryStatementContext *simpleLinearQueryStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FocusedLinearQueryStatementPartContext* focusedLinearQueryStatementPart();

  class  FocusedLinearQueryAndPrimitiveResultStatementPartContext : public antlr4::ParserRuleContext {
  public:
    FocusedLinearQueryAndPrimitiveResultStatementPartContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UseGraphClauseContext *useGraphClause();
    SimpleLinearQueryStatementContext *simpleLinearQueryStatement();
    PrimitiveResultStatementContext *primitiveResultStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FocusedLinearQueryAndPrimitiveResultStatementPartContext* focusedLinearQueryAndPrimitiveResultStatementPart();

  class  FocusedPrimitiveResultStatementContext : public antlr4::ParserRuleContext {
  public:
    FocusedPrimitiveResultStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UseGraphClauseContext *useGraphClause();
    PrimitiveResultStatementContext *primitiveResultStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FocusedPrimitiveResultStatementContext* focusedPrimitiveResultStatement();

  class  FocusedNestedQuerySpecificationContext : public antlr4::ParserRuleContext {
  public:
    FocusedNestedQuerySpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UseGraphClauseContext *useGraphClause();
    NestedProcedureSpecificationContext *nestedProcedureSpecification();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FocusedNestedQuerySpecificationContext* focusedNestedQuerySpecification();

  class  AmbientLinearQueryStatementContext : public antlr4::ParserRuleContext {
  public:
    AmbientLinearQueryStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PrimitiveResultStatementContext *primitiveResultStatement();
    SimpleLinearQueryStatementContext *simpleLinearQueryStatement();
    NestedProcedureSpecificationContext *nestedProcedureSpecification();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AmbientLinearQueryStatementContext* ambientLinearQueryStatement();

  class  SimpleLinearQueryStatementContext : public antlr4::ParserRuleContext {
  public:
    SimpleLinearQueryStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PrimitiveQueryStatementContext *> primitiveQueryStatement();
    PrimitiveQueryStatementContext* primitiveQueryStatement(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleLinearQueryStatementContext* simpleLinearQueryStatement();

  class  PrimitiveQueryStatementContext : public antlr4::ParserRuleContext {
  public:
    PrimitiveQueryStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MatchStatementContext *matchStatement();
    LetStatementContext *letStatement();
    ForStatementContext *forStatement();
    FilterStatementContext *filterStatement();
    OrderByAndPageStatementContext *orderByAndPageStatement();
    CallQueryStatementContext *callQueryStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimitiveQueryStatementContext* primitiveQueryStatement();

  class  MatchStatementContext : public antlr4::ParserRuleContext {
  public:
    MatchStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleMatchStatementContext *simpleMatchStatement();
    OptionalMatchStatementContext *optionalMatchStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MatchStatementContext* matchStatement();

  class  SimpleMatchStatementContext : public antlr4::ParserRuleContext {
  public:
    SimpleMatchStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MATCH();
    GraphPatternContext *graphPattern();
    GraphPatternYieldClauseContext *graphPatternYieldClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleMatchStatementContext* simpleMatchStatement();

  class  OptionalMatchStatementContext : public antlr4::ParserRuleContext {
  public:
    OptionalMatchStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTIONAL();
    OptionalOperandContext *optionalOperand();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptionalMatchStatementContext* optionalMatchStatement();

  class  OptionalOperandContext : public antlr4::ParserRuleContext {
  public:
    OptionalOperandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleMatchStatementContext *simpleMatchStatement();
    antlr4::tree::TerminalNode *LEFT_BRACE();
    MatchStatementBlockContext *matchStatementBlock();
    antlr4::tree::TerminalNode *RIGHT_BRACE();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptionalOperandContext* optionalOperand();

  class  MatchStatementBlockContext : public antlr4::ParserRuleContext {
  public:
    MatchStatementBlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<MatchStatementContext *> matchStatement();
    MatchStatementContext* matchStatement(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MatchStatementBlockContext* matchStatementBlock();

  class  CallQueryStatementContext : public antlr4::ParserRuleContext {
  public:
    CallQueryStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CallProcedureStatementContext *callProcedureStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CallQueryStatementContext* callQueryStatement();

  class  FilterStatementContext : public antlr4::ParserRuleContext {
  public:
    FilterStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FILTER();
    WhereClauseContext *whereClause();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FilterStatementContext* filterStatement();

  class  LetStatementContext : public antlr4::ParserRuleContext {
  public:
    LetStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LET();
    LetVariableDefinitionListContext *letVariableDefinitionList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LetStatementContext* letStatement();

  class  LetVariableDefinitionListContext : public antlr4::ParserRuleContext {
  public:
    LetVariableDefinitionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LetVariableDefinitionContext *> letVariableDefinition();
    LetVariableDefinitionContext* letVariableDefinition(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LetVariableDefinitionListContext* letVariableDefinitionList();

  class  LetVariableDefinitionContext : public antlr4::ParserRuleContext {
  public:
    LetVariableDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ValueVariableDefinitionContext *valueVariableDefinition();
    ValueVariableContext *valueVariable();
    antlr4::tree::TerminalNode *EQUALS_OPERATOR();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LetVariableDefinitionContext* letVariableDefinition();

  class  ForStatementContext : public antlr4::ParserRuleContext {
  public:
    ForStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FOR();
    ForItemContext *forItem();
    ForOrdinalityOrOffsetContext *forOrdinalityOrOffset();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ForStatementContext* forStatement();

  class  ForItemContext : public antlr4::ParserRuleContext {
  public:
    ForItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ForItemAliasContext *forItemAlias();
    ExpressionAtomContext *expressionAtom();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ForItemContext* forItem();

  class  ForItemAliasContext : public antlr4::ParserRuleContext {
  public:
    ForItemAliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *IN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ForItemAliasContext* forItemAlias();

  class  ForOrdinalityOrOffsetContext : public antlr4::ParserRuleContext {
  public:
    ForOrdinalityOrOffsetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WITH();
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *ORDINALITY();
    antlr4::tree::TerminalNode *OFFSET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ForOrdinalityOrOffsetContext* forOrdinalityOrOffset();

  class  OrderByAndPageStatementContext : public antlr4::ParserRuleContext {
  public:
    OrderByAndPageStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OrderByClauseContext *orderByClause();
    OffsetClauseContext *offsetClause();
    LimitClauseContext *limitClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrderByAndPageStatementContext* orderByAndPageStatement();

  class  PrimitiveResultStatementContext : public antlr4::ParserRuleContext {
  public:
    PrimitiveResultStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ReturnStatementContext *returnStatement();
    OrderByAndPageStatementContext *orderByAndPageStatement();
    antlr4::tree::TerminalNode *FINISH();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimitiveResultStatementContext* primitiveResultStatement();

  class  ReturnStatementContext : public antlr4::ParserRuleContext {
  public:
    ReturnStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RETURN();
    ReturnStatementBodyContext *returnStatementBody();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReturnStatementContext* returnStatement();

  class  ReturnStatementBodyContext : public antlr4::ParserRuleContext {
  public:
    ReturnStatementBodyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASTERISK();
    ReturnItemListContext *returnItemList();
    SetQuantifierContext *setQuantifier();
    GroupByClauseContext *groupByClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReturnStatementBodyContext* returnStatementBody();

  class  ReturnItemListContext : public antlr4::ParserRuleContext {
  public:
    ReturnItemListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ReturnItemContext *> returnItem();
    ReturnItemContext* returnItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReturnItemListContext* returnItemList();

  class  ReturnItemContext : public antlr4::ParserRuleContext {
  public:
    ReturnItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();
    ReturnItemAliasContext *returnItemAlias();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReturnItemContext* returnItem();

  class  ReturnItemAliasContext : public antlr4::ParserRuleContext {
  public:
    ReturnItemAliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AS();
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReturnItemAliasContext* returnItemAlias();

  class  SelectStatementContext : public antlr4::ParserRuleContext {
  public:
    SelectStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SELECT();
    antlr4::tree::TerminalNode *ASTERISK();
    SelectItemListContext *selectItemList();
    SetQuantifierContext *setQuantifier();
    SelectStatementBodyContext *selectStatementBody();
    WhereClauseContext *whereClause();
    GroupByClauseContext *groupByClause();
    HavingClauseContext *havingClause();
    OrderByClauseContext *orderByClause();
    OffsetClauseContext *offsetClause();
    LimitClauseContext *limitClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectStatementContext* selectStatement();

  class  SelectItemListContext : public antlr4::ParserRuleContext {
  public:
    SelectItemListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SelectItemContext *> selectItem();
    SelectItemContext* selectItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectItemListContext* selectItemList();

  class  SelectItemContext : public antlr4::ParserRuleContext {
  public:
    SelectItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();
    SelectItemAliasContext *selectItemAlias();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectItemContext* selectItem();

  class  SelectItemAliasContext : public antlr4::ParserRuleContext {
  public:
    SelectItemAliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AS();
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectItemAliasContext* selectItemAlias();

  class  HavingClauseContext : public antlr4::ParserRuleContext {
  public:
    HavingClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *HAVING();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HavingClauseContext* havingClause();

  class  SelectStatementBodyContext : public antlr4::ParserRuleContext {
  public:
    SelectStatementBodyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FROM();
    SelectGraphMatchListContext *selectGraphMatchList();
    SelectQuerySpecificationContext *selectQuerySpecification();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectStatementBodyContext* selectStatementBody();

  class  SelectGraphMatchListContext : public antlr4::ParserRuleContext {
  public:
    SelectGraphMatchListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SelectGraphMatchContext *> selectGraphMatch();
    SelectGraphMatchContext* selectGraphMatch(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectGraphMatchListContext* selectGraphMatchList();

  class  SelectGraphMatchContext : public antlr4::ParserRuleContext {
  public:
    SelectGraphMatchContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphExpressionContext *graphExpression();
    MatchStatementContext *matchStatement();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectGraphMatchContext* selectGraphMatch();

  class  SelectQuerySpecificationContext : public antlr4::ParserRuleContext {
  public:
    SelectQuerySpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NestedProcedureSpecificationContext *nestedProcedureSpecification();
    GraphExpressionContext *graphExpression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectQuerySpecificationContext* selectQuerySpecification();

  class  CallProcedureStatementContext : public antlr4::ParserRuleContext {
  public:
    CallProcedureStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CALL();
    ProcedureCallContext *procedureCall();
    antlr4::tree::TerminalNode *OPTIONAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CallProcedureStatementContext* callProcedureStatement();

  class  ProcedureCallContext : public antlr4::ParserRuleContext {
  public:
    ProcedureCallContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    InlineProcedureCallContext *inlineProcedureCall();
    NamedProcedureCallContext *namedProcedureCall();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProcedureCallContext* procedureCall();

  class  InlineProcedureCallContext : public antlr4::ParserRuleContext {
  public:
    InlineProcedureCallContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NestedProcedureSpecificationContext *nestedProcedureSpecification();
    VariableScopeClauseContext *variableScopeClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InlineProcedureCallContext* inlineProcedureCall();

  class  VariableScopeClauseContext : public antlr4::ParserRuleContext {
  public:
    VariableScopeClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    BindingVariableReferenceListContext *bindingVariableReferenceList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VariableScopeClauseContext* variableScopeClause();

  class  BindingVariableReferenceListContext : public antlr4::ParserRuleContext {
  public:
    BindingVariableReferenceListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<BindingVariableReferenceContext *> bindingVariableReference();
    BindingVariableReferenceContext* bindingVariableReference(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingVariableReferenceListContext* bindingVariableReferenceList();

  class  NamedProcedureCallContext : public antlr4::ParserRuleContext {
  public:
    NamedProcedureCallContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ProcedureReferenceContext *procedureReference();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    ProcedureArgumentListContext *procedureArgumentList();
    YieldClauseContext *yieldClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamedProcedureCallContext* namedProcedureCall();

  class  ProcedureArgumentListContext : public antlr4::ParserRuleContext {
  public:
    ProcedureArgumentListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ProcedureArgumentContext *> procedureArgument();
    ProcedureArgumentContext* procedureArgument(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProcedureArgumentListContext* procedureArgumentList();

  class  ProcedureArgumentContext : public antlr4::ParserRuleContext {
  public:
    ProcedureArgumentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProcedureArgumentContext* procedureArgument();

  class  UseGraphClauseContext : public antlr4::ParserRuleContext {
  public:
    UseGraphClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *USE();
    GraphExpressionContext *graphExpression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UseGraphClauseContext* useGraphClause();

  class  AtSchemaClauseContext : public antlr4::ParserRuleContext {
  public:
    AtSchemaClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AT();
    SchemaReferenceContext *schemaReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AtSchemaClauseContext* atSchemaClause();

  class  BindingVariableReferenceContext : public antlr4::ParserRuleContext {
  public:
    BindingVariableReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableContext *bindingVariable();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingVariableReferenceContext* bindingVariableReference();

  class  ElementVariableReferenceContext : public antlr4::ParserRuleContext {
  public:
    ElementVariableReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableReferenceContext *bindingVariableReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementVariableReferenceContext* elementVariableReference();

  class  PathVariableReferenceContext : public antlr4::ParserRuleContext {
  public:
    PathVariableReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableReferenceContext *bindingVariableReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathVariableReferenceContext* pathVariableReference();

  class  ParameterContext : public antlr4::ParserRuleContext {
  public:
    ParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ParameterNameContext *parameterName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterContext* parameter();

  class  GraphPatternYieldClauseContext : public antlr4::ParserRuleContext {
  public:
    GraphPatternYieldClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *YIELD();
    GraphPatternYieldItemListContext *graphPatternYieldItemList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphPatternYieldClauseContext* graphPatternYieldClause();

  class  GraphPatternYieldItemListContext : public antlr4::ParserRuleContext {
  public:
    GraphPatternYieldItemListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<GraphPatternYieldItemContext *> graphPatternYieldItem();
    GraphPatternYieldItemContext* graphPatternYieldItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *NO();
    antlr4::tree::TerminalNode *BINDINGS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphPatternYieldItemListContext* graphPatternYieldItemList();

  class  GraphPatternYieldItemContext : public antlr4::ParserRuleContext {
  public:
    GraphPatternYieldItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementVariableReferenceContext *elementVariableReference();
    PathVariableReferenceContext *pathVariableReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphPatternYieldItemContext* graphPatternYieldItem();

  class  GraphPatternContext : public antlr4::ParserRuleContext {
  public:
    GraphPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathPatternListContext *pathPatternList();
    MatchModeContext *matchMode();
    KeepClauseContext *keepClause();
    GraphPatternWhereClauseContext *graphPatternWhereClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphPatternContext* graphPattern();

  class  MatchModeContext : public antlr4::ParserRuleContext {
  public:
    MatchModeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RepeatableElementsMatchModeContext *repeatableElementsMatchMode();
    DifferentEdgesMatchModeContext *differentEdgesMatchMode();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MatchModeContext* matchMode();

  class  RepeatableElementsMatchModeContext : public antlr4::ParserRuleContext {
  public:
    RepeatableElementsMatchModeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REPEATABLE();
    ElementBindingsOrElementsContext *elementBindingsOrElements();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RepeatableElementsMatchModeContext* repeatableElementsMatchMode();

  class  DifferentEdgesMatchModeContext : public antlr4::ParserRuleContext {
  public:
    DifferentEdgesMatchModeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DIFFERENT();
    EdgeBindingsOrEdgesContext *edgeBindingsOrEdges();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DifferentEdgesMatchModeContext* differentEdgesMatchMode();

  class  ElementBindingsOrElementsContext : public antlr4::ParserRuleContext {
  public:
    ElementBindingsOrElementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ELEMENT();
    antlr4::tree::TerminalNode *BINDINGS();
    antlr4::tree::TerminalNode *ELEMENTS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementBindingsOrElementsContext* elementBindingsOrElements();

  class  EdgeBindingsOrEdgesContext : public antlr4::ParserRuleContext {
  public:
    EdgeBindingsOrEdgesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EDGE_SYNONYM();
    antlr4::tree::TerminalNode *BINDINGS();
    antlr4::tree::TerminalNode *EDGES_SYNONYM();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeBindingsOrEdgesContext* edgeBindingsOrEdges();

  class  PathPatternListContext : public antlr4::ParserRuleContext {
  public:
    PathPatternListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PathPatternContext *> pathPattern();
    PathPatternContext* pathPattern(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathPatternListContext* pathPatternList();

  class  PathPatternContext : public antlr4::ParserRuleContext {
  public:
    PathPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathPatternExpressionContext *pathPatternExpression();
    PathVariableDeclarationContext *pathVariableDeclaration();
    PathPatternPrefixContext *pathPatternPrefix();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathPatternContext* pathPattern();

  class  PathVariableDeclarationContext : public antlr4::ParserRuleContext {
  public:
    PathVariableDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathVariableContext *pathVariable();
    antlr4::tree::TerminalNode *EQUALS_OPERATOR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathVariableDeclarationContext* pathVariableDeclaration();

  class  KeepClauseContext : public antlr4::ParserRuleContext {
  public:
    KeepClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *KEEP();
    PathPatternPrefixContext *pathPatternPrefix();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KeepClauseContext* keepClause();

  class  GraphPatternWhereClauseContext : public antlr4::ParserRuleContext {
  public:
    GraphPatternWhereClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHERE();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphPatternWhereClauseContext* graphPatternWhereClause();

  class  PathPatternPrefixContext : public antlr4::ParserRuleContext {
  public:
    PathPatternPrefixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathModePrefixContext *pathModePrefix();
    PathSearchPrefixContext *pathSearchPrefix();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathPatternPrefixContext* pathPatternPrefix();

  class  PathModePrefixContext : public antlr4::ParserRuleContext {
  public:
    PathModePrefixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathModeContext *pathMode();
    PathOrPathsContext *pathOrPaths();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathModePrefixContext* pathModePrefix();

  class  PathModeContext : public antlr4::ParserRuleContext {
  public:
    PathModeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WALK();
    antlr4::tree::TerminalNode *TRAIL();
    antlr4::tree::TerminalNode *SIMPLE();
    antlr4::tree::TerminalNode *ACYCLIC();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathModeContext* pathMode();

  class  PathSearchPrefixContext : public antlr4::ParserRuleContext {
  public:
    PathSearchPrefixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AllPathSearchContext *allPathSearch();
    AnyPathSearchContext *anyPathSearch();
    ShortestPathSearchContext *shortestPathSearch();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathSearchPrefixContext* pathSearchPrefix();

  class  AllPathSearchContext : public antlr4::ParserRuleContext {
  public:
    AllPathSearchContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALL();
    PathModeContext *pathMode();
    PathOrPathsContext *pathOrPaths();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AllPathSearchContext* allPathSearch();

  class  PathOrPathsContext : public antlr4::ParserRuleContext {
  public:
    PathOrPathsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PATH();
    antlr4::tree::TerminalNode *PATHS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathOrPathsContext* pathOrPaths();

  class  AnyPathSearchContext : public antlr4::ParserRuleContext {
  public:
    AnyPathSearchContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ANY();
    NumberOfPathsContext *numberOfPaths();
    PathModeContext *pathMode();
    PathOrPathsContext *pathOrPaths();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AnyPathSearchContext* anyPathSearch();

  class  NumberOfPathsContext : public antlr4::ParserRuleContext {
  public:
    NumberOfPathsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnsignedIntegerSpecificationContext *unsignedIntegerSpecification();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumberOfPathsContext* numberOfPaths();

  class  ShortestPathSearchContext : public antlr4::ParserRuleContext {
  public:
    ShortestPathSearchContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AllShortestPathSearchContext *allShortestPathSearch();
    AnyShortestPathSearchContext *anyShortestPathSearch();
    CountedShortestPathSearchContext *countedShortestPathSearch();
    CountedShortestGroupSearchContext *countedShortestGroupSearch();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ShortestPathSearchContext* shortestPathSearch();

  class  AllShortestPathSearchContext : public antlr4::ParserRuleContext {
  public:
    AllShortestPathSearchContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *SHORTEST();
    PathModeContext *pathMode();
    PathOrPathsContext *pathOrPaths();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AllShortestPathSearchContext* allShortestPathSearch();

  class  AnyShortestPathSearchContext : public antlr4::ParserRuleContext {
  public:
    AnyShortestPathSearchContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ANY();
    antlr4::tree::TerminalNode *SHORTEST();
    PathModeContext *pathMode();
    PathOrPathsContext *pathOrPaths();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AnyShortestPathSearchContext* anyShortestPathSearch();

  class  CountedShortestPathSearchContext : public antlr4::ParserRuleContext {
  public:
    CountedShortestPathSearchContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHORTEST();
    NumberOfPathsContext *numberOfPaths();
    PathModeContext *pathMode();
    PathOrPathsContext *pathOrPaths();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CountedShortestPathSearchContext* countedShortestPathSearch();

  class  CountedShortestGroupSearchContext : public antlr4::ParserRuleContext {
  public:
    CountedShortestGroupSearchContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHORTEST();
    NumberOfGroupsContext *numberOfGroups();
    antlr4::tree::TerminalNode *GROUP();
    antlr4::tree::TerminalNode *GROUPS();
    PathModeContext *pathMode();
    PathOrPathsContext *pathOrPaths();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CountedShortestGroupSearchContext* countedShortestGroupSearch();

  class  NumberOfGroupsContext : public antlr4::ParserRuleContext {
  public:
    NumberOfGroupsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnsignedIntegerSpecificationContext *unsignedIntegerSpecification();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumberOfGroupsContext* numberOfGroups();

  class  PathPatternExpressionContext : public antlr4::ParserRuleContext {
  public:
    PathPatternExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathTermContext *pathTerm();
    PathMultisetAlternationContext *pathMultisetAlternation();
    PathPatternUnionContext *pathPatternUnion();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathPatternExpressionContext* pathPatternExpression();

  class  PathMultisetAlternationContext : public antlr4::ParserRuleContext {
  public:
    PathMultisetAlternationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PathTermContext *> pathTerm();
    PathTermContext* pathTerm(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MULTISET_ALTERNATION_OPERATOR();
    antlr4::tree::TerminalNode* MULTISET_ALTERNATION_OPERATOR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathMultisetAlternationContext* pathMultisetAlternation();

  class  PathPatternUnionContext : public antlr4::ParserRuleContext {
  public:
    PathPatternUnionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PathTermContext *> pathTerm();
    PathTermContext* pathTerm(size_t i);
    std::vector<antlr4::tree::TerminalNode *> VERTICAL_BAR();
    antlr4::tree::TerminalNode* VERTICAL_BAR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathPatternUnionContext* pathPatternUnion();

  class  PathTermContext : public antlr4::ParserRuleContext {
  public:
    PathTermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PathFactorContext *> pathFactor();
    PathFactorContext* pathFactor(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathTermContext* pathTerm();

  class  PathFactorContext : public antlr4::ParserRuleContext {
  public:
    PathFactorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathPrimaryContext *pathPrimary();
    QuantifiedPathPrimaryContext *quantifiedPathPrimary();
    QuestionedPathPrimaryContext *questionedPathPrimary();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathFactorContext* pathFactor();

  class  QuantifiedPathPrimaryContext : public antlr4::ParserRuleContext {
  public:
    QuantifiedPathPrimaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathPrimaryContext *pathPrimary();
    GraphPatternQuantifierContext *graphPatternQuantifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QuantifiedPathPrimaryContext* quantifiedPathPrimary();

  class  QuestionedPathPrimaryContext : public antlr4::ParserRuleContext {
  public:
    QuestionedPathPrimaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PathPrimaryContext *pathPrimary();
    antlr4::tree::TerminalNode *QUESTION_MARK();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QuestionedPathPrimaryContext* questionedPathPrimary();

  class  PathPrimaryContext : public antlr4::ParserRuleContext {
  public:
    PathPrimaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementPatternContext *elementPattern();
    ParenthesizedPathPatternExpressionContext *parenthesizedPathPatternExpression();
    SimplifiedPathPatternExpressionContext *simplifiedPathPatternExpression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathPrimaryContext* pathPrimary();

  class  ElementPatternContext : public antlr4::ParserRuleContext {
  public:
    ElementPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NodePatternContext *nodePattern();
    EdgePatternContext *edgePattern();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementPatternContext* elementPattern();

  class  NodePatternContext : public antlr4::ParserRuleContext {
  public:
    NodePatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    ElementPatternFillerContext *elementPatternFiller();
    antlr4::tree::TerminalNode *RIGHT_PAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodePatternContext* nodePattern();

  class  ElementPatternFillerContext : public antlr4::ParserRuleContext {
  public:
    ElementPatternFillerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementVariableDeclarationContext *elementVariableDeclaration();
    IsLabelExpressionContext *isLabelExpression();
    ElementPatternPredicateContext *elementPatternPredicate();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementPatternFillerContext* elementPatternFiller();

  class  ElementVariableDeclarationContext : public antlr4::ParserRuleContext {
  public:
    ElementVariableDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementVariableContext *elementVariable();
    antlr4::tree::TerminalNode *TEMP();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementVariableDeclarationContext* elementVariableDeclaration();

  class  IsLabelExpressionContext : public antlr4::ParserRuleContext {
  public:
    IsLabelExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IsOrColonContext *isOrColon();
    LabelExpressionContext *labelExpression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IsLabelExpressionContext* isLabelExpression();

  class  IsOrColonContext : public antlr4::ParserRuleContext {
  public:
    IsOrColonContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IS();
    antlr4::tree::TerminalNode *COLON();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IsOrColonContext* isOrColon();

  class  ElementPatternPredicateContext : public antlr4::ParserRuleContext {
  public:
    ElementPatternPredicateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementPatternWhereClauseContext *elementPatternWhereClause();
    ElementPropertySpecificationContext *elementPropertySpecification();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementPatternPredicateContext* elementPatternPredicate();

  class  ElementPatternWhereClauseContext : public antlr4::ParserRuleContext {
  public:
    ElementPatternWhereClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHERE();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementPatternWhereClauseContext* elementPatternWhereClause();

  class  ElementPropertySpecificationContext : public antlr4::ParserRuleContext {
  public:
    ElementPropertySpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_BRACE();
    PropertyKeyValuePairListContext *propertyKeyValuePairList();
    antlr4::tree::TerminalNode *RIGHT_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementPropertySpecificationContext* elementPropertySpecification();

  class  PropertyKeyValuePairListContext : public antlr4::ParserRuleContext {
  public:
    PropertyKeyValuePairListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PropertyKeyValuePairContext *> propertyKeyValuePair();
    PropertyKeyValuePairContext* propertyKeyValuePair(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyKeyValuePairListContext* propertyKeyValuePairList();

  class  PropertyKeyValuePairContext : public antlr4::ParserRuleContext {
  public:
    PropertyKeyValuePairContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropertyNameContext *propertyName();
    antlr4::tree::TerminalNode *COLON();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyKeyValuePairContext* propertyKeyValuePair();

  class  EdgePatternContext : public antlr4::ParserRuleContext {
  public:
    EdgePatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FullEdgePatternContext *fullEdgePattern();
    AbbreviatedEdgePatternContext *abbreviatedEdgePattern();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgePatternContext* edgePattern();

  class  FullEdgePatternContext : public antlr4::ParserRuleContext {
  public:
    FullEdgePatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FullEdgePointingLeftContext *fullEdgePointingLeft();
    FullEdgeUndirectedContext *fullEdgeUndirected();
    FullEdgePointingRightContext *fullEdgePointingRight();
    FullEdgeLeftOrUndirectedContext *fullEdgeLeftOrUndirected();
    FullEdgeUndirectedOrRightContext *fullEdgeUndirectedOrRight();
    FullEdgeLeftOrRightContext *fullEdgeLeftOrRight();
    FullEdgeAnyDirectionContext *fullEdgeAnyDirection();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullEdgePatternContext* fullEdgePattern();

  class  FullEdgePointingLeftContext : public antlr4::ParserRuleContext {
  public:
    FullEdgePointingLeftContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> MINUS_SIGN();
    antlr4::tree::TerminalNode* MINUS_SIGN(size_t i);
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    ElementPatternFillerContext *elementPatternFiller();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullEdgePointingLeftContext* fullEdgePointingLeft();

  class  FullEdgeUndirectedContext : public antlr4::ParserRuleContext {
  public:
    FullEdgeUndirectedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> TILDE();
    antlr4::tree::TerminalNode* TILDE(size_t i);
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    ElementPatternFillerContext *elementPatternFiller();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullEdgeUndirectedContext* fullEdgeUndirected();

  class  FullEdgePointingRightContext : public antlr4::ParserRuleContext {
  public:
    FullEdgePointingRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MINUS_SIGN();
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    ElementPatternFillerContext *elementPatternFiller();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();
    antlr4::tree::TerminalNode *RIGHT_ARROW();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullEdgePointingRightContext* fullEdgePointingRight();

  class  FullEdgeLeftOrUndirectedContext : public antlr4::ParserRuleContext {
  public:
    FullEdgeLeftOrUndirectedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> TILDE();
    antlr4::tree::TerminalNode* TILDE(size_t i);
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    ElementPatternFillerContext *elementPatternFiller();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullEdgeLeftOrUndirectedContext* fullEdgeLeftOrUndirected();

  class  FullEdgeUndirectedOrRightContext : public antlr4::ParserRuleContext {
  public:
    FullEdgeUndirectedOrRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TILDE();
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    ElementPatternFillerContext *elementPatternFiller();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();
    antlr4::tree::TerminalNode *TILDE_RIGHT_ARROW();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullEdgeUndirectedOrRightContext* fullEdgeUndirectedOrRight();

  class  FullEdgeLeftOrRightContext : public antlr4::ParserRuleContext {
  public:
    FullEdgeLeftOrRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    antlr4::tree::TerminalNode *MINUS_SIGN();
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    ElementPatternFillerContext *elementPatternFiller();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();
    antlr4::tree::TerminalNode *RIGHT_ARROW();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullEdgeLeftOrRightContext* fullEdgeLeftOrRight();

  class  FullEdgeAnyDirectionContext : public antlr4::ParserRuleContext {
  public:
    FullEdgeAnyDirectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> MINUS_SIGN();
    antlr4::tree::TerminalNode* MINUS_SIGN(size_t i);
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    ElementPatternFillerContext *elementPatternFiller();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullEdgeAnyDirectionContext* fullEdgeAnyDirection();

  class  AbbreviatedEdgePatternContext : public antlr4::ParserRuleContext {
  public:
    AbbreviatedEdgePatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    AbbreviatedEdgePatternContext() = default;
    void copyFrom(AbbreviatedEdgePatternContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  AbbreviatedEdgeLeftOrUndirectedContext : public AbbreviatedEdgePatternContext {
  public:
    AbbreviatedEdgeLeftOrUndirectedContext(AbbreviatedEdgePatternContext *ctx);

    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    antlr4::tree::TerminalNode *TILDE();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AbbreviatedEdgePointingLeftContext : public AbbreviatedEdgePatternContext {
  public:
    AbbreviatedEdgePointingLeftContext(AbbreviatedEdgePatternContext *ctx);

    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    antlr4::tree::TerminalNode *MINUS_SIGN();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AbbreviatedEdgePointingRightContext : public AbbreviatedEdgePatternContext {
  public:
    AbbreviatedEdgePointingRightContext(AbbreviatedEdgePatternContext *ctx);

    antlr4::tree::TerminalNode *RIGHT_ARROW();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AbbreviatedEdgeUndirectedOrRightContext : public AbbreviatedEdgePatternContext {
  public:
    AbbreviatedEdgeUndirectedOrRightContext(AbbreviatedEdgePatternContext *ctx);

    antlr4::tree::TerminalNode *TILDE_RIGHT_ARROW();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AbbreviatedEdgeUndirectedContext : public AbbreviatedEdgePatternContext {
  public:
    AbbreviatedEdgeUndirectedContext(AbbreviatedEdgePatternContext *ctx);

    antlr4::tree::TerminalNode *TILDE();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AbbreviatedEdgeLeftOrRightContext : public AbbreviatedEdgePatternContext {
  public:
    AbbreviatedEdgeLeftOrRightContext(AbbreviatedEdgePatternContext *ctx);

    antlr4::tree::TerminalNode *LEFT_MINUS_RIGHT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AbbreviatedEdgeAnyDirectionContext : public AbbreviatedEdgePatternContext {
  public:
    AbbreviatedEdgeAnyDirectionContext(AbbreviatedEdgePatternContext *ctx);

    antlr4::tree::TerminalNode *MINUS_SIGN();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  AbbreviatedEdgePatternContext* abbreviatedEdgePattern();

  class  ParenthesizedPathPatternExpressionContext : public antlr4::ParserRuleContext {
  public:
    ParenthesizedPathPatternExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    PathPatternExpressionContext *pathPatternExpression();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    SubpathVariableDeclarationContext *subpathVariableDeclaration();
    PathModePrefixContext *pathModePrefix();
    ParenthesizedPathPatternWhereClauseContext *parenthesizedPathPatternWhereClause();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParenthesizedPathPatternExpressionContext* parenthesizedPathPatternExpression();

  class  SubpathVariableDeclarationContext : public antlr4::ParserRuleContext {
  public:
    SubpathVariableDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SubpathVariableContext *subpathVariable();
    antlr4::tree::TerminalNode *EQUALS_OPERATOR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SubpathVariableDeclarationContext* subpathVariableDeclaration();

  class  ParenthesizedPathPatternWhereClauseContext : public antlr4::ParserRuleContext {
  public:
    ParenthesizedPathPatternWhereClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHERE();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParenthesizedPathPatternWhereClauseContext* parenthesizedPathPatternWhereClause();

  class  InsertGraphPatternContext : public antlr4::ParserRuleContext {
  public:
    InsertGraphPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    InsertPathPatternListContext *insertPathPatternList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertGraphPatternContext* insertGraphPattern();

  class  InsertPathPatternListContext : public antlr4::ParserRuleContext {
  public:
    InsertPathPatternListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<InsertPathPatternContext *> insertPathPattern();
    InsertPathPatternContext* insertPathPattern(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertPathPatternListContext* insertPathPatternList();

  class  InsertPathPatternContext : public antlr4::ParserRuleContext {
  public:
    InsertPathPatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<InsertNodePatternContext *> insertNodePattern();
    InsertNodePatternContext* insertNodePattern(size_t i);
    std::vector<InsertEdgePatternContext *> insertEdgePattern();
    InsertEdgePatternContext* insertEdgePattern(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertPathPatternContext* insertPathPattern();

  class  InsertNodePatternContext : public antlr4::ParserRuleContext {
  public:
    InsertNodePatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    InsertElementPatternFillerContext *insertElementPatternFiller();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertNodePatternContext* insertNodePattern();

  class  InsertEdgePatternContext : public antlr4::ParserRuleContext {
  public:
    InsertEdgePatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    InsertEdgePointingLeftContext *insertEdgePointingLeft();
    InsertEdgePointingRightContext *insertEdgePointingRight();
    InsertEdgeUndirectedContext *insertEdgeUndirected();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertEdgePatternContext* insertEdgePattern();

  class  InsertEdgePointingLeftContext : public antlr4::ParserRuleContext {
  public:
    InsertEdgePointingLeftContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> MINUS_SIGN();
    antlr4::tree::TerminalNode* MINUS_SIGN(size_t i);
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();
    InsertElementPatternFillerContext *insertElementPatternFiller();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertEdgePointingLeftContext* insertEdgePointingLeft();

  class  InsertEdgePointingRightContext : public antlr4::ParserRuleContext {
  public:
    InsertEdgePointingRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MINUS_SIGN();
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();
    antlr4::tree::TerminalNode *RIGHT_ARROW();
    InsertElementPatternFillerContext *insertElementPatternFiller();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertEdgePointingRightContext* insertEdgePointingRight();

  class  InsertEdgeUndirectedContext : public antlr4::ParserRuleContext {
  public:
    InsertEdgeUndirectedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> TILDE();
    antlr4::tree::TerminalNode* TILDE(size_t i);
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();
    InsertElementPatternFillerContext *insertElementPatternFiller();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertEdgeUndirectedContext* insertEdgeUndirected();

  class  InsertElementPatternFillerContext : public antlr4::ParserRuleContext {
  public:
    InsertElementPatternFillerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementVariableDeclarationContext *elementVariableDeclaration();
    LabelAndPropertySetSpecificationContext *labelAndPropertySetSpecification();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertElementPatternFillerContext* insertElementPatternFiller();

  class  LabelAndPropertySetSpecificationContext : public antlr4::ParserRuleContext {
  public:
    LabelAndPropertySetSpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IsOrColonContext *isOrColon();
    LabelSetSpecificationContext *labelSetSpecification();
    ElementPropertySpecificationContext *elementPropertySpecification();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabelAndPropertySetSpecificationContext* labelAndPropertySetSpecification();

  class  LabelExpressionContext : public antlr4::ParserRuleContext {
  public:
    LabelExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LabelTermContext *> labelTerm();
    LabelTermContext* labelTerm(size_t i);
    std::vector<antlr4::tree::TerminalNode *> VERTICAL_BAR();
    antlr4::tree::TerminalNode* VERTICAL_BAR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabelExpressionContext* labelExpression();

  class  LabelTermContext : public antlr4::ParserRuleContext {
  public:
    LabelTermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LabelFactorContext *> labelFactor();
    LabelFactorContext* labelFactor(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AMPERSAND();
    antlr4::tree::TerminalNode* AMPERSAND(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabelTermContext* labelTerm();

  class  LabelFactorContext : public antlr4::ParserRuleContext {
  public:
    LabelFactorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LabelPrimaryContext *labelPrimary();
    antlr4::tree::TerminalNode *EXCLAMATION_MARK();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabelFactorContext* labelFactor();

  class  LabelPrimaryContext : public antlr4::ParserRuleContext {
  public:
    LabelPrimaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LabelNameContext *labelName();
    WildcardLabelContext *wildcardLabel();
    ParenthesizedLabelExpressionContext *parenthesizedLabelExpression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabelPrimaryContext* labelPrimary();

  class  WildcardLabelContext : public antlr4::ParserRuleContext {
  public:
    WildcardLabelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PERCENT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WildcardLabelContext* wildcardLabel();

  class  ParenthesizedLabelExpressionContext : public antlr4::ParserRuleContext {
  public:
    ParenthesizedLabelExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    LabelExpressionContext *labelExpression();
    antlr4::tree::TerminalNode *RIGHT_PAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParenthesizedLabelExpressionContext* parenthesizedLabelExpression();

  class  GraphPatternQuantifierContext : public antlr4::ParserRuleContext {
  public:
    GraphPatternQuantifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASTERISK();
    antlr4::tree::TerminalNode *PLUS_SIGN();
    FixedQuantifierContext *fixedQuantifier();
    GeneralQuantifierContext *generalQuantifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphPatternQuantifierContext* graphPatternQuantifier();

  class  FixedQuantifierContext : public antlr4::ParserRuleContext {
  public:
    FixedQuantifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_BRACE();
    antlr4::tree::TerminalNode *UNSIGNED_DECIMAL_INTEGER();
    antlr4::tree::TerminalNode *RIGHT_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedQuantifierContext* fixedQuantifier();

  class  GeneralQuantifierContext : public antlr4::ParserRuleContext {
  public:
    GeneralQuantifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_BRACE();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RIGHT_BRACE();
    LowerBoundContext *lowerBound();
    UpperBoundContext *upperBound();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GeneralQuantifierContext* generalQuantifier();

  class  LowerBoundContext : public antlr4::ParserRuleContext {
  public:
    LowerBoundContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_DECIMAL_INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LowerBoundContext* lowerBound();

  class  UpperBoundContext : public antlr4::ParserRuleContext {
  public:
    UpperBoundContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_DECIMAL_INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UpperBoundContext* upperBound();

  class  SimplifiedPathPatternExpressionContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedPathPatternExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimplifiedDefaultingLeftContext *simplifiedDefaultingLeft();
    SimplifiedDefaultingUndirectedContext *simplifiedDefaultingUndirected();
    SimplifiedDefaultingRightContext *simplifiedDefaultingRight();
    SimplifiedDefaultingLeftOrUndirectedContext *simplifiedDefaultingLeftOrUndirected();
    SimplifiedDefaultingUndirectedOrRightContext *simplifiedDefaultingUndirectedOrRight();
    SimplifiedDefaultingLeftOrRightContext *simplifiedDefaultingLeftOrRight();
    SimplifiedDefaultingAnyDirectionContext *simplifiedDefaultingAnyDirection();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedPathPatternExpressionContext* simplifiedPathPatternExpression();

  class  SimplifiedDefaultingLeftContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedDefaultingLeftContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_MINUS_SLASH();
    SimplifiedContentsContext *simplifiedContents();
    antlr4::tree::TerminalNode *SOLIDUS();
    antlr4::tree::TerminalNode *MINUS_SIGN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedDefaultingLeftContext* simplifiedDefaultingLeft();

  class  SimplifiedDefaultingUndirectedContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedDefaultingUndirectedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TILDE_SLASH();
    SimplifiedContentsContext *simplifiedContents();
    antlr4::tree::TerminalNode *SOLIDUS();
    antlr4::tree::TerminalNode *TILDE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedDefaultingUndirectedContext* simplifiedDefaultingUndirected();

  class  SimplifiedDefaultingRightContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedDefaultingRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MINUS_SLASH();
    SimplifiedContentsContext *simplifiedContents();
    antlr4::tree::TerminalNode *SLASH_MINUS_RIGHT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedDefaultingRightContext* simplifiedDefaultingRight();

  class  SimplifiedDefaultingLeftOrUndirectedContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedDefaultingLeftOrUndirectedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_TILDE_SLASH();
    SimplifiedContentsContext *simplifiedContents();
    antlr4::tree::TerminalNode *SOLIDUS();
    antlr4::tree::TerminalNode *TILDE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedDefaultingLeftOrUndirectedContext* simplifiedDefaultingLeftOrUndirected();

  class  SimplifiedDefaultingUndirectedOrRightContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedDefaultingUndirectedOrRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TILDE_SLASH();
    SimplifiedContentsContext *simplifiedContents();
    antlr4::tree::TerminalNode *SLASH_TILDE_RIGHT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedDefaultingUndirectedOrRightContext* simplifiedDefaultingUndirectedOrRight();

  class  SimplifiedDefaultingLeftOrRightContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedDefaultingLeftOrRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_MINUS_SLASH();
    SimplifiedContentsContext *simplifiedContents();
    antlr4::tree::TerminalNode *SLASH_MINUS_RIGHT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedDefaultingLeftOrRightContext* simplifiedDefaultingLeftOrRight();

  class  SimplifiedDefaultingAnyDirectionContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedDefaultingAnyDirectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MINUS_SLASH();
    SimplifiedContentsContext *simplifiedContents();
    antlr4::tree::TerminalNode *SOLIDUS();
    antlr4::tree::TerminalNode *MINUS_SIGN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedDefaultingAnyDirectionContext* simplifiedDefaultingAnyDirection();

  class  SimplifiedContentsContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedContentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimplifiedTermContext *simplifiedTerm();
    SimplifiedPathUnionContext *simplifiedPathUnion();
    SimplifiedMultisetAlternationContext *simplifiedMultisetAlternation();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedContentsContext* simplifiedContents();

  class  SimplifiedPathUnionContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedPathUnionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SimplifiedTermContext *> simplifiedTerm();
    SimplifiedTermContext* simplifiedTerm(size_t i);
    std::vector<antlr4::tree::TerminalNode *> VERTICAL_BAR();
    antlr4::tree::TerminalNode* VERTICAL_BAR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedPathUnionContext* simplifiedPathUnion();

  class  SimplifiedMultisetAlternationContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedMultisetAlternationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SimplifiedTermContext *> simplifiedTerm();
    SimplifiedTermContext* simplifiedTerm(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MULTISET_ALTERNATION_OPERATOR();
    antlr4::tree::TerminalNode* MULTISET_ALTERNATION_OPERATOR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedMultisetAlternationContext* simplifiedMultisetAlternation();

  class  SimplifiedTermContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedTermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SimplifiedFactorLowContext *> simplifiedFactorLow();
    SimplifiedFactorLowContext* simplifiedFactorLow(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedTermContext* simplifiedTerm();

  class  SimplifiedFactorLowContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedFactorLowContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SimplifiedFactorHighContext *> simplifiedFactorHigh();
    SimplifiedFactorHighContext* simplifiedFactorHigh(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AMPERSAND();
    antlr4::tree::TerminalNode* AMPERSAND(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedFactorLowContext* simplifiedFactorLow();

  class  SimplifiedFactorHighContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedFactorHighContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimplifiedTertiaryContext *simplifiedTertiary();
    SimplifiedQuantifiedContext *simplifiedQuantified();
    SimplifiedQuestionedContext *simplifiedQuestioned();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedFactorHighContext* simplifiedFactorHigh();

  class  SimplifiedQuantifiedContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedQuantifiedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimplifiedTertiaryContext *simplifiedTertiary();
    GraphPatternQuantifierContext *graphPatternQuantifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedQuantifiedContext* simplifiedQuantified();

  class  SimplifiedQuestionedContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedQuestionedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimplifiedTertiaryContext *simplifiedTertiary();
    antlr4::tree::TerminalNode *QUESTION_MARK();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedQuestionedContext* simplifiedQuestioned();

  class  SimplifiedTertiaryContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedTertiaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimplifiedDirectionOverrideContext *simplifiedDirectionOverride();
    SimplifiedSecondaryContext *simplifiedSecondary();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedTertiaryContext* simplifiedTertiary();

  class  SimplifiedDirectionOverrideContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedDirectionOverrideContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimplifiedOverrideLeftContext *simplifiedOverrideLeft();
    SimplifiedOverrideUndirectedContext *simplifiedOverrideUndirected();
    SimplifiedOverrideRightContext *simplifiedOverrideRight();
    SimplifiedOverrideLeftOrUndirectedContext *simplifiedOverrideLeftOrUndirected();
    SimplifiedOverrideUndirectedOrRightContext *simplifiedOverrideUndirectedOrRight();
    SimplifiedOverrideLeftOrRightContext *simplifiedOverrideLeftOrRight();
    SimplifiedOverrideAnyDirectionContext *simplifiedOverrideAnyDirection();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedDirectionOverrideContext* simplifiedDirectionOverride();

  class  SimplifiedOverrideLeftContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedOverrideLeftContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    SimplifiedSecondaryContext *simplifiedSecondary();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedOverrideLeftContext* simplifiedOverrideLeft();

  class  SimplifiedOverrideUndirectedContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedOverrideUndirectedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TILDE();
    SimplifiedSecondaryContext *simplifiedSecondary();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedOverrideUndirectedContext* simplifiedOverrideUndirected();

  class  SimplifiedOverrideRightContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedOverrideRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimplifiedSecondaryContext *simplifiedSecondary();
    antlr4::tree::TerminalNode *RIGHT_ANGLE_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedOverrideRightContext* simplifiedOverrideRight();

  class  SimplifiedOverrideLeftOrUndirectedContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedOverrideLeftOrUndirectedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    antlr4::tree::TerminalNode *TILDE();
    SimplifiedSecondaryContext *simplifiedSecondary();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedOverrideLeftOrUndirectedContext* simplifiedOverrideLeftOrUndirected();

  class  SimplifiedOverrideUndirectedOrRightContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedOverrideUndirectedOrRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TILDE();
    SimplifiedSecondaryContext *simplifiedSecondary();
    antlr4::tree::TerminalNode *RIGHT_ANGLE_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedOverrideUndirectedOrRightContext* simplifiedOverrideUndirectedOrRight();

  class  SimplifiedOverrideLeftOrRightContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedOverrideLeftOrRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    SimplifiedSecondaryContext *simplifiedSecondary();
    antlr4::tree::TerminalNode *RIGHT_ANGLE_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedOverrideLeftOrRightContext* simplifiedOverrideLeftOrRight();

  class  SimplifiedOverrideAnyDirectionContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedOverrideAnyDirectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MINUS_SIGN();
    SimplifiedSecondaryContext *simplifiedSecondary();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedOverrideAnyDirectionContext* simplifiedOverrideAnyDirection();

  class  SimplifiedSecondaryContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedSecondaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimplifiedPrimaryContext *simplifiedPrimary();
    SimplifiedNegationContext *simplifiedNegation();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedSecondaryContext* simplifiedSecondary();

  class  SimplifiedNegationContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedNegationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXCLAMATION_MARK();
    SimplifiedPrimaryContext *simplifiedPrimary();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedNegationContext* simplifiedNegation();

  class  SimplifiedPrimaryContext : public antlr4::ParserRuleContext {
  public:
    SimplifiedPrimaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LabelNameContext *labelName();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    SimplifiedContentsContext *simplifiedContents();
    antlr4::tree::TerminalNode *RIGHT_PAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimplifiedPrimaryContext* simplifiedPrimary();

  class  WhereClauseContext : public antlr4::ParserRuleContext {
  public:
    WhereClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHERE();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WhereClauseContext* whereClause();

  class  YieldClauseContext : public antlr4::ParserRuleContext {
  public:
    YieldClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *YIELD();
    YieldItemListContext *yieldItemList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  YieldClauseContext* yieldClause();

  class  YieldItemListContext : public antlr4::ParserRuleContext {
  public:
    YieldItemListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<YieldItemContext *> yieldItem();
    YieldItemContext* yieldItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  YieldItemListContext* yieldItemList();

  class  YieldItemContext : public antlr4::ParserRuleContext {
  public:
    YieldItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    YieldItemNameContext *yieldItemName();
    YieldItemAliasContext *yieldItemAlias();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  YieldItemContext* yieldItem();

  class  YieldItemNameContext : public antlr4::ParserRuleContext {
  public:
    YieldItemNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FieldNameContext *fieldName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  YieldItemNameContext* yieldItemName();

  class  YieldItemAliasContext : public antlr4::ParserRuleContext {
  public:
    YieldItemAliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AS();
    BindingVariableContext *bindingVariable();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  YieldItemAliasContext* yieldItemAlias();

  class  GroupByClauseContext : public antlr4::ParserRuleContext {
  public:
    GroupByClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GROUP();
    antlr4::tree::TerminalNode *BY();
    GroupingElementListContext *groupingElementList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupByClauseContext* groupByClause();

  class  GroupingElementListContext : public antlr4::ParserRuleContext {
  public:
    GroupingElementListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<GroupingElementContext *> groupingElement();
    GroupingElementContext* groupingElement(size_t i);
    antlr4::tree::TerminalNode *COMMA();
    EmptyGroupingSetContext *emptyGroupingSet();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupingElementListContext* groupingElementList();

  class  GroupingElementContext : public antlr4::ParserRuleContext {
  public:
    GroupingElementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingVariableReferenceContext *bindingVariableReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupingElementContext* groupingElement();

  class  EmptyGroupingSetContext : public antlr4::ParserRuleContext {
  public:
    EmptyGroupingSetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EmptyGroupingSetContext* emptyGroupingSet();

  class  OrderByClauseContext : public antlr4::ParserRuleContext {
  public:
    OrderByClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ORDER();
    antlr4::tree::TerminalNode *BY();
    SortSpecificationListContext *sortSpecificationList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrderByClauseContext* orderByClause();

  class  AggregateFunctionContext : public antlr4::ParserRuleContext {
  public:
    AggregateFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    AggregateFunctionContext() = default;
    void copyFrom(AggregateFunctionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GqlCountAllFunctionContext : public AggregateFunctionContext {
  public:
    GqlCountAllFunctionContext(AggregateFunctionContext *ctx);

    antlr4::tree::TerminalNode *COUNT();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *ASTERISK();
    antlr4::tree::TerminalNode *RIGHT_PAREN();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlGeneralSetFunctionContext : public AggregateFunctionContext {
  public:
    GqlGeneralSetFunctionContext(AggregateFunctionContext *ctx);

    GeneralSetFunctionTypeContext *generalSetFunctionType();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    SetQuantifierContext *setQuantifier();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlBinarySetFunctionContext : public AggregateFunctionContext {
  public:
    GqlBinarySetFunctionContext(AggregateFunctionContext *ctx);

    GQLParser::ExpressionContext *lhs = nullptr;
    GQLParser::ExpressionContext *rhs = nullptr;
    BinarySetFunctionTypeContext *binarySetFunctionType();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    SetQuantifierContext *setQuantifier();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  AggregateFunctionContext* aggregateFunction();

  class  GeneralSetFunctionTypeContext : public antlr4::ParserRuleContext {
  public:
    GeneralSetFunctionTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AVG();
    antlr4::tree::TerminalNode *COUNT();
    antlr4::tree::TerminalNode *MAX();
    antlr4::tree::TerminalNode *MIN();
    antlr4::tree::TerminalNode *SUM();
    antlr4::tree::TerminalNode *COLLECT();
    antlr4::tree::TerminalNode *STDDEV_SAMP();
    antlr4::tree::TerminalNode *STDDEV_POP();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GeneralSetFunctionTypeContext* generalSetFunctionType();

  class  SetQuantifierContext : public antlr4::ParserRuleContext {
  public:
    SetQuantifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DISTINCT();
    antlr4::tree::TerminalNode *ALL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetQuantifierContext* setQuantifier();

  class  BinarySetFunctionTypeContext : public antlr4::ParserRuleContext {
  public:
    BinarySetFunctionTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PERCENTILE_CONT();
    antlr4::tree::TerminalNode *PERCENTILE_DISC();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BinarySetFunctionTypeContext* binarySetFunctionType();

  class  SortSpecificationListContext : public antlr4::ParserRuleContext {
  public:
    SortSpecificationListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SortSpecificationContext *> sortSpecification();
    SortSpecificationContext* sortSpecification(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SortSpecificationListContext* sortSpecificationList();

  class  SortSpecificationContext : public antlr4::ParserRuleContext {
  public:
    SortSpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SortKeyContext *sortKey();
    OrderingSpecificationContext *orderingSpecification();
    NullOrderingContext *nullOrdering();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SortSpecificationContext* sortSpecification();

  class  SortKeyContext : public antlr4::ParserRuleContext {
  public:
    SortKeyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SortKeyContext* sortKey();

  class  OrderingSpecificationContext : public antlr4::ParserRuleContext {
  public:
    OrderingSpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASC();
    antlr4::tree::TerminalNode *ASCENDING();
    antlr4::tree::TerminalNode *DESC();
    antlr4::tree::TerminalNode *DESCENDING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrderingSpecificationContext* orderingSpecification();

  class  NullOrderingContext : public antlr4::ParserRuleContext {
  public:
    NullOrderingContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NULLS();
    antlr4::tree::TerminalNode *FIRST();
    antlr4::tree::TerminalNode *LAST();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NullOrderingContext* nullOrdering();

  class  LimitClauseContext : public antlr4::ParserRuleContext {
  public:
    LimitClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LIMIT();
    UnsignedIntegerSpecificationContext *unsignedIntegerSpecification();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LimitClauseContext* limitClause();

  class  OffsetClauseContext : public antlr4::ParserRuleContext {
  public:
    OffsetClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OffsetSynonymContext *offsetSynonym();
    UnsignedIntegerSpecificationContext *unsignedIntegerSpecification();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OffsetClauseContext* offsetClause();

  class  OffsetSynonymContext : public antlr4::ParserRuleContext {
  public:
    OffsetSynonymContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OFFSET();
    antlr4::tree::TerminalNode *SKIP_();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OffsetSynonymContext* offsetSynonym();

  class  NestedGraphTypeSpecificationContext : public antlr4::ParserRuleContext {
  public:
    NestedGraphTypeSpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_BRACE();
    GraphTypeSpecificationBodyContext *graphTypeSpecificationBody();
    antlr4::tree::TerminalNode *RIGHT_BRACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NestedGraphTypeSpecificationContext* nestedGraphTypeSpecification();

  class  GraphTypeSpecificationBodyContext : public antlr4::ParserRuleContext {
  public:
    GraphTypeSpecificationBodyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementTypeDefinitionListContext *elementTypeDefinitionList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphTypeSpecificationBodyContext* graphTypeSpecificationBody();

  class  ElementTypeDefinitionListContext : public antlr4::ParserRuleContext {
  public:
    ElementTypeDefinitionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ElementTypeDefinitionContext *> elementTypeDefinition();
    ElementTypeDefinitionContext* elementTypeDefinition(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementTypeDefinitionListContext* elementTypeDefinitionList();

  class  ElementTypeDefinitionContext : public antlr4::ParserRuleContext {
  public:
    ElementTypeDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NodeTypeDefinitionContext *nodeTypeDefinition();
    EdgeTypeDefinitionContext *edgeTypeDefinition();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementTypeDefinitionContext* elementTypeDefinition();

  class  NodeTypeDefinitionContext : public antlr4::ParserRuleContext {
  public:
    NodeTypeDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NodeTypePatternContext *nodeTypePattern();
    antlr4::tree::TerminalNode *NODE_SYNONYM();
    NodeTypePhraseContext *nodeTypePhrase();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeTypeDefinitionContext* nodeTypeDefinition();

  class  NodeTypePatternContext : public antlr4::ParserRuleContext {
  public:
    NodeTypePatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    NodeTypeNameContext *nodeTypeName();
    NodeTypeFillerContext *nodeTypeFiller();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeTypePatternContext* nodeTypePattern();

  class  NodeTypePhraseContext : public antlr4::ParserRuleContext {
  public:
    NodeTypePhraseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NodeTypeNameContext *nodeTypeName();
    antlr4::tree::TerminalNode *TYPE();
    NodeTypeFillerContext *nodeTypeFiller();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeTypePhraseContext* nodeTypePhrase();

  class  NodeTypeNameContext : public antlr4::ParserRuleContext {
  public:
    NodeTypeNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementTypeNameContext *elementTypeName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeTypeNameContext* nodeTypeName();

  class  NodeTypeFillerContext : public antlr4::ParserRuleContext {
  public:
    NodeTypeFillerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NodeTypeLabelSetDefinitionContext *nodeTypeLabelSetDefinition();
    NodeTypePropertyTypeSetDefinitionContext *nodeTypePropertyTypeSetDefinition();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeTypeFillerContext* nodeTypeFiller();

  class  NodeTypeLabelSetDefinitionContext : public antlr4::ParserRuleContext {
  public:
    NodeTypeLabelSetDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LabelSetDefinitionContext *labelSetDefinition();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeTypeLabelSetDefinitionContext* nodeTypeLabelSetDefinition();

  class  NodeTypePropertyTypeSetDefinitionContext : public antlr4::ParserRuleContext {
  public:
    NodeTypePropertyTypeSetDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropertyTypeSetDefinitionContext *propertyTypeSetDefinition();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeTypePropertyTypeSetDefinitionContext* nodeTypePropertyTypeSetDefinition();

  class  EdgeTypeDefinitionContext : public antlr4::ParserRuleContext {
  public:
    EdgeTypeDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EdgeTypePatternContext *edgeTypePattern();
    antlr4::tree::TerminalNode *EDGE_SYNONYM();
    EdgeTypePhraseContext *edgeTypePhrase();
    EdgeKindContext *edgeKind();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeTypeDefinitionContext* edgeTypeDefinition();

  class  EdgeTypePatternContext : public antlr4::ParserRuleContext {
  public:
    EdgeTypePatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FullEdgeTypePatternContext *fullEdgeTypePattern();
    AbbreviatedEdgeTypePatternContext *abbreviatedEdgeTypePattern();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeTypePatternContext* edgeTypePattern();

  class  EdgeTypePhraseContext : public antlr4::ParserRuleContext {
  public:
    EdgeTypePhraseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EdgeTypeNameContext *edgeTypeName();
    antlr4::tree::TerminalNode *TYPE();
    EdgeTypeFillerContext *edgeTypeFiller();
    EndpointDefinitionContext *endpointDefinition();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeTypePhraseContext* edgeTypePhrase();

  class  EdgeTypeNameContext : public antlr4::ParserRuleContext {
  public:
    EdgeTypeNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementTypeNameContext *elementTypeName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeTypeNameContext* edgeTypeName();

  class  EdgeTypeFillerContext : public antlr4::ParserRuleContext {
  public:
    EdgeTypeFillerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EdgeTypeLabelSetDefinitionContext *edgeTypeLabelSetDefinition();
    EdgeTypePropertyTypeSetDefinitionContext *edgeTypePropertyTypeSetDefinition();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeTypeFillerContext* edgeTypeFiller();

  class  EdgeTypeLabelSetDefinitionContext : public antlr4::ParserRuleContext {
  public:
    EdgeTypeLabelSetDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LabelSetDefinitionContext *labelSetDefinition();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeTypeLabelSetDefinitionContext* edgeTypeLabelSetDefinition();

  class  EdgeTypePropertyTypeSetDefinitionContext : public antlr4::ParserRuleContext {
  public:
    EdgeTypePropertyTypeSetDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropertyTypeSetDefinitionContext *propertyTypeSetDefinition();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeTypePropertyTypeSetDefinitionContext* edgeTypePropertyTypeSetDefinition();

  class  FullEdgeTypePatternContext : public antlr4::ParserRuleContext {
  public:
    FullEdgeTypePatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FullEdgeTypePatternPointingRightContext *fullEdgeTypePatternPointingRight();
    FullEdgeTypePatternPointingLeftContext *fullEdgeTypePatternPointingLeft();
    FullEdgeTypePatternUndirectedContext *fullEdgeTypePatternUndirected();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullEdgeTypePatternContext* fullEdgeTypePattern();

  class  FullEdgeTypePatternPointingRightContext : public antlr4::ParserRuleContext {
  public:
    FullEdgeTypePatternPointingRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SourceNodeTypeReferenceContext *sourceNodeTypeReference();
    ArcTypePointingRightContext *arcTypePointingRight();
    DestinationNodeTypeReferenceContext *destinationNodeTypeReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullEdgeTypePatternPointingRightContext* fullEdgeTypePatternPointingRight();

  class  FullEdgeTypePatternPointingLeftContext : public antlr4::ParserRuleContext {
  public:
    FullEdgeTypePatternPointingLeftContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DestinationNodeTypeReferenceContext *destinationNodeTypeReference();
    ArcTypePointingLeftContext *arcTypePointingLeft();
    SourceNodeTypeReferenceContext *sourceNodeTypeReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullEdgeTypePatternPointingLeftContext* fullEdgeTypePatternPointingLeft();

  class  FullEdgeTypePatternUndirectedContext : public antlr4::ParserRuleContext {
  public:
    FullEdgeTypePatternUndirectedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SourceNodeTypeReferenceContext *sourceNodeTypeReference();
    ArcTypeUndirectedContext *arcTypeUndirected();
    DestinationNodeTypeReferenceContext *destinationNodeTypeReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullEdgeTypePatternUndirectedContext* fullEdgeTypePatternUndirected();

  class  ArcTypePointingRightContext : public antlr4::ParserRuleContext {
  public:
    ArcTypePointingRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MINUS_SIGN();
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    ArcTypeFillerContext *arcTypeFiller();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();
    antlr4::tree::TerminalNode *RIGHT_ARROW();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArcTypePointingRightContext* arcTypePointingRight();

  class  ArcTypePointingLeftContext : public antlr4::ParserRuleContext {
  public:
    ArcTypePointingLeftContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> MINUS_SIGN();
    antlr4::tree::TerminalNode* MINUS_SIGN(size_t i);
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    ArcTypeFillerContext *arcTypeFiller();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArcTypePointingLeftContext* arcTypePointingLeft();

  class  ArcTypeUndirectedContext : public antlr4::ParserRuleContext {
  public:
    ArcTypeUndirectedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> TILDE();
    antlr4::tree::TerminalNode* TILDE(size_t i);
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    ArcTypeFillerContext *arcTypeFiller();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArcTypeUndirectedContext* arcTypeUndirected();

  class  ArcTypeFillerContext : public antlr4::ParserRuleContext {
  public:
    ArcTypeFillerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EdgeTypeNameContext *edgeTypeName();
    EdgeTypeFillerContext *edgeTypeFiller();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArcTypeFillerContext* arcTypeFiller();

  class  AbbreviatedEdgeTypePatternContext : public antlr4::ParserRuleContext {
  public:
    AbbreviatedEdgeTypePatternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AbbreviatedEdgeTypePatternPointingRightContext *abbreviatedEdgeTypePatternPointingRight();
    AbbreviatedEdgeTypePatternPointingLeftContext *abbreviatedEdgeTypePatternPointingLeft();
    AbbreviatedEdgeTypePatternUndirectedContext *abbreviatedEdgeTypePatternUndirected();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AbbreviatedEdgeTypePatternContext* abbreviatedEdgeTypePattern();

  class  AbbreviatedEdgeTypePatternPointingRightContext : public antlr4::ParserRuleContext {
  public:
    AbbreviatedEdgeTypePatternPointingRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SourceNodeTypeReferenceContext *sourceNodeTypeReference();
    antlr4::tree::TerminalNode *RIGHT_ARROW();
    DestinationNodeTypeReferenceContext *destinationNodeTypeReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AbbreviatedEdgeTypePatternPointingRightContext* abbreviatedEdgeTypePatternPointingRight();

  class  AbbreviatedEdgeTypePatternPointingLeftContext : public antlr4::ParserRuleContext {
  public:
    AbbreviatedEdgeTypePatternPointingLeftContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DestinationNodeTypeReferenceContext *destinationNodeTypeReference();
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    antlr4::tree::TerminalNode *MINUS_SIGN();
    SourceNodeTypeReferenceContext *sourceNodeTypeReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AbbreviatedEdgeTypePatternPointingLeftContext* abbreviatedEdgeTypePatternPointingLeft();

  class  AbbreviatedEdgeTypePatternUndirectedContext : public antlr4::ParserRuleContext {
  public:
    AbbreviatedEdgeTypePatternUndirectedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SourceNodeTypeReferenceContext *sourceNodeTypeReference();
    antlr4::tree::TerminalNode *TILDE();
    DestinationNodeTypeReferenceContext *destinationNodeTypeReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AbbreviatedEdgeTypePatternUndirectedContext* abbreviatedEdgeTypePatternUndirected();

  class  NodeTypeReferenceContext : public antlr4::ParserRuleContext {
  public:
    NodeTypeReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SourceNodeTypeReferenceContext *sourceNodeTypeReference();
    DestinationNodeTypeReferenceContext *destinationNodeTypeReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeTypeReferenceContext* nodeTypeReference();

  class  SourceNodeTypeReferenceContext : public antlr4::ParserRuleContext {
  public:
    SourceNodeTypeReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    SourceNodeTypeNameContext *sourceNodeTypeName();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    NodeTypeFillerContext *nodeTypeFiller();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SourceNodeTypeReferenceContext* sourceNodeTypeReference();

  class  DestinationNodeTypeReferenceContext : public antlr4::ParserRuleContext {
  public:
    DestinationNodeTypeReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    DestinationNodeTypeNameContext *destinationNodeTypeName();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    NodeTypeFillerContext *nodeTypeFiller();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DestinationNodeTypeReferenceContext* destinationNodeTypeReference();

  class  EdgeKindContext : public antlr4::ParserRuleContext {
  public:
    EdgeKindContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DIRECTED();
    antlr4::tree::TerminalNode *UNDIRECTED();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeKindContext* edgeKind();

  class  EndpointDefinitionContext : public antlr4::ParserRuleContext {
  public:
    EndpointDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CONNECTING();
    EndpointPairDefinitionContext *endpointPairDefinition();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EndpointDefinitionContext* endpointDefinition();

  class  EndpointPairDefinitionContext : public antlr4::ParserRuleContext {
  public:
    EndpointPairDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EndpointPairDefinitionPointingRightContext *endpointPairDefinitionPointingRight();
    EndpointPairDefinitionPointingLeftContext *endpointPairDefinitionPointingLeft();
    EndpointPairDefinitionUndirectedContext *endpointPairDefinitionUndirected();
    AbbreviatedEdgeTypePatternContext *abbreviatedEdgeTypePattern();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EndpointPairDefinitionContext* endpointPairDefinition();

  class  EndpointPairDefinitionPointingRightContext : public antlr4::ParserRuleContext {
  public:
    EndpointPairDefinitionPointingRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    SourceNodeTypeNameContext *sourceNodeTypeName();
    ConnectorPointingRightContext *connectorPointingRight();
    DestinationNodeTypeNameContext *destinationNodeTypeName();
    antlr4::tree::TerminalNode *RIGHT_PAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EndpointPairDefinitionPointingRightContext* endpointPairDefinitionPointingRight();

  class  EndpointPairDefinitionPointingLeftContext : public antlr4::ParserRuleContext {
  public:
    EndpointPairDefinitionPointingLeftContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    DestinationNodeTypeNameContext *destinationNodeTypeName();
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    antlr4::tree::TerminalNode *MINUS_SIGN();
    SourceNodeTypeNameContext *sourceNodeTypeName();
    antlr4::tree::TerminalNode *RIGHT_PAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EndpointPairDefinitionPointingLeftContext* endpointPairDefinitionPointingLeft();

  class  EndpointPairDefinitionUndirectedContext : public antlr4::ParserRuleContext {
  public:
    EndpointPairDefinitionUndirectedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    SourceNodeTypeNameContext *sourceNodeTypeName();
    ConnectorUndirectedContext *connectorUndirected();
    DestinationNodeTypeNameContext *destinationNodeTypeName();
    antlr4::tree::TerminalNode *RIGHT_PAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EndpointPairDefinitionUndirectedContext* endpointPairDefinitionUndirected();

  class  ConnectorPointingRightContext : public antlr4::ParserRuleContext {
  public:
    ConnectorPointingRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TO();
    antlr4::tree::TerminalNode *RIGHT_ARROW();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConnectorPointingRightContext* connectorPointingRight();

  class  ConnectorUndirectedContext : public antlr4::ParserRuleContext {
  public:
    ConnectorUndirectedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TO();
    antlr4::tree::TerminalNode *TILDE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConnectorUndirectedContext* connectorUndirected();

  class  SourceNodeTypeNameContext : public antlr4::ParserRuleContext {
  public:
    SourceNodeTypeNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementTypeNameContext *elementTypeName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SourceNodeTypeNameContext* sourceNodeTypeName();

  class  DestinationNodeTypeNameContext : public antlr4::ParserRuleContext {
  public:
    DestinationNodeTypeNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementTypeNameContext *elementTypeName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DestinationNodeTypeNameContext* destinationNodeTypeName();

  class  LabelSetDefinitionContext : public antlr4::ParserRuleContext {
  public:
    LabelSetDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LABEL();
    LabelNameContext *labelName();
    antlr4::tree::TerminalNode *LABELS();
    LabelSetSpecificationContext *labelSetSpecification();
    IsOrColonContext *isOrColon();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabelSetDefinitionContext* labelSetDefinition();

  class  PropertyTypeSetDefinitionContext : public antlr4::ParserRuleContext {
  public:
    PropertyTypeSetDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_BRACE();
    antlr4::tree::TerminalNode *RIGHT_BRACE();
    PropertyTypeDefinitionListContext *propertyTypeDefinitionList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyTypeSetDefinitionContext* propertyTypeSetDefinition();

  class  PropertyTypeDefinitionListContext : public antlr4::ParserRuleContext {
  public:
    PropertyTypeDefinitionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PropertyTypeDefinitionContext *> propertyTypeDefinition();
    PropertyTypeDefinitionContext* propertyTypeDefinition(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyTypeDefinitionListContext* propertyTypeDefinitionList();

  class  PropertyTypeDefinitionContext : public antlr4::ParserRuleContext {
  public:
    PropertyTypeDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropertyNameContext *propertyName();
    PropertyValueTypeContext *propertyValueType();
    TypedContext *typed();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyTypeDefinitionContext* propertyTypeDefinition();

  class  PropertyValueTypeContext : public antlr4::ParserRuleContext {
  public:
    PropertyValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ValueTypeContext *valueType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyValueTypeContext* propertyValueType();

  class  BindingTableTypeContext : public antlr4::ParserRuleContext {
  public:
    BindingTableTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TABLE();
    FieldTypesSpecificationContext *fieldTypesSpecification();
    antlr4::tree::TerminalNode *BINDING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingTableTypeContext* bindingTableType();

  class  ValueTypeContext : public antlr4::ParserRuleContext {
  public:
    ValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ValueTypeContext() = default;
    void copyFrom(ValueTypeContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  DynamicPropertyValueTypeContext : public ValueTypeContext {
  public:
    DynamicPropertyValueTypeContext(ValueTypeContext *ctx);

    antlr4::tree::TerminalNode *PROPERTY();
    antlr4::tree::TerminalNode *VALUE();
    antlr4::tree::TerminalNode *ANY();
    NotNullContext *notNull();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PredefTypeContext : public ValueTypeContext {
  public:
    PredefTypeContext(ValueTypeContext *ctx);

    PredefinedTypeContext *predefinedType();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ListType1Context : public ValueTypeContext {
  public:
    ListType1Context(ValueTypeContext *ctx);

    ListValueTypeNameContext *listValueTypeName();
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    ValueTypeContext *valueType();
    antlr4::tree::TerminalNode *RIGHT_ANGLE_BRACKET();
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    MaxLengthContext *maxLength();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();
    NotNullContext *notNull();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RecordType1Context : public ValueTypeContext {
  public:
    RecordType1Context(ValueTypeContext *ctx);

    antlr4::tree::TerminalNode *RECORD();
    antlr4::tree::TerminalNode *ANY();
    NotNullContext *notNull();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ClosedDynamicUnionType2Context : public ValueTypeContext {
  public:
    ClosedDynamicUnionType2Context(ValueTypeContext *ctx);

    std::vector<ValueTypeContext *> valueType();
    ValueTypeContext* valueType(size_t i);
    std::vector<antlr4::tree::TerminalNode *> VERTICAL_BAR();
    antlr4::tree::TerminalNode* VERTICAL_BAR(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RecordType2Context : public ValueTypeContext {
  public:
    RecordType2Context(ValueTypeContext *ctx);

    FieldTypesSpecificationContext *fieldTypesSpecification();
    antlr4::tree::TerminalNode *RECORD();
    NotNullContext *notNull();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ListType2Context : public ValueTypeContext {
  public:
    ListType2Context(ValueTypeContext *ctx);

    ValueTypeContext *valueType();
    ListValueTypeNameContext *listValueTypeName();
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    MaxLengthContext *maxLength();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();
    NotNullContext *notNull();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PathTypeContext : public ValueTypeContext {
  public:
    PathTypeContext(ValueTypeContext *ctx);

    PathValueTypeContext *pathValueType();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  OpenDynamicUnionTypeContext : public ValueTypeContext {
  public:
    OpenDynamicUnionTypeContext(ValueTypeContext *ctx);

    antlr4::tree::TerminalNode *ANY();
    antlr4::tree::TerminalNode *VALUE();
    NotNullContext *notNull();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ClosedDynamicUnionType1Context : public ValueTypeContext {
  public:
    ClosedDynamicUnionType1Context(ValueTypeContext *ctx);

    antlr4::tree::TerminalNode *ANY();
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    std::vector<ValueTypeContext *> valueType();
    ValueTypeContext* valueType(size_t i);
    antlr4::tree::TerminalNode *RIGHT_ANGLE_BRACKET();
    antlr4::tree::TerminalNode *VALUE();
    std::vector<antlr4::tree::TerminalNode *> VERTICAL_BAR();
    antlr4::tree::TerminalNode* VERTICAL_BAR(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ValueTypeContext* valueType();
  ValueTypeContext* valueType(int precedence);
  class  TypedContext : public antlr4::ParserRuleContext {
  public:
    TypedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOUBLE_COLON();
    antlr4::tree::TerminalNode *TYPED();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypedContext* typed();

  class  PredefinedTypeContext : public antlr4::ParserRuleContext {
  public:
    PredefinedTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BooleanTypeContext *booleanType();
    CharacterStringTypeContext *characterStringType();
    ByteStringTypeContext *byteStringType();
    NumericTypeContext *numericType();
    TemporalTypeContext *temporalType();
    ReferenceValueTypeContext *referenceValueType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PredefinedTypeContext* predefinedType();

  class  BooleanTypeContext : public antlr4::ParserRuleContext {
  public:
    BooleanTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BOOL();
    antlr4::tree::TerminalNode *BOOLEAN();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BooleanTypeContext* booleanType();

  class  CharacterStringTypeContext : public antlr4::ParserRuleContext {
  public:
    CharacterStringTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *VARCHAR();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    MaxLengthContext *maxLength();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CharacterStringTypeContext* characterStringType();

  class  ByteStringTypeContext : public antlr4::ParserRuleContext {
  public:
    ByteStringTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BYTES();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    MaxLengthContext *maxLength();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    NotNullContext *notNull();
    MinLengthContext *minLength();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *BINARY();
    FixedLengthContext *fixedLength();
    antlr4::tree::TerminalNode *VARBINARY();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ByteStringTypeContext* byteStringType();

  class  MinLengthContext : public antlr4::ParserRuleContext {
  public:
    MinLengthContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_DECIMAL_INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MinLengthContext* minLength();

  class  MaxLengthContext : public antlr4::ParserRuleContext {
  public:
    MaxLengthContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_DECIMAL_INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MaxLengthContext* maxLength();

  class  FixedLengthContext : public antlr4::ParserRuleContext {
  public:
    FixedLengthContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_DECIMAL_INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedLengthContext* fixedLength();

  class  NumericTypeContext : public antlr4::ParserRuleContext {
  public:
    NumericTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExactNumericTypeContext *exactNumericType();
    ApproximateNumericTypeContext *approximateNumericType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumericTypeContext* numericType();

  class  ExactNumericTypeContext : public antlr4::ParserRuleContext {
  public:
    ExactNumericTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BinaryExactNumericTypeContext *binaryExactNumericType();
    DecimalExactNumericTypeContext *decimalExactNumericType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExactNumericTypeContext* exactNumericType();

  class  BinaryExactNumericTypeContext : public antlr4::ParserRuleContext {
  public:
    BinaryExactNumericTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SignedBinaryExactNumericTypeContext *signedBinaryExactNumericType();
    UnsignedBinaryExactNumericTypeContext *unsignedBinaryExactNumericType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BinaryExactNumericTypeContext* binaryExactNumericType();

  class  SignedBinaryExactNumericTypeContext : public antlr4::ParserRuleContext {
  public:
    SignedBinaryExactNumericTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INT8();
    NotNullContext *notNull();
    antlr4::tree::TerminalNode *INT16();
    antlr4::tree::TerminalNode *INT32();
    antlr4::tree::TerminalNode *INT64();
    antlr4::tree::TerminalNode *INT128();
    antlr4::tree::TerminalNode *INT256();
    antlr4::tree::TerminalNode *SMALLINT();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    PrecisionContext *precision();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    antlr4::tree::TerminalNode *BIGINT();
    VerboseBinaryExactNumericTypeContext *verboseBinaryExactNumericType();
    antlr4::tree::TerminalNode *SIGNED();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignedBinaryExactNumericTypeContext* signedBinaryExactNumericType();

  class  UnsignedBinaryExactNumericTypeContext : public antlr4::ParserRuleContext {
  public:
    UnsignedBinaryExactNumericTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UINT8();
    NotNullContext *notNull();
    antlr4::tree::TerminalNode *UINT16();
    antlr4::tree::TerminalNode *UINT32();
    antlr4::tree::TerminalNode *UINT64();
    antlr4::tree::TerminalNode *UINT128();
    antlr4::tree::TerminalNode *UINT256();
    antlr4::tree::TerminalNode *USMALLINT();
    antlr4::tree::TerminalNode *UINT();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    PrecisionContext *precision();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    antlr4::tree::TerminalNode *UBIGINT();
    antlr4::tree::TerminalNode *UNSIGNED();
    VerboseBinaryExactNumericTypeContext *verboseBinaryExactNumericType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnsignedBinaryExactNumericTypeContext* unsignedBinaryExactNumericType();

  class  VerboseBinaryExactNumericTypeContext : public antlr4::ParserRuleContext {
  public:
    VerboseBinaryExactNumericTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER8();
    NotNullContext *notNull();
    antlr4::tree::TerminalNode *INTEGER16();
    antlr4::tree::TerminalNode *INTEGER32();
    antlr4::tree::TerminalNode *INTEGER64();
    antlr4::tree::TerminalNode *INTEGER128();
    antlr4::tree::TerminalNode *INTEGER256();
    antlr4::tree::TerminalNode *SMALL();
    antlr4::tree::TerminalNode *INTEGER();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    PrecisionContext *precision();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    antlr4::tree::TerminalNode *BIG();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VerboseBinaryExactNumericTypeContext* verboseBinaryExactNumericType();

  class  DecimalExactNumericTypeContext : public antlr4::ParserRuleContext {
  public:
    DecimalExactNumericTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *DEC();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    PrecisionContext *precision();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    antlr4::tree::TerminalNode *COMMA();
    ScaleContext *scale();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DecimalExactNumericTypeContext* decimalExactNumericType();

  class  PrecisionContext : public antlr4::ParserRuleContext {
  public:
    PrecisionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_DECIMAL_INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrecisionContext* precision();

  class  ScaleContext : public antlr4::ParserRuleContext {
  public:
    ScaleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNSIGNED_DECIMAL_INTEGER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ScaleContext* scale();

  class  ApproximateNumericTypeContext : public antlr4::ParserRuleContext {
  public:
    ApproximateNumericTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FLOAT16();
    NotNullContext *notNull();
    antlr4::tree::TerminalNode *FLOAT32();
    antlr4::tree::TerminalNode *FLOAT64();
    antlr4::tree::TerminalNode *FLOAT128();
    antlr4::tree::TerminalNode *FLOAT256();
    antlr4::tree::TerminalNode *FLOAT();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    PrecisionContext *precision();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    antlr4::tree::TerminalNode *COMMA();
    ScaleContext *scale();
    antlr4::tree::TerminalNode *REAL();
    antlr4::tree::TerminalNode *DOUBLE();
    antlr4::tree::TerminalNode *PRECISION();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ApproximateNumericTypeContext* approximateNumericType();

  class  TemporalTypeContext : public antlr4::ParserRuleContext {
  public:
    TemporalTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TemporalInstantTypeContext *temporalInstantType();
    TemporalDurationTypeContext *temporalDurationType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TemporalTypeContext* temporalType();

  class  TemporalInstantTypeContext : public antlr4::ParserRuleContext {
  public:
    TemporalInstantTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DatetimeTypeContext *datetimeType();
    LocaldatetimeTypeContext *localdatetimeType();
    DateTypeContext *dateType();
    TimeTypeContext *timeType();
    LocaltimeTypeContext *localtimeType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TemporalInstantTypeContext* temporalInstantType();

  class  TemporalDurationTypeContext : public antlr4::ParserRuleContext {
  public:
    TemporalDurationTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DurationTypeContext *durationType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TemporalDurationTypeContext* temporalDurationType();

  class  DatetimeTypeContext : public antlr4::ParserRuleContext {
  public:
    DatetimeTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ZONED();
    antlr4::tree::TerminalNode *DATETIME();
    NotNullContext *notNull();
    antlr4::tree::TerminalNode *TIMESTAMP();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *TIMEZONE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DatetimeTypeContext* datetimeType();

  class  LocaldatetimeTypeContext : public antlr4::ParserRuleContext {
  public:
    LocaldatetimeTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCAL();
    antlr4::tree::TerminalNode *DATETIME();
    NotNullContext *notNull();
    antlr4::tree::TerminalNode *TIMESTAMP();
    antlr4::tree::TerminalNode *WITHOUT();
    antlr4::tree::TerminalNode *TIMEZONE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LocaldatetimeTypeContext* localdatetimeType();

  class  DateTypeContext : public antlr4::ParserRuleContext {
  public:
    DateTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DATE();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DateTypeContext* dateType();

  class  TimeTypeContext : public antlr4::ParserRuleContext {
  public:
    TimeTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ZONED();
    antlr4::tree::TerminalNode *TIME();
    NotNullContext *notNull();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *TIMEZONE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TimeTypeContext* timeType();

  class  LocaltimeTypeContext : public antlr4::ParserRuleContext {
  public:
    LocaltimeTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCAL();
    antlr4::tree::TerminalNode *TIME();
    NotNullContext *notNull();
    antlr4::tree::TerminalNode *WITHOUT();
    antlr4::tree::TerminalNode *TIMEZONE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LocaltimeTypeContext* localtimeType();

  class  DurationTypeContext : public antlr4::ParserRuleContext {
  public:
    DurationTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DURATION();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DurationTypeContext* durationType();

  class  ReferenceValueTypeContext : public antlr4::ParserRuleContext {
  public:
    ReferenceValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphReferenceValueTypeContext *graphReferenceValueType();
    BindingTableReferenceValueTypeContext *bindingTableReferenceValueType();
    NodeReferenceValueTypeContext *nodeReferenceValueType();
    EdgeReferenceValueTypeContext *edgeReferenceValueType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReferenceValueTypeContext* referenceValueType();

  class  GraphReferenceValueTypeContext : public antlr4::ParserRuleContext {
  public:
    GraphReferenceValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OpenGraphReferenceValueTypeContext *openGraphReferenceValueType();
    ClosedGraphReferenceValueTypeContext *closedGraphReferenceValueType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphReferenceValueTypeContext* graphReferenceValueType();

  class  ClosedGraphReferenceValueTypeContext : public antlr4::ParserRuleContext {
  public:
    ClosedGraphReferenceValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GRAPH();
    NestedGraphTypeSpecificationContext *nestedGraphTypeSpecification();
    antlr4::tree::TerminalNode *PROPERTY();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClosedGraphReferenceValueTypeContext* closedGraphReferenceValueType();

  class  OpenGraphReferenceValueTypeContext : public antlr4::ParserRuleContext {
  public:
    OpenGraphReferenceValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ANY();
    antlr4::tree::TerminalNode *GRAPH();
    antlr4::tree::TerminalNode *PROPERTY();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OpenGraphReferenceValueTypeContext* openGraphReferenceValueType();

  class  BindingTableReferenceValueTypeContext : public antlr4::ParserRuleContext {
  public:
    BindingTableReferenceValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingTableTypeContext *bindingTableType();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingTableReferenceValueTypeContext* bindingTableReferenceValueType();

  class  NodeReferenceValueTypeContext : public antlr4::ParserRuleContext {
  public:
    NodeReferenceValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OpenNodeReferenceValueTypeContext *openNodeReferenceValueType();
    ClosedNodeReferenceValueTypeContext *closedNodeReferenceValueType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeReferenceValueTypeContext* nodeReferenceValueType();

  class  ClosedNodeReferenceValueTypeContext : public antlr4::ParserRuleContext {
  public:
    ClosedNodeReferenceValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NodeTypeDefinitionContext *nodeTypeDefinition();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClosedNodeReferenceValueTypeContext* closedNodeReferenceValueType();

  class  OpenNodeReferenceValueTypeContext : public antlr4::ParserRuleContext {
  public:
    OpenNodeReferenceValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NODE_SYNONYM();
    antlr4::tree::TerminalNode *ANY();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OpenNodeReferenceValueTypeContext* openNodeReferenceValueType();

  class  EdgeReferenceValueTypeContext : public antlr4::ParserRuleContext {
  public:
    EdgeReferenceValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OpenEdgeReferenceValueTypeContext *openEdgeReferenceValueType();
    ClosedEdgeReferenceValueTypeContext *closedEdgeReferenceValueType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EdgeReferenceValueTypeContext* edgeReferenceValueType();

  class  ClosedEdgeReferenceValueTypeContext : public antlr4::ParserRuleContext {
  public:
    ClosedEdgeReferenceValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EdgeTypeDefinitionContext *edgeTypeDefinition();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClosedEdgeReferenceValueTypeContext* closedEdgeReferenceValueType();

  class  OpenEdgeReferenceValueTypeContext : public antlr4::ParserRuleContext {
  public:
    OpenEdgeReferenceValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EDGE_SYNONYM();
    antlr4::tree::TerminalNode *ANY();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OpenEdgeReferenceValueTypeContext* openEdgeReferenceValueType();

  class  ListValueTypeNameContext : public antlr4::ParserRuleContext {
  public:
    ListValueTypeNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ListValueTypeNameSynonymContext *listValueTypeNameSynonym();
    antlr4::tree::TerminalNode *GROUP();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ListValueTypeNameContext* listValueTypeName();

  class  ListValueTypeNameSynonymContext : public antlr4::ParserRuleContext {
  public:
    ListValueTypeNameSynonymContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LIST();
    antlr4::tree::TerminalNode *ARRAY();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ListValueTypeNameSynonymContext* listValueTypeNameSynonym();

  class  FieldTypesSpecificationContext : public antlr4::ParserRuleContext {
  public:
    FieldTypesSpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_BRACE();
    antlr4::tree::TerminalNode *RIGHT_BRACE();
    FieldTypeListContext *fieldTypeList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FieldTypesSpecificationContext* fieldTypesSpecification();

  class  FieldTypeListContext : public antlr4::ParserRuleContext {
  public:
    FieldTypeListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<FieldTypeContext *> fieldType();
    FieldTypeContext* fieldType(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FieldTypeListContext* fieldTypeList();

  class  PathValueTypeContext : public antlr4::ParserRuleContext {
  public:
    PathValueTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PATH();
    NotNullContext *notNull();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathValueTypeContext* pathValueType();

  class  NotNullContext : public antlr4::ParserRuleContext {
  public:
    NotNullContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *NULL_();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NotNullContext* notNull();

  class  FieldTypeContext : public antlr4::ParserRuleContext {
  public:
    FieldTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FieldNameContext *fieldName();
    ValueTypeContext *valueType();
    TypedContext *typed();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FieldTypeContext* fieldType();

  class  SchemaReferenceContext : public antlr4::ParserRuleContext {
  public:
    SchemaReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AbsoluteCatalogSchemaReferenceContext *absoluteCatalogSchemaReference();
    RelativeCatalogSchemaReferenceContext *relativeCatalogSchemaReference();
    ReferenceParameterContext *referenceParameter();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SchemaReferenceContext* schemaReference();

  class  AbsoluteCatalogSchemaReferenceContext : public antlr4::ParserRuleContext {
  public:
    AbsoluteCatalogSchemaReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SOLIDUS();
    AbsoluteDirectoryPathContext *absoluteDirectoryPath();
    SchemaNameContext *schemaName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AbsoluteCatalogSchemaReferenceContext* absoluteCatalogSchemaReference();

  class  CatalogSchemaParentAndNameContext : public antlr4::ParserRuleContext {
  public:
    CatalogSchemaParentAndNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AbsoluteDirectoryPathContext *absoluteDirectoryPath();
    SchemaNameContext *schemaName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CatalogSchemaParentAndNameContext* catalogSchemaParentAndName();

  class  RelativeCatalogSchemaReferenceContext : public antlr4::ParserRuleContext {
  public:
    RelativeCatalogSchemaReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PredefinedSchemaReferenceContext *predefinedSchemaReference();
    RelativeDirectoryPathContext *relativeDirectoryPath();
    SchemaNameContext *schemaName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RelativeCatalogSchemaReferenceContext* relativeCatalogSchemaReference();

  class  PredefinedSchemaReferenceContext : public antlr4::ParserRuleContext {
  public:
    PredefinedSchemaReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *HOME_SCHEMA();
    antlr4::tree::TerminalNode *CURRENT_SCHEMA();
    antlr4::tree::TerminalNode *PERIOD();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PredefinedSchemaReferenceContext* predefinedSchemaReference();

  class  AbsoluteDirectoryPathContext : public antlr4::ParserRuleContext {
  public:
    AbsoluteDirectoryPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SOLIDUS();
    SimpleDirectoryPathContext *simpleDirectoryPath();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AbsoluteDirectoryPathContext* absoluteDirectoryPath();

  class  RelativeDirectoryPathContext : public antlr4::ParserRuleContext {
  public:
    RelativeDirectoryPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> DOUBLE_PERIOD();
    antlr4::tree::TerminalNode* DOUBLE_PERIOD(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SOLIDUS();
    antlr4::tree::TerminalNode* SOLIDUS(size_t i);
    SimpleDirectoryPathContext *simpleDirectoryPath();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RelativeDirectoryPathContext* relativeDirectoryPath();

  class  SimpleDirectoryPathContext : public antlr4::ParserRuleContext {
  public:
    SimpleDirectoryPathContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DirectoryNameContext *> directoryName();
    DirectoryNameContext* directoryName(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SOLIDUS();
    antlr4::tree::TerminalNode* SOLIDUS(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleDirectoryPathContext* simpleDirectoryPath();

  class  GraphReferenceContext : public antlr4::ParserRuleContext {
  public:
    GraphReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CatalogObjectParentReferenceContext *catalogObjectParentReference();
    GraphNameContext *graphName();
    DelimitedGraphNameContext *delimitedGraphName();
    HomeGraphContext *homeGraph();
    ReferenceParameterContext *referenceParameter();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphReferenceContext* graphReference();

  class  CatalogGraphParentAndNameContext : public antlr4::ParserRuleContext {
  public:
    CatalogGraphParentAndNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphNameContext *graphName();
    CatalogObjectParentReferenceContext *catalogObjectParentReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CatalogGraphParentAndNameContext* catalogGraphParentAndName();

  class  HomeGraphContext : public antlr4::ParserRuleContext {
  public:
    HomeGraphContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *HOME_PROPERTY_GRAPH();
    antlr4::tree::TerminalNode *HOME_GRAPH();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HomeGraphContext* homeGraph();

  class  GraphTypeReferenceContext : public antlr4::ParserRuleContext {
  public:
    GraphTypeReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CatalogGraphTypeParentAndNameContext *catalogGraphTypeParentAndName();
    ReferenceParameterContext *referenceParameter();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GraphTypeReferenceContext* graphTypeReference();

  class  CatalogGraphTypeParentAndNameContext : public antlr4::ParserRuleContext {
  public:
    CatalogGraphTypeParentAndNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GraphTypeNameContext *graphTypeName();
    CatalogObjectParentReferenceContext *catalogObjectParentReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CatalogGraphTypeParentAndNameContext* catalogGraphTypeParentAndName();

  class  BindingTableReferenceContext : public antlr4::ParserRuleContext {
  public:
    BindingTableReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CatalogObjectParentReferenceContext *catalogObjectParentReference();
    BindingTableNameContext *bindingTableName();
    DelimitedBindingTableNameContext *delimitedBindingTableName();
    ReferenceParameterContext *referenceParameter();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BindingTableReferenceContext* bindingTableReference();

  class  CatalogBindingTableParentAndNameContext : public antlr4::ParserRuleContext {
  public:
    CatalogBindingTableParentAndNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BindingTableNameContext *bindingTableName();
    CatalogObjectParentReferenceContext *catalogObjectParentReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CatalogBindingTableParentAndNameContext* catalogBindingTableParentAndName();

  class  ProcedureReferenceContext : public antlr4::ParserRuleContext {
  public:
    ProcedureReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CatalogProcedureParentAndNameContext *catalogProcedureParentAndName();
    ReferenceParameterContext *referenceParameter();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProcedureReferenceContext* procedureReference();

  class  CatalogProcedureParentAndNameContext : public antlr4::ParserRuleContext {
  public:
    CatalogProcedureParentAndNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ProcedureNameContext *procedureName();
    CatalogObjectParentReferenceContext *catalogObjectParentReference();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CatalogProcedureParentAndNameContext* catalogProcedureParentAndName();

  class  CatalogObjectParentReferenceContext : public antlr4::ParserRuleContext {
  public:
    CatalogObjectParentReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SchemaReferenceContext *schemaReference();
    antlr4::tree::TerminalNode *SOLIDUS();
    std::vector<ObjectNameContext *> objectName();
    ObjectNameContext* objectName(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PERIOD();
    antlr4::tree::TerminalNode* PERIOD(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CatalogObjectParentReferenceContext* catalogObjectParentReference();

  class  ReferenceParameterContext : public antlr4::ParserRuleContext {
  public:
    ReferenceParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ParameterContext *parameter();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReferenceParameterContext* referenceParameter();

  class  ExternalObjectReferenceContext : public antlr4::ParserRuleContext {
  public:
    ExternalObjectReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *I_DONT_KNOW_3();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExternalObjectReferenceContext* externalObjectReference();

  class  ComparisonPredicateCondContext : public antlr4::ParserRuleContext {
  public:
    ComparisonPredicateCondContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CompOpContext *compOp();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComparisonPredicateCondContext* comparisonPredicateCond();

  class  CompOpContext : public antlr4::ParserRuleContext {
  public:
    CompOpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EQUALS_OPERATOR();
    antlr4::tree::TerminalNode *NOT_EQUALS_OPERATOR();
    antlr4::tree::TerminalNode *LEFT_ANGLE_BRACKET();
    antlr4::tree::TerminalNode *RIGHT_ANGLE_BRACKET();
    antlr4::tree::TerminalNode *LESS_THAN_OR_EQUALS_OPERATOR();
    antlr4::tree::TerminalNode *GREATER_THAN_OR_EQUALS_OPERATOR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CompOpContext* compOp();

  class  NullPredicateCondContext : public antlr4::ParserRuleContext {
  public:
    NullPredicateCondContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IS();
    antlr4::tree::TerminalNode *NULL_();
    antlr4::tree::TerminalNode *NOT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NullPredicateCondContext* nullPredicateCond();

  class  NormalizedPredicateCondContext : public antlr4::ParserRuleContext {
  public:
    NormalizedPredicateCondContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IS();
    antlr4::tree::TerminalNode *NORMALIZED();
    antlr4::tree::TerminalNode *NOT();
    NormalFormContext *normalForm();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NormalizedPredicateCondContext* normalizedPredicateCond();

  class  DirectedPredicateCondContext : public antlr4::ParserRuleContext {
  public:
    DirectedPredicateCondContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IS();
    antlr4::tree::TerminalNode *DIRECTED();
    antlr4::tree::TerminalNode *NOT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DirectedPredicateCondContext* directedPredicateCond();

  class  LabeledPredicateCondContext : public antlr4::ParserRuleContext {
  public:
    LabeledPredicateCondContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LabelExpressionContext *labelExpression();
    antlr4::tree::TerminalNode *IS();
    antlr4::tree::TerminalNode *LABELED();
    antlr4::tree::TerminalNode *COLON();
    antlr4::tree::TerminalNode *NOT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabeledPredicateCondContext* labeledPredicateCond();

  class  SourceDestinationPredicateCondContext : public antlr4::ParserRuleContext {
  public:
    SourceDestinationPredicateCondContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IS();
    antlr4::tree::TerminalNode *OF();
    ElementVariableReferenceContext *elementVariableReference();
    antlr4::tree::TerminalNode *SOURCE();
    antlr4::tree::TerminalNode *DESTINATION();
    antlr4::tree::TerminalNode *NOT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SourceDestinationPredicateCondContext* sourceDestinationPredicateCond();

  class  UnsignedValueSpecificationContext : public antlr4::ParserRuleContext {
  public:
    UnsignedValueSpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnsignedLiteralContext *unsignedLiteral();
    ParameterValueSpecificationContext *parameterValueSpecification();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnsignedValueSpecificationContext* unsignedValueSpecification();

  class  UnsignedIntegerSpecificationContext : public antlr4::ParserRuleContext {
  public:
    UnsignedIntegerSpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerLiteralContext *integerLiteral();
    ParameterContext *parameter();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnsignedIntegerSpecificationContext* unsignedIntegerSpecification();

  class  ParameterValueSpecificationContext : public antlr4::ParserRuleContext {
  public:
    ParameterValueSpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ParameterContext *parameter();
    PredefinedParameterContext *predefinedParameter();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterValueSpecificationContext* parameterValueSpecification();

  class  PredefinedParameterContext : public antlr4::ParserRuleContext {
  public:
    PredefinedParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CURRENT_USER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PredefinedParameterContext* predefinedParameter();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExpressionContext() = default;
    void copyFrom(ExpressionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GqlPredicateExpressionContext : public ExpressionContext {
  public:
    GqlPredicateExpressionContext(ExpressionContext *ctx);

    ExpressionPredicateContext *expressionPredicate();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlNotExpressionContext : public ExpressionContext {
  public:
    GqlNotExpressionContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *NOT();
    ExpressionContext *expression();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlLogicalXorExpressionContext : public ExpressionContext {
  public:
    GqlLogicalXorExpressionContext(ExpressionContext *ctx);

    GQLParser::ExpressionContext *lhs = nullptr;
    GQLParser::ExpressionContext *rhs = nullptr;
    antlr4::tree::TerminalNode *XOR();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlLogicalOrExpressionContext : public ExpressionContext {
  public:
    GqlLogicalOrExpressionContext(ExpressionContext *ctx);

    GQLParser::ExpressionContext *lhs = nullptr;
    GQLParser::ExpressionContext *rhs = nullptr;
    antlr4::tree::TerminalNode *OR();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlLogicalAndExpressionContext : public ExpressionContext {
  public:
    GqlLogicalAndExpressionContext(ExpressionContext *ctx);

    GQLParser::ExpressionContext *lhs = nullptr;
    GQLParser::ExpressionContext *rhs = nullptr;
    antlr4::tree::TerminalNode *AND();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExpressionContext* expression();
  ExpressionContext* expression(int precedence);
  class  ExpressionPredicateContext : public antlr4::ParserRuleContext {
  public:
    ExpressionPredicateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExpressionPredicateContext() = default;
    void copyFrom(ExpressionPredicateContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GqlAtomExpressionContext : public ExpressionPredicateContext {
  public:
    GqlAtomExpressionContext(ExpressionPredicateContext *ctx);

    ExpressionAtomContext *expressionAtom();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlNormalizedExpressionContext : public ExpressionPredicateContext {
  public:
    GqlNormalizedExpressionContext(ExpressionPredicateContext *ctx);

    ExpressionPredicateContext *expressionPredicate();
    NormalizedPredicateCondContext *normalizedPredicateCond();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlSameExpressionContext : public ExpressionPredicateContext {
  public:
    GqlSameExpressionContext(ExpressionPredicateContext *ctx);

    antlr4::tree::TerminalNode *SAME();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    std::vector<ElementVariableReferenceContext *> elementVariableReference();
    ElementVariableReferenceContext* elementVariableReference(size_t i);
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlDirectedExpressionContext : public ExpressionPredicateContext {
  public:
    GqlDirectedExpressionContext(ExpressionPredicateContext *ctx);

    ElementVariableReferenceContext *elementVariableReference();
    DirectedPredicateCondContext *directedPredicateCond();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlBooleanTestExpressionContext : public ExpressionPredicateContext {
  public:
    GqlBooleanTestExpressionContext(ExpressionPredicateContext *ctx);

    ExpressionPredicateContext *expressionPredicate();
    antlr4::tree::TerminalNode *IS();
    TruthValueContext *truthValue();
    antlr4::tree::TerminalNode *NOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlGraphRefValueExpressionContext : public ExpressionPredicateContext {
  public:
    GqlGraphRefValueExpressionContext(ExpressionPredicateContext *ctx);

    antlr4::tree::TerminalNode *GRAPH();
    GraphExpressionContext *graphExpression();
    antlr4::tree::TerminalNode *PROPERTY();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlAllDifferentExpressionContext : public ExpressionPredicateContext {
  public:
    GqlAllDifferentExpressionContext(ExpressionPredicateContext *ctx);

    antlr4::tree::TerminalNode *ALL_DIFFERENT();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    std::vector<ElementVariableReferenceContext *> elementVariableReference();
    ElementVariableReferenceContext* elementVariableReference(size_t i);
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlExistsExpressionContext : public ExpressionPredicateContext {
  public:
    GqlExistsExpressionContext(ExpressionPredicateContext *ctx);

    antlr4::tree::TerminalNode *EXISTS();
    antlr4::tree::TerminalNode *LEFT_BRACE();
    GraphPatternContext *graphPattern();
    antlr4::tree::TerminalNode *RIGHT_BRACE();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    MatchStatementBlockContext *matchStatementBlock();
    NestedProcedureSpecificationContext *nestedProcedureSpecification();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlLetExpressionContext : public ExpressionPredicateContext {
  public:
    GqlLetExpressionContext(ExpressionPredicateContext *ctx);

    antlr4::tree::TerminalNode *LET();
    LetVariableDefinitionListContext *letVariableDefinitionList();
    antlr4::tree::TerminalNode *IN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *END();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlComparisonExpressionContext : public ExpressionPredicateContext {
  public:
    GqlComparisonExpressionContext(ExpressionPredicateContext *ctx);

    GQLParser::ExpressionPredicateContext *lhs = nullptr;
    GQLParser::ExpressionPredicateContext *rhs = nullptr;
    CompOpContext *compOp();
    std::vector<ExpressionPredicateContext *> expressionPredicate();
    ExpressionPredicateContext* expressionPredicate(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlLabeledExpressionContext : public ExpressionPredicateContext {
  public:
    GqlLabeledExpressionContext(ExpressionPredicateContext *ctx);

    ElementVariableReferenceContext *elementVariableReference();
    LabeledPredicateCondContext *labeledPredicateCond();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlPropertyExistExpressionContext : public ExpressionPredicateContext {
  public:
    GqlPropertyExistExpressionContext(ExpressionPredicateContext *ctx);

    antlr4::tree::TerminalNode *PROPERTY_EXISTS();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    ElementVariableReferenceContext *elementVariableReference();
    antlr4::tree::TerminalNode *COMMA();
    PropertyNameContext *propertyName();
    antlr4::tree::TerminalNode *RIGHT_PAREN();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlNullExpressionContext : public ExpressionPredicateContext {
  public:
    GqlNullExpressionContext(ExpressionPredicateContext *ctx);

    ExpressionPredicateContext *expressionPredicate();
    NullPredicateCondContext *nullPredicateCond();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlSourceDestinationExpressionContext : public ExpressionPredicateContext {
  public:
    GqlSourceDestinationExpressionContext(ExpressionPredicateContext *ctx);

    ElementVariableReferenceContext *elementVariableReference();
    SourceDestinationPredicateCondContext *sourceDestinationPredicateCond();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlBindingTableValueExpressionContext : public ExpressionPredicateContext {
  public:
    GqlBindingTableValueExpressionContext(ExpressionPredicateContext *ctx);

    antlr4::tree::TerminalNode *TABLE();
    BindingTableExpressionContext *bindingTableExpression();
    antlr4::tree::TerminalNode *BINDING();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExpressionPredicateContext* expressionPredicate();
  ExpressionPredicateContext* expressionPredicate(int precedence);
  class  ExpressionAtomContext : public antlr4::ParserRuleContext {
  public:
    ExpressionAtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExpressionAtomContext() = default;
    void copyFrom(ExpressionAtomContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GqlParameterExpressionContext : public ExpressionAtomContext {
  public:
    GqlParameterExpressionContext(ExpressionAtomContext *ctx);

    ParameterValueSpecificationContext *parameterValueSpecification();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlUnaryExpressionContext : public ExpressionAtomContext {
  public:
    GqlUnaryExpressionContext(ExpressionAtomContext *ctx);

    UnaryOperatorContext *unaryOperator();
    ExpressionAtomContext *expressionAtom();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlPropertyReferenceContext : public ExpressionAtomContext {
  public:
    GqlPropertyReferenceContext(ExpressionAtomContext *ctx);

    ExpressionAtomContext *expressionAtom();
    antlr4::tree::TerminalNode *PERIOD();
    PropertyNameContext *propertyName();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlHighArithmeticExpressionContext : public ExpressionAtomContext {
  public:
    GqlHighArithmeticExpressionContext(ExpressionAtomContext *ctx);

    GQLParser::ExpressionAtomContext *lhs = nullptr;
    antlr4::Token *op = nullptr;
    GQLParser::ExpressionAtomContext *rhs = nullptr;
    std::vector<ExpressionAtomContext *> expressionAtom();
    ExpressionAtomContext* expressionAtom(size_t i);
    antlr4::tree::TerminalNode *ASTERISK();
    antlr4::tree::TerminalNode *SOLIDUS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlValueQueryExpressionContext : public ExpressionAtomContext {
  public:
    GqlValueQueryExpressionContext(ExpressionAtomContext *ctx);

    antlr4::tree::TerminalNode *VALUE();
    NestedProcedureSpecificationContext *nestedProcedureSpecification();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlLiteralExpressionContext : public ExpressionAtomContext {
  public:
    GqlLiteralExpressionContext(ExpressionAtomContext *ctx);

    UnsignedLiteralContext *unsignedLiteral();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlCollectionExpressionContext : public ExpressionAtomContext {
  public:
    GqlCollectionExpressionContext(ExpressionAtomContext *ctx);

    CollectionValueConstructorContext *collectionValueConstructor();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlConcatenationExpressionContext : public ExpressionAtomContext {
  public:
    GqlConcatenationExpressionContext(ExpressionAtomContext *ctx);

    std::vector<ExpressionAtomContext *> expressionAtom();
    ExpressionAtomContext* expressionAtom(size_t i);
    antlr4::tree::TerminalNode *CONCATENATION_OPERATOR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlLowArithmeticExpressionContext : public ExpressionAtomContext {
  public:
    GqlLowArithmeticExpressionContext(ExpressionAtomContext *ctx);

    GQLParser::ExpressionAtomContext *lhs = nullptr;
    antlr4::Token *op = nullptr;
    GQLParser::ExpressionAtomContext *rhs = nullptr;
    std::vector<ExpressionAtomContext *> expressionAtom();
    ExpressionAtomContext* expressionAtom(size_t i);
    antlr4::tree::TerminalNode *PLUS_SIGN();
    antlr4::tree::TerminalNode *MINUS_SIGN();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlParenthesizedExpressionContext : public ExpressionAtomContext {
  public:
    GqlParenthesizedExpressionContext(ExpressionAtomContext *ctx);

    antlr4::tree::TerminalNode *LEFT_PAREN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RIGHT_PAREN();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlFunctionExpressionContext : public ExpressionAtomContext {
  public:
    GqlFunctionExpressionContext(ExpressionAtomContext *ctx);

    FunctionCallContext *functionCall();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlVariableExpressionContext : public ExpressionAtomContext {
  public:
    GqlVariableExpressionContext(ExpressionAtomContext *ctx);

    VariableContext *variable();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExpressionAtomContext* expressionAtom();
  ExpressionAtomContext* expressionAtom(int precedence);
  class  TruthValueContext : public antlr4::ParserRuleContext {
  public:
    TruthValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *K_TRUE();
    antlr4::tree::TerminalNode *K_FALSE();
    antlr4::tree::TerminalNode *UNKNOWN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TruthValueContext* truthValue();

  class  UnaryOperatorContext : public antlr4::ParserRuleContext {
  public:
    UnaryOperatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXCLAMATION_MARK();
    antlr4::tree::TerminalNode *PLUS_SIGN();
    antlr4::tree::TerminalNode *MINUS_SIGN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnaryOperatorContext* unaryOperator();

  class  FunctionCallContext : public antlr4::ParserRuleContext {
  public:
    FunctionCallContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumericFunctionContext *numericFunction();
    AggregateFunctionContext *aggregateFunction();
    CaseFunctionContext *caseFunction();
    CastFunctionContext *castFunction();
    ElementIdFunctionContext *elementIdFunction();
    DatetimeValueFunctionContext *datetimeValueFunction();
    DurationFunctionContext *durationFunction();
    ListFunctionContext *listFunction();
    StringFunctionContext *stringFunction();
    LabelsFunctionContext *labelsFunction();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionCallContext* functionCall();

  class  NumericFunctionContext : public antlr4::ParserRuleContext {
  public:
    NumericFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    NumericFunctionContext() = default;
    void copyFrom(NumericFunctionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GqlTwoArgScalarFunctionContext : public NumericFunctionContext {
  public:
    GqlTwoArgScalarFunctionContext(NumericFunctionContext *ctx);

    TwoArgNumericFunctionNameContext *twoArgNumericFunctionName();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    std::vector<FunctionParameterContext *> functionParameter();
    FunctionParameterContext* functionParameter(size_t i);
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RIGHT_PAREN();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlOneArgScalarFunctionContext : public NumericFunctionContext {
  public:
    GqlOneArgScalarFunctionContext(NumericFunctionContext *ctx);

    OneArgNumericFunctionNameContext *oneArgNumericFunctionName();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    FunctionParameterContext *functionParameter();
    antlr4::tree::TerminalNode *RIGHT_PAREN();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  NumericFunctionContext* numericFunction();

  class  FunctionParameterContext : public antlr4::ParserRuleContext {
  public:
    FunctionParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnsignedLiteralContext *unsignedLiteral();
    VariableContext *variable();
    PropertyReferenceContext *propertyReference();
    FunctionCallContext *functionCall();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionParameterContext* functionParameter();

  class  PropertyReferenceContext : public antlr4::ParserRuleContext {
  public:
    PropertyReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VariableContext *variable();
    antlr4::tree::TerminalNode *PERIOD();
    PropertyNameContext *propertyName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyReferenceContext* propertyReference();

  class  OneArgNumericFunctionNameContext : public antlr4::ParserRuleContext {
  public:
    OneArgNumericFunctionNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CHAR_LENGTH();
    antlr4::tree::TerminalNode *CHARACTER_LENGTH();
    antlr4::tree::TerminalNode *BYTE_LENGTH();
    antlr4::tree::TerminalNode *OCTET_LENGTH();
    antlr4::tree::TerminalNode *PATH_LENGTH();
    antlr4::tree::TerminalNode *ABS();
    antlr4::tree::TerminalNode *SIN();
    antlr4::tree::TerminalNode *COS();
    antlr4::tree::TerminalNode *TAN();
    antlr4::tree::TerminalNode *COT();
    antlr4::tree::TerminalNode *SINH();
    antlr4::tree::TerminalNode *COSH();
    antlr4::tree::TerminalNode *TANH();
    antlr4::tree::TerminalNode *ASIN();
    antlr4::tree::TerminalNode *ACOS();
    antlr4::tree::TerminalNode *ATAN();
    antlr4::tree::TerminalNode *DEGREES();
    antlr4::tree::TerminalNode *RADIANS();
    antlr4::tree::TerminalNode *LOG10();
    antlr4::tree::TerminalNode *LN();
    antlr4::tree::TerminalNode *EXP();
    antlr4::tree::TerminalNode *SQRT();
    antlr4::tree::TerminalNode *FLOOR();
    antlr4::tree::TerminalNode *CEIL();
    antlr4::tree::TerminalNode *CEILING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OneArgNumericFunctionNameContext* oneArgNumericFunctionName();

  class  TwoArgNumericFunctionNameContext : public antlr4::ParserRuleContext {
  public:
    TwoArgNumericFunctionNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MOD();
    antlr4::tree::TerminalNode *LOG();
    antlr4::tree::TerminalNode *POWER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TwoArgNumericFunctionNameContext* twoArgNumericFunctionName();

  class  StringFunctionContext : public antlr4::ParserRuleContext {
  public:
    StringFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    StringFunctionContext() = default;
    void copyFrom(StringFunctionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GqlSubstringFunctionContext : public StringFunctionContext {
  public:
    GqlSubstringFunctionContext(StringFunctionContext *ctx);

    antlr4::Token *dir = nullptr;
    GQLParser::ExpressionAtomContext *str = nullptr;
    GQLParser::ExpressionAtomContext *strLen = nullptr;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    std::vector<ExpressionAtomContext *> expressionAtom();
    ExpressionAtomContext* expressionAtom(size_t i);
    antlr4::tree::TerminalNode *LEFT();
    antlr4::tree::TerminalNode *RIGHT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlMultiTrimStringFunctionContext : public StringFunctionContext {
  public:
    GqlMultiTrimStringFunctionContext(StringFunctionContext *ctx);

    antlr4::Token *dir = nullptr;
    GQLParser::ExpressionAtomContext *trimSrc = nullptr;
    GQLParser::ExpressionAtomContext *delChar = nullptr;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    std::vector<ExpressionAtomContext *> expressionAtom();
    ExpressionAtomContext* expressionAtom(size_t i);
    antlr4::tree::TerminalNode *BTRIM();
    antlr4::tree::TerminalNode *LTRIM();
    antlr4::tree::TerminalNode *RTRIM();
    antlr4::tree::TerminalNode *COMMA();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlFoldStringFunctionContext : public StringFunctionContext {
  public:
    GqlFoldStringFunctionContext(StringFunctionContext *ctx);

    antlr4::Token *dir = nullptr;
    antlr4::tree::TerminalNode *LEFT_PAREN();
    ExpressionAtomContext *expressionAtom();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    antlr4::tree::TerminalNode *UPPER();
    antlr4::tree::TerminalNode *LOWER();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlSingleTrimStringFunctionContext : public StringFunctionContext {
  public:
    GqlSingleTrimStringFunctionContext(StringFunctionContext *ctx);

    GQLParser::ExpressionAtomContext *trimSrc = nullptr;
    antlr4::tree::TerminalNode *TRIM();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    std::vector<ExpressionAtomContext *> expressionAtom();
    ExpressionAtomContext* expressionAtom(size_t i);
    antlr4::tree::TerminalNode *FROM();
    TrimSpecificationContext *trimSpecification();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlNormStringFunctionContext : public StringFunctionContext {
  public:
    GqlNormStringFunctionContext(StringFunctionContext *ctx);

    antlr4::tree::TerminalNode *NORMALIZE();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    ExpressionAtomContext *expressionAtom();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    antlr4::tree::TerminalNode *COMMA();
    NormalFormContext *normalForm();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  StringFunctionContext* stringFunction();

  class  ListFunctionContext : public antlr4::ParserRuleContext {
  public:
    ListFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ListFunctionContext() = default;
    void copyFrom(ListFunctionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GqlElementsOfPathFunctionContext : public ListFunctionContext {
  public:
    GqlElementsOfPathFunctionContext(ListFunctionContext *ctx);

    antlr4::tree::TerminalNode *ELEMENTS();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    ExpressionAtomContext *expressionAtom();
    antlr4::tree::TerminalNode *RIGHT_PAREN();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlListTrimFunctionContext : public ListFunctionContext {
  public:
    GqlListTrimFunctionContext(ListFunctionContext *ctx);

    GQLParser::ExpressionAtomContext *list = nullptr;
    GQLParser::ExpressionAtomContext *trim = nullptr;
    antlr4::tree::TerminalNode *TRIM();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    std::vector<ExpressionAtomContext *> expressionAtom();
    ExpressionAtomContext* expressionAtom(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ListFunctionContext* listFunction();

  class  CaseFunctionContext : public antlr4::ParserRuleContext {
  public:
    CaseFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    CaseFunctionContext() = default;
    void copyFrom(CaseFunctionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GqlCoalesceCaseFunctionContext : public CaseFunctionContext {
  public:
    GqlCoalesceCaseFunctionContext(CaseFunctionContext *ctx);

    antlr4::tree::TerminalNode *COALESCE();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlSimpleCaseFunctionContext : public CaseFunctionContext {
  public:
    GqlSimpleCaseFunctionContext(CaseFunctionContext *ctx);

    antlr4::tree::TerminalNode *CASE();
    ExpressionAtomContext *expressionAtom();
    antlr4::tree::TerminalNode *END();
    std::vector<SimpleWhenClauseContext *> simpleWhenClause();
    SimpleWhenClauseContext* simpleWhenClause(size_t i);
    ElseClauseContext *elseClause();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlNullIfCaseFunctionContext : public CaseFunctionContext {
  public:
    GqlNullIfCaseFunctionContext(CaseFunctionContext *ctx);

    GQLParser::ExpressionContext *lhs = nullptr;
    GQLParser::ExpressionContext *rhs = nullptr;
    antlr4::tree::TerminalNode *NULLIF();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlSearchedCaseFunctionContext : public CaseFunctionContext {
  public:
    GqlSearchedCaseFunctionContext(CaseFunctionContext *ctx);

    antlr4::tree::TerminalNode *CASE();
    antlr4::tree::TerminalNode *END();
    std::vector<SearchedWhenClauseContext *> searchedWhenClause();
    SearchedWhenClauseContext* searchedWhenClause(size_t i);
    ElseClauseContext *elseClause();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  CaseFunctionContext* caseFunction();

  class  LabelsFunctionContext : public antlr4::ParserRuleContext {
  public:
    LabelsFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LABELS();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    VariableContext *variable();
    antlr4::tree::TerminalNode *RIGHT_PAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabelsFunctionContext* labelsFunction();

  class  SimpleWhenClauseContext : public antlr4::ParserRuleContext {
  public:
    SimpleWhenClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHEN();
    std::vector<WhenOperandContext *> whenOperand();
    WhenOperandContext* whenOperand(size_t i);
    antlr4::tree::TerminalNode *THEN();
    ExpressionContext *expression();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleWhenClauseContext* simpleWhenClause();

  class  SearchedWhenClauseContext : public antlr4::ParserRuleContext {
  public:
    SearchedWhenClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHEN();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *THEN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SearchedWhenClauseContext* searchedWhenClause();

  class  ElseClauseContext : public antlr4::ParserRuleContext {
  public:
    ElseClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ELSE();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElseClauseContext* elseClause();

  class  WhenOperandContext : public antlr4::ParserRuleContext {
  public:
    WhenOperandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionAtomContext *expressionAtom();
    ComparisonPredicateCondContext *comparisonPredicateCond();
    NullPredicateCondContext *nullPredicateCond();
    DirectedPredicateCondContext *directedPredicateCond();
    LabeledPredicateCondContext *labeledPredicateCond();
    SourceDestinationPredicateCondContext *sourceDestinationPredicateCond();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WhenOperandContext* whenOperand();

  class  CastFunctionContext : public antlr4::ParserRuleContext {
  public:
    CastFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CAST();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *AS();
    ValueTypeContext *valueType();
    antlr4::tree::TerminalNode *RIGHT_PAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CastFunctionContext* castFunction();

  class  ElementIdFunctionContext : public antlr4::ParserRuleContext {
  public:
    ElementIdFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ELEMENT_ID();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    VariableContext *variable();
    antlr4::tree::TerminalNode *RIGHT_PAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementIdFunctionContext* elementIdFunction();

  class  DatetimeValueFunctionContext : public antlr4::ParserRuleContext {
  public:
    DatetimeValueFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DateFunctionContext *dateFunction();
    TimeFunctionContext *timeFunction();
    DatetimeFunctionContext *datetimeFunction();
    LocalTimeFunctionContext *localTimeFunction();
    LocalDatetimeFunctionContext *localDatetimeFunction();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DatetimeValueFunctionContext* datetimeValueFunction();

  class  DateFunctionContext : public antlr4::ParserRuleContext {
  public:
    DateFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CURRENT_DATE();
    antlr4::tree::TerminalNode *DATE();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    DateFunctionParametersContext *dateFunctionParameters();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DateFunctionContext* dateFunction();

  class  TimeFunctionContext : public antlr4::ParserRuleContext {
  public:
    TimeFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CURRENT_TIME();
    antlr4::tree::TerminalNode *ZONED_TIME();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    TimeFunctionParametersContext *timeFunctionParameters();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TimeFunctionContext* timeFunction();

  class  LocalTimeFunctionContext : public antlr4::ParserRuleContext {
  public:
    LocalTimeFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCAL_TIME();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    TimeFunctionParametersContext *timeFunctionParameters();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LocalTimeFunctionContext* localTimeFunction();

  class  DatetimeFunctionContext : public antlr4::ParserRuleContext {
  public:
    DatetimeFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CURRENT_TIMESTAMP();
    antlr4::tree::TerminalNode *ZONED_DATETIME();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    DatetimeFunctionParametersContext *datetimeFunctionParameters();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DatetimeFunctionContext* datetimeFunction();

  class  LocalDatetimeFunctionContext : public antlr4::ParserRuleContext {
  public:
    LocalDatetimeFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOCAL_TIMESTAMP();
    antlr4::tree::TerminalNode *LOCAL_DATETIME();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    DatetimeFunctionParametersContext *datetimeFunctionParameters();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LocalDatetimeFunctionContext* localDatetimeFunction();

  class  DateFunctionParametersContext : public antlr4::ParserRuleContext {
  public:
    DateFunctionParametersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DateStringContext *dateString();
    RecordValueConstructorContext *recordValueConstructor();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DateFunctionParametersContext* dateFunctionParameters();

  class  TimeFunctionParametersContext : public antlr4::ParserRuleContext {
  public:
    TimeFunctionParametersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TimeStringContext *timeString();
    RecordValueConstructorContext *recordValueConstructor();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TimeFunctionParametersContext* timeFunctionParameters();

  class  DatetimeFunctionParametersContext : public antlr4::ParserRuleContext {
  public:
    DatetimeFunctionParametersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DatetimeStringContext *datetimeString();
    RecordValueConstructorContext *recordValueConstructor();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DatetimeFunctionParametersContext* datetimeFunctionParameters();

  class  DateStringContext : public antlr4::ParserRuleContext {
  public:
    DateStringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnbrokenCharacterStringLiteralContext *unbrokenCharacterStringLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DateStringContext* dateString();

  class  TimeStringContext : public antlr4::ParserRuleContext {
  public:
    TimeStringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnbrokenCharacterStringLiteralContext *unbrokenCharacterStringLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TimeStringContext* timeString();

  class  DatetimeStringContext : public antlr4::ParserRuleContext {
  public:
    DatetimeStringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnbrokenCharacterStringLiteralContext *unbrokenCharacterStringLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DatetimeStringContext* datetimeString();

  class  DurationFunctionContext : public antlr4::ParserRuleContext {
  public:
    DurationFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    DurationFunctionContext() = default;
    void copyFrom(DurationFunctionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GqlDatetimeSubtractionFunctionContext : public DurationFunctionContext {
  public:
    GqlDatetimeSubtractionFunctionContext(DurationFunctionContext *ctx);

    antlr4::tree::TerminalNode *DURATION_BETWEEN();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    std::vector<ExpressionAtomContext *> expressionAtom();
    ExpressionAtomContext* expressionAtom(size_t i);
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RIGHT_PAREN();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GqlDurationFunctionContext : public DurationFunctionContext {
  public:
    GqlDurationFunctionContext(DurationFunctionContext *ctx);

    antlr4::tree::TerminalNode *DURATION();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    DurationFunctionParametersContext *durationFunctionParameters();
    antlr4::tree::TerminalNode *RIGHT_PAREN();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  DurationFunctionContext* durationFunction();

  class  DurationFunctionParametersContext : public antlr4::ParserRuleContext {
  public:
    DurationFunctionParametersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DurationStringContext *durationString();
    RecordValueConstructorContext *recordValueConstructor();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DurationFunctionParametersContext* durationFunctionParameters();

  class  DurationStringContext : public antlr4::ParserRuleContext {
  public:
    DurationStringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnbrokenCharacterStringLiteralContext *unbrokenCharacterStringLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DurationStringContext* durationString();

  class  GeneralFunctionContext : public antlr4::ParserRuleContext {
  public:
    GeneralFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FunctionNameContext *functionName();
    antlr4::tree::TerminalNode *LEFT_PAREN();
    antlr4::tree::TerminalNode *RIGHT_PAREN();
    ProcedureArgumentListContext *procedureArgumentList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GeneralFunctionContext* generalFunction();

  class  CollectionValueConstructorContext : public antlr4::ParserRuleContext {
  public:
    CollectionValueConstructorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ListValueConstructorContext *listValueConstructor();
    RecordValueConstructorContext *recordValueConstructor();
    PathValueConstructorContext *pathValueConstructor();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CollectionValueConstructorContext* collectionValueConstructor();

  class  TrimSpecificationContext : public antlr4::ParserRuleContext {
  public:
    TrimSpecificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEADING();
    antlr4::tree::TerminalNode *TRAILING();
    antlr4::tree::TerminalNode *BOTH();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TrimSpecificationContext* trimSpecification();

  class  NormalFormContext : public antlr4::ParserRuleContext {
  public:
    NormalFormContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NFC();
    antlr4::tree::TerminalNode *NFD();
    antlr4::tree::TerminalNode *NFKC();
    antlr4::tree::TerminalNode *NFKD();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NormalFormContext* normalForm();

  class  ListValueConstructorContext : public antlr4::ParserRuleContext {
  public:
    ListValueConstructorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();
    ListValueTypeNameContext *listValueTypeName();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ListValueConstructorContext* listValueConstructor();

  class  RecordValueConstructorContext : public antlr4::ParserRuleContext {
  public:
    RecordValueConstructorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_BRACE();
    antlr4::tree::TerminalNode *RIGHT_BRACE();
    antlr4::tree::TerminalNode *RECORD();
    std::vector<FieldContext *> field();
    FieldContext* field(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RecordValueConstructorContext* recordValueConstructor();

  class  FieldContext : public antlr4::ParserRuleContext {
  public:
    GQLParser::FieldNameContext *key = nullptr;
    GQLParser::ExpressionContext *value = nullptr;
    FieldContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COLON();
    FieldNameContext *fieldName();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FieldContext* field();

  class  PathValueConstructorContext : public antlr4::ParserRuleContext {
  public:
    PathValueConstructorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PATH();
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    std::vector<ExpressionAtomContext *> expressionAtom();
    ExpressionAtomContext* expressionAtom(size_t i);
    antlr4::tree::TerminalNode *RIGHT_BRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathValueConstructorContext* pathValueConstructor();

  class  UnsignedLiteralContext : public antlr4::ParserRuleContext {
  public:
    UnsignedLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnsignedNumericLiteralContext *unsignedNumericLiteral();
    GeneralLiteralContext *generalLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnsignedLiteralContext* unsignedLiteral();

  class  GeneralLiteralContext : public antlr4::ParserRuleContext {
  public:
    GeneralLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PredefinedTypeLiteralContext *predefinedTypeLiteral();
    ListLiteralContext *listLiteral();
    RecordLiteralContext *recordLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GeneralLiteralContext* generalLiteral();

  class  ListLiteralContext : public antlr4::ParserRuleContext {
  public:
    ListLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_BRACKET();
    antlr4::tree::TerminalNode *RIGHT_BRACKET();
    ListValueTypeNameContext *listValueTypeName();
    std::vector<GeneralLiteralContext *> generalLiteral();
    GeneralLiteralContext* generalLiteral(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ListLiteralContext* listLiteral();

  class  RecordLiteralContext : public antlr4::ParserRuleContext {
  public:
    RecordLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFT_BRACE();
    antlr4::tree::TerminalNode *RIGHT_BRACE();
    antlr4::tree::TerminalNode *RECORD();
    std::vector<RecordFieldLiteralContext *> recordFieldLiteral();
    RecordFieldLiteralContext* recordFieldLiteral(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RecordLiteralContext* recordLiteral();

  class  RecordFieldLiteralContext : public antlr4::ParserRuleContext {
  public:
    GQLParser::VariableContext *key = nullptr;
    GQLParser::GeneralLiteralContext *value = nullptr;
    RecordFieldLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COLON();
    VariableContext *variable();
    GeneralLiteralContext *generalLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RecordFieldLiteralContext* recordFieldLiteral();

  class  KeywordContext : public antlr4::ParserRuleContext {
  public:
    KeywordContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ABS();
    antlr4::tree::TerminalNode *ACOS();
    antlr4::tree::TerminalNode *ACYCLIC();
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *ALL_DIFFERENT();
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *ANY();
    antlr4::tree::TerminalNode *ARRAY();
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *ASC();
    antlr4::tree::TerminalNode *ASCENDING();
    antlr4::tree::TerminalNode *ASIN();
    antlr4::tree::TerminalNode *AT();
    antlr4::tree::TerminalNode *ATAN();
    antlr4::tree::TerminalNode *AVG();
    antlr4::tree::TerminalNode *BIG();
    antlr4::tree::TerminalNode *BIGINT();
    antlr4::tree::TerminalNode *BINARY();
    antlr4::tree::TerminalNode *BINDING();
    antlr4::tree::TerminalNode *BINDINGS();
    antlr4::tree::TerminalNode *BOOL();
    antlr4::tree::TerminalNode *BOOLEAN();
    antlr4::tree::TerminalNode *BOTH();
    antlr4::tree::TerminalNode *BTRIM();
    antlr4::tree::TerminalNode *BY();
    antlr4::tree::TerminalNode *BYTE_LENGTH();
    antlr4::tree::TerminalNode *BYTES();
    antlr4::tree::TerminalNode *CALL();
    antlr4::tree::TerminalNode *CASE();
    antlr4::tree::TerminalNode *CAST();
    antlr4::tree::TerminalNode *CEIL();
    antlr4::tree::TerminalNode *CEILING();
    antlr4::tree::TerminalNode *CHAR_LENGTH();
    antlr4::tree::TerminalNode *CHARACTER_LENGTH();
    antlr4::tree::TerminalNode *CHARACTERISTICS();
    antlr4::tree::TerminalNode *CLOSE();
    antlr4::tree::TerminalNode *COALESCE();
    antlr4::tree::TerminalNode *COLLECT();
    antlr4::tree::TerminalNode *COMMIT();
    antlr4::tree::TerminalNode *CONNECTING();
    antlr4::tree::TerminalNode *COPY();
    antlr4::tree::TerminalNode *COS();
    antlr4::tree::TerminalNode *COSH();
    antlr4::tree::TerminalNode *COT();
    antlr4::tree::TerminalNode *COUNT();
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *CURRENT_DATE();
    antlr4::tree::TerminalNode *CURRENT_GRAPH();
    antlr4::tree::TerminalNode *CURRENT_PROPERTY_GRAPH();
    antlr4::tree::TerminalNode *CURRENT_SCHEMA();
    antlr4::tree::TerminalNode *CURRENT_TIME();
    antlr4::tree::TerminalNode *CURRENT_TIMESTAMP();
    antlr4::tree::TerminalNode *CURRENT_USER();
    antlr4::tree::TerminalNode *DATE();
    antlr4::tree::TerminalNode *DATETIME();
    antlr4::tree::TerminalNode *DEC();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *DEGREES();
    antlr4::tree::TerminalNode *DELETE();
    antlr4::tree::TerminalNode *DESC();
    antlr4::tree::TerminalNode *DESCENDING();
    antlr4::tree::TerminalNode *DESTINATION();
    antlr4::tree::TerminalNode *DETACH();
    antlr4::tree::TerminalNode *DIFFERENT();
    antlr4::tree::TerminalNode *DIRECTED();
    antlr4::tree::TerminalNode *DISTINCT();
    antlr4::tree::TerminalNode *DOUBLE();
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *DURATION();
    antlr4::tree::TerminalNode *DURATION_BETWEEN();
    antlr4::tree::TerminalNode *ELEMENT();
    antlr4::tree::TerminalNode *ELEMENT_ID();
    antlr4::tree::TerminalNode *ELEMENTS();
    antlr4::tree::TerminalNode *ELSE();
    antlr4::tree::TerminalNode *END();
    antlr4::tree::TerminalNode *EXCEPT();
    antlr4::tree::TerminalNode *EXISTS();
    antlr4::tree::TerminalNode *EXP();
    antlr4::tree::TerminalNode *K_FALSE();
    antlr4::tree::TerminalNode *FILTER();
    antlr4::tree::TerminalNode *FINISH();
    antlr4::tree::TerminalNode *FIRST();
    antlr4::tree::TerminalNode *FLOAT();
    antlr4::tree::TerminalNode *FLOAT128();
    antlr4::tree::TerminalNode *FLOAT16();
    antlr4::tree::TerminalNode *FLOAT256();
    antlr4::tree::TerminalNode *FLOAT32();
    antlr4::tree::TerminalNode *FLOAT64();
    antlr4::tree::TerminalNode *FLOOR();
    antlr4::tree::TerminalNode *FOR();
    antlr4::tree::TerminalNode *FROM();
    antlr4::tree::TerminalNode *GRAPH();
    antlr4::tree::TerminalNode *GROUP();
    antlr4::tree::TerminalNode *GROUPS();
    antlr4::tree::TerminalNode *HAVING();
    antlr4::tree::TerminalNode *HOME_GRAPH();
    antlr4::tree::TerminalNode *HOME_PROPERTY_GRAPH();
    antlr4::tree::TerminalNode *HOME_SCHEMA();
    antlr4::tree::TerminalNode *I_DONT_KNOW_1();
    antlr4::tree::TerminalNode *I_DONT_KNOW_2();
    antlr4::tree::TerminalNode *I_DONT_KNOW_3();
    antlr4::tree::TerminalNode *I_DONT_KNOW_4();
    antlr4::tree::TerminalNode *I_DONT_KNOW_5();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *IN();
    antlr4::tree::TerminalNode *INSERT();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *INT8();
    antlr4::tree::TerminalNode *INT16();
    antlr4::tree::TerminalNode *INT32();
    antlr4::tree::TerminalNode *INT64();
    antlr4::tree::TerminalNode *INT128();
    antlr4::tree::TerminalNode *INT256();
    antlr4::tree::TerminalNode *INTEGER();
    antlr4::tree::TerminalNode *INTEGER8();
    antlr4::tree::TerminalNode *INTEGER16();
    antlr4::tree::TerminalNode *INTEGER32();
    antlr4::tree::TerminalNode *INTEGER64();
    antlr4::tree::TerminalNode *INTEGER128();
    antlr4::tree::TerminalNode *INTEGER256();
    antlr4::tree::TerminalNode *INTERSECT();
    antlr4::tree::TerminalNode *IS();
    antlr4::tree::TerminalNode *KEEP();
    antlr4::tree::TerminalNode *LABEL();
    antlr4::tree::TerminalNode *LABELED();
    antlr4::tree::TerminalNode *LABELS();
    antlr4::tree::TerminalNode *LAST();
    antlr4::tree::TerminalNode *LEADING();
    antlr4::tree::TerminalNode *LEFT();
    antlr4::tree::TerminalNode *LET();
    antlr4::tree::TerminalNode *LIKE();
    antlr4::tree::TerminalNode *LIMIT();
    antlr4::tree::TerminalNode *LIST();
    antlr4::tree::TerminalNode *LN();
    antlr4::tree::TerminalNode *LOCAL();
    antlr4::tree::TerminalNode *LOCAL_DATETIME();
    antlr4::tree::TerminalNode *LOCAL_TIME();
    antlr4::tree::TerminalNode *LOCAL_TIMESTAMP();
    antlr4::tree::TerminalNode *LOG();
    antlr4::tree::TerminalNode *LOG10();
    antlr4::tree::TerminalNode *LOWER();
    antlr4::tree::TerminalNode *LTRIM();
    antlr4::tree::TerminalNode *MATCH();
    antlr4::tree::TerminalNode *MAX();
    antlr4::tree::TerminalNode *MIN();
    antlr4::tree::TerminalNode *MOD();
    antlr4::tree::TerminalNode *NEXT();
    antlr4::tree::TerminalNode *NFC();
    antlr4::tree::TerminalNode *NFD();
    antlr4::tree::TerminalNode *NFKC();
    antlr4::tree::TerminalNode *NFKD();
    antlr4::tree::TerminalNode *NO();
    antlr4::tree::TerminalNode *NODETACH();
    antlr4::tree::TerminalNode *NORMALIZE();
    antlr4::tree::TerminalNode *NORMALIZED();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *NULL_();
    antlr4::tree::TerminalNode *NULLIF();
    antlr4::tree::TerminalNode *NULLS();
    antlr4::tree::TerminalNode *OCTET_LENGTH();
    antlr4::tree::TerminalNode *OF();
    antlr4::tree::TerminalNode *OFFSET();
    antlr4::tree::TerminalNode *SKIP_();
    antlr4::tree::TerminalNode *ONLY();
    antlr4::tree::TerminalNode *OPEN();
    antlr4::tree::TerminalNode *OPTIONAL();
    antlr4::tree::TerminalNode *OR();
    antlr4::tree::TerminalNode *ORDER();
    antlr4::tree::TerminalNode *ORDINALITY();
    antlr4::tree::TerminalNode *OTHERWISE();
    antlr4::tree::TerminalNode *PARAMETER();
    antlr4::tree::TerminalNode *PARAMETERS();
    antlr4::tree::TerminalNode *PATH();
    antlr4::tree::TerminalNode *PATH_LENGTH();
    antlr4::tree::TerminalNode *PATHS();
    antlr4::tree::TerminalNode *PERCENTILE_CONT();
    antlr4::tree::TerminalNode *PERCENTILE_DISC();
    antlr4::tree::TerminalNode *POWER();
    antlr4::tree::TerminalNode *PRECISION();
    antlr4::tree::TerminalNode *PROPERTY();
    antlr4::tree::TerminalNode *PROPERTY_EXISTS();
    antlr4::tree::TerminalNode *RADIANS();
    antlr4::tree::TerminalNode *READ();
    antlr4::tree::TerminalNode *REAL();
    antlr4::tree::TerminalNode *RECORD();
    antlr4::tree::TerminalNode *REMOVE();
    antlr4::tree::TerminalNode *REPEATABLE();
    antlr4::tree::TerminalNode *REPLACE();
    antlr4::tree::TerminalNode *RESET();
    antlr4::tree::TerminalNode *RETURN();
    antlr4::tree::TerminalNode *ROLLBACK();
    antlr4::tree::TerminalNode *RTRIM();
    antlr4::tree::TerminalNode *RIGHT();
    antlr4::tree::TerminalNode *SAME();
    antlr4::tree::TerminalNode *SCHEMA();
    antlr4::tree::TerminalNode *SELECT();
    antlr4::tree::TerminalNode *SESSION();
    antlr4::tree::TerminalNode *SET();
    antlr4::tree::TerminalNode *SHORTEST();
    antlr4::tree::TerminalNode *SIGNED();
    antlr4::tree::TerminalNode *SIMPLE();
    antlr4::tree::TerminalNode *SIN();
    antlr4::tree::TerminalNode *SINH();
    antlr4::tree::TerminalNode *SMALL();
    antlr4::tree::TerminalNode *SMALLINT();
    antlr4::tree::TerminalNode *SOURCE();
    antlr4::tree::TerminalNode *SQRT();
    antlr4::tree::TerminalNode *START();
    antlr4::tree::TerminalNode *STDDEV_POP();
    antlr4::tree::TerminalNode *STDDEV_SAMP();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *SUM();
    antlr4::tree::TerminalNode *TABLE();
    antlr4::tree::TerminalNode *TAN();
    antlr4::tree::TerminalNode *TANH();
    antlr4::tree::TerminalNode *TEMP();
    antlr4::tree::TerminalNode *THEN();
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *TIMESTAMP();
    antlr4::tree::TerminalNode *TIMEZONE();
    antlr4::tree::TerminalNode *TO();
    antlr4::tree::TerminalNode *TRAIL();
    antlr4::tree::TerminalNode *TRAILING();
    antlr4::tree::TerminalNode *TRANSACTION();
    antlr4::tree::TerminalNode *TRIM();
    antlr4::tree::TerminalNode *K_TRUE();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *TYPED();
    antlr4::tree::TerminalNode *UBIGINT();
    antlr4::tree::TerminalNode *UINT();
    antlr4::tree::TerminalNode *UINT128();
    antlr4::tree::TerminalNode *UINT16();
    antlr4::tree::TerminalNode *UINT256();
    antlr4::tree::TerminalNode *UINT32();
    antlr4::tree::TerminalNode *UINT64();
    antlr4::tree::TerminalNode *UINT8();
    antlr4::tree::TerminalNode *UNDIRECTED();
    antlr4::tree::TerminalNode *UNION();
    antlr4::tree::TerminalNode *UNKNOWN();
    antlr4::tree::TerminalNode *UNSIGNED();
    antlr4::tree::TerminalNode *UPPER();
    antlr4::tree::TerminalNode *USE();
    antlr4::tree::TerminalNode *USMALLINT();
    antlr4::tree::TerminalNode *VALUE();
    antlr4::tree::TerminalNode *VARBINARY();
    antlr4::tree::TerminalNode *VARCHAR();
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *WALK();
    antlr4::tree::TerminalNode *WHEN();
    antlr4::tree::TerminalNode *WHERE();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *WITHOUT();
    antlr4::tree::TerminalNode *WRITE();
    antlr4::tree::TerminalNode *XOR();
    antlr4::tree::TerminalNode *YIELD();
    antlr4::tree::TerminalNode *ZONE();
    antlr4::tree::TerminalNode *ZONED();
    antlr4::tree::TerminalNode *ZONED_DATETIME();
    antlr4::tree::TerminalNode *ZONED_TIME();
    antlr4::tree::TerminalNode *INTERVAL_DAY();
    antlr4::tree::TerminalNode *INTERVAL_WEEK();
    antlr4::tree::TerminalNode *INTERVAL_MONTH();
    antlr4::tree::TerminalNode *INTERVAL_YEAR();
    antlr4::tree::TerminalNode *EDGE_SYNONYM();
    antlr4::tree::TerminalNode *EDGES_SYNONYM();
    antlr4::tree::TerminalNode *NODE_SYNONYM();
    antlr4::tree::TerminalNode *KEY_WORD();
    antlr4::tree::TerminalNode *PRE_RESERVED_WORD();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KeywordContext* keyword();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool compositeQueryExpressionSempred(CompositeQueryExpressionContext *_localctx, size_t predicateIndex);
  bool valueTypeSempred(ValueTypeContext *_localctx, size_t predicateIndex);
  bool expressionSempred(ExpressionContext *_localctx, size_t predicateIndex);
  bool expressionPredicateSempred(ExpressionPredicateContext *_localctx, size_t predicateIndex);
  bool expressionAtomSempred(ExpressionAtomContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

