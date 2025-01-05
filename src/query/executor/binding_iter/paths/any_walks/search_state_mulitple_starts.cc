#include "search_state_multiple_starts.h"

using namespace Paths::Any;

void MultiSourceSearchState::print(
    std::ostream& os,
    std::function<void(std::ostream& os, ObjectId)> print_node,
    std::function<void(std::ostream& os, ObjectId, bool)> print_edge,
    bool begin_at_left) const
{
    return;
}
