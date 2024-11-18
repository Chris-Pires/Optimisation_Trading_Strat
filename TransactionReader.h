// TransactionReader.h
#pragma once

#include <string>
#include <vector>
#include "Transaction.h"

class TransactionReader {
private:
    Transaction parseLine(const std::string& line, int lineNumber);

public:
    std::vector<Transaction> readTransactions(const std::string& filename);
};
