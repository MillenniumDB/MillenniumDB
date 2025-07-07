#pragma once

#include "storage/index/hnsw/hnsw_entry.h"

#include <boost/container/flat_set.hpp>

namespace HNSW {

using HNSWEntryFlatMap = boost::container::flat_multiset<Entry, EntryLess>;

} // namespace HNSW
