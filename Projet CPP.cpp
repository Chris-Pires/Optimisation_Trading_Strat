#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <chrono>
#include <stdexcept>

// Définition d'un enum pour les types de transactions
enum class TransactionType { BUY, SELL, SELL_SHORT };

// Fonction pour convertir une chaîne en TransactionType
TransactionType stringToTransactionType(const std::string& type) {
    if (type == "BUY") return TransactionType::BUY;
    if (type == "SELL") return TransactionType::SELL;
    if (type == "SELL SHORT") return TransactionType::SELL_SHORT;

    // Si le type de transaction n'est pas reconnu, lancez une exception.
    throw std::invalid_argument("Unknown transaction type: " + type);
}

// Définition de la classe Transaction
// Définition de la classe Transaction
class Transaction {
private:
    std::string seller;
    std::string buyer;
    int quantity;
    double unitPrice;
    TransactionType type;

public:
    Transaction(const std::string& seller, const std::string& buyer, int quantity, double unitPrice, TransactionType type)
        : seller(seller), buyer(buyer), quantity(quantity), unitPrice(unitPrice), type(type) {}

    double getTotalPrice() const {
        return quantity * unitPrice;
    }

    TransactionType getType() const {
        return type;
    }

    // Ajout des getters
    const std::string& getSeller() const {
        return seller;
    }

    const std::string& getBuyer() const {
        return buyer;
    }

    int getQuantity() const {
        return quantity;
    }

    double getUnitPrice() const {
        return unitPrice;
    }
};


// Classe pour lire les transactions à partir d'un fichier CSV
class TransactionReader {
private:
    Transaction parseLine(const std::string& line, int lineNumber) {
        std::istringstream iss(line);
        std::string seller, buyer, typeStr;
        int quantity;
        double unitPrice;
        char delimiter;

        if (!(std::getline(iss, seller, ',') &&
            std::getline(iss, buyer, ',') &&
            (iss >> quantity >> delimiter && delimiter == ',') &&
            (iss >> unitPrice >> delimiter && delimiter == ',') &&
            std::getline(iss, typeStr))) {
            throw std::runtime_error("Parsing error at line " + std::to_string(lineNumber) + ": cannot extract fields correctly.");
        }

        if (quantity <= 0) {
            throw std::runtime_error("Invalid quantity at line " + std::to_string(lineNumber) + ": quantity must be positive.");
        }

        if (unitPrice < 0.0) {
            throw std::runtime_error("Invalid unit price at line " + std::to_string(lineNumber) + ": unit price cannot be negative.");
        }

        try {
            return Transaction(seller, buyer, quantity, unitPrice, stringToTransactionType(typeStr));
        }
        catch (const std::invalid_argument& e) {
            // Catch and rethrow with line number information
            throw std::runtime_error("Invalid transaction type at line " + std::to_string(lineNumber) + ": " + e.what());
        }
    }

public:
    std::vector<Transaction> readTransactions(const std::string& filename) {
        std::vector<Transaction> transactions;
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename);
        }

        std::string line;
        int lineNumber = 0;
        // Ignorer la première ligne qui contient les en-têtes
        std::getline(file, line);

        // Lire et traiter les lignes suivantes comme des transactions
        while (getline(file, line)) {
            lineNumber++;
            try {
                transactions.push_back(parseLine(line, lineNumber));
            }
            catch (const std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
                // Décidez si vous voulez continuer avec l'erreur ou arrêter
                // throw; // Pour arrêter
            }
        }

        return transactions;
    }
};




// Classe pour traiter les transactions et conserver des statistiques
class TransactionResolver {
private:
    std::map<TransactionType, int> transactionCounts;
    double totalAmount = 0;

public:
    void processTransaction(const Transaction& transaction) {
        transactionCounts[transaction.getType()]++;
        if (transaction.getType() == TransactionType::BUY || transaction.getType() == TransactionType::SELL) {
            totalAmount += transaction.getTotalPrice();
        }
    }

    void displayStats() const {
        std::cout << "Statistiques des Transactions:\n";
        for (const auto& pair : transactionCounts) {
            std::string typeStr = (pair.first == TransactionType::BUY) ? "BUY" : (pair.first == TransactionType::SELL) ? "SELL" : "SELL SHORT";
            std::cout << typeStr << ": " << pair.second << std::endl;
        }
        std::cout << "Montant total: " << totalAmount << std::endl;
    }
};


// Fonctions pour encapsuler les étapes de traitement

std::vector<Transaction> loadTransactions(const std::string& filename) {
    TransactionReader reader;
    return reader.readTransactions(filename);
}

void processAllTransactions(const std::vector<Transaction>& transactions, TransactionResolver& resolver) {
    for (const auto& transaction : transactions) {
        resolver.processTransaction(transaction);
    }
}

void printElapsedTime(const std::chrono::high_resolution_clock::time_point& start,
    const std::chrono::high_resolution_clock::time_point& end,
    const std::string& message) {
    std::chrono::duration<double> elapsed = end - start;
    std::cout << message << elapsed.count() << "s\n";
}


// Classe utilitaire pour mesurer et afficher le temps écoulé
class Timer {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    Timer(const std::string& name) : name(name), start(Clock::now()) {}

    ~Timer() {
        auto end = Clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << name << ": " << elapsed.count() << "s\n";
    }

private:
    std::string name;
    TimePoint start;
};


// Fonction principale
int main() {
    try {
        std::vector<Transaction> transactions; // Déclarez la variable dans la portée appropriée
        {
            Timer timer("Temps de lecture des transactions");
            transactions = loadTransactions("C:\Users\Chris\Desktop\Projet_CPPPPPP\transactions_csv.csv");

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