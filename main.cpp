#include "Allocator.h"
#include "Tester.h"
#include <iostream>

using namespace std;

int main() {
    const size_t page_count = 128;
    Allocator allocator(page_count);
    Tester tester(&allocator);
    void *pointers[1488];
    size_t pointers_count = 0;
    int menu_status = -1;
    while (menu_status != 0) {
        allocator.mem_dump();
        cout << "Choose the action:" << endl << "1 - Allocate memory" << endl << "2 - Test allocator" << endl;
        if (pointers_count != 0) {
            cout << "3 - Free memory" << endl << "4 - Reallocate memory" << endl;
        }
        cout << "0 - Exit" << endl;

        cin >> menu_status;
        switch (menu_status) {
            case 1: {
                size_t size;
                cout << "Enter the size of memory to allocate" << endl;

                cin >> size;
                void *ptr = allocator.mem_alloc(size);
                if (ptr != nullptr) {
                    pointers[pointers_count++] = ptr;
                }
                cout << "Current pointer: " << ptr << endl;
                break;
            }
            case 2: {
                size_t iteration_counter;
                cout << "Enter the number of iterations" << endl;

                cin >> iteration_counter;
                tester.test(iteration_counter, pointers, &pointers_count);
                break;
            }
            case 3: {
                cout << "Choose block of memory to free:" << endl;
                for (size_t i = 0; i < pointers_count; i++) {
                    cout << i << " - " << pointers[i] << endl;
                }

                size_t n;
                cin >> n;
                if (n < pointers_count) {
                    allocator.mem_free(pointers[n]);
                    for (size_t i = n + 1; i < pointers_count; i++) {
                        pointers[i - 1] = pointers[i];
                    }
                    pointers_count--;
                }
                break;
            }
            case 4: {
                cout << "Choose block of memory to reallocate:" << endl;
                for (size_t i = 0; i < pointers_count; i++) {
                    cout << i << " - " << pointers[i] << endl;
                }

                size_t n;
                cin >> n;
                if (n < pointers_count) {
                    size_t size;
                    cout << "Enter the size of memory to reallocate" << endl;

                    cin >> size;
                    void *ptr = allocator.mem_realloc(pointers[n], size);
                    if (ptr != nullptr) {
                        pointers[n] = ptr;
                    }
                    cout << "Current pointer: " << ptr << endl;
                }
                break;
            }
            default:
                break;
        }
        cin.get();
    }
    return 0;
}