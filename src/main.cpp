// main.cpp
// Expense Tracker — menu-driven console app.
//
// Build:  g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o expense-tracker
// Run:    ./expense-tracker
//
// Data is saved automatically to expenses.txt after every change
// (add / delete / undo) and loaded again at startup.

#include "ExpenseTracker.h"
#include "FileHandler.h"
#include <iostream>
#include <limits>
#include <string>

static void showMenu() {
    std::cout << "\n========== EXPENSE TRACKER ==========\n";
    std::cout << "1. Add Expense\n";
    std::cout << "2. View All Expenses\n";
    std::cout << "3. Search by Category\n";
    std::cout << "4. Search by Amount Range\n";
    std::cout << "5. Search by Exact Amount (Binary Search)\n";
    std::cout << "6. Sort by Amount (Low to High)\n";
    std::cout << "7. Sort by Date (Oldest First)\n";
    std::cout << "8. Delete Expense\n";
    std::cout << "9. Undo Last Action\n";
    std::cout << "10. Category-wise Total\n";
    std::cout << "11. Monthly Summary\n";
    std::cout << "12. Exit\n";
    std::cout << "=====================================\n";
}

// Read an int safely: wrong input does not crash the program.
static int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Invalid input. Please enter a number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

static double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (value < 0) {
                std::cout << "Amount cannot be negative.\n";
                continue;
            }
            return value;
        }
        std::cout << "Invalid input. Please enter a number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

// Very basic date check: YYYY-MM-DD (10 chars, dashes at 4 and 7).
static bool validDate(const std::string& d) {
    return d.size() == 10 && d[4] == '-' && d[7] == '-';
}

int main() {
    const std::string filename = "expenses.txt";
    ExpenseTracker tracker;

    if (FileHandler::loadFromFile(tracker, filename)) {
        std::cout << "Loaded " << tracker.getCount()
                  << " expense(s) from " << filename << ".\n";
    } else {
        std::cout << "No saved data found. Starting fresh.\n";
    }

    while (true) {
        showMenu();
        int choice = readInt("Enter your choice: ");

        if (choice == 1) {
            std::string date;
            do {
                date = readLine("Date (YYYY-MM-DD): ");
                if (!validDate(date)) {
                    std::cout << "Wrong format. Use YYYY-MM-DD, e.g. 2026-11-26.\n";
                }
            } while (!validDate(date));
            std::string category = readLine("Category (Food/Transport/Books/...): ");
            std::string description = readLine("Description: ");
            double amount = readDouble("Amount (Rs): ");
            tracker.addExpense(date, category, description, amount);
            FileHandler::saveToFile(tracker, filename);
        } else if (choice == 2) {
            tracker.viewAll();
        } else if (choice == 3) {
            std::string category = readLine("Enter category to search: ");
            tracker.searchCategory(category);
        } else if (choice == 4) {
            double low = readDouble("Minimum amount (Rs): ");
            double high = readDouble("Maximum amount (Rs): ");
            tracker.searchAmountRange(low, high);
        } else if (choice == 5) {
            double amount = readDouble("Enter exact amount (Rs): ");
            tracker.binarySearchByAmount(amount);
        } else if (choice == 6) {
            tracker.sortByAmount();
            std::cout << "Sorted by amount (low to high).\n";
            tracker.viewAll();
        } else if (choice == 7) {
            tracker.sortByDate();
            std::cout << "Sorted by date (oldest first).\n";
            tracker.viewAll();
        } else if (choice == 8) {
            int id = readInt("Enter expense ID to delete: ");
            if (tracker.deleteExpense(id)) {
                std::cout << "Expense #" << id << " deleted.\n";
                FileHandler::saveToFile(tracker, filename);
            } else {
                std::cout << "No expense found with ID " << id << ".\n";
            }
        } else if (choice == 9) {
            tracker.undo();
            FileHandler::saveToFile(tracker, filename);
        } else if (choice == 10) {
            tracker.categoryTotals();
        } else if (choice == 11) {
            std::string month = readLine("Enter month (YYYY-MM): ");
            tracker.monthlySummary(month);
        } else if (choice == 12) {
            FileHandler::saveToFile(tracker, filename);
            std::cout << "Data saved. Goodbye!\n";
            break;
        } else {
            std::cout << "Invalid choice. Please enter 1-12.\n";
        }
    }
    return 0;
}
