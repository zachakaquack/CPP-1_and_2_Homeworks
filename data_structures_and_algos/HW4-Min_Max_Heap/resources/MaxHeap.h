#pragma once
#include <algorithm> // For std::swap
#include <iostream>

template <typename T> class MaxHeap {
  private:
    T *arr;       // Pointer to the array (dynamically allocated)
    int capacity; // Maximum capacity of the heap
    int size;     // Current size of the heap

    // Helper function to maintain the max-heap property for a subtree rooted at
    // index i
    void maxHeapify(int i) {
        int largest = i;       // Initialize largest as root
        int left = 2 * i + 1;  // Left child index
        int right = 2 * i + 2; // Right child index

        // If left child is larger than root
        if (left < size && arr[left] > arr[largest]) {
            largest = left;
        }

        // If right child is larger than largest so far
        if (right < size && arr[right] > arr[largest]) {
            largest = right;
        }

        // If largest is not root
        if (largest != i) {
            std::swap(arr[i], arr[largest]);
            // Recursively heapify the affected sub-tree
            maxHeapify(largest);
        }
    }

  public:
    // Constructor
    MaxHeap(int cap) : capacity(cap), size(0) {
        arr = new T[capacity]; // Allocate memory on the heap
    }

    // Destructor to free allocated memory
    ~MaxHeap() { delete[] arr; }

    // Function to insert a new key
    void insert(int key) {
        if (size == capacity) {
            std::cout << "Heap is full. Cannot insert more elements"
                      << std::endl;
            return;
        }

        // Insert new element at the end
        int i = size;
        arr[i] = key;
        size++;

        // Fix the max-heap property if it is violated
        while (i != 0 && arr[(i - 1) / 2] < arr[i]) {
            std::swap(arr[i], arr[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    // Function to extract the maximum element (root)
    int extractMax() {
        if (size <= 0) {
            return -1; // Or throw an exception
        }
        if (size == 1) {
            size--;
            return arr[0];
        }

        // Store the maximum value and remove it from heap
        int root = arr[0];
        arr[0] = arr[size - 1];
        size--;
        maxHeapify(0); // Heapify the new root

        return root;
    }

    // Function to get the maximum element (root) without removing it
    int getMax() const {
        if (size <= 0) {
            return -1;
        }
        return arr[0];
    }

    void printHeap() const {
        for (int i = 0; i < size; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }
};
