// FileHandler.h
// Save / load expenses from a plain text file (fstream — core C++,
// no external library, no database).
//
// File format (one expense per line, fields separated by '|'):
//   id|date|category|description|amount
// Example:
//   3|2026-11-03|Books|C++ programming book|1200

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "ExpenseTracker.h"
#include <string>

namespace FileHandler {
    // Save all expenses. Returns false if the file cannot be opened.
    bool saveToFile(const ExpenseTracker& tracker, const std::string& filename);

    // Load expenses (clears nothing — call on a fresh tracker).
    // Returns false if the file does not exist yet (first run: normal).
    bool loadFromFile(ExpenseTracker& tracker, const std::string& filename);
}

#endif
