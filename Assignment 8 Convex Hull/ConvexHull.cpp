// Kenneth Shipley
// CSCI 4270
// Programming Assignment 08
#include<iostream>
#include<vector>

using namespace std;

// Structure for a point
struct point{
    int x;
    int y;
};

// Maximum possible input for the number of points
// Does not matter if actual points are lower
// It will always generate a 1000x1000 matrix
const int MAXSIZE = 1000;

// Function to compare if two points are the same
bool isEqual(point, point);

int main(){

    // Reads in number of points
    int numPoints;
    cin >> numPoints;
    vector<point> points(numPoints);

    // Put points in an vector
    for(int i = 0; i < numPoints; i++){
        cin >> points[i].x;
        cin >> points[i].y; 
    }

    // Create an adjacency matrix with all points initialized to 0 for the convex hull
    int adj[MAXSIZE][MAXSIZE] = {};
    
    int above = 0;
    int below = 0;
    int right = 0;
    int left = 0;
    // Since we are essentailly making an undirected matrix, we only need to look at the upper half of the matrix
    for(int i = 0; i < numPoints; i++){
        for(int j = i; j < numPoints; j++){
            above = below = right = left = 0;
            // Don't need to check the diagonal
            if(i != j){
                point linePoint1 = points[i];
                point linePoint2 = points[j];
                int dy = linePoint1.y-linePoint2.y;
                int dx = linePoint1.x-linePoint2.x;
                // Avoids a division by 0
                if(dx == 0){
                    // Checks the other points to see where the point is in relation to the line
                    for(int k = 0; k < numPoints; k++){
                        // Avoids checking points the points that make up the line
                        if(!isEqual(points[k], linePoint1) && !isEqual(points[k], linePoint2)){
                                double lineX = linePoint1.x;
                                if(points[k].x > lineX){
                                    right++;
                                }
                                else if(points[k].x < lineX){
                                    left++;
                                }
                        }
                    }
                    // If all points are on the left or right then this must be a part of the convex hull
                    if(left == 0 || right == 0){
                        adj[i][j] = 1;
                        adj[j][i] = 1;
                    }
                }
                else{
                    double slope = double(dy)/dx;
                    double lineYIntercept = linePoint1.y - linePoint1.x * slope;
                    // Checks the other points to see where the point is in relation to the line
                    for(int k = 0; k < numPoints; k++){
                        // Avoids checking points the points that make up the line
                        if(!isEqual(points[k], linePoint1) && !isEqual(points[k], linePoint2)){
                            double lineY = slope * points[k].x + lineYIntercept;
                            if(points[k].y > lineY){
                                above++;
                            }
                            else if(points[k].y < lineY){
                                below++;
                            }
                        }
                    }
                    // If all points are above or below the line then it must be a part of the convex hull
                    if(above == 0 || below == 0){
                        adj[i][j] = 1;
                        adj[j][i] = 1;
                    }             
                }
            }
        }
    }

// Prints out the points that make up the edges for the convex hull
for(int i = 0; i < numPoints; i++){
    for(int j = i; j < numPoints; j++){
        if(adj[i][j] == 1){
            cout << "(" << points[i].x << "," << points[i].y << ") to (" << points[j].x << "," << points[j].y << ")" << endl;
        }
    }
}

}

// Function to compare if two points are the same
bool isEqual(point p1, point p2){
    if(p1.x == p2.x && p1.y == p2.y){
        return true;
    }
    return false;
}