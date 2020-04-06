//
// Created by valoleg on 06.04.2020.
//
#include "Allocator.h"

Allocator::Allocator(size_t memory_size) {
    this->memory_size = memory_size;
    memory = new char[this->memory_size];
    auto *head = (header *) memory;
    head->is_busy = false;
    head->prev_size = 0;
    head->size = this->memory_size - sizeof(header);
}

Allocator::~Allocator() {
    delete[] memory;
}

void mem_copy(size_t old_ptr, size_t new_ptr, size_t length) {
    auto *p = (size_t *) old_ptr;
    auto *cp = (size_t *) new_ptr;
    for (int i = 0; i < (length / 4); i++) {
        *cp = *p;
        p++;
        cp++;
    }
}

void Allocator::mem_dump() {
    auto *head = (header *) memory;
    int i = 1;
    while (head < ((header *) ((size_t) memory + memory_size))) {
        cout << "Block #" << i << ":\t";
        if (head->is_busy) {
            cout << "Busy";
        } else {
            cout << "Free";
        }
        cout << "\t" << head->size << " bytes" << "\t" << ((void *) ((size_t) head + 12)) << endl;
        head = (header *) ((size_t) head + head->size + sizeof(header));
        i++;
    }
}

void *Allocator::mem_alloc(size_t size) {
    size_t size4 = size;
    while ((size4 % 4) != 0) {
        size4++;
    }
    auto *ptr = new size_t;
    *ptr = 0;
    auto *head = (header *) memory;
    while ((*ptr == NULL) && ((size_t) head < ((size_t) memory + memory_size))) {
        while ((((head->is_busy) && ((size_t) head < ((size_t) memory + memory_size))) || ((!head->is_busy) &&
                                                                                           (head->size < size4))) &&
               ((size_t) head < ((size_t) memory + memory_size))) {
            head = (header *) ((size_t) head + head->size + sizeof(header));
        }
        if ((size_t) head < ((size_t) memory + memory_size)) {
            if (head->size >= (size4 + sizeof(header))) {
                auto *new_head = (header *) ((size_t) head + head->size - size4);
                auto *next_head = (header *) ((size_t) head + head->size +
                                              sizeof(header));
                if (((size_t) next_head) < ((size_t) memory) + memory_size) {
                    next_head->prev_size = size4;
                }
                head->size = head->size - size4 - sizeof(header);
                new_head->is_busy = true;
                new_head->size = size4;
                new_head->prev_size = head->size;
                *ptr = (size_t) new_head + sizeof(header);
            } else {
                if (head->size == size4) {
                    head->is_busy = true;
                    *ptr = (size_t) head + sizeof(header);
                } else {
                    head = (header *) ((size_t) head + head->size + sizeof(header));
                }
            }
        }
    }
    return (void *) *ptr;
}

