#include "query/executor//binding_iter/paths/index_provider/path_index.h"
#include <unordered_map>

namespace Paths {

/*
Provides indexes for QuadModel.
*/
class QuadModelIndexProvider : public IndexProvider {
private:
    // Store info about assigned indexes for each transition
    std::unordered_map<uint64_t, IndexType> t_info;
    std::unordered_map<uint64_t, IndexType> t_inv_info;

    // Interruption
    bool* interruption_requested;

    // for unfixed start node
    std::unique_ptr<EdgeIter> get_btree_iter(uint64_t type_id, bool inverse);

    // for fixed start node
    std::unique_ptr<EdgeIter> get_btree_iter(uint64_t type_id, bool inverse, uint64_t node_id);

public:
    QuadModelIndexProvider(std::unordered_map<uint64_t, IndexType> t_info,
                           std::unordered_map<uint64_t, IndexType> t_inv_info,
                           bool* interruption_requested) :
        t_info     (std::move(t_info)),
        t_inv_info (std::move(t_inv_info)),
        interruption_requested (interruption_requested) { }

    bool node_exists(uint64_t node_id) override;

    // for unfixed start node
    std::unique_ptr<EdgeIter> get_iter(uint64_t type_id, bool inverse) override;

    // for fixed start node
    std::unique_ptr<EdgeIter> get_iter(uint64_t type_id, bool inverse, uint64_t node_id) override;

    std::unique_ptr<EdgeIter> get_outgoing(uint64_t node_id) override;

    std::unique_ptr<EdgeIter> get_incoming(uint64_t node_id) override;
};
} // namespace Paths
