// Kenneth Shipley
// CSCI 4270
// Assignment 09

#include <iostream>
#include <vector>
#include "matrix.cpp"

std::vector<int> getVerticesOfMaxClique(const matrix&, const std::vector<int>&, int);
void displayVertices(std::ostream&, const std::vector<int>&);
void searchCombinationsForClique(std::vector<int> edges, matrix old, int offset, int k, int combNum, 
                                std::vector<int>& combination, bool& quitIfFound, std::vector<bool>& combinationCliques, 
                                std::vector<int>& nodesOfClique);
bool checkForClique(matrix small);
void createSmallMatrix(matrix old, matrix& small, std::vector<int> combinationVec);

int main() {
    matrix A = matrix::loadEdgeList(std::cin);

    std::cout << A << std::endl;

    std::vector<int> clique;
    std::vector<int> maxClique = getVerticesOfMaxClique(A, clique, 0);

    std::cout << "Vertices in Max Clique:" << std::endl;
    displayVertices(std::cout, maxClique);

    std::cin.ignore();
    std::cin.get();
    return 0;
}

void displayVertices(std::ostream& out, const std::vector<int>& vertices) {
    for (int i = 0; i < vertices.size(); i++) {
        out << vertices.at(i) << " ";
    }
    out << std::endl;
}

// Get vetices of max clique using a method similar to binary search
std::vector<int> getVerticesOfMaxClique(const matrix& m, const std::vector<int>& clique, int index) {
    int left = index + 1;
    int right = m.getHeight();
    int pivot = left + (right-left)/2;

    // Fill a list with indexes of graph
    std::vector<int> edges;
    for(int i = 0; i < m.getHeight(); i++){
        edges.push_back(i);
    }

    // Principle of true unless found otherwise
    bool foundClique = true;

    // Keep track of max clique
    int maxCliqueSize = 0;
    std::vector<int> maxClique;

    // Temporary data storage for getting combinations
    std::vector<int> tempData;

    while(left != right){
        // boolean to stop searching combinations if a clique is found
        bool quitIfFound = false;
        // Yes
        std::vector<bool> combinationCliques;
        // Storage for nodes in a clique if one is found
        std::vector<int> nodesOfClique;
        // Search combinations for a clique
        searchCombinationsForClique(edges, m, 0, pivot, pivot, tempData, quitIfFound, combinationCliques, nodesOfClique);
        // Since the function will stop if a clique is found, the last element of the bool vector will determine 
        // if there was a clique within the combinations
        foundClique = combinationCliques[combinationCliques.size()-1];
        // If the clique is larger than the max, update it
        if(nodesOfClique.size() > maxCliqueSize){
            maxCliqueSize = nodesOfClique.size();
            maxClique.clear();
            maxClique = nodesOfClique;
        }
        // If clique is found, check upper bound
        if(foundClique == true){
            left = pivot + 1;
            pivot = left + (right-left)/2;
        }
        // If no clique is found, check upper bound
        else{
            right = pivot;
            pivot = left + (right-left)/2;
            foundClique = true;
        }
    }
    return maxClique;
}

// Edges vector is a list of edge numbers
// Old matrix is original matrix
// k is number of nodes per combination
// combNum is size of new smaller matrix that will be made
// Combination vector is a temporary storage to hold combinations
// quitIfFound bool will quit searching for combinations if a clique is found
// boolCliques vector will store the bools for whether or not a clique is found
// nodesOfCLique vector will store the nodes of a clique if one is found
void searchCombinationsForClique(std::vector<int> edges, matrix old, int offset, int k, int combNum, 
                                std::vector<int>& combination, bool& quitIfFound, std::vector<bool>& boolCliques, 
                                std::vector<int>& nodesOfClique) {
    if (k == 0 && !quitIfFound) {
        matrix small(combNum, combNum);
        createSmallMatrix(old, small, combination);\
        bool foundClique = checkForClique(small);
        quitIfFound = foundClique;
        boolCliques.push_back(foundClique);
        if(foundClique == 1){
            for(int i = 0; i < combination.size(); i++){
                nodesOfClique.push_back(combination[i]);
            }
        }
        return;
    }

    for (int i = offset; i <= edges.size() - k; ++i) {
        if(!quitIfFound){
            combination.push_back(edges[i]);
            searchCombinationsForClique(edges, old, i+1, k-1, combNum, combination, quitIfFound, boolCliques, nodesOfClique);
            combination.pop_back();
        }
    }
}


// Checks to see if a set of nodes has a clique
bool checkForClique(matrix small){

bool foundClique = true;
for(int i = 0; i < small.getHeight(); i++){
    for(int j = i; j < small.getHeight(); j++){
        if(i != j && small.get(i,j) == 0){
            foundClique = false;
            break;
        }
    }
    if(foundClique == false){
        break;
    }
}

return foundClique;
}


// Creates a smaller matrix based on a subset of nodes from a larger matrix;
void createSmallMatrix(matrix old, matrix& small, std::vector<int> combinationVec){

for(int i = 0; i < small.getHeight(); i++){
    for(int j = 0; j < small.getHeight(); j++){
        small.set(i, j, old.get(combinationVec[i],combinationVec[j]));
    }
}
}