void *Allocator::mem_realloc(void *ptr, size_t size) {
    auto *re_ptr = new size_t;
    *re_ptr = 0;
    if (ptr == nullptr) {
        *re_ptr = (size_t) mem_alloc(size);
    } else {
        size_t size4 = size;
        while ((size4 % 4) != 0) {
            size4++;
        }
        auto *head = (header *) ((size_t) ptr - sizeof(header));
        size_t prev_size = head->size;
        if (size4 != head->size) {
            auto *prev_head = (header *) ((size_t) head - head->prev_size - sizeof(header));
            auto *next_head = (header *) ((size_t) head + head->size + sizeof(header));
            if ((((size_t) prev_head) >= ((size_t) memory)) && (((size_t) next_head) < ((size_t)
                                                                                                memory +
                                                                                        memory_size))) {
                if ((!prev_head->is_busy) && (!next_head->is_busy)) {
                    if ((prev_head->size + head->size + next_head->size + 2 * sizeof(header))
                        >= size4) {
                        size_t sum_size = prev_head->size + head->size + next_head->size +
                                          2 * sizeof(header);
                        prev_head->is_busy = true;
                        prev_head->size = size4;
                        *re_ptr = (size_t) prev_head + sizeof(header);
                        auto *next_next_head = (header *) ((size_t) next_head +
                                                           next_head->size + sizeof(header));
                        size_t copy_size;
                        if (prev_size > size4) {
                            copy_size = prev_size;
                        } else {
                            copy_size = size4;
                        }
                        mem_copy((size_t) ptr, *re_ptr, copy_size);
                        if (sum_size != size4) {
                            auto *new_head = (header *) ((size_t) prev_head +
                                                         prev_head->size + sizeof(header));
                            new_head->is_busy = false;
                            new_head->prev_size = prev_head->size;
                            new_head->size = sum_size - new_head->prev_size -
                                             sizeof(header);
                            if ((size_t) next_next_head < ((size_t) memory +
                                                           memory_size)) {
                                next_next_head->prev_size = new_head->size;
                            }
                        } else {
                            if ((size_t) next_next_head < ((size_t) memory +
                                                           memory_size)) {
                                next_next_head->prev_size = prev_head->size;
                            }
                        }
                    } else {
                        *re_ptr = (size_t) mem_alloc(size);
                        if (*re_ptr != NULL) {
                            mem_copy((size_t) ptr, *re_ptr, head->size);
                            mem_free(ptr);
                        }
                    }
                } else {
                    if (!prev_head->is_busy) {
                        if ((prev_head->size + head->size + sizeof(header)) >= size4) {
                            size_t sum_size = prev_head->size + head->size +
                                              sizeof(header);
                            prev_head->is_busy = true;
                            prev_head->size = size4;
                            *re_ptr = (size_t) prev_head + sizeof(header);
                            size_t copy_size;
                            if (prev_size > size4) {
                                copy_size = prev_size;
                            } else {
                                copy_size = size4;
                            }
                            mem_copy((size_t) ptr, *re_ptr, copy_size);
                            if (sum_size != size4) {
                                auto *new_head = (header *) ((size_t)
                                                                     prev_head + prev_head->size +
                                                             sizeof(header));
                                new_head->is_busy = false;
                                new_head->prev_size = prev_head->size;
                                new_head->size = sum_size - prev_head->size -
                                                 sizeof(header);
                                next_head->prev_size = new_head->size;
                            } else {
                                next_head->prev_size = prev_head->size;
                            }
                        } else {
                            *re_ptr = (size_t) mem_alloc(size);
                            if (*re_ptr != NULL) {
                                mem_copy((size_t) ptr, *re_ptr, head->size);
                                mem_free(ptr);
                            }
                        }
                    } else {
                        if (!next_head->is_busy) {
                            if ((head->size + next_head->size + sizeof(header)) >=
                                size4) {
                                size_t sum_size = head->size + next_head->size +
                                                  sizeof(header);
                                head->size = size4;
                                *re_ptr = (size_t) head + sizeof(header);
                                auto *next_next_head = (header *) ((size_t)
                                                                           next_head + next_head->size +
                                                                   sizeof(header));
                                if (sum_size != size4) {
                                    auto *new_head = (header *)
                                            ((size_t) head + head->size + sizeof(header));
                                    new_head->is_busy = false;
                                    new_head->prev_size = head->size;
                                    new_head->size = sum_size - head->size -
                                                     sizeof(header);
                                    if (((size_t) next_next_head) < ((size_t)
                                                                             memory + memory_size)) {
                                        next_next_head->prev_size = new_head->size;
                                    }
                                } else {
                                    if (((size_t) next_next_head) < ((size_t)
                                                                             memory + memory_size)) {
                                        next_next_head->prev_size = head->size;
                                    }
                                }
                            } else {
                                *re_ptr = (size_t) mem_alloc(size);
                                if (*re_ptr != NULL) {
                                    mem_copy((size_t) ptr, *re_ptr, head->size);
                                    mem_free(ptr);
                                }
                            }
                        } else {
                            if (size4 > head->size) {
                                *re_ptr = (size_t) mem_alloc(size);
                                if (*re_ptr != NULL) {
                                    mem_copy((size_t) ptr, *re_ptr, head->size);
                                    mem_free(ptr);
                                }
                            } else {
                                auto *new_head = (header *) ((size_t) head
                                                             + size4 + sizeof(header));
                                new_head->is_busy = false;
                                new_head->size = head->size - size4 -
                                                 sizeof(header);
                                head->size = size4;
                                new_head->prev_size = head->size;
                                next_head->prev_size = new_head->size;
                                *re_ptr = (size_t) ptr;
                            }
                        }
                    }
                }
            } else {
                if (((size_t) prev_head) >= ((size_t) memory)) {
                    if (!prev_head->is_busy) {
                        if ((prev_head->size + head->size + sizeof(header)) >= size4) {
                            size_t sum_size = prev_head->size + head->size +
                                              sizeof(header);
                            prev_head->is_busy = true;
                            prev_head->size = size4;
                            *re_ptr = (size_t) prev_head + sizeof(header);
                            size_t copy_size;
                            if (prev_size > size4) {
                                copy_size = prev_size;
                            } else {
                                copy_size = size4;
                            }
                            mem_copy((size_t) ptr, *re_ptr, copy_size);
                            if (sum_size != size4) {
                                auto *new_head = (header *) ((size_t)
                                                                     prev_head + prev_head->size +
                                                             sizeof(header));
                                new_head->is_busy = false;
                                new_head->prev_size = prev_head->size;
                                new_head->size = sum_size - prev_head->size -
                                                 sizeof(header);
                            }
                        } else {
                            *re_ptr = (size_t) mem_alloc(size);
                            if (*re_ptr != NULL) {
                                mem_copy((size_t) ptr, *re_ptr, head->size);
                                mem_free(ptr);
                            }
                        }
                    } else {
                        if (size4 > head->size) {
                            *re_ptr = (size_t) mem_alloc(size);
                            if (*re_ptr != NULL) {
                                mem_copy((size_t) ptr, *re_ptr, head->size);
                                mem_free(ptr);
                            }
                        } else {
                            auto *new_head = (header *) ((size_t) head + size4
                                                         + sizeof(header));
                            new_head->is_busy = false;
                            new_head->size = head->size - size4 - sizeof(header);
                            head->size = size4;
                            new_head->prev_size = head->size;
                            *re_ptr = (size_t) ptr;
                        }
                    }
                } else {
                    if (((size_t) next_head) < ((size_t) memory + memory_size)) {
                        if (!next_head->is_busy) {
                            if ((head->size + next_head->size + sizeof(header)) >=
                                size4) {
                                size_t sum_size = head->size + next_head->size +
                                                  sizeof(header);
                                head->size = size4;
                                *re_ptr = (size_t) head + sizeof(header);
                                auto *next_next_head = (header *) ((size_t)
                                                                           next_head + next_head->size +
                                                                   sizeof(header));
                                if (sum_size != size4) {
                                    auto *new_head = (header *)
                                            ((size_t) head + head->size + sizeof(header));
                                    new_head->is_busy = false;
                                    new_head->prev_size = head->size;
                                    new_head->size = sum_size - head->size -
                                                     sizeof(header);
                                    if (((size_t) next_next_head) < ((size_t)
                                                                             memory + memory_size)) {
                                        next_next_head->prev_size = new_head->size;
                                    }
                                } else {
                                    if (((size_t) next_next_head) < ((size_t)
                                                                             memory + memory_size)) {
                                        next_next_head->prev_size = head->size;
                                    }
                                }
                            } else {
                                *re_ptr = (size_t) mem_alloc(size);
                                if (*re_ptr != NULL) {
                                    mem_copy((size_t) ptr, *re_ptr, head->size);
                                    mem_free(ptr);
                                }
                            }
                        } else {
                            *re_ptr = (size_t) mem_alloc(size);
                            if (*re_ptr != NULL) {
                                mem_copy((size_t) ptr, *re_ptr, head->size);
                                mem_free(ptr);
                            }
                        }
                    } else {
                        if (size4 > head->size) {
                            *re_ptr = 0;
                        } else {
                            auto *new_head = (header *) ((size_t) head + size4
                                                         + sizeof(header));
                            new_head->prev_size = size4;
                            new_head->is_busy = false;
                            new_head->size = head->size - size4 - sizeof(header);
                            head->size = size4;
                        }
                    }
                }
            }
        } else {
            *re_ptr = (size_t) ptr;
        }
    }
    return (void *) *re_ptr;
};

