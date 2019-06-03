#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>

class matrix {
private:
    int mHeight;
    int mWidth;
    std::vector<double> mData;

    void checkForValidHeight(int) const;
    void checkForValidWidth(int) const;
    void checkForMatchingDimensions(int, int) const;
    
public:
    matrix(int, int);
    matrix(int, int, const std::vector<double>&);
    int getHeight() const;
    int getWidth() const;
    double get(int, int) const;
    void set(int, int, double);
    matrix plus(const matrix&) const;
    matrix minus(const matrix&) const;
    matrix times(const matrix&) const;
    static matrix loadAdjacencyMatrix(std::istream&);
    static matrix loadEdgeList(std::istream&);
    friend std::ostream& operator<<(std::ostream&, const matrix&);
};
