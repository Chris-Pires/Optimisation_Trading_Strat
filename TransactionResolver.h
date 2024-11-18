// TransactionResolver.h
#pragma once

#include <map>
#include <iostream>
#include "Transaction.h"

class TransactionResolver {
private:
    std::map<TransactionType, int> transactionCounts;
    double totalAmount;

public:
    TransactionResolver();

    void processTransaction(const Transaction& transaction);
    void displayStats() const;
};
