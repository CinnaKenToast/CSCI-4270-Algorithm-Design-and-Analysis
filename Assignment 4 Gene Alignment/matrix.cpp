// Kenneth Shipley
// CSCI 4270
// Assignment 3
// Multiplies two matricies together

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

matrix matrix::timesTraditional(const matrix& right) const {
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

matrix matrix::timesFast(const matrix& right) const {
    if (mWidth != right.mHeight) {
        throw std::logic_error("Width of left matrix does not equal height of right.");
    }

    int nHeight = mHeight;
    int nWidth = right.mWidth;

    matrix mul(nHeight, nWidth);

    strassen(nHeight, *this, right, mul);

    return mul;
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

void strassen(int n, const matrix& F, const matrix& S, matrix& X) {
    if (n == 2) {
        X = F.timesTraditional(S);
        return;
    }

    matrix aMatrix(n/2,n/2);
    matrix bMatrix(n/2,n/2);
    matrix cMatrix(n/2,n/2);
    matrix dMatrix(n/2,n/2);

// copy quadrants into individual matricies
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            // top left
            if(i < n/2 && j < n/2){
                aMatrix.set(i%(n/2),j%(n/2),F.get(i,j));
            }
            // top right
            if(i < n/2 && j >= n/2){
                bMatrix.set(i%(n/2),j%(n/2),F.get(i,j));
            }
            // bottom left
            if(i >= n/2 && j < n/2){
                cMatrix.set(i%(n/2),j%(n/2),F.get(i,j));
            }
            // bottom right
            if(i >= n/2 && j >= n/2){
                dMatrix.set(i%(n/2),j%(n/2),F.get(i,j));
            }
        }

    }

    /*
    std::cout << "a= " << std::endl << aMatrix;
    std::cout << "b= " << std::endl << bMatrix;
    std::cout << "c= " << std::endl << cMatrix;
    std::cout << "d= " << std::endl << dMatrix;
    */

    matrix aaMatrix(n/2,n/2);
    matrix bbMatrix(n/2,n/2);
    matrix ccMatrix(n/2,n/2);
    matrix ddMatrix(n/2,n/2);

// copy quadrants into individual matricies
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            // top left
            if(i < n/2 && j < n/2){
                aaMatrix.set(i%(n/2),j%(n/2),S.get(i,j));
            }
            // top right
            if(i < n/2 && j >= n/2){
                bbMatrix.set(i%(n/2),j%(n/2),S.get(i,j));
            }
            // bottom left
            if(i >= n/2 && j < n/2){
                ccMatrix.set(i%(n/2),j%(n/2),S.get(i,j));
            }
            // bottom right
            if(i >= n/2 && j >= n/2){
                ddMatrix.set(i%(n/2),j%(n/2),S.get(i,j));
            }
        }

    }

    /*
    std::cout << "aa= " << std::endl << aaMatrix;
    std::cout << "bb= " << std::endl << bbMatrix;
    std::cout << "cc= " << std::endl << ccMatrix;
    std::cout << "dd= " << std::endl << ddMatrix;
    */

    matrix M1(n/2,n/2);
    matrix M2(n/2,n/2);
    matrix M3(n/2,n/2);
    matrix M4(n/2,n/2);
    matrix M5(n/2,n/2);
    matrix M6(n/2,n/2);
    matrix M7(n/2,n/2);
   
// create M matricies    
    strassen(n/2,aMatrix.plus(dMatrix), aaMatrix.plus(ddMatrix),M1);
    strassen(n/2,cMatrix.plus(dMatrix), aaMatrix,M2);
    strassen(n/2,aMatrix, bbMatrix.minus(ddMatrix),M3);
    strassen(n/2,dMatrix, ccMatrix.minus(aaMatrix),M4);
    strassen(n/2,aMatrix.plus(bMatrix), ddMatrix,M5);
    strassen(n/2,cMatrix.minus(aMatrix), aaMatrix.plus(bbMatrix),M6);
    strassen(n/2,bMatrix.minus(dMatrix), ccMatrix.plus(ddMatrix),M7);

    /*
    std::cout << "M1= " << std::endl << M1;
    std::cout << "M2= " << std::endl << M2;
    std::cout << "M3= " << std::endl << M3;
    std::cout << "M4= " << std::endl << M4;
    std::cout << "M5= " << std::endl << M5;
    std::cout << "M6= " << std::endl << M6;
    std::cout << "M7= " << std::endl << M7;
    */

// create x matricies
    matrix x1(n/2,n/2);
    matrix x2(n/2,n/2);
    matrix x3(n/2,n/2);
    matrix x4(n/2,n/2);

    x1 = M1.plus(M4).minus(M5).plus(M7);
    x2 = M3.plus(M5);
    x3 = M2.plus(M4);
    x4 = M1.minus(M2).plus(M3).plus(M6);

// Create final matrix from x1,x2,x3,x4
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i < n/2 && j < n/2){
                X.set(i,j,x1.get(i%(n/2),j%(n/2)));
            }
            if(i < n/2 && j >= n/2){
                X.set(i,j,x2.get(i%(n/2),j%(n/2)));
            }
            if(i >= n/2 && j < n/2){
                X.set(i,j,x3.get(i%(n/2),j%(n/2)));
            }
            if(i >= n/2 && j >= n/2){
                X.set(i,j,x4.get(i%(n/2),j%(n/2)));
            }
        }

    }

}
