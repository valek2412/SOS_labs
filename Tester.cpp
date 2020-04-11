//
// Created by valoleg on 11.04.2020.
//

#include "Tester.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const string menu_delimiter = "------------------------------------------------------------------";

Tester::Tester(Allocator *allocator) {
    this->allocator = allocator;
}

void Tester::test(size_t iteration_counter, void **pointers, size_t *pointers_count) {
    srand((unsigned) time(nullptr));
    for (size_t i = 0; i < iteration_counter; i++) {
        cout << menu_delimiter << endl << i + 1 << " iteration" << endl;
        int option;
        if (*pointers_count == 0) {
            option = 0;
        } else {
            option = rand() % 3;
        }
        switch (option) {
            case 0: {
                auto size_class = (size_t) (rand() * 4 / RAND_MAX);
                if (size_class == 4) {
                    size_class--;
                }
                size_t s;
                srand((unsigned) time(nullptr));
                switch (size_class) {
                    case 0: {
                        s = (size_t) (rand() * (allocator->get_page_size() / 32) / RAND_MAX);
                        break;
                    }
                    case 1: {
                        s = (size_t) (rand() * (allocator->get_page_size() / 2) / RAND_MAX);
                        break;
                    }
                    case 2: {
                        s = (size_t) (rand() * (4 * allocator->get_page_size()) / RAND_MAX);
                        break;
                    }
                    case 3: {
                        s = (size_t) (rand() * (allocator->get_page_size() * allocator->get_page_count()) / RAND_MAX);
                        break;
                    }
                    default:
                        break;
                }
                if (s == 0) {
                    s++;
                }
                cout << "mem_alloc(" << s << ")" << endl;
                void *ptr = allocator->mem_alloc(s);
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
                for (size_t j = e + 1; j < *pointers_count; j++) {
                    pointers[j - 1] = pointers[j];
                }
                *pointers_count = *pointers_count - 1;
                break;
            }
            case 2: {
                auto size_class = (size_t) (rand() * 4 / RAND_MAX);
                if (size_class == 4) {
                    size_class--;
                }
                size_t s;
                srand((unsigned) time(nullptr));
                switch (size_class) {
                    case 0: {
                        s = (size_t) (rand() * (allocator->get_page_size() / 32) / RAND_MAX);
                        break;
                    }
                    case 1: {
                        s = (size_t) (rand() * (allocator->get_page_size() / 2) / RAND_MAX);
                        break;
                    }
                    case 2: {
                        s = (size_t) (rand() * (4 * allocator->get_page_size()) / RAND_MAX);
                        break;
                    }
                    case 3: {
                        s = (size_t) (rand() * (allocator->get_page_size() * allocator->get_page_count()) / RAND_MAX);
                        break;
                    }
                    default:
                        break;
                }
                srand((unsigned) time(nullptr));
                auto e = (size_t) (rand() * (*pointers_count) / RAND_MAX);
                cout << "mem_realloc(" << pointers[e] << ", " << s << ")" << endl;
                void *ptr = allocator->mem_realloc(pointers[e], s);
                if (ptr != nullptr) {
                    pointers[e] = ptr;
                }
                cout << "Current pointer: " << ptr << endl;
                break;
            }
        }
        allocator->mem_dump();
        cout << menu_delimiter << endl;
    }
}
