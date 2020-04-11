//
// Created by valoleg on 11.04.2020.
//

#include "Allocator.h"
#include <iostream>

using namespace std;

const string menu_separator = "-------------------------------------------------------";


Allocator::Allocator(size_t page_count) {
    this->page_count = page_count;
    if (this->page_count < 9) {
        this->page_count = 9;
    }
    size_t memorySize = (page_size + sizeof(page_header)) * this->page_count + sizeof(memory_header);
    memory = new char[memorySize];
    auto *m_header = (memory_header *) memory;
    m_header->page16 = nullptr;
    m_header->page16_count = 0;
    m_header->page32 = nullptr;
    m_header->page32_count = 0;
    m_header->page64 = nullptr;
    m_header->page64_count = 0;
    m_header->page128 = nullptr;
    m_header->page128_count = 0;
    m_header->page256 = nullptr;
    m_header->page256_count = 0;
    m_header->page512 = nullptr;
    m_header->page512_count = 0;
    m_header->page1024 = nullptr;
    m_header->page1024_count = 0;
    m_header->page2048 = nullptr;
    m_header->page2048_count = 0;
    m_header->page_full = (void *) ((size_t) memory + sizeof(memory_header));
    m_header->page_full_count = this->page_count;
    auto *p_header = (page_header *) (m_header->page_full);
    for (size_t i = 0; i < this->page_count; i++) {
        p_header->status = 0;
        p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
        p_header->free_blocks_count = 1;
        p_header->block_size = page_size;
        if (i < (this->page_count - 1)) {
            p_header->next_page = (void *) ((size_t) p_header + sizeof(page_header) + page_size);
        } else {
            p_header->next_page = nullptr;
        }
        p_header = (page_header *) p_header->next_page;
    }
}

Allocator::~Allocator() {
    delete[] memory;
}

