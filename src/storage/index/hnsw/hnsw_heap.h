#pragma once

#include "misc/min_max_heap.h"
#include "storage/index/hnsw/hnsw_entry.h"

namespace HNSW {

using HNSWHeap = MinMaxHeap<Entry, EntryLess>;

}
