// Transaction.cpp
#include "Transaction.h"

Transaction::Transaction(const std::string& seller, const std::string& buyer, int quantity, double unitPrice, TransactionType type)
    : seller(seller), buyer(buyer), quantity(quantity), unitPrice(unitPrice), type(type) {}

double Transaction::getTotalPrice() const {
    return quantity * unitPrice;
}

TransactionType Transaction::getType() const {
    return type;
}

const std::string& Transaction::getSeller() const {
    return seller;
}

const std::string& Transaction::getBuyer() const {
    return buyer;
}

int Transaction::getQuantity() const {
    return quantity;
}

double Transaction::getUnitPrice() const {
    return unitPrice;
}
