//
// Created by valoleg on 11.04.2020.
//

#ifndef LAB_2_TESTER_H
#define LAB_2_TESTER_H

#include "Allocator.h"

class Tester {

public:
    explicit Tester(Allocator *allocator);

    void test(size_t iteration_counter, void **pointers, size_t *pointers_count);

private:
    Allocator *allocator;

};

#endif //LAB_2_TESTER_H
