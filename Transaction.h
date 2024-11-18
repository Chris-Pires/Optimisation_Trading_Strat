// Transaction.h
#pragma once

#include <string>

enum class TransactionType { BUY, SELL, SELL_SHORT };

class Transaction {
private:
    std::string seller;
    std::string buyer;
    int quantity;
    double unitPrice;
    TransactionType type;

public:
    Transaction(const std::string& seller, const std::string& buyer, int quantity, double unitPrice, TransactionType type);

    double getTotalPrice() const;
    TransactionType getType() const;
    const std::string& getSeller() const;
    const std::string& getBuyer() const;
    int getQuantity() const;
    double getUnitPrice() const;
};