void Allocator::mem_free(void *ptr) {
    if (ptr == nullptr) {
        return;
    }
    auto *head = (header *) ((size_t) ptr - sizeof(header));
    if (!head->is_busy) {
        return;
    }
    auto *prev_head = (header *) ((size_t) head - head->prev_size - sizeof(header));
    auto *next_head = (header *) ((size_t) head + head->size + sizeof(header));
    if ((((size_t) prev_head) >= ((size_t) memory)) && (((size_t) next_head) < ((size_t) memory +
                                                                                memory_size))) {
        if ((!prev_head->is_busy) && (!next_head->is_busy)) {
            auto *next_next_head = (header *) ((size_t) next_head + next_head->size +
                                               sizeof(header));
            prev_head->size = prev_head->size + head->size + next_head->size + 2 * sizeof(header);
            if (((size_t) next_next_head) < ((size_t) memory + memory_size)) {
                next_next_head->prev_size = prev_head->size;
            }
        } else {
            if (!prev_head->is_busy) {
                prev_head->size = prev_head->size + head->size + sizeof(header);
                next_head->prev_size = prev_head->size;
            } else {
                if (!next_head->is_busy) {
                    auto *next_next_head = (header *) ((size_t) next_head + next_head->size + sizeof(header));
                    head->is_busy = false;
                    head->size = head->size + next_head->size + sizeof(header);
                    if (((size_t) next_next_head) < ((size_t) memory + memory_size)) {
                        next_next_head->prev_size = head->size;
                    }
                } else {
                    head->is_busy = false;
                }
            }
        }
    } else {
        if (((size_t) prev_head) >= ((size_t) memory)) {
            if (!prev_head->is_busy) {
                prev_head->size = prev_head->size + head->size + sizeof(header);
            } else {
                head->is_busy = false;
            }
        } else {
            if (((size_t) next_head) < ((size_t) memory + memory_size)) {
                if (!next_head->is_busy) {
                    auto *next_next_head = (header *) ((size_t) next_head + next_head->size + sizeof(header));
                    head->is_busy = false;
                    head->size = head->size + next_head->size + sizeof(header);
                    if (((size_t) next_next_head) < ((size_t) memory + memory_size)) {
                        next_next_head->prev_size = head->size;
                    }
                } else {
                    head->is_busy = false;
                }
            } else {
                head->is_busy = false;
            }
        }
    }
}
