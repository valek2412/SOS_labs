#include "Allocator.h"
#include "Tester.h"
#include <iostream>
#include <ctime>

using namespace std;

const string menu_separator = "-------------------------------------------------------";


int main() {
    const size_t memory_size = 4096;
    Allocator allocator(memory_size);
    void *pointers[228];
    size_t pointers_count = 0;
    int menu_status = -1;
    Tester tester(&allocator);
    cout << menu_separator << endl << "Memory size: " << memory_size << endl;
    while (menu_status != 0) {
        allocator.mem_dump();
        cout << menu_separator << endl
             << "Choose option:" << endl
             << "1 - Allocate memory" << endl
             << "2 - Test allocator" << endl;
        if (pointers_count != 0) {
            cout << "3 - Free memory" << endl
                 << "4 - Reallocate memory" << endl;
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
                size_t iteration_count;
                cout << "Enter the number of iterations" << endl;
                cin >> iteration_count;
                tester.test(memory_size, pointers, &pointers_count, iteration_count);
                break;
            }
            case 3: {
                cout << "Choose block of memory to free or 0 to free all:" << endl;
                for (int i = 0; i < pointers_count; i++) {
                    cout << i << " - " << pointers[i] << endl;
                }
                int n;
                cin >> n;
                if (n == 0) {
                    for (int i = 0; i < pointers_count; i++) {
                        allocator.mem_free(pointers[i]);
                    }
                    break;
                }
                if (n < pointers_count) {
                    allocator.mem_free(pointers[n]);
                    for (int i = n + 1; i < pointers_count; i++) {
                        pointers[i - 1] = pointers[i];
                    }
                    pointers_count--;
                }
                break;
            }
            case 4: {
                cout << "Choose block of memory to reallocate:" << endl;
                for (int i = 0; i < pointers_count; i++) {
                    cout << i << " - " << pointers[i] << endl;
                }
                int n;
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