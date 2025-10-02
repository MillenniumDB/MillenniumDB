#include "trie_node.h"

#include <cassert>

#include "storage/file_id.h"
#include "storage/index/text_search/trie.h"
#include "storage/index/text_search/utils.h"
#include "storage/page/page.h"
#include "system/buffer_manager.h"

namespace TextSearch {

Node::Node(Trie& trie, Page* page, uint64_t page_offset) :
    trie(trie),
    page(page),
    page_offset(page_offset)
{
    init_pointers_using_page();
}

Node::Node(Trie& trie, uint64_t total_offset) :
    trie(trie)
{
    auto page_number = total_offset / Page::SIZE;
    page_offset = total_offset % Page::SIZE;
    page = &buffer_manager.get_page_readonly(trie.file_id, page_number);
    init_pointers_using_page();
}

Node::Node(
    Trie& trie,
    const uint8_t* new_string,
    size_t string_length,
    size_t child_count,
    Node* child_node,
    uint8_t child_char
) :
    trie(trie)
{
    assert(child_count > 0 || child_node == nullptr);

    uint64_t capacity = size(string_length, child_count);

    // Get some space and a page for the new node.
    auto [page_number, page_offset_] = trie.get_space(capacity);
    page_offset = page_offset_;
    page = &buffer_manager.get_page_editable(trie.file_id, page_number);

    init_pointers_using_string_length(string_length);

    auto new_node_id = trie.get_new_id();

    // Write the fields of the new node.
    write_xbytes(new_node_id, node_id_ptr, NODE_ID_SIZE);
    write_xbytes(capacity, capacity_ptr, CAPACITY_SIZE);
    *str_len_ptr = string_length;

    if (new_string != nullptr) {
        std::memcpy(string_ptr, new_string, string_length);
    }
    // Set the char for the edge between the new node and this node.
    *children_ptr = *(string_ptr + string_length);

    // Make the child node point to this node.
    if (child_node != nullptr) {
        *child_count_ptr = 1;
        write_xbytes(child_char, children_ptr, CHILD_CHAR_SIZE);
        write_xbytes(child_node->node_pos(), children_ptr + CHILD_CHAR_SIZE, CHILD_POINTER_SIZE);
    } else {
        *child_count_ptr = 0;
    }
}

std::unique_ptr<Node> Node::clone()
{
    buffer_manager.pin(*page);
    return std::make_unique<Node>(trie, page, page_offset);
}

Node::~Node()
{
    assert(page != nullptr);
    buffer_manager.unpin(*page);
}

void Node::upgrade_to_editable()
{
    if (!buffer_manager.is_editable(*page)) {
        auto new_page = &buffer_manager.get_page_editable(page->page_id.file_id, page->get_page_number());
        buffer_manager.unpin(*page);
        page = new_page;
        init_pointers_using_page();
    }
}

void Node::init_pointers_using_page()
{
    node_id_ptr = reinterpret_cast<uint8_t*>(page->get_bytes()) + page_offset;
    capacity_ptr = node_id_ptr + NODE_ID_SIZE;
    document_count_ptr = capacity_ptr + CAPACITY_SIZE;
    str_len_ptr = document_count_ptr + DOCUMENT_COUNT_SIZE;
    auto string_length = read_xbytes(str_len_ptr, STR_LEN_SIZE);
    child_count_ptr = str_len_ptr + STR_LEN_SIZE;
    string_ptr = child_count_ptr + CHILD_COUNT_SIZE;
    children_ptr = string_ptr + string_length;
}

void Node::init_pointers_using_string_length(size_t string_length)
{
    node_id_ptr = reinterpret_cast<uint8_t*>(page->get_bytes()) + page_offset;
    capacity_ptr = node_id_ptr + NODE_ID_SIZE;
    document_count_ptr = capacity_ptr + CAPACITY_SIZE;
    str_len_ptr = document_count_ptr + DOCUMENT_COUNT_SIZE;
    child_count_ptr = str_len_ptr + STR_LEN_SIZE;
    string_ptr = child_count_ptr + CHILD_COUNT_SIZE;
    children_ptr = string_ptr + string_length;
}

size_t Node::size(size_t string_length, size_t child_count) const
{
    return NODE_ID_SIZE + CAPACITY_SIZE + DOCUMENT_COUNT_SIZE + STR_LEN_SIZE + CHILD_COUNT_SIZE
         + string_length + child_count * CHILD_SIZE;
}

// Splits the node and returns the newly inserted node between parent and this node.
std::unique_ptr<Node> Node::split(Node* parent, size_t parent_child_idx, size_t split_position)
{
    upgrade_to_editable();

    // Create the node that will be inserted between the parent node and this node.
    auto new_node = std::make_unique<Node>(
        trie,
        string_ptr,
        split_position,
        1,
        this,
        string_ptr[split_position]
    );

    // The parent node will now point to the new node.
    if (parent != nullptr) {
        parent->update_child_page_pointer(parent_child_idx, new_node->node_pos());
    }

    // Next we will update this node, which will be the child of the new node.
    // The -1 accounts for the char of the edge.
    *str_len_ptr = *str_len_ptr - split_position - 1;

    // Remove prefix from the string
    std::memmove(string_ptr, string_ptr + split_position + 1, *str_len_ptr);

    // Move over the children
    std::memmove(string_ptr + *str_len_ptr, children_ptr, *child_count_ptr * CHILD_SIZE);

    return new_node;
}

std::unique_ptr<Node> Node::insert_child(Node* parent, size_t parent_child_idx, const uint8_t* string)
{
    // The string should be non-empty
    assert(string != nullptr && *string != '\0');

    auto string_length = std::strlen(reinterpret_cast<const char*>(string)) - 1;
    auto string_length_to_insert = std::min(string_length, STR_LEN_MAX);
    string_length -= string_length_to_insert;

    // Calculate needed capacity
    uint64_t needed_capacity = size(*str_len_ptr, *child_count_ptr + 1);

    auto child_char = *string;
    string++;
    auto child_node = std::make_unique<Node>(trie, string, string_length_to_insert, 0, nullptr, 0);
    string += string_length_to_insert;

    // Check if we have enough capacity
    auto capacity = read_xbytes(capacity_ptr, CAPACITY_SIZE);
    if (capacity < needed_capacity) {
        // Not enough capacity, we have to move this node to a bigger capacity
        auto [new_page_number, new_page_offset] = trie.get_space(needed_capacity);
        auto new_page = &buffer_manager.get_page_editable(trie.file_id, new_page_number);

        // Copy over everything
        std::memcpy(new_page->get_bytes() + new_page_offset, page->get_bytes() + page_offset, capacity);

        // Switch to new page
        buffer_manager.unpin(*page);
        page = new_page;
        page_offset = new_page_offset;

        // Update pointers and capacity
        init_pointers_using_page();
        write_xbytes(needed_capacity, capacity_ptr, CAPACITY_SIZE);

        // Update parent page pointer
        if (parent != nullptr) {
            // This is not the root node
            parent->update_child_page_pointer(parent_child_idx, node_pos());
        } else {
            // This is the root node
            // We suppose the root node will have enough capacity to be full
            assert(false);
        }
    } else {
        upgrade_to_editable();
    }

    auto child_idx = *child_count_ptr;
    children_ptr[child_idx * CHILD_SIZE] = child_char;

    auto child_ptr = children_ptr + child_idx * CHILD_SIZE + CHILD_CHAR_SIZE;
    write_xbytes(child_node->node_pos(), child_ptr, CHILD_POINTER_SIZE);
    *child_count_ptr = child_idx + 1;

    if (string_length > 0) {
        // we still need to insert more
        return child_node->insert_child(this, child_idx, string);
    }

    child_node->increment_document_count();
    return child_node;
}

void Node::increment_document_count()
{
    auto document_count = read_xbytes(document_count_ptr, DOCUMENT_COUNT_SIZE);
    if (document_count < DOCUMENT_COUNT_MAX) {
        upgrade_to_editable();
        write_xbytes(document_count + 1, document_count_ptr, DOCUMENT_COUNT_SIZE);
    }
}

uint64_t Node::insert_string(Node* parent, size_t parent_child_idx, const uint8_t* str)
{
    auto string_ptr_copy = string_ptr;
    size_t split_position = 0;

    // Calculate common prefix length
    while (split_position < *str_len_ptr && *str == *string_ptr_copy) {
        str++;
        string_ptr_copy++;
        split_position++;
    }

    if (split_position < *str_len_ptr) {
        // We have to split this node
        auto new_node = split(parent, parent_child_idx, split_position);
        return new_node->insert_string(parent, parent_child_idx, str - split_position);
    } else if (*str == '\0') {
        // str belongs in this node
        increment_document_count();
        return read_xbytes(node_id_ptr, NODE_ID_SIZE);
    } else {
        // We have to check children of this node
        for (size_t child_idx = 0; child_idx < *child_count_ptr; child_idx++) {
            auto child_char = children_ptr[child_idx * CHILD_SIZE];

            if (child_char == *str) {
                auto child = &children_ptr[child_idx * CHILD_SIZE + CHILD_CHAR_SIZE];
                auto node_pos = read_xbytes(child, CHILD_POINTER_SIZE);
                Node child_node(trie, node_pos);
                return child_node.insert_string(this, child_idx, str + 1);
            }
        }

        // No child char matched, we need to add a new child
        auto leaf = insert_child(parent, parent_child_idx, str);
        return read_xbytes(leaf->node_id_ptr, NODE_ID_SIZE);
    }
}

void Node::print_trie_node(std::ostream& os)
{
    auto node_id = read_xbytes(node_id_ptr, NODE_ID_SIZE);
    auto document_count = read_xbytes(document_count_ptr, DOCUMENT_COUNT_SIZE);
    os << node_id << " [label=\"'";
    os << escape(string_ptr, *str_len_ptr);
    os << "'\\n" << node_id << " | " << document_count << "\"]\n";

    for (size_t child = 0; child < *child_count_ptr; child++) {
        auto child_char_ptr = &children_ptr[child * CHILD_SIZE];
        auto child_page_pointer_ptr = &children_ptr[child * CHILD_SIZE + CHILD_CHAR_SIZE];
        auto child_page_pointer = read_xbytes(child_page_pointer_ptr, CHILD_POINTER_SIZE);

        auto child_node = std::make_unique<Node>(trie, child_page_pointer);
        auto child_node_id = read_xbytes(child_node->node_id_ptr, NODE_ID_SIZE);
        os << node_id << " -> " << child_node_id;
        os << " [label=\"'" << escape(child_char_ptr, 1) << "'\"]\n";

        child_node->print_trie_node(os);
    }
}

void Node::update_child_page_pointer(size_t parent_child_idx, uint64_t node_pos)
{
    upgrade_to_editable();

    auto ptr = &children_ptr[parent_child_idx * CHILD_SIZE + CHILD_CHAR_SIZE];
    write_xbytes(node_pos, ptr, CHILD_POINTER_SIZE);
}

} // namespace TextSearch