void *Allocator::mem_alloc(size_t size) {
    struct memory_header *m_header = (memory_header *) memory;
    struct page_header *p_header = nullptr;
    if (size <= (page_size / 2)) {
        if (size > (page_size / 4)) {
            if (m_header->page2048 != NULL) {
                p_header = (page_header *) m_header->page2048;
                while ((p_header->free_block == NULL) && (p_header->next_page != NULL)) {
                    p_header = (page_header *) p_header->next_page;
                }
                if (p_header->free_block != NULL) {
                    size_t *next_block_ptr = (size_t *) p_header->free_block;
                    p_header->free_block = (void *) *next_block_ptr;
                    p_header->free_blocks_count = p_header->free_blocks_count - 1;
                    return (void *) next_block_ptr;
                } else {
                    if (m_header->page_full == NULL) {
                        return mem_alloc(4096);
                    } else {
                        p_header = (page_header *) m_header->page_full;
                        p_header->status = 1;
                        m_header->page_full = p_header->next_page;
                        p_header->next_page = m_header->page2048;
                        p_header->block_size = 2048;
                        p_header->free_blocks_count = page_size / 2048;
                        size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) + page_size -
                                                        p_header->block_size);
                        size_t next_block = NULL;
                        while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                            *block_ptr = next_block;
                            next_block = (size_t) block_ptr;
                            block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                        }
                        p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                        m_header->page_full_count = m_header->page_full_count - 1;
                        m_header->page2048 = (void *) p_header;
                        m_header->page2048_count = m_header->page2048_count + 1;
                        return mem_alloc(size);
                    }
                }
            } else {
                if (m_header->page_full_count == 0) {
                    return NULL;
                }
                p_header = (page_header *) m_header->page_full;
                p_header->status = 1;
                m_header->page_full = p_header->next_page;
                p_header->next_page = m_header->page2048;
                p_header->block_size = 2048;
                p_header->free_blocks_count = page_size / 2048;
                size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) + page_size -
                                                p_header->block_size);
                size_t next_block = NULL;
                while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                    *block_ptr = next_block;
                    next_block = (size_t) block_ptr;
                    block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                }
                p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                m_header->page_full_count = m_header->page_full_count - 1;
                m_header->page2048 = (void *) p_header;
                m_header->page2048_count = m_header->page2048_count + 1;
                return mem_alloc(size);
            }
        } else {
            if (size > (page_size / 8)) {
                if (m_header->page1024 != NULL) {
                    p_header = (page_header *) m_header->page1024;
                    while ((p_header->free_block == NULL) && (p_header->next_page != NULL)) {
                        p_header = (page_header *) p_header->next_page;
                    }
                    if (p_header->free_block != NULL) {
                        size_t *next_block_ptr = (size_t *) p_header->free_block;
                        p_header->free_block = (void *) *next_block_ptr;
                        p_header->free_blocks_count = p_header->free_blocks_count - 1;
                        return (void *) next_block_ptr;
                    } else {
                        if (m_header->page_full == NULL) {
                            return mem_alloc(2048);
                        } else {
                            p_header = (page_header *) m_header->page_full;
                            p_header->status = 1;
                            m_header->page_full = p_header->next_page;
                            p_header->next_page = m_header->page1024;
                            p_header->block_size = 1024;
                            p_header->free_blocks_count = page_size / 1024;
                            size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) + page_size -
                                                            p_header->block_size);
                            size_t next_block = NULL;
                            while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                                *block_ptr = next_block;
                                next_block = (size_t) block_ptr;
                                block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                            }
                            p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                            m_header->page_full_count = m_header->page_full_count - 1;
                            m_header->page1024 = (void *) p_header;
                            m_header->page1024_count = m_header->page1024_count + 1;
                            return mem_alloc(size);
                        }
                    }
                } else {
                    p_header = (page_header *) m_header->page_full;
                    p_header->status = 1;
                    m_header->page_full = p_header->next_page;
                    p_header->next_page = m_header->page1024;
                    p_header->block_size = 1024;
                    p_header->free_blocks_count = page_size / 1024;
                    size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) + page_size -
                                                    p_header->block_size);
                    size_t next_block = NULL;
                    while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                        *block_ptr = next_block;
                        next_block = (size_t) block_ptr;
                        block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                    }
                    p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                    m_header->page_full_count = m_header->page_full_count - 1;
                    m_header->page1024 = (void *) p_header;
                    m_header->page1024_count = m_header->page1024_count + 1;
                    return mem_alloc(size);
                }
            } else {
                if (size > (page_size / 16)) {
                    if (m_header->page512 != NULL) {
                        p_header = (page_header *) m_header->page512;
                        while ((p_header->free_block == NULL) && (p_header->next_page != NULL)) {
                            p_header = (page_header *) p_header->next_page;
                        }
                        if (p_header->free_block != NULL) {
                            size_t *next_block_ptr = (size_t *) p_header->free_block;
                            p_header->free_block = (void *) *next_block_ptr;
                            p_header->free_blocks_count = p_header->free_blocks_count - 1;
                            return (void *) next_block_ptr;
                        } else {
                            if (m_header->page_full == NULL) {
                                return mem_alloc(1024);
                            } else {
                                p_header = (page_header *) m_header->page_full;
                                p_header->status = 1;
                                m_header->page_full = p_header->next_page;
                                p_header->next_page = m_header->page512;
                                p_header->block_size = 512;
                                p_header->free_blocks_count = page_size / 512;
                                size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) + page_size -
                                                                p_header->block_size);
                                size_t next_block = NULL;
                                while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                                    *block_ptr = next_block;
                                    next_block = (size_t) block_ptr;
                                    block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                                }
                                p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                m_header->page_full_count = m_header->page_full_count - 1;
                                m_header->page512 = (void *) p_header;
                                m_header->page512_count = m_header->page512_count + 1;
                                return mem_alloc(size);
                            }
                        }
                    } else {
                        p_header = (page_header *) m_header->page_full;
                        p_header->status = 1;
                        m_header->page_full = p_header->next_page;
                        p_header->next_page = m_header->page512;
                        p_header->block_size = 512;
                        p_header->free_blocks_count = page_size / 512;
                        size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) + page_size -
                                                        p_header->block_size);
                        size_t next_block = NULL;
                        while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                            *block_ptr = next_block;
                            next_block = (size_t) block_ptr;
                            block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                        }
                        p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                        m_header->page_full_count = m_header->page_full_count - 1;
                        m_header->page512 = (void *) p_header;
                        m_header->page512_count = m_header->page512_count + 1;
                        return mem_alloc(size);
                    }
                } else {
                    if (size > (page_size / 32)) {
                        if (m_header->page256 != NULL) {
                            p_header = (page_header *) m_header->page256;
                            while ((p_header->free_block == NULL) && (p_header->next_page != NULL)) {
                                p_header = (page_header *) p_header->next_page;
                            }
                            if (p_header->free_block != NULL) {
                                size_t *next_block_ptr = (size_t *) p_header->free_block;
                                p_header->free_block = (void *) *next_block_ptr;
                                p_header->free_blocks_count = p_header->free_blocks_count - 1;
                                return (void *) next_block_ptr;
                            } else {
                                if (m_header->page_full == NULL) {
                                    return mem_alloc(512);
                                } else {
                                    p_header = (page_header *) m_header->page_full;
                                    p_header->status = 1;
                                    m_header->page_full = p_header->next_page;
                                    p_header->next_page = m_header->page256;
                                    p_header->block_size = 256;
                                    p_header->free_blocks_count = page_size / 256;
                                    size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) + page_size -
                                                                    p_header->block_size);
                                    size_t next_block = NULL;
                                    while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                                        *block_ptr = next_block;
                                        next_block = (size_t) block_ptr;
                                        block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                                    }
                                    p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                    m_header->page_full_count = m_header->page_full_count - 1;
                                    m_header->page256 = (void *) p_header;
                                    m_header->page256_count = m_header->page256_count + 1;
                                    return mem_alloc(size);
                                }
                            }
                        } else {
                            p_header = (page_header *) m_header->page_full;
                            p_header->status = 1;
                            m_header->page_full = p_header->next_page;
                            p_header->next_page = m_header->page256;
                            p_header->block_size = 256;
                            p_header->free_blocks_count = page_size / 256;
                            size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) + page_size -
                                                            p_header->block_size);
                            size_t next_block = NULL;
                            while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                                *block_ptr = next_block;
                                next_block = (size_t) block_ptr;
                                block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                            }
                            p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                            m_header->page_full_count = m_header->page_full_count - 1;
                            m_header->page256 = (void *) p_header;
                            m_header->page256_count = m_header->page256_count + 1;
                            return mem_alloc(size);
                        }
                    } else {
                        if (size > (page_size / 64)) {
                            if (m_header->page128 != NULL) {
                                p_header = (page_header *) m_header->page128;
                                while ((p_header->free_block == NULL) && (p_header->next_page != NULL)) {
                                    p_header = (page_header *) p_header->next_page;
                                }
                                if (p_header->free_block != NULL) {
                                    size_t *next_block_ptr = (size_t *) p_header->free_block;
                                    p_header->free_block = (void *) *next_block_ptr;
                                    p_header->free_blocks_count = p_header->free_blocks_count - 1;
                                    return (void *) next_block_ptr;
                                } else {
                                    if (m_header->page_full == NULL) {
                                        return mem_alloc(256);
                                    } else {
                                        p_header = (page_header *) m_header->page_full;
                                        p_header->status = 1;
                                        m_header->page_full = p_header->next_page;
                                        p_header->next_page = m_header->page128;
                                        p_header->block_size = 128;
                                        p_header->free_blocks_count = page_size / 128;
                                        size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) +
                                                                        page_size - p_header->block_size);
                                        size_t next_block = NULL;
                                        while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                                            *block_ptr = next_block;
                                            next_block = (size_t) block_ptr;
                                            block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                                        }
                                        p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                        m_header->page_full_count = m_header->page_full_count - 1;
                                        m_header->page128 = (void *) p_header;
                                        m_header->page128_count = m_header->page128_count + 1;
                                        return mem_alloc(size);
                                    }
                                }
                            } else {
                                p_header = (page_header *) m_header->page_full;
                                p_header->status = 1;
                                m_header->page_full = p_header->next_page;
                                p_header->next_page = m_header->page128;
                                p_header->block_size = 128;
                                p_header->free_blocks_count = page_size / 128;
                                size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) + page_size -
                                                                p_header->block_size);
                                size_t next_block = NULL;
                                while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                                    *block_ptr = next_block;
                                    next_block = (size_t) block_ptr;
                                    block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                                }
                                p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                m_header->page_full_count = m_header->page_full_count - 1;
                                m_header->page128 = (void *) p_header;
                                m_header->page128_count = m_header->page128_count + 1;
                                return mem_alloc(size);
                            }
                        } else {
                            if (size > (page_size / 128)) {
                                if (m_header->page64 != NULL) {
                                    p_header = (page_header *) m_header->page64;
                                    while ((p_header->free_block == NULL) && (p_header->next_page != NULL)) {
                                        p_header = (page_header *) p_header->next_page;
                                    }
                                    if (p_header->free_block != NULL) {
                                        size_t *next_block_ptr = (size_t *) p_header->free_block;
                                        p_header->free_block = (void *) *next_block_ptr;
                                        p_header->free_blocks_count = p_header->free_blocks_count - 1;
                                        return (void *) next_block_ptr;
                                    } else {
                                        if (m_header->page_full == NULL) {
                                            return mem_alloc(128);
                                        } else {
                                            p_header = (page_header *) m_header->page_full;
                                            p_header->status = 1;
                                            m_header->page_full = p_header->next_page;
                                            p_header->next_page = m_header->page64;
                                            p_header->block_size = 64;
                                            p_header->free_blocks_count = page_size / 64;
                                            size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) +
                                                                            page_size - p_header->block_size);
                                            size_t next_block = NULL;
                                            while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                                                *block_ptr = next_block;
                                                next_block = (size_t) block_ptr;
                                                block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                                            }
                                            p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                            m_header->page_full_count = m_header->page_full_count - 1;
                                            m_header->page64 = (void *) p_header;
                                            m_header->page64_count = m_header->page64_count + 1;
                                            return mem_alloc(size);
                                        }
                                    }
                                } else {
                                    p_header = (page_header *) m_header->page_full;
                                    p_header->status = 1;
                                    m_header->page_full = p_header->next_page;
                                    p_header->next_page = m_header->page64;
                                    p_header->block_size = 64;
                                    p_header->free_blocks_count = page_size / 64;
                                    size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) + page_size -
                                                                    p_header->block_size);
                                    size_t next_block = NULL;
                                    while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                                        *block_ptr = next_block;
                                        next_block = (size_t) block_ptr;
                                        block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                                    }
                                    p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                    m_header->page_full_count = m_header->page_full_count - 1;
                                    m_header->page64 = (void *) p_header;
                                    m_header->page64_count = m_header->page64_count + 1;
                                    return mem_alloc(size);
                                }
                            } else {
                                if (size > (page_size / 256)) {
                                    if (m_header->page32 != NULL) {
                                        p_header = (page_header *) m_header->page32;
                                        while ((p_header->free_block == NULL) && (p_header->next_page != NULL)) {
                                            p_header = (page_header *) p_header->next_page;
                                        }
                                        if (p_header->free_block != NULL) {
                                            size_t *next_block_ptr = (size_t *) p_header->free_block;
                                            p_header->free_block = (void *) *next_block_ptr;
                                            p_header->free_blocks_count = p_header->free_blocks_count - 1;
                                            return (void *) next_block_ptr;
                                        } else {
                                            if (m_header->page_full == NULL) {
                                                return mem_alloc(64);
                                            } else {
                                                p_header = (page_header *) m_header->page_full;
                                                p_header->status = 1;
                                                m_header->page_full = p_header->next_page;
                                                p_header->next_page = m_header->page32;
                                                p_header->block_size = 32;
                                                p_header->free_blocks_count = page_size / 32;
                                                size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) +
                                                                                page_size - p_header->block_size);
                                                size_t next_block = NULL;
                                                while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                                                    *block_ptr = next_block;
                                                    next_block = (size_t) block_ptr;
                                                    block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                                                }
                                                p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                                m_header->page_full_count = m_header->page_full_count - 1;
                                                m_header->page32 = (void *) p_header;
                                                m_header->page32_count = m_header->page32_count + 1;
                                                return mem_alloc(size);
                                            }
                                        }
                                    } else {
                                        p_header = (page_header *) m_header->page_full;
                                        p_header->status = 1;
                                        m_header->page_full = p_header->next_page;
                                        p_header->next_page = m_header->page32;
                                        p_header->block_size = 32;
                                        p_header->free_blocks_count = page_size / 32;
                                        size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) +
                                                                        page_size - p_header->block_size);
                                        size_t next_block = NULL;
                                        while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                                            *block_ptr = next_block;
                                            next_block = (size_t) block_ptr;
                                            block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                                        }
                                        p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                        m_header->page_full_count = m_header->page_full_count - 1;
                                        m_header->page32 = (void *) p_header;
                                        m_header->page32_count = m_header->page32_count + 1;
                                        return mem_alloc(size);
                                    }
                                } else {
                                    if (m_header->page16 != NULL) {
                                        p_header = (page_header *) m_header->page16;
                                        while ((p_header->free_block == NULL) && (p_header->next_page != NULL)) {
                                            p_header = (page_header *) p_header->next_page;
                                        }
                                        if (p_header->free_block != NULL) {
                                            size_t *next_block_ptr = (size_t *) p_header->free_block;
                                            p_header->free_block = (void *) *next_block_ptr;
                                            p_header->free_blocks_count = p_header->free_blocks_count - 1;
                                            return (void *) next_block_ptr;
                                        } else {
                                            if (m_header->page_full == NULL) {
                                                return mem_alloc(32);
                                            } else {
                                                p_header = (page_header *) m_header->page_full;
                                                p_header->status = 1;
                                                m_header->page_full = p_header->next_page;
                                                p_header->next_page = m_header->page16;
                                                p_header->block_size = 16;
                                                p_header->free_blocks_count = page_size / 16;
                                                size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) +
                                                                                page_size - p_header->block_size);
                                                size_t next_block = NULL;
                                                while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                                                    *block_ptr = next_block;
                                                    next_block = (size_t) block_ptr;
                                                    block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                                                }
                                                p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                                m_header->page_full_count = m_header->page_full_count - 1;
                                                m_header->page16 = (void *) p_header;
                                                m_header->page16_count = m_header->page16_count + 1;
                                                return mem_alloc(size);
                                            }
                                        }
                                    } else {
                                        p_header = (page_header *) m_header->page_full;
                                        p_header->status = 1;
                                        m_header->page_full = p_header->next_page;
                                        p_header->next_page = m_header->page16;
                                        p_header->block_size = 16;
                                        p_header->free_blocks_count = page_size / 16;
                                        size_t *block_ptr = (size_t *) ((size_t) p_header + sizeof(page_header) +
                                                                        page_size - p_header->block_size);
                                        size_t next_block = NULL;
                                        while ((size_t) block_ptr >= ((size_t) p_header + sizeof(page_header))) {
                                            *block_ptr = next_block;
                                            next_block = (size_t) block_ptr;
                                            block_ptr = (size_t *) ((size_t) block_ptr - p_header->block_size);
                                        }
                                        p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                        m_header->page_full_count = m_header->page_full_count - 1;
                                        m_header->page16 = (void *) p_header;
                                        m_header->page16_count = m_header->page16_count + 1;
                                        return mem_alloc(size);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        size_t needed_pages = size / page_size;
        if ((size % page_size) != 0) {
            needed_pages++;
        }
        if (m_header->page_full_count < needed_pages) {
            return NULL;
        } else {
            p_header = (page_header *) m_header->page_full;
            struct page_header *next_page_header = p_header;
            for (size_t i = 0; i < needed_pages; i++) {
                next_page_header->status = 2;
                next_page_header->free_block = NULL;
                next_page_header->free_blocks_count = needed_pages;
                next_page_header = (page_header *) next_page_header->next_page;
            }
            m_header->page_full = (void *) next_page_header;
            m_header->page_full_count = m_header->page_full_count - needed_pages;
            return (void *) ((size_t) p_header + sizeof(page_header));
        }
    }
}

