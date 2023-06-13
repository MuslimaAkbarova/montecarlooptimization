#include <iostream>
#include <vector>
#include <random>

struct Stock {
    std::string symbol;
    double expectedReturn;
    double volatility;
};

std::vector<double> calculateOptimizedWeightsMC(const std::vector<Stock>& portfolio, double riskFreeRate, double targetReturn, int numSimulations) {
    size_t numAssets = portfolio.size();
    std::vector<double> weights(numAssets, 0.0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dist(0.0, 1.0);

    std::vector<double> returns(numAssets, 0.0);
    double maxReturn = 0.0;

    for (int i = 0; i < numSimulations; ++i) {
        double totalWeight = 0.0;
        for (size_t j = 0; j < numAssets; ++j) {
            returns[j] = portfolio[j].expectedReturn + portfolio[j].volatility * dist(gen);
            totalWeight += returns[j];
        }

        if (totalWeight >= targetReturn && totalWeight > maxReturn) {
            maxReturn = totalWeight;
            for (size_t j = 0; j < numAssets; ++j) {
                weights[j] = returns[j];
            }
        }
    }

    double totalWeight = 0.0;
    for (size_t i = 0; i < numAssets; ++i) {
        weights[i] /= maxReturn;
        totalWeight += weights[i];
    }

    double adjustmentFactor = targetReturn / totalWeight;
    for (size_t i = 0; i < numAssets; ++i) {
        weights[i] *= adjustmentFactor;
    }

    return weights;
}

int main() {
    std::vector<Stock> portfolio = {
            {"AAPL", 0.08, 0.2},
            {"GOOG", 0.12, 0.3},
            {"MSFT", 0.1, 0.25},
            {"AMZN", 0.15, 0.35}
    };

    double riskFreeRate = 0.05;
    double targetReturn = 0.1;
    int numSimulations = 100000;

    std::vector<double> weights = calculateOptimizedWeightsMC(portfolio, riskFreeRate, targetReturn, numSimulations);

    std::cout << "Optimized Weights:\n";
    for (size_t i = 0; i < portfolio.size(); ++i) {
        std::cout << portfolio[i].symbol << ": " << weights[i] << "\n";
    }

    return 0;
}
