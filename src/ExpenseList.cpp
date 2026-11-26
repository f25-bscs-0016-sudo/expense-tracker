// ExpenseList.cpp
// Singly linked list + merge sort implementation.

#include "ExpenseList.h"
#include <iomanip>
#include <iostream>
#include <string>

// ---------- table printing helpers (local to this file) ----------

static void printHeader() {
    std::cout << std::left
              << std::setw(5) << "ID"
              << std::setw(13) << "Date"
              << std::setw(15) << "Category"
              << std::setw(30) << "Description"
              << std::setw(12) << "Amount (Rs)" << '\n';
    std::cout << std::string(75, '-') << '\n';
}

static void printRow(const Expense& e) {
    std::cout << std::left
              << std::setw(5) << e.id
              << std::setw(13) << e.date
              << std::setw(15) << e.category
              << std::setw(30) << e.description
              << std::fixed << std::setprecision(2)
              << std::setw(12) << e.amount << '\n';
}

// ---------- constructor / destructor ----------

ExpenseList::ExpenseList() : head(nullptr), tail(nullptr), count(0) {}

ExpenseList::~ExpenseList() {
    Node* cur = head;
    while (cur) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;  // free every node, no memory leak
    }
}

// ---------- basic list operations ----------

void ExpenseList::insertAtEnd(const Expense& e) {
    Node* node = new Node(e);
    if (!head) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    count++;
}

bool ExpenseList::removeById(int id) {
    if (!head) return false;

    if (head->data.id == id) {
        Node* tmp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete tmp;
        count--;
        return true;
    }

    Node* cur = head;
    while (cur->next && cur->next->data.id != id) {
        cur = cur->next;
    }
    if (!cur->next) return false;  // id not found

    Node* tmp = cur->next;
    cur->next = tmp->next;
    if (tmp == tail) tail = cur;
    delete tmp;
    count--;
    return true;
}

Node* ExpenseList::findById(int id) const {
    Node* cur = head;
    while (cur) {
        if (cur->data.id == id) return cur;
        cur = cur->next;
    }
    return nullptr;
}

void ExpenseList::display() const {
    if (!head) {
        std::cout << "No expenses recorded yet.\n";
        return;
    }
    printHeader();
    Node* cur = head;
    while (cur) {
        printRow(cur->data);
        cur = cur->next;
    }
    std::cout << std::string(75, '-') << '\n';
    std::cout << "Total: Rs " << std::fixed << std::setprecision(2)
              << totalAmount() << "  (" << count << " expense(s))\n";
}

double ExpenseList::totalAmount() const {
    double total = 0.0;
    Node* cur = head;
    while (cur) {
        total += cur->data.amount;
        cur = cur->next;
    }
    return total;
}

// ---------- linear search ----------

void ExpenseList::searchByCategory(const std::string& category) const {
    bool found = false;
    Node* cur = head;
    while (cur) {
        if (cur->data.category == category) {
            if (!found) printHeader();
            printRow(cur->data);
            found = true;
        }
        cur = cur->next;
    }
    if (!found) {
        std::cout << "No expenses found in category '" << category << "'.\n";
    }
}

void ExpenseList::searchByAmountRange(double low, double high) const {
    bool found = false;
    Node* cur = head;
    while (cur) {
        if (cur->data.amount >= low && cur->data.amount <= high) {
            if (!found) printHeader();
            printRow(cur->data);
            found = true;
        }
        cur = cur->next;
    }
    if (!found) {
        std::cout << "No expenses found between Rs " << low << " and Rs " << high << ".\n";
    }
}

// ---------- merge sort on the linked list ----------

// Slow/fast pointer trick: slow ends up at the middle node.
Node* ExpenseList::getMiddle(Node* start) {
    if (!start) return nullptr;
    Node* slow = start;
    Node* fast = start->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// Merge two sorted lists into one sorted list.
Node* ExpenseList::mergeSorted(Node* a, Node* b, bool byAmount) {
    if (!a) return b;
    if (!b) return a;

    bool takeA;
    if (byAmount) {
        takeA = a->data.amount <= b->data.amount;
    } else {
        // Dates are YYYY-MM-DD, so plain text comparison = chronological order.
        takeA = a->data.date <= b->data.date;
    }

    if (takeA) {
        a->next = mergeSorted(a->next, b, byAmount);
        return a;
    }
    b->next = mergeSorted(a, b->next, byAmount);
    return b;
}

Node* ExpenseList::mergeSortRec(Node* start, bool byAmount) {
    if (!start || !start->next) return start;  // 0 or 1 node: already sorted

    Node* mid = getMiddle(start);
    Node* right = mid->next;
    mid->next = nullptr;  // split into two halves

    Node* left = mergeSortRec(start, byAmount);
    right = mergeSortRec(right, byAmount);
    return mergeSorted(left, right, byAmount);
}

void ExpenseList::fixTail() {
    tail = head;
    if (tail) {
        while (tail->next) tail = tail->next;
    }
}

void ExpenseList::sortByAmount() {
    head = mergeSortRec(head, true);
    fixTail();
}

void ExpenseList::sortByDate() {
    head = mergeSortRec(head, false);
    fixTail();
}

// ---------- helper ----------

std::vector<Expense> ExpenseList::toVector() const {
    std::vector<Expense> v;
    Node* cur = head;
    while (cur) {
        v.push_back(cur->data);
        cur = cur->next;
    }
    return v;
}