void Allocator::mem_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    struct memory_header *m_header = (memory_header *) memory;
    struct page_header *p_header = (page_header *) ((size_t) memory + sizeof(memory_header));
    while (((size_t) ptr < (size_t) p_header) || ((size_t) ptr > ((size_t) p_header + sizeof(page_header) + page_size))) {
        p_header = (page_header *) ((size_t) p_header + sizeof(page_header) + page_size);
    }
    if (p_header->status == 2) {
        struct page_header *page_ptr = p_header;
        size_t page_ptr_count = p_header->free_blocks_count;
        for (size_t i = 0; i < page_ptr_count; i++) {
            page_ptr->status = 0;
            page_ptr->free_blocks_count = 1;
            page_ptr->free_block = (void *) ((size_t) page_ptr + sizeof(page_header));
            if (i < (page_ptr_count - 1)) {
                page_ptr = (page_header *) page_ptr->next_page;
            }
        }
        page_ptr->next_page = m_header->page_full;
        m_header->page_full = (void *) p_header;
        m_header->page_full_count = m_header->page_full_count + page_ptr_count;
    } else {
        if (p_header->status == 1) {
            size_t *next_block_ptr = (size_t *) ptr;
            *next_block_ptr = (size_t) p_header->free_block;
            p_header->free_block = ptr;
            p_header->free_blocks_count = p_header->free_blocks_count + 1;
            if (p_header->free_blocks_count == (page_size / p_header->block_size)) {
                if (p_header->block_size == 2048) {
                    struct page_header *page_ptr = (page_header *) m_header->page2048;
                    if ((size_t) p_header == (size_t) page_ptr) {
                        m_header->page2048 = p_header->next_page;
                        m_header->page2048_count = m_header->page2048_count - 1;
                        p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                        p_header->status = 0;
                        p_header->block_size = page_size;
                        p_header->free_blocks_count = 1;
                        p_header->next_page = m_header->page_full;
                        m_header->page_full = (void *) p_header;
                        m_header->page_full_count = m_header->page_full_count + 1;
                    } else {
                        while ((size_t) p_header != (size_t) (page_ptr->next_page)) {
                            page_ptr = (page_header *) p_header->next_page;
                        }
                        page_ptr->next_page = p_header->next_page;
                        m_header->page2048_count = m_header->page2048_count - 1;
                        p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                        p_header->status = 0;
                        p_header->block_size = page_size;
                        p_header->free_blocks_count = 1;
                        p_header->next_page = m_header->page_full;
                        m_header->page_full = (void *) p_header;
                        m_header->page_full_count = m_header->page_full_count + 1;
                    }
                } else {
                    if (p_header->block_size == 1024) {
                        struct page_header *page_ptr = (page_header *) m_header->page1024;
                        if ((size_t) p_header == (size_t) page_ptr) {
                            m_header->page1024 = p_header->next_page;
                            m_header->page1024_count = m_header->page1024_count - 1;
                            p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                            p_header->status = 0;
                            p_header->block_size = page_size;
                            p_header->free_blocks_count = 1;
                            p_header->next_page = m_header->page_full;
                            m_header->page_full = (void *) p_header;
                            m_header->page_full_count = m_header->page_full_count + 1;
                        } else {
                            while ((size_t) p_header != (size_t) (page_ptr->next_page)) {
                                page_ptr = (page_header *) p_header->next_page;
                            }
                            page_ptr->next_page = p_header->next_page;
                            m_header->page1024_count = m_header->page1024_count - 1;
                            p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                            p_header->status = 0;
                            p_header->block_size = page_size;
                            p_header->free_blocks_count = 1;
                            p_header->next_page = m_header->page_full;
                            m_header->page_full = (void *) p_header;
                            m_header->page_full_count = m_header->page_full_count + 1;
                        }
                    } else {
                        if (p_header->block_size == 512) {
                            struct page_header *page_ptr = (page_header *) m_header->page512;
                            if ((size_t) p_header == (size_t) page_ptr) {
                                m_header->page512 = p_header->next_page;
                                m_header->page512_count = m_header->page512_count - 1;
                                p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                p_header->status = 0;
                                p_header->block_size = page_size;
                                p_header->free_blocks_count = 1;
                                p_header->next_page = m_header->page_full;
                                m_header->page_full = (void *) p_header;
                                m_header->page_full_count = m_header->page_full_count + 1;
                            } else {
                                while ((size_t) p_header != (size_t) (page_ptr->next_page)) {
                                    page_ptr = (page_header *) p_header->next_page;
                                }
                                page_ptr->next_page = p_header->next_page;
                                m_header->page512_count = m_header->page512_count - 1;
                                p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                p_header->status = 0;
                                p_header->block_size = page_size;
                                p_header->free_blocks_count = 1;
                                p_header->next_page = m_header->page_full;
                                m_header->page_full = (void *) p_header;
                                m_header->page_full_count = m_header->page_full_count + 1;
                            }
                        } else {
                            if (p_header->block_size == 256) {
                                struct page_header *page_ptr = (page_header *) m_header->page256;
                                if ((size_t) p_header == (size_t) page_ptr) {
                                    m_header->page256 = p_header->next_page;
                                    m_header->page256_count = m_header->page256_count - 1;
                                    p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                    p_header->status = 0;
                                    p_header->block_size = page_size;
                                    p_header->free_blocks_count = 1;
                                    p_header->next_page = m_header->page_full;
                                    m_header->page_full = (void *) p_header;
                                    m_header->page_full_count = m_header->page_full_count + 1;
                                } else {
                                    while ((size_t) p_header != (size_t) (page_ptr->next_page)) {
                                        page_ptr = (page_header *) p_header->next_page;
                                    }
                                    page_ptr->next_page = p_header->next_page;
                                    m_header->page256_count = m_header->page256_count - 1;
                                    p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                    p_header->status = 0;
                                    p_header->block_size = page_size;
                                    p_header->free_blocks_count = 1;
                                    p_header->next_page = m_header->page_full;
                                    m_header->page_full = (void *) p_header;
                                    m_header->page_full_count = m_header->page_full_count + 1;
                                }
                            } else {
                                if (p_header->block_size == 128) {
                                    struct page_header *page_ptr = (page_header *) m_header->page128;
                                    if ((size_t) p_header == (size_t) page_ptr) {
                                        m_header->page128 = p_header->next_page;
                                        m_header->page128_count = m_header->page128_count - 1;
                                        p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                        p_header->status = 0;
                                        p_header->block_size = page_size;
                                        p_header->free_blocks_count = 1;
                                        p_header->next_page = m_header->page_full;
                                        m_header->page_full = (void *) p_header;
                                        m_header->page_full_count = m_header->page_full_count + 1;
                                    } else {
                                        while ((size_t) p_header != (size_t) (page_ptr->next_page)) {
                                            page_ptr = (page_header *) p_header->next_page;
                                        }
                                        page_ptr->next_page = p_header->next_page;
                                        m_header->page128_count = m_header->page128_count - 1;
                                        p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                        p_header->status = 0;
                                        p_header->block_size = page_size;
                                        p_header->free_blocks_count = 1;
                                        p_header->next_page = m_header->page_full;
                                        m_header->page_full = (void *) p_header;
                                        m_header->page_full_count = m_header->page_full_count + 1;
                                    }
                                } else {
                                    if (p_header->block_size == 64) {
                                        struct page_header *page_ptr = (page_header *) m_header->page64;
                                        if ((size_t) p_header == (size_t) page_ptr) {
                                            m_header->page64 = p_header->next_page;
                                            m_header->page64_count = m_header->page64_count - 1;
                                            p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                            p_header->status = 0;
                                            p_header->block_size = page_size;
                                            p_header->free_blocks_count = 1;
                                            p_header->next_page = m_header->page_full;
                                            m_header->page_full = (void *) p_header;
                                            m_header->page_full_count = m_header->page_full_count + 1;
                                        } else {
                                            while ((size_t) p_header != (size_t) (page_ptr->next_page)) {
                                                page_ptr = (page_header *) p_header->next_page;
                                            }
                                            page_ptr->next_page = p_header->next_page;
                                            m_header->page64_count = m_header->page64_count - 1;
                                            p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                            p_header->status = 0;
                                            p_header->block_size = page_size;
                                            p_header->free_blocks_count = 1;
                                            p_header->next_page = m_header->page_full;
                                            m_header->page_full = (void *) p_header;
                                            m_header->page_full_count = m_header->page_full_count + 1;
                                        }
                                    } else {
                                        if (p_header->block_size == 32) {
                                            struct page_header *page_ptr = (page_header *) m_header->page32;
                                            if ((size_t) p_header == (size_t) page_ptr) {
                                                m_header->page32 = p_header->next_page;
                                                m_header->page32_count = m_header->page32_count - 1;
                                                p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                                p_header->status = 0;
                                                p_header->block_size = page_size;
                                                p_header->free_blocks_count = 1;
                                                p_header->next_page = m_header->page_full;
                                                m_header->page_full = (void *) p_header;
                                                m_header->page_full_count = m_header->page_full_count + 1;
                                            } else {
                                                while ((size_t) p_header != (size_t) (page_ptr->next_page)) {
                                                    page_ptr = (page_header *) p_header->next_page;
                                                }
                                                page_ptr->next_page = p_header->next_page;
                                                m_header->page32_count = m_header->page32_count - 1;
                                                p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                                p_header->status = 0;
                                                p_header->block_size = page_size;
                                                p_header->free_blocks_count = 1;
                                                p_header->next_page = m_header->page_full;
                                                m_header->page_full = (void *) p_header;
                                                m_header->page_full_count = m_header->page_full_count + 1;
                                            }
                                        } else {
                                            struct page_header *page_ptr = (page_header *) m_header->page16;
                                            if ((size_t) p_header == (size_t) page_ptr) {
                                                m_header->page16 = p_header->next_page;
                                                m_header->page16_count = m_header->page16_count - 1;
                                                p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                                p_header->status = 0;
                                                p_header->block_size = page_size;
                                                p_header->free_blocks_count = 1;
                                                p_header->next_page = m_header->page_full;
                                                m_header->page_full = (void *) p_header;
                                                m_header->page_full_count = m_header->page_full_count + 1;
                                            } else {
                                                while ((size_t) p_header != (size_t) (page_ptr->next_page)) {
                                                    page_ptr = (page_header *) p_header->next_page;
                                                }
                                                page_ptr->next_page = p_header->next_page;
                                                m_header->page16_count = m_header->page16_count - 1;
                                                p_header->free_block = (void *) ((size_t) p_header + sizeof(page_header));
                                                p_header->status = 0;
                                                p_header->block_size = page_size;
                                                p_header->free_blocks_count = 1;
                                                p_header->next_page = m_header->page_full;
                                                m_header->page_full = (void *) p_header;
                                                m_header->page_full_count = m_header->page_full_count + 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void *Allocator::mem_realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return mem_alloc(size);
    }
    struct memory_header *m_header = (memory_header *) memory;
    struct page_header *p_header = (page_header *) ((size_t) memory + sizeof(memory_header));
    while (((size_t) ptr < (size_t) p_header) || ((size_t) ptr > ((size_t) p_header + sizeof(page_header) + page_size))) {
        p_header = (page_header *) ((size_t) p_header + sizeof(page_header) + page_size);
    }
    if (((p_header->block_size / 2) < size) && (p_header->block_size >= size)) {
        return ptr;
    }
    void *result_ptr = mem_alloc(size);
    if (result_ptr == NULL) {
        return NULL;
    }
    char *ptr1 = (char *) ptr;
    char *ptr2 = (char *) result_ptr;
    if ((p_header->status == 1) || ((p_header->status == 2) && (p_header->free_blocks_count == 1)) ||
        ((p_header->status == 2) && (size <= page_size))) {
        size_t copy_size;
        if (size < p_header->block_size) {
            copy_size = size;
        } else {
            copy_size = p_header->block_size;
        }
        for (size_t i = 0; i < copy_size; i++) {
            *ptr2 = *ptr1;
            ptr1++;
            ptr2++;
        }
    } else {
        struct page_header *copy_header = (page_header *) ((size_t) result_ptr - sizeof(page_header));
        size_t copy_pages_count;
        if (p_header->free_blocks_count < copy_header->free_blocks_count) {
            copy_pages_count = p_header->free_blocks_count;
        } else {
            copy_pages_count = copy_header->free_blocks_count;
        }
        for (size_t i = 0; i < copy_pages_count; i++) {
            for (size_t j = 0; j < page_size; j++) {
                *ptr2 = *ptr1;
                ptr1++;
                ptr2++;
            }
            ptr1 = (char *) ((size_t) p_header->next_page + sizeof(page_header));
            ptr2 = (char *) ((size_t) copy_header->next_page + sizeof(page_header));
        }
    }
    mem_free(ptr);
    return result_ptr;
}

void Allocator::mem_dump() {
    struct memory_header *m_header = (memory_header *) memory;
    struct page_header *p_header;
    size_t total_memory = page_count * page_size;
    size_t total_free_memory = 0;
    cout << menu_separator << endl;
    cout << "Dump of memory:" << endl;
    cout << "The number of pages: " << page_count << endl;
    cout << "The size of page: " << page_size << endl;
    cout << menu_separator << endl;
    cout << "Free pages" << endl;
    cout << menu_separator << endl;
    cout << "The number of pages: " << m_header->page_full_count << endl;
    p_header = (page_header *) m_header->page_full;
    total_free_memory += m_header->page_full_count * page_size;
    for (size_t i = 0; i < m_header->page_full_count; i++) {
        cout << "Page #" << i << ": " << p_header << endl;
        p_header = (page_header *) p_header->next_page;
    }
    cout << menu_separator << endl;
    cout << "Pages divided to blocks of 2048 bytes" << endl;
    cout << menu_separator << endl;
    cout << "The number of pages: " << m_header->page2048_count << endl;
    p_header = (page_header *) m_header->page2048;
    for (size_t i = 0; i < m_header->page2048_count; i++) {
        cout << "Page #" << i << ": " << p_header << endl;
        cout << "The number of free blocks: " << p_header->free_blocks_count << endl;
        total_free_memory += p_header->free_blocks_count * p_header->block_size;
        void *free_block = p_header->free_block;
        for (size_t j = 0; j < p_header->free_blocks_count; j++) {
            cout << "Block #" << j << ": " << free_block << endl;
            free_block = (void *) *((size_t *) free_block);
        }
        p_header = (page_header *) p_header->next_page;
    }
    cout << menu_separator << endl;
    cout << "Pages divided to blocks of 1024 bytes" << endl;
    cout << menu_separator << endl;
    cout << "The number of pages: " << m_header->page1024_count << endl;
    p_header = (page_header *) m_header->page1024;
    for (size_t i = 0; i < m_header->page1024_count; i++) {
        cout << "Page #" << i << ": " << p_header << endl;
        cout << "The number of free blocks: " << p_header->free_blocks_count << endl;
        total_free_memory += p_header->free_blocks_count * p_header->block_size;
        void *free_block = p_header->free_block;
        for (size_t j = 0; j < p_header->free_blocks_count; j++) {
            cout << "Block #" << j << ": " << free_block << endl;
            free_block = (void *) *((size_t *) free_block);
        }
        p_header = (page_header *) p_header->next_page;
    }
    cout << menu_separator << endl;
    cout << "Pages divided to blocks of 512 bytes" << endl;
    cout << menu_separator << endl;
    cout << "The number of pages: " << m_header->page512_count << endl;
    p_header = (page_header *) m_header->page512;
    for (size_t i = 0; i < m_header->page512_count; i++) {
        cout << "Page #" << i << ": " << p_header << endl;
        cout << "The number of free blocks: " << p_header->free_blocks_count << endl;
        total_free_memory += p_header->free_blocks_count * p_header->block_size;
        void *free_block = p_header->free_block;
        for (size_t j = 0; j < p_header->free_blocks_count; j++) {
            cout << "Block #" << j << ": " << free_block << endl;
            free_block = (void *) *((size_t *) free_block);
        }
        p_header = (page_header *) p_header->next_page;
    }
    cout << menu_separator << endl;
    cout << "Pages divided to blocks of 256 bytes" << endl;
    cout << menu_separator << endl;
    cout << "The number of pages: " << m_header->page256_count << endl;
    p_header = (page_header *) m_header->page256;
    for (size_t i = 0; i < m_header->page256_count; i++) {
        cout << "Page #" << i << ": " << p_header << endl;
        cout << "The number of free blocks: " << p_header->free_blocks_count << endl;
        total_free_memory += p_header->free_blocks_count * p_header->block_size;
        void *free_block = p_header->free_block;
        for (size_t j = 0; j < p_header->free_blocks_count; j++) {
            cout << "Block #" << j << ": " << free_block << endl;
            free_block = (void *) *((size_t *) free_block);
        }
        p_header = (page_header *) p_header->next_page;
    }
    cout << menu_separator << endl;
    cout << "Pages divided to blocks of 128 bytes" << endl;
    cout << menu_separator << endl;
    cout << "The number of pages: " << m_header->page128_count << endl;
    p_header = (page_header *) m_header->page128;
    for (size_t i = 0; i < m_header->page128_count; i++) {
        cout << "Page #" << i << ": " << p_header << endl;
        cout << "The number of free blocks: " << p_header->free_blocks_count << endl;
        total_free_memory += p_header->free_blocks_count * p_header->block_size;
        void *free_block = p_header->free_block;
        for (size_t j = 0; j < p_header->free_blocks_count; j++) {
            cout << "Block #" << j << ": " << free_block << endl;
            free_block = (void *) *((size_t *) free_block);
        }
        p_header = (page_header *) p_header->next_page;
    }
    cout << menu_separator << endl;
    cout << "Pages divided to blocks of 64 bytes" << endl;
    cout << menu_separator << endl;
    cout << "The number of pages: " << m_header->page64_count << endl;
    p_header = (page_header *) m_header->page64;
    for (size_t i = 0; i < m_header->page64_count; i++) {
        cout << "Page #" << i << ": " << p_header << endl;
        cout << "The number of free blocks: " << p_header->free_blocks_count << endl;
        total_free_memory += p_header->free_blocks_count * p_header->block_size;
        void *free_block = p_header->free_block;
        for (size_t j = 0; j < p_header->free_blocks_count; j++) {
            cout << "Block #" << j << ": " << free_block << endl;
            free_block = (void *) *((size_t *) free_block);
        }
        p_header = (page_header *) p_header->next_page;
    }
    cout << menu_separator << endl;
    cout << "Pages divided to blocks of 32 bytes" << endl;
    cout << menu_separator << endl;
    cout << "The number of pages: " << m_header->page32_count << endl;
    p_header = (page_header *) m_header->page32;
    for (size_t i = 0; i < m_header->page32_count; i++) {
        cout << "Page #" << i << ": " << p_header << endl;
        cout << "The number of free blocks: " << p_header->free_blocks_count << endl;
        total_free_memory += p_header->free_blocks_count * p_header->block_size;
        void *free_block = p_header->free_block;
        for (size_t j = 0; j < p_header->free_blocks_count; j++) {
            cout << "Block #" << j << ": " << free_block << endl;
            free_block = (void *) *((size_t *) free_block);
        }
        p_header = (page_header *) p_header->next_page;
    }
    cout << menu_separator << endl;
    cout << "Pages divided to blocks of 16 bytes" << endl;
    cout << menu_separator << endl;
    cout << "The number of pages: " << m_header->page16_count << endl;
    p_header = (page_header *) m_header->page16;
    for (size_t i = 0; i < m_header->page16_count; i++) {
        cout << "Page #" << i << ": " << p_header << endl;
        cout << "The number of free blocks: " << p_header->free_blocks_count << endl;
        total_free_memory += p_header->free_blocks_count * p_header->block_size;
        void *free_block = p_header->free_block;
        for (size_t j = 0; j < p_header->free_blocks_count; j++) {
            cout << "Block #" << j << ": " << free_block << endl;
            free_block = (void *) *((size_t *) free_block);
        }
        p_header = (page_header *) p_header->next_page;
    }
    cout << menu_separator << endl;
    cout << "Total memory: " << total_memory << endl;
    cout << "Total busy memory: " << (total_memory - total_free_memory) << endl;
    cout << "Total free memory: " << total_free_memory << endl;
    cout << menu_separator << endl;
}

size_t Allocator::get_page_size() {
    return page_size;
}

size_t Allocator::get_page_count() {
    return page_count;
}
