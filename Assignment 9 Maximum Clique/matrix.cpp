#include "matrix.h"

void matrix::checkForValidHeight(int height) const {
    if (height < 1) {
        throw std::logic_error("height must be at least 1");
    }
}

void matrix::checkForValidWidth(int width) const {
    if (width < 1) {
        throw std::logic_error("width must be at least 1");
    }
}

void matrix::checkForMatchingDimensions(int height, int width) const {
    if (mHeight != height) {
        throw std::logic_error("Height of left matrix does not equal height of right.");
    }

    if (mWidth != width) {
        throw std::logic_error("Width of left matrix does not equal width of right.");
    }
}

int matrix::getHeight() const {
    return mHeight;
}

int matrix::getWidth() const {
    return mWidth;
}

double matrix::get(int y, int x) const {
    return mData[y * mWidth + x];
}

void matrix::set(int y, int x, double value) {
    mData[y * mWidth + x] = value;
}

matrix::matrix(int height, int width) : mData(height * width) {
    checkForValidHeight(height);
    checkForValidWidth(width);

    mHeight = height;
    mWidth = width;
}

matrix::matrix(int height, int width, const std::vector<double>& data) {
    checkForValidHeight(height);
    checkForValidWidth(width);

    if (height * width != data.size()) {
        throw std::logic_error("height * width must equal the length of the data vector");
    }

    mHeight = height;
    mWidth = width;
    mData = data;
}

matrix matrix::plus(const matrix& right) const {
    checkForMatchingDimensions(right.mHeight, right.mWidth);
    matrix sum(mHeight, mWidth);

    for (int i = 0; i < mHeight; i++) {
        for (int j = 0; j < mWidth; j++) {
            sum.set(i, j, get(i, j) + right.get(i, j));
        }
    }

    return sum;
}

matrix matrix::minus(const matrix& right) const {
    checkForMatchingDimensions(right.mHeight, right.mWidth);
    matrix difference(mHeight, mWidth);
    
    for (int i = 0; i < mHeight; i++) {
        for (int j = 0; j < mWidth; j++) {
            difference.set(i, j, get(i, j) - right.get(i, j));
        }
    }

    return difference;
}

matrix matrix::times(const matrix& right) const {
    if (mWidth != right.mHeight) {
        throw std::logic_error("Width of left matrix does not equal height of right.");
    }

    int nHeight = mHeight;
    int nWidth = right.mWidth;

    std::vector<double> values(nHeight * nWidth);  

    for (int i = 0; i < nHeight; i++) {
        for (int j = 0; j < nWidth; j++) {
            double sum = 0;
            for (int k = 0; k < mWidth; k++) {
                sum += get(i, k) * right.get(k, j);
            }

            values[i * nWidth + j] = sum;
        }
    }

    matrix mul(nHeight, nWidth, values);
    return mul;
}

matrix matrix::loadAdjacencyMatrix(std::istream& in) {
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

matrix matrix::loadEdgeList(std::istream& in) {
    int vertices;
    int edges;
    int a;
    int b;
    double value;

    in >> vertices >> edges;
    matrix m(vertices, vertices);

    for (int i = 0; i < edges; i++) {
        in >> a >> b >> value;
        m.set(a, b, value);
    }

    return m;
}

std::ostream& operator<<(std::ostream& os, const matrix& obj) {
    os << std::setprecision(5);

    for (int i = 0; i < obj.getHeight(); i++) {

        if (i == 0) {
            os << " [ [";
        }
        else {
            os << "   [";
        }

        for (int j = 0; j < obj.getWidth(); j++) {
            os << " " << std::setw(6) << obj.get(i,j);
        }

        if (i+1 == obj.getHeight()) {
            os << " ] ] "
               << obj.getHeight() << "x" << obj.getWidth()
               << std::endl << std::endl;
        }
        else {
            os << " ]" << std::endl;
        }
    }

    return os;
}
