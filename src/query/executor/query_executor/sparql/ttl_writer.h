#pragma once

#include <ostream>

#include "graph_models/object_id.h"

void escape(std::ostream& os, const std::string& string);

void write_and_escape_ttl(std::ostream& os, ObjectId oid);