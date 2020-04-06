//
// Created by valoleg on 06.04.2020.
//

#ifndef LAB1_TESTER_H
#define LAB1_TESTER_H


#include "Allocator.h"


class Tester {

public:
    explicit Tester(Allocator *allocator);

    void test(size_t iteration_counter, void **pointers, size_t *pointers_count, size_t iteration_count);

private:
    Allocator *allocator;

};


#endif //LAB1_TESTER_H
