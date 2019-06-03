//Kenneth Shipley
// CSCI 422
//Programming Assignment 06
#include <iostream>
#include <vector>
#include <string>
#include "matrix.h"

struct product {
    std::string name;
    int weight;
    int value;
};

void getWeightLimitAndNumberOfProducts(std::istream&, int&, int&);
std::vector<product> readInAllProducts(std::istream&, int);
void displayAllCollectedItems(std::ostream&, const std::vector<product>&);
int computeTotalWeight(const std::vector<product>&);
int computeTotalValue(const std::vector<product>&);
std::vector<product> collectItemsIntoKnapsack(int, const std::vector<product>&);

int main() {
    int weightLimitOfKnapsack;
    int numProducts;
    std::vector<product> knapsack;

    getWeightLimitAndNumberOfProducts(std::cin, weightLimitOfKnapsack, numProducts);
    std::vector<product> products = readInAllProducts(std::cin, numProducts);

    knapsack = collectItemsIntoKnapsack(weightLimitOfKnapsack, products);

    displayAllCollectedItems(std::cout, knapsack);
    return 0;
}

void getWeightLimitAndNumberOfProducts(std::istream& in, int& weightLimitOfKnapsack, int& numProducts) {
    in >> weightLimitOfKnapsack;
    in >> numProducts;
}

std::vector<product> readInAllProducts(std::istream& in, int numProducts) {
    std::vector<product> products;

    for (int i = 0; i < numProducts; i++) {
        std::string name;
        int weight;
        int value;

        in >> name;
        in >> weight;
        in >> value;

        product p;
        p.name = name;
        p.weight = weight;
        p.value = value;

        products.push_back(p);
    }

    return products;
}

void displayAllCollectedItems(std::ostream& out, const std::vector<product>& knapsack) {
    for (int i = 0; i < knapsack.size(); i++) {
        out << "Name: " << knapsack[i].name
            << " Weight: " << knapsack[i].weight
            << " Value: " << knapsack[i].value << std::endl;
    }

    out << "Total Weight: " << computeTotalWeight(knapsack) << std::endl;
    out << "Total Value : " << computeTotalValue(knapsack) << std::endl;
}

int computeTotalWeight(const std::vector<product>& products) {
    int totalWeight = 0;
    for (int i = 0; i < products.size(); i++) {
        totalWeight += products[i].weight;
    }
    return totalWeight;
}

int computeTotalValue(const std::vector<product>& products) {
    int totalValue = 0;
    for (int i = 0; i < products.size(); i++) {
        totalValue += products[i].value;
    }
    return totalValue;
}

std::vector<product> collectItemsIntoKnapsack(int weightLimitOfKnapsack, const std::vector<product>& products) {
    std::vector<product> knapsack;
    
    // num_products is the number of products.
    // weight is the weight limit of the knapsack
    // Create a matrix K that is the size of (num_products+1, weights+1)
    matrix K(products.size()+1,weightLimitOfKnapsack+1);

    // For each cell in the matrix K, do the following:
    // i is 0 to num_products
    // w is 0 to weight
    //     if i is 0 or w is 0
    //         Write 0 to K(i,w)
    //     if products[i-1] can fit in the bag
    //     (the weight is less than or equal to w)
    //         Then find the following:
    //         A: the value at K(i-1,w)
    //         B: the value of product[i-1] plus K(i-1,w - products[i-1]'s weight)
    //         Write the larger of the two values (A or B) to K(i,w)
    //     if products[i-1] can't fit in the bag, then
    //         Write K(i-1,w) to K(i-1,w)

    
    for(int i = 0; i <= products.size(); i++){
        for(int w = 0; w <= weightLimitOfKnapsack; w++){
            if(i == 0 || w == 0){
                K.set(i, w, 0);
            }
            else if(products[i-1].weight <= w){
                double value = K.get(i-1,w);
                double otherValue = products[i-1].value + K.get(i-1, w-products[i-1].weight);
                double larger = value;
                if(otherValue > larger){
                    larger = otherValue;
                }
                K.set(i,w,larger);
            }
            else{
                K.set(i,w,K.get(i-1,w));
            }
        }

    }

    // We need to get the values back out of the K matrix.
    // Set i to num_products
    // Set w to weight
    // while K(i,w) does not equal 0
    //     while K(i-1,w) equals K(i,w) // Move Up
    //         set i to i-1
    //     put the element at products[i-1] into the knapsack
    //     Set v to be the K(i,w) - product[i-1]'s value
    //     while K(i,w) does not equal v // Move Left
    //         set w to w-1
    //     

    int i = products.size();
    int w = weightLimitOfKnapsack;

    while(K.get(i,w) != 0){
        int v = 0;
        while(K.get(i-1,w) == K.get(i,w)){
            i = i - 1;
        }
        knapsack.push_back(products[i-1]);
        v = K.get(i,w) - products[i-1].value;
        while(K.get(i,w) != v){
            w = w - 1;
        }
    }

    return knapsack;
}
