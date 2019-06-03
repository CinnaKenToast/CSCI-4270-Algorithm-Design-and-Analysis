// Kenneth Shipley
// CSCI 4270
// Programming Assignment 05 
// Solves the classic 0-1 Knapsack problem using a brute force method
// Runtime complexity of O(2^n)

#include <iostream>
#include <vector>
#include <string>

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
std::vector<product> collectItemsIntoKnapsack(int, std::vector<product>, const std::vector<product>&, int);
std::vector<product> copyKnapsack(std::vector<product>);

int main() {
    int weightLimitOfKnapsack;
    int numProducts;
    std::vector<product> knapsack;

    getWeightLimitAndNumberOfProducts(std::cin, weightLimitOfKnapsack, numProducts);
    std::vector<product> products = readInAllProducts(std::cin, numProducts);

    knapsack = collectItemsIntoKnapsack(weightLimitOfKnapsack, knapsack, products, 0);

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

// Display the items in the knapsack
void displayAllCollectedItems(std::ostream& out, const std::vector<product>& knapsack) {
    std::cout << std::endl << "Stolen Items: " << std::endl;
    std::cout << "------------" << std::endl;

    for(int i = 0; i < knapsack.size(); i ++){
        std::cout << "Name: " << knapsack[i].name;
        std::cout << " | Weight: " << knapsack[i].weight;
        std::cout << " | Value: " << knapsack[i].value << std::endl;
    }
    std::cout << std::endl << "Total Weight: " << computeTotalWeight(knapsack) << std::endl;
    std::cout << "Total Value: " << computeTotalValue(knapsack) << std::endl;
    
}

// Compute the total weight of the items in this vector of products
int computeTotalWeight(const std::vector<product>& products) {
    int weight = 0;
    for(int i = 0; i < products.size(); i++){
        weight += products[i].weight;
    }
    return weight;
}

// Compute the total value of the items in this vector of products
int computeTotalValue(const std::vector<product>& products) {
    int value = 0;
    for(int i = 0; i < products.size(); i++){
        value += products[i].value;
    }
    return value;
}

// Copies one knapsack to another
std::vector<product> copyKnapsack(std::vector<product> knapsack){
    std::vector<product> newKnapsack;
    
    for(int i = 0; i < knapsack.size(); i++){
        product prod;
        prod.name = knapsack[i].name;
        prod.weight = knapsack[i].weight;
        prod.value = knapsack[i].value;
        newKnapsack.push_back(prod);
    }
    return newKnapsack;
}

std::vector<product> collectItemsIntoKnapsack(int weightLimitOfKnapsack, std::vector<product> knapsack, const std::vector<product>& products, int index) {

    // If the index is greater than or equal to the size of the products,
    // then return knapsack
    if(index >= products.size()){
        return knapsack; 
    }
    else{
        // Get a version of knapsack in which you do not pick up the item at position index.
        std::vector<product> rejectKnapsack = copyKnapsack(knapsack);
        std::vector<product> grabKnapsack = copyKnapsack(knapsack);
        rejectKnapsack = collectItemsIntoKnapsack(weightLimitOfKnapsack, knapsack, products, index + 1);


        // If you are able to add the item at the position index to the knapsack, then
        // ... Add that item to the knapsack
        // ... Get a version of the knapsack in which you do pick up the item at position index.

        if(products[index].weight <= weightLimitOfKnapsack){
            knapsack.push_back(products[index]);
            grabKnapsack = collectItemsIntoKnapsack(weightLimitOfKnapsack - products[index].weight, knapsack, products, index + 1);
        }
        // Compare your two knapsacks.
        // Return the knapsack with the larger value.

        if(computeTotalValue(rejectKnapsack) > computeTotalValue(grabKnapsack)){
            return rejectKnapsack;
        }
        else{
            return grabKnapsack;
        }
    }
}
