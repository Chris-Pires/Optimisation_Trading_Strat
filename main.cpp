// main.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include "Transaction.h"
#include "TransactionReader.h"
#include "TransactionResolver.h"

void printElapsedTime(const std::chrono::high_resolution_clock::time_point& start,
    const std::chrono::high_resolution_clock::time_point& end,
    const std::string& message);

int main() {
    try {
        std::vector<Transaction> transactions;
        {
            Timer timer("Temps de lecture des transactions");
            TransactionReader reader;
            transactions = reader.readTransactions("C:\\Users\\Chris\\Desktop\\Projet_CPP\\transactions_csv.csv");
        }

        TransactionResolver resolver;
        {
            Timer timer("Temps de traitement des transactions");
            processAllTransactions(transactions, resolver);
        }

        resolver.displayStats();
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
