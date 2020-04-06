//
// Created by valoleg on 06.04.2020.
//

#include "Tester.h"

const string menu_separator = "-------------------------------------------------------";


Tester::Tester(Allocator *allocator) {
    this->allocator = allocator;
}

void Tester::test(size_t m_size, void **pointers, size_t *pointers_count, size_t iteration_count) {
    srand((unsigned) time(nullptr));
    for (int i = 0; i < iteration_count; i++) {
        cout << menu_separator << endl << i + 1 << " iteration" << endl;
        int option;
        if (*pointers_count == 0) {
            option = 0;
        } else {
            option = rand() % 3;
        }
        switch (option) {
            case 0: {
                auto size = (size_t) (rand() * (m_size / 20) / RAND_MAX);
                if (size == 0) {
                    size++;
                }
                cout << "mem_alloc(" << size << ")" << endl;
                void *ptr = allocator->mem_alloc(size);
                if (ptr != nullptr) {
                    pointers[*pointers_count] = ptr;
                    *pointers_count = *pointers_count + 1;
                }
                cout << "Current pointer: " << ptr << endl;
                break;
            }
            case 1: {
                auto e = (size_t) (rand() * (*pointers_count) / RAND_MAX);
                cout << "mem_free(" << pointers[e] << ")" << endl;
                allocator->mem_free(pointers[e]);
                for (int i = e + 1; i < *pointers_count; i++) {
                    pointers[i - 1] = pointers[i];
                }
                *pointers_count = *pointers_count - 1;
                break;
            }
            case 2: {
                auto s = (size_t) (rand() * (m_size / 20) / RAND_MAX);
                srand((unsigned) time(nullptr));
                auto e = (size_t) (rand() * (*pointers_count) / RAND_MAX);
                cout << "mem_realloc(" << pointers[e] << ", " << s << ")" << endl;
                void *ptr = allocator->mem_realloc(pointers[e], s);
                if (ptr != NULL) {
                    pointers[e] = ptr;
                }
                cout << "Current pointer: " << ptr << endl;
                break;
            }
            default:
                break;
        }
        allocator->mem_dump();
        cout << menu_separator << endl;
    }
}