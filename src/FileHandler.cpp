// FileHandler.cpp
// Plain-text persistence with fstream.

#include "FileHandler.h"
#include <fstream>
#include <sstream>

bool FileHandler::saveToFile(const ExpenseTracker& tracker, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) return false;
    for (const Expense& e : tracker.getList().toVector()) {
        out << e.id << '|' << e.date << '|' << e.category << '|'
            << e.description << '|' << e.amount << '\n';
    }
    return true;
}

bool FileHandler::loadFromFile(ExpenseTracker& tracker, const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;  // no file yet — first run, start empty

    std::string line;
    int maxId = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idStr, date, category, description, amountStr;
        std::getline(ss, idStr, '|');
        std::getline(ss, date, '|');
        std::getline(ss, category, '|');
        std::getline(ss, description, '|');
        std::getline(ss, amountStr, '|');

        try {
            int id = std::stoi(idStr);
            double amount = std::stod(amountStr);
            tracker.addLoadedExpense(Expense(id, date, category, description, amount));
            if (id > maxId) maxId = id;
        } catch (...) {
            // Malformed line — skip it and keep loading the rest.
            continue;
        }
    }
    tracker.setNextId(maxId + 1);
    return true;
}
