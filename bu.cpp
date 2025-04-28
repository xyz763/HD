#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib> // For rand()
#include <omp.h>   // OpenMP

using namespace std;

// Bubble Sort (Sequential + Parallel)
void bubbleSort(vector<int>& a, bool parallel) {
    int n = a.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        #pragma omp parallel for shared(swapped) if(parallel) schedule(dynamic)
        for (int j = 0; j < n - i - 1; ++j) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // Already sorted
    }
}

// Function to measure sorting time
void timeSort(const string& name, vector<int> a, void(*sortFn)(vector<int>&, bool), bool parallel) {
    auto start = chrono::high_resolution_clock::now();
    sortFn(a, parallel);
    auto end = chrono::high_resolution_clock::now();
    cout << name << ": " << chrono::duration<double>(end - start).count() << " seconds\n";
}

int main() {
    int N;
    cout << "Enter array size: ";
    cin >> N;
    
    vector<int> a(N);
    cout << "1. Random Input\n2. Manual Input\nChoose input method (1/2): ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        for (int& x : a) x = rand() % 10000; // Random numbers
        cout << "Random array generated.\n";
    } else {
        cout << "Enter " << N << " elements:\n";
        for (int& x : a) cin >> x;
    }

    timeSort("Sequential Bubble Sort", a, bubbleSort, false);
    timeSort("Parallel Bubble Sort", a, bubbleSort, true);

    return 0;
}