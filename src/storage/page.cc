#include "page.h"

#include <cassert>

#include "storage/file_id.h"
#include "storage/file_manager.h"

Page::Page(PageId page_id, char* bytes) noexcept :
    page_id(page_id),
    pins(1),
    bytes(bytes),
    dirty(false) { }


Page::Page() noexcept :
    page_id(FileId(FileId::UNASSIGNED), 0),
    pins(0),
    bytes(nullptr),
    dirty(false) { }


void Page::operator=(const Page& other) noexcept {
    assert(pins == 0 && "Cannot reassign page if it is pinned");
    this->flush();
    this->page_id = other.page_id;
    this->pins    = other.pins;
    this->dirty   = other.dirty;
    this->bytes   = other.bytes;
}


void Page::reset() {
    assert(pins == 0 && "Cannot reset page if it is pinned");
    this->page_id = PageId(FileId(FileId::UNASSIGNED), 0);
    this->pins    = 0;
    this->dirty   = false;
    this->bytes   = nullptr;
}


void Page::flush() {
    if (dirty) {
        file_manager.flush(page_id, bytes);
        dirty = false;
    }
}
