#include <filesystem>
#include <iostream>
#include <vector>

#include "query/query_context.h"
#include "storage/index/tensor_store/tensor_store.h"
#include "storage/index/tensor_store/tensor_store_manager.h"
#include "system/buffer_manager.h"
#include "system/file_manager.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"

namespace fs = std::filesystem;

const std::string DB_DIRECTORY = "test_tensor_store_db";
constexpr std::chrono::seconds DEFAULT_TIMEOUT = std::chrono::seconds(10);
constexpr auto VTENSOR_FRAME_POOL_SIZE_BYTES = 1024 * 1024; // 1MB

// Insert
bool test_insert_tensors(TensorStoreManager& tensor_store_manager)
{
    constexpr auto name = "test_insert";
    constexpr auto tensors_dim = 32;
    constexpr auto num_tensors = VTENSOR_FRAME_POOL_SIZE_BYTES / (sizeof(float) * tensors_dim);

    tensor_store_manager.create_tensor_store(name, tensors_dim);

    TensorStore* tensor_store;
    const bool found = tensor_store_manager.get_tensor_store(name, &tensor_store);
    if (!found) {
        return false;
    }

    if (tensor_store->tensors_dim() != tensors_dim) {
        return false;
    }

    // Prepare tensors
    std::vector<std::vector<float>> inserted_tensors(num_tensors, std::vector<float>(tensors_dim));
    for (uint64_t i = 0; i < num_tensors; ++i) {
        for (uint64_t j = 0; j < tensors_dim; ++j) {
            inserted_tensors[i][j] = static_cast<float>(i);
        }
    }

    { // Insert
        auto version_scope = buffer_manager.init_version_editable();
        get_query_ctx().prepare(*version_scope, DEFAULT_TIMEOUT);

        for (uint64_t i = 0; i < num_tensors; ++i) {
            if (!tensor_store->insert(ObjectId(i), inserted_tensors[i].data())) {
                return false;
            }
        }
    }

    if (tensor_store->size() != num_tensors) {
        return false;
    }

    { // Check that all they are all equal
        auto version_scope = buffer_manager.init_version_readonly();
        get_query_ctx().prepare(*version_scope, DEFAULT_TIMEOUT);

        for (uint64_t i = 0; i < num_tensors; ++i) {
            VTensor* vtensor_ptr;
            const bool found = tensor_store->get(ObjectId(i), &vtensor_ptr);
            if (!found) {
                return false;
            }
            auto stored_tensor = vtensor_ptr->get_tensor();
            for (uint64_t j = 0; j < tensors_dim; ++j) {
                if (stored_tensor[j] != inserted_tensors[i][j]) {
                    TensorStore::unpin(*vtensor_ptr);
                    return false;
                }
            }
            TensorStore::unpin(*vtensor_ptr);
        }
    }

    return true;
}

