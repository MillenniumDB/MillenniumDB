#pragma once

#include <variant>

#include "base/ids/object_id.h"
#include "base/ids/var_id.h"

using Id = std::variant<VarId, ObjectId>;
