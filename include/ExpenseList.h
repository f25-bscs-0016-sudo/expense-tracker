// ExpenseList.h
// Singly Linked List that stores all Expense records.
//
// DSA concepts used here:
//   - Singly linked list (insert at end in O(1) with a tail pointer,
//     delete/search by linear traversal)
//   - Merge Sort on a linked list (split with slow/fast pointers, merge
//     two sorted lists) — the natural sort for linked lists.

#ifndef EXPENSE_LIST_H
#define EXPENSE_LIST_H

#include "Expense.h"
#include <string>
#include <vector>

class ExpenseList {
private:
    Node* head;
    Node* tail;
    int count;

    // Merge sort helpers (work on the linked list directly)
    Node* getMiddle(Node* start);
    Node* mergeSorted(Node* a, Node* b, bool byAmount);
    Node* mergeSortRec(Node* start, bool byAmount);
    void fixTail();  // recompute tail after a sort

public:
    ExpenseList();
    ~ExpenseList();

    void insertAtEnd(const Expense& e);   // O(1) with tail pointer
    bool removeById(int id);              // linear search + delete
    Node* findById(int id) const;         // linear search, nullptr if not found
    void display() const;                 // print the whole list as a table
    int size() const { return count; }
    bool isEmpty() const { return head == nullptr; }
    double totalAmount() const;

    // Linear search (list is unsorted, so linear search is the right tool)
    void searchByCategory(const std::string& category) const;
    void searchByAmountRange(double low, double high) const;

    // Merge sort — O(n log n)
    void sortByAmount();  // ascending by amount
    void sortByDate();    // ascending by date (YYYY-MM-DD compares correctly as text)

    // Copy the list into a vector (keeps current order).
    // Used for binary search and for totals.
    std::vector<Expense> toVector() const;
};

#endif
