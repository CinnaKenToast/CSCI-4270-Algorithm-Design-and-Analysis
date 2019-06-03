// Kenneth Shipley
// CSCI 4270
// Assignment 3
// Multiplies two matricies together
#include <iostream>
#include <vector>
#include "matrix.h"

matrix read_matrix(std::istream&);

int main() {
    matrix A = read_matrix(std::cin);
    matrix B = read_matrix(std::cin);

    // To finish your homework assignment,
    // This line must be changed from "timesTraditional"
    // to "timesFast" and still produce the same output.
    std::cout << A << B << A.timesFast(B);

    std::cin.ignore();
    std::cin.get();
    return 0;
}

matrix read_matrix(std::istream& in) {
    int height;
    int width;
    std::vector<double> data;
    double value;

    in >> height >> width;

    for (int i = 0; i < height * width; i++) {
        in >> value;
        data.push_back(value);
    }

    matrix m(height, width, data);
    return m;
}
