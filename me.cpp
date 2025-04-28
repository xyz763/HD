#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib> // For rand()
#include <omp.h>   // OpenMP

using namespace std;

// Merge two sorted subarrays
void merge(vector<int>& a, int l, int m, int r) {
    vector<int> L(a.begin() + l, a.begin() + m + 1);
    vector<int> R(a.begin() + m + 1, a.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < L.size() && j < R.size()) {
        a[k++] = (L[i] < R[j]) ? L[i++] : R[j++];
    }
    while (i < L.size()) a[k++] = L[i++];
    while (j < R.size()) a[k++] = R[j++];
}

// Merge Sort (Sequential + Parallel)
void mergeSort(vector<int>& a, int l, int r, bool parallel) {
    if (l >= r) return;
    int m = (l + r) / 2;
    if (parallel && r - l > 100) {
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSort(a, l, m, parallel);

            #pragma omp section
            mergeSort(a, m + 1, r, parallel);
        }
    } else {
        mergeSort(a, l, m, false);
        mergeSort(a, m + 1, r, false);
    }
    merge(a, l, m, r);
}

// Function to measure sorting time
void timeSort(const string& name, vector<int> a, void(*sortFn)(vector<int>&, int, int, bool), bool parallel) {
    auto start = chrono::high_resolution_clock::now();
    sortFn(a, 0, a.size() - 1, parallel);
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

    timeSort("Sequential Merge Sort", a, mergeSort, false);
    timeSort("Parallel Merge Sort", a, mergeSort, true);

    return 0;
}
