//
// Created by valoleg on 11.04.2020.
//

#ifndef LAB_2_ALLOCATOR_H
#define LAB_2_ALLOCATOR_H

#include <cstdlib>

struct memory_header {
    void *page16;
    size_t page16_count;
    void *page32;
    size_t page32_count;
    void *page64;
    size_t page64_count;
    void *page128;
    size_t page128_count;
    void *page256;
    size_t page256_count;
    void *page512;
    size_t page512_count;
    void *page1024;
    size_t page1024_count;
    void *page2048;
    size_t page2048_count;
    void *page_full;
    size_t page_full_count;
};

struct page_header {
    char status;
    size_t block_size;
    size_t free_blocks_count;
    void *free_block;
    void *next_page;
};

class Allocator {

public:
    explicit Allocator(size_t page_count);

    ~Allocator();

    void *mem_alloc(size_t size);

    void mem_free(void *ptr);

    void *mem_realloc(void *ptr, size_t size);

    void mem_dump();

    size_t get_page_size();

    size_t get_page_count();

private:
    static const size_t page_size = 4096;
    void *memory;
    size_t page_count;

};

#endif //LAB_2_ALLOCATOR_H