bool test_remove_tensors(TensorStoreManager& tensor_store_manager)
{
    constexpr auto name = "test_remove";
    constexpr auto tensors_dim = 32;
    constexpr auto num_tensors = VTENSOR_FRAME_POOL_SIZE_BYTES / (2 * sizeof(float) * tensors_dim);

    tensor_store_manager.create_tensor_store(name, tensors_dim);

    TensorStore* tensor_store;
    const bool found = tensor_store_manager.get_tensor_store(name, &tensor_store);

    if (!found) {
        return false;
    }

    if (tensor_store->tensors_dim() != tensors_dim) {
        return false;
    }

    // Prepare tensors
    std::vector<std::vector<float>> inserted_tensors(num_tensors, std::vector<float>(tensors_dim));
    for (uint64_t i = 0; i < num_tensors; ++i) {
        for (uint64_t j = 0; j < tensors_dim; ++j) {
            inserted_tensors[i][j] = static_cast<float>(i);
        }
    }

    { // Insert
        auto version_scope = buffer_manager.init_version_editable();
        get_query_ctx().prepare(*version_scope, DEFAULT_TIMEOUT);

        for (uint64_t i = 0; i < num_tensors; ++i) {
            if (!tensor_store->insert(ObjectId(i), inserted_tensors[i].data())) {
                return false;
            }
        }
    }

    if (tensor_store->size() != num_tensors) {
        return false;
    }

    { // Remove all
        auto version_scope = buffer_manager.init_version_editable();
        get_query_ctx().prepare(*version_scope, DEFAULT_TIMEOUT);

        for (uint64_t i = 0; i < num_tensors; ++i) {
            bool removed = tensor_store->remove(ObjectId(i));
            if (!removed) {
                return false;
            }
        }
    }

    if (!tensor_store->empty()) {
        return false;
    }

    { // Insert them back to use all tombstones
        auto version_scope = buffer_manager.init_version_editable();
        get_query_ctx().prepare(*version_scope, DEFAULT_TIMEOUT);

        for (uint64_t i = 0; i < num_tensors; ++i) {
            if (!tensor_store->insert(ObjectId(i), inserted_tensors[i].data())) {
                return false;
            }
        }
    }

    if (tensor_store->size() != num_tensors) {
        return false;
    }

    { // Add an extra tensor (equal to the first)
        auto version_scope = buffer_manager.init_version_editable();
        get_query_ctx().prepare(*version_scope, DEFAULT_TIMEOUT);

        if (!tensor_store->insert(ObjectId(num_tensors), inserted_tensors[0].data())) {
            return false;
        }
    }

    { // Check that all they are all equal
        auto version_scope = buffer_manager.init_version_readonly();
        get_query_ctx().prepare(*version_scope, DEFAULT_TIMEOUT);

        for (uint64_t i = 0; i < num_tensors; ++i) {
            VTensor* vtensor_ptr;
            const bool found = tensor_store->get(ObjectId(i), &vtensor_ptr);
            if (!found) {
                return false;
            }
            auto stored_tensor = vtensor_ptr->get_tensor();
            for (uint64_t j = 0; j < tensors_dim; ++j) {
                if (stored_tensor[j] != inserted_tensors[i][j]) {
                    TensorStore::unpin(*vtensor_ptr);
                    return false;
                }
            }
            TensorStore::unpin(*vtensor_ptr);
        }

        // Don't forget the last one! :)
        VTensor* vtensor_ptr;
        const bool found = tensor_store->get(ObjectId(num_tensors), &vtensor_ptr);
        if (!found) {
            return false;
        }
        auto stored_tensor = vtensor_ptr->get_tensor();
        for (uint64_t j = 0; j < tensors_dim; ++j) {
            if (stored_tensor[j] != inserted_tensors[0][j]) {
                TensorStore::unpin(*vtensor_ptr);
                return false;
            }
        }
        TensorStore::unpin(*vtensor_ptr);
    }

    return true;
}

