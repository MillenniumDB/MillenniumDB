#include "trie_node.h"

#include <cassert>
#include <cmath>

#include "storage/index/text_search/trie.h"
#include "storage/index/text_search/utils.h"
#include "storage/page/unversioned_page.h"
#include "system/buffer_manager.h"


namespace TextSearch {

Node::Node(Trie& trie, UPage* page, uint64_t page_offset) :
    trie(trie),
    page(page),
    page_offset(page_offset)
{
    init_pointers_using_page();
}


Node::Node(Trie& trie, uint64_t page_pointer) : trie(trie) {
    auto page_number = page_pointer / UPage::SIZE;
    page_offset = page_pointer % UPage::SIZE;
    page = &buffer_manager.get_unversioned_page(trie.file_id, page_number);
    init_pointers_using_page();
}


Node::Node(
    Trie& trie,
    const unsigned char* new_string,
    size_t string_length,
    size_t child_count,
    Node* child_node,
    unsigned char child_char
) : trie(trie)
{
    assert(child_count > 0 || child_node == nullptr);

    uint64_t capacity = size(string_length, child_count);

    // Get some space and a page for the new node.
    auto [page_number, page_offset_] = get_space(capacity);
    page_offset = page_offset_;
    page = &buffer_manager.get_unversioned_page(trie.file_id, page_number);

    init_pointers_using_string_length(string_length);

    // Get an id for the new node.
    auto node_id = read_bytes(trie.next_id_ptr, NODE_ID_SIZE);
    write_bytes(trie.next_id_ptr, NODE_ID_SIZE, node_id + 1);
    trie.root_page.make_dirty();

    // Write the fields of the new node.
    write_bytes(node_id_ptr, NODE_ID_SIZE, node_id);
    write_bytes(capacity_ptr, CAPACITY_SIZE, capacity);
    *str_len_ptr = string_length;

    if (new_string != nullptr) {
        std::memcpy(string_ptr, new_string, string_length);
    }
    // Set the char for the edge between the new node and this node.
    *children_ptr = *(string_ptr + string_length);

    // Make the child node point to this node.
    if (child_node != nullptr) {
        *child_count_ptr = 1;
        write_bytes(children_ptr, CHILD_CHAR_SIZE, child_char);
        write_bytes(children_ptr + CHILD_CHAR_SIZE, CHILD_POINTER_SIZE, child_node->page_pointer());
    } else {
        *child_count_ptr = 0;
    }

    page->make_dirty();
}


Node::~Node() {
    assert(page != nullptr);
    buffer_manager.unpin(*page);
}


void Node::init_pointers_using_page() {
    node_id_ptr = reinterpret_cast<unsigned char*>(page->get_bytes()) + page_offset;
    capacity_ptr       = node_id_ptr + NODE_ID_SIZE;
    document_count_ptr = capacity_ptr + CAPACITY_SIZE;
    str_len_ptr        = document_count_ptr + DOCUMENT_COUNT_SIZE;
    auto string_length = read_bytes(str_len_ptr, STR_LEN_SIZE);
    child_count_ptr    = str_len_ptr + STR_LEN_SIZE;
    string_ptr         = child_count_ptr + CHILD_COUNT_SIZE;
    children_ptr       = string_ptr + string_length;
}


void Node::init_pointers_using_string_length(size_t string_length) {
    node_id_ptr = reinterpret_cast<unsigned char*>(page->get_bytes()) + page_offset;
    capacity_ptr       = node_id_ptr + NODE_ID_SIZE;
    document_count_ptr = capacity_ptr + CAPACITY_SIZE;
    str_len_ptr        = document_count_ptr + DOCUMENT_COUNT_SIZE;
    child_count_ptr    = str_len_ptr + STR_LEN_SIZE;
    string_ptr         = child_count_ptr + CHILD_COUNT_SIZE;
    children_ptr       = string_ptr + string_length;
}


std::unique_ptr<Node> Node::clone() {
    buffer_manager.pin(*page);
    return std::make_unique<Node>(trie, page, page_offset);
}


size_t Node::size() {
    auto string_length = read_bytes(str_len_ptr, STR_LEN_SIZE);
    auto child_count = read_bytes(child_count_ptr, CHILD_COUNT_SIZE);

    return NODE_ID_SIZE
         + CAPACITY_SIZE
         + DOCUMENT_COUNT_SIZE
         + STR_LEN_SIZE
         + CHILD_COUNT_SIZE
         + string_length
         + child_count * CHILD_SIZE;
}


size_t Node::size(size_t string_length, size_t child_count) {
    return NODE_ID_SIZE
         + CAPACITY_SIZE
         + DOCUMENT_COUNT_SIZE
         + STR_LEN_SIZE
         + CHILD_COUNT_SIZE
         + string_length
         + child_count * CHILD_SIZE;
}


// Splits the node and returns the newly inserted node between parent and this node.
std::unique_ptr<Node> Node::split(Node* parent, unsigned char* parent_child_page_pointer_ptr, size_t split_position) {
    // Create the node that will be inserted between the parent node and this node.
    auto new_node = std::make_unique<Node>(trie, string_ptr, split_position, 1, this, string_ptr[split_position]);

    // The parent node will now point to the new node.
    if (parent != nullptr) {
        write_bytes(parent_child_page_pointer_ptr, CHILD_POINTER_SIZE, new_node->page_pointer());
        parent->page->make_dirty();
    }

    // Next we will update this node, which will be the child of the new node.
    // The -1 accounts for the char of the edge.
    *str_len_ptr = *str_len_ptr - split_position - 1;

    // Remove prefix from the string
    std::memmove(string_ptr, string_ptr + split_position + 1, *str_len_ptr);

    // Move over the children
    std::memmove(string_ptr + *str_len_ptr, children_ptr, *child_count_ptr * CHILD_SIZE);

    page->make_dirty();
    return new_node;
}


std::unique_ptr<Node> Node::insert_child(Node* parent, unsigned char* parent_child_page_pointer_ptr, const unsigned char* string) {
    // The string should be non-empty
    assert(string != nullptr && *string != '\0');

    auto string_length = std::strlen(reinterpret_cast<const char*>(string)) - 1;
    auto string_length_to_insert = std::min(string_length, STR_LEN_MAX);
    string_length -= string_length_to_insert;

    // Calculate needed capacity
    uint64_t needed_capacity = size(*str_len_ptr, *child_count_ptr + 1);

    auto child_char = *string;
    string++;
    auto child = std::make_unique<Node>(trie, string, string_length_to_insert, 0, nullptr, 0);
    string += string_length_to_insert;

    // Check if we have enough capacity
    auto capacity = read_bytes(capacity_ptr, CAPACITY_SIZE);
    if (capacity < needed_capacity) {
        // Not enough capacity, we have to move this node to a bigger capacity
        auto [new_page_number, new_page_offset] = get_space(needed_capacity);
        auto new_page = &buffer_manager.get_unversioned_page(trie.file_id, new_page_number);

        // Copy over everything
        std::memcpy(new_page->get_bytes() + new_page_offset, page->get_bytes() + page_offset, capacity);

        // Switch to new page
        buffer_manager.unpin(*page);
        page = new_page;
        page_offset = new_page_offset;

        // Update pointers and capacity
        init_pointers_using_page();
        write_bytes(capacity_ptr, CAPACITY_SIZE, needed_capacity);

        // Update parent page pointer
        if (parent == nullptr) {
            // This is the root node
            write_bytes(trie.root_page_pointer_ptr, Trie::PAGE_POINTER_SIZE, page_pointer());
            trie.root_page.make_dirty();
        } else {
            // This is not the root node
            write_bytes(parent_child_page_pointer_ptr, CHILD_POINTER_SIZE, page_pointer());
            parent->page->make_dirty();
        }
    }

    children_ptr[*child_count_ptr * CHILD_SIZE] = child_char;

    auto child_page_pointer_ptr = children_ptr + *child_count_ptr * CHILD_SIZE + CHILD_CHAR_SIZE;
    write_bytes(child_page_pointer_ptr, CHILD_POINTER_SIZE, child->page_pointer());

    *child_count_ptr = *child_count_ptr + 1;

    if (string_length > 0) {
        child = child->insert_child(this, child_page_pointer_ptr, string);
    } else {
        child->increment_document_count();
    }

    page->make_dirty();
    return child;
}


void Node::increment_document_count() {
    auto document_count = read_bytes(document_count_ptr, DOCUMENT_COUNT_SIZE);
    if (document_count < DOCUMENT_COUNT_MAX) {
        write_bytes(document_count_ptr, DOCUMENT_COUNT_SIZE, document_count + 1);
        page->make_dirty();
    }
}


uint64_t Node::insert_string(Node* parent, unsigned char* parent_child_page_pointer_ptr, const unsigned char* string_to_insert) {
    auto string_ptr_copy = string_ptr;
    size_t split_position = 0;

    // Calculate common prefix length
    while (split_position < *str_len_ptr && *string_to_insert == *string_ptr_copy) {
        string_to_insert++;
        string_ptr_copy++;
        split_position++;
    }

    if (split_position < *str_len_ptr) {
        // We have to split this node
        auto new_node = split(parent, parent_child_page_pointer_ptr, split_position);
        return new_node->insert_string(parent, parent_child_page_pointer_ptr, string_to_insert - split_position);
    } else if (*string_to_insert == '\0') {
        // string_to_insert belongs in this node
        increment_document_count();
        return read_bytes(node_id_ptr, NODE_ID_SIZE);
    } else {
        // We have to check children of this node
        for (size_t child = 0; child < *child_count_ptr; child++) {
            auto child_char = children_ptr[child * CHILD_SIZE];

            if (child_char == *string_to_insert) {
                auto child_pointer_ptr = &children_ptr[child * CHILD_SIZE + CHILD_CHAR_SIZE];
                auto child_pointer = read_bytes(child_pointer_ptr, CHILD_POINTER_SIZE);
                auto child_node = std::make_unique<Node>(trie, child_pointer);
                return child_node->insert_string(this, child_pointer_ptr, string_to_insert + 1);
            }
        }

        // No child char matched, we need to add a new child
        auto child = insert_child(parent, parent_child_page_pointer_ptr, string_to_insert);
        return read_bytes(child->node_id_ptr, NODE_ID_SIZE);
    }
}


void Node::print_trie_node(std::ostream& os) {
    auto node_id = read_bytes(node_id_ptr, NODE_ID_SIZE);
    auto document_count = read_bytes(document_count_ptr, DOCUMENT_COUNT_SIZE);
    os << node_id << " [label=\"'";
    os << escape(string_ptr, *str_len_ptr);
    os << "'\\n" << node_id << " | "<< document_count << "\"]\n";

    for (size_t child = 0; child < *child_count_ptr; child++) {
        auto child_char_ptr         = &children_ptr[child * CHILD_SIZE];
        auto child_page_pointer_ptr = &children_ptr[child * CHILD_SIZE + CHILD_CHAR_SIZE];
        auto child_page_pointer     = read_bytes(child_page_pointer_ptr, CHILD_POINTER_SIZE);

        auto child_node = std::make_unique<Node>(trie, child_page_pointer);
        auto child_node_id = read_bytes(child_node->node_id_ptr, NODE_ID_SIZE);
        os << node_id << " -> " << child_node_id;
        os << " [label=\"'" << escape(child_char_ptr, 1) << "'\"]\n";

        child_node->print_trie_node(os);
    }
}


std::pair<uint64_t, uint64_t> Node::get_space(uint64_t& capacity) {
    // Make sure capacity is a power of 2
    capacity = std::pow(2, std::ceil(std::log2(capacity)));

    // First check garbage for available free space
    uint64_t page_pointer;
    if (trie.garbage.search_and_pop_capacity(capacity, page_pointer)) {
        // Found free space in garbage
        return { page_pointer / UPage::SIZE, page_pointer % UPage::SIZE };
    }

    // Check if the space fits in the remainder of the last page
    page_pointer = read_bytes(trie.end_page_pointer_ptr, Trie::PAGE_POINTER_SIZE);
    auto available_space = UPage::SIZE - (page_pointer % UPage::SIZE);
    if (available_space < capacity) {
        // Not enough space in the remainder of page.
        // Add it to the garbage if it is useful.
        if (available_space >= trie.CAPACITY) {
            trie.garbage.add_capacity(available_space, page_pointer);
        }

        page_pointer += available_space;
    }

    // Update end_page_pointer
    write_bytes(trie.end_page_pointer_ptr, Trie::PAGE_POINTER_SIZE, page_pointer + capacity);
    trie.root_page.make_dirty();

    return { page_pointer / UPage::SIZE, page_pointer % UPage::SIZE };
}

} // namespace TextSearch
