// ExpenseTracker.cpp
// Tracker operations: undo (stack), binary search, hashing totals.

#include "ExpenseTracker.h"
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

// ---------- merge sort + binary search on a vector copy ----------
// (Used only by binarySearchByAmount: the vector must be sorted first.)

static void mergeVector(std::vector<Expense>& v, int l, int m, int r) {
    std::vector<Expense> left(v.begin() + l, v.begin() + m + 1);
    std::vector<Expense> right(v.begin() + m + 1, v.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < (int)left.size() && j < (int)right.size()) {
        if (left[i].amount <= right[j].amount) v[k++] = left[i++];
        else v[k++] = right[j++];
    }
    while (i < (int)left.size()) v[k++] = left[i++];
    while (j < (int)right.size()) v[k++] = right[j++];
}

static void mergeSortVector(std::vector<Expense>& v, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortVector(v, l, m);
        mergeSortVector(v, m + 1, r);
        mergeVector(v, l, m, r);
    }
}

static int binarySearchAmount(const std::vector<Expense>& v, double amount) {
    int l = 0, r = (int)v.size() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (v[m].amount == amount) return m;
        if (v[m].amount < amount) l = m + 1;
        else r = m - 1;
    }
    return -1;  // not found
}

// ---------- tracker ----------

ExpenseTracker::ExpenseTracker() : nextId(1) {}

void ExpenseTracker::addExpense(const std::string& date, const std::string& category,
                                const std::string& description, double amount) {
    Expense e(nextId++, date, category, description, amount);
    list.insertAtEnd(e);
    undoStack.push({Action::ADD, e});
    std::cout << "Expense added with ID " << e.id << ".\n";
}

void ExpenseTracker::addLoadedExpense(const Expense& e) {
    list.insertAtEnd(e);  // no undo entry, no new id — loading only
}

bool ExpenseTracker::deleteExpense(int id) {
    Node* node = list.findById(id);
    if (!node) return false;
    Expense e = node->data;
    if (list.removeById(id)) {
        undoStack.push({Action::DELETE, e});
        return true;
    }
    return false;
}

void ExpenseTracker::undo() {
    if (undoStack.empty()) {
        std::cout << "Nothing to undo.\n";
        return;
    }
    Action a = undoStack.top();
    undoStack.pop();

    if (a.type == Action::ADD) {
        list.removeById(a.expense.id);
        std::cout << "Undid add: removed expense #" << a.expense.id << ".\n";
    } else {
        // Deleted expense comes back (at the end of the list).
        list.insertAtEnd(a.expense);
        std::cout << "Undid delete: restored expense #" << a.expense.id << ".\n";
    }
}

void ExpenseTracker::searchCategory(const std::string& category) const {
    list.searchByCategory(category);
}

void ExpenseTracker::searchAmountRange(double low, double high) const {
    list.searchByAmountRange(low, high);
}

void ExpenseTracker::binarySearchByAmount(double amount) const {
    std::vector<Expense> v = list.toVector();
    if (v.empty()) {
        std::cout << "No expenses recorded yet.\n";
        return;
    }
    mergeSortVector(v, 0, (int)v.size() - 1);  // must sort before binary search
    int idx = binarySearchAmount(v, amount);
    if (idx == -1) {
        std::cout << "No expense of exactly Rs " << std::fixed << std::setprecision(2)
                  << amount << " found.\n";
        return;
    }
    std::cout << "Found: #" << v[idx].id << " | " << v[idx].date << " | "
              << v[idx].category << " | " << v[idx].description << " | Rs "
              << std::fixed << std::setprecision(2) << v[idx].amount << "\n";
}

void ExpenseTracker::categoryTotals() const {
    // Hashing: one pass, category -> (total, count).
    std::unordered_map<std::string, double> totals;
    std::unordered_map<std::string, int> counts;
    for (const Expense& e : list.toVector()) {
        totals[e.category] += e.amount;
        counts[e.category]++;
    }
    if (totals.empty()) {
        std::cout << "No expenses recorded yet.\n";
        return;
    }
    std::cout << std::left << std::setw(15) << "Category"
              << std::setw(10) << "Count"
              << std::setw(15) << "Total (Rs)" << '\n';
    std::cout << std::string(40, '-') << '\n';
    for (const auto& pair : totals) {
        std::cout << std::left << std::setw(15) << pair.first
                  << std::setw(10) << counts[pair.first]
                  << std::fixed << std::setprecision(2)
                  << std::setw(15) << pair.second << '\n';
    }
}

void ExpenseTracker::monthlySummary(const std::string& month) const {
    double total = 0.0;
    int count = 0;
    for (const Expense& e : list.toVector()) {
        // Date starts with "YYYY-MM" -> belongs to this month.
        if (e.date.rfind(month, 0) == 0) {
            total += e.amount;
            count++;
        }
    }
    if (count == 0) {
        std::cout << "No expenses found for " << month << ".\n";
        return;
    }
    std::cout << "Month: " << month << " | Expenses: " << count
              << " | Total spent: Rs " << std::fixed << std::setprecision(2)
              << total << '\n';
}
