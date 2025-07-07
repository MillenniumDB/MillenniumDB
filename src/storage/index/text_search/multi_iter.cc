#include "multi_iter.h"


namespace TextSearch {


MultiIter::MultiIter(std::vector<std::unique_ptr<IndexIter>> index_iters) {
    for (auto& iter : index_iters) {
        iters.push_back(std::make_unique<OrderedIter>(std::move(iter)));
    }
}


uint64_t MultiIter::get_table_pointer() const {
    return table_pointer;
}


uint64_t MultiIter::get_score() const {
    return score;
}


bool MultiIter::next() {
    uint64_t max_table_pointer = 0;
    uint64_t max_table_pointer_score = 0;
    uint64_t max_table_pointer_idx = UINT64_MAX;
    uint64_t current_iter = 0;

    while (current_iter != max_table_pointer_idx) {
        if (iters[current_iter]->next()) {
            auto table_pointer = iters[current_iter]->get_table_pointer();
            auto score = iters[current_iter]->get_score();
            if (table_pointer == max_table_pointer) {
                current_iter = (current_iter + 1) % iters.size();
            } else if (table_pointer > max_table_pointer) {
                max_table_pointer = table_pointer;
                max_table_pointer_idx = current_iter;
                max_table_pointer_score = score;
                current_iter = (current_iter + 1) % iters.size();
            }
        } else {
            return false;
        }
    }

    table_pointer = max_table_pointer;
    score = max_table_pointer_score;
    return true;
}


} // namespace TextSearch
