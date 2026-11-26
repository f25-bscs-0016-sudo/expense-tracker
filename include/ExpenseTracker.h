// ExpenseTracker.h
// The tracker: owns the linked list and adds the higher-level features.
//
// DSA concepts used here:
//   - Stack (std::stack, LIFO) for Undo: every add/delete pushes an
//     Action; undo pops the last action and reverses it.
//   - Hashing (std::unordered_map) for category-wise totals: one pass
//     over the data, O(1) average lookup per category.
//   - Binary Search on a sorted copy (by amount) — O(log n), only
//     valid because the copy is sorted first (merge sort).

#ifndef EXPENSE_TRACKER_H
#define EXPENSE_TRACKER_H

#include "ExpenseList.h"
#include <stack>
#include <string>

struct Action {
    enum Type { ADD, DELETE } type;
    Expense expense;  // the expense that was added or deleted
};

class ExpenseTracker {
private:
    ExpenseList list;
    std::stack<Action> undoStack;  // Stack (LIFO) — undo history
    int nextId;

public:
    ExpenseTracker();

    void addExpense(const std::string& date, const std::string& category,
                    const std::string& description, double amount);
    bool deleteExpense(int id);
    void undo();  // undo the last add/delete

    void viewAll() const { list.display(); }
    void searchCategory(const std::string& category) const;
    void searchAmountRange(double low, double high) const;
    void binarySearchByAmount(double amount) const;  // exact-amount search

    void sortByAmount() { list.sortByAmount(); }
    void sortByDate() { list.sortByDate(); }

    void categoryTotals() const;                       // hashing
    void monthlySummary(const std::string& month) const;  // month = "YYYY-MM"

    double getTotal() const { return list.totalAmount(); }
    int getCount() const { return list.size(); }
    const ExpenseList& getList() const { return list; }

    // Used by the file loader: insert a record with its saved id,
    // without touching the undo stack.
    void addLoadedExpense(const Expense& e);
    void setNextId(int id) { nextId = id; }
    int getNextId() const { return nextId; }
};

#endif