bool test_persistance([[maybe_unused]] TensorStoreManager& tensor_store_manager)
{
    // This will not use the TensorStoreManager interface, this is just trying to check
    // that the TensorStore can be persisted in disk correctly
    constexpr auto name = "test_persistance";
    constexpr auto tensors_dim = 32;
    constexpr auto num_tensors = VTENSOR_FRAME_POOL_SIZE_BYTES / (2 * sizeof(float) * tensors_dim);

    std::vector<float> minus_one_tensor(tensors_dim, -1.0f);
    std::vector<float> plus_one_tensor(tensors_dim, 1.0f);

    { // Create, load and do some modifications to the TensorStore

        auto tensor_store = TensorStore::create(name, tensors_dim, VTENSOR_FRAME_POOL_SIZE_BYTES);

        { // Insert -1 in every position
            auto version_scope = buffer_manager.init_version_editable();
            get_query_ctx().prepare(*version_scope, DEFAULT_TIMEOUT);

            for (uint64_t i = 0; i < num_tensors; ++i) {
                if (!tensor_store->insert(ObjectId(i), minus_one_tensor.data())) {
                    return false;
                }
            }
        }

        if (tensor_store->size() != num_tensors) {
            return false;
        }

        { // Replace the first half with +1 in every position
            auto version_scope = buffer_manager.init_version_editable();
            get_query_ctx().prepare(*version_scope, DEFAULT_TIMEOUT);

            for (uint64_t i = 0; i < num_tensors / 2; ++i) {
                if (tensor_store->insert(ObjectId(i), plus_one_tensor.data())) {
                    return false;
                }
            }
        }

        if (tensor_store->size() != num_tensors) {
            return false;
        }

        { // Remove every even tensor
            auto version_scope = buffer_manager.init_version_editable();
            get_query_ctx().prepare(*version_scope, DEFAULT_TIMEOUT);

            for (uint64_t i = 0; i < num_tensors; i += 2) {
                tensor_store->remove(ObjectId(i));
            }
        }

        if (tensor_store->size() != num_tensors - (num_tensors / 2)) {
            return false;
        }
    }

    { // Load the TensorStore again
        auto tensor_store = TensorStore::load(name, VTENSOR_FRAME_POOL_SIZE_BYTES);

        // Check size
        if (tensor_store->size() != num_tensors - (num_tensors / 2)) {
            return false;
        }

        { // Check values
            auto version_scope = buffer_manager.init_version_readonly();
            get_query_ctx().prepare(*version_scope, DEFAULT_TIMEOUT);

            for (uint64_t i = 0; i < num_tensors - (num_tensors / 2); ++i) {
                if (i % 2 == 0) {
                    // Even must be missing
                    const bool found = tensor_store->get(ObjectId(i), nullptr);
                    if (found) {
                        return false;
                    }
                } else {
                    // Odd must exist
                    VTensor* vtensor_ptr;
                    const bool found = tensor_store->get(ObjectId(i), &vtensor_ptr);
                    if (!found) {
                        return false;
                    }

                    const float test_value = i < (num_tensors / 2) ? 1.0f : -1.0f;

                    const auto stored_tensor = vtensor_ptr->get_tensor();
                    for (uint64_t j = 0; j < tensors_dim; ++j) {
                        if (stored_tensor[j] != test_value) {
                            TensorStore::unpin(*vtensor_ptr);
                            return false;
                        }
                    }
                    TensorStore::unpin(*vtensor_ptr);
                }
            }
        }
    }

    return true;
}

int main()
{
    uint64_t load_strings = StringManager::DEFAULT_STATIC_BUFFER;
    uint64_t versioned_pages_buffer = BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE;
    uint64_t private_pages_buffer = BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE;
    uint64_t max_threads = 1;

    fs::remove_all(DB_DIRECTORY);

    FileManager::init(DB_DIRECTORY);
    BufferManager::init(versioned_pages_buffer, private_pages_buffer, load_strings, max_threads);
    TmpManager::init(max_threads);


    {
        TensorStoreManager tsm;
        tsm.init();

        {
            QueryContext qc;
            QueryContext::set_query_ctx(&qc);

            using TestFunction = bool(TensorStoreManager&);

            std::vector<TestFunction*> tests = { &test_insert_tensors,
                                                 &test_remove_tensors,
                                                 &test_persistance };

            for (uint64_t i = 0; i < tests.size(); ++i) {
                try {
                    if (!tests[i](tsm)) {
                        std::cerr << "Test #" << i << " failed" << std::endl;
                        return EXIT_FAILURE;
                    }
                    std::cout << "Test #" << i << " passed" << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Test #" << i << " failed due to an exception: " << e.what() << std::endl;
                    return EXIT_FAILURE;
                }
            }
        }
    }

    tmp_manager.~TmpManager();
    buffer_manager.~BufferManager();
    file_manager.~FileManager();

    return EXIT_SUCCESS;
}