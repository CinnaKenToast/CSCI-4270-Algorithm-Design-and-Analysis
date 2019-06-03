// Kenneth Shipley
// CSCI 4270 
// Programming Assignment 04
// This program will find the optimal alignment distance of
// two gene sequences

#include <iostream>
#include <string>
#include "matrix.cpp"
#include <vector>

using namespace std;

int main() {

    string gene1;
    string gene2;

    cout << "Enter first sequence: ";
    cin >> gene1;
    cout << "Enter second sequence: ";
    cin >> gene2;

    // Make matrix
    vector<double> aVec((gene1.length()+1)*(gene2.length()+1),0);
    matrix a(gene1.length()+1, gene2.length()+1, aVec);

    // In last column, starting from the bottom with 0 and going up, add 2
    //then do the same for rows going to the left
    for(int j = gene1.length() + 1; j >= 0; j--){
        a.set(gene1.length()+1-j, gene2.length(), 2*(j-1));
    }
    for(int i = gene2.length() + 1; i >=0; i--){
        a.set(gene1.length(),gene2.length()+1-i, 2*(i-1)); 
    }

    for(int j = gene1.length()-1; j >= 0; j--){
        for(int i = gene2.length()-1; i >= 0; i--){
            // if the character in gene 1 and gene 2 are the same, 
            // set current to be the same as diagonal
            if(gene1.at(j) == gene2.at(i)){
                a.set(j,i, a.get(j+1,i+1));
            }
            // if not find the smallest of (right+2),(bottom+2), and (diagonal+1)
            // set current element as the smallest
            else{
                a.set(j,i,a.get(j+1,i)+2);
                if(a.get(j,i) > a.get(j,i+1)+2){
                    a.set(i,j,a.get(j,i+1)+2);
                }
                if(a.get(j,i) > a.get(j+1,i+1)+1){
                    a.set(j,i,a.get(j+1,i+1)+1);
                }
            }
        }
    }
    
    cout << "Optimal Alignment Distance: " << a.get(0,0) << endl;

    system("pause");
return 0;
}