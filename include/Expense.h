// Expense.h
// One expense record + the linked-list node that holds it.
// Keep it simple: a struct is enough, no fancy OOP needed here.

#ifndef EXPENSE_H
#define EXPENSE_H

#include <string>

struct Expense {
    int id;                    // unique id, assigned automatically
    std::string date;          // YYYY-MM-DD  (e.g. 2026-11-26)
    std::string category;      // Food, Transport, Books, ...
    std::string description;   // short note, e.g. "University canteen lunch"
    double amount;             // amount in PKR

    Expense()
        : id(0), date(""), category(""), description(""), amount(0.0) {}

    Expense(int id, const std::string& date, const std::string& category,
            const std::string& description, double amount)
        : id(id), date(date), category(category), description(description), amount(amount) {}
};

// Singly linked list node: data + pointer to the next node.
struct Node {
    Expense data;
    Node* next;

    explicit Node(const Expense& e) : data(e), next(nullptr) {}
};

#endif
