// Kenneth Shipley
// CSCI 4270
// Programming Assignment 07
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <vector>

/**
 * Throws an error if the sector is less than 0 or greater than
 * or equal to the sector size squared.
 * 
 * @param sector a sector
 * @param a sector size
 */
void throwErrorIfSectorIsIncorrect(int sector, int sectorSize) {
    if (sector < 0 || sectorSize * sectorSize <= sector) {
        throw std::logic_error("the sector must be from 0 to less than sectorSize*sectorSize");
    }
}

/**
 * Displays the board.
 *
 * @param out the output stream
 * @param sectorSize the sector size of our board
 * @param board the board we are trying to solve
 */
void displayBoard(std::ostream& out, int sectorSize, const std::vector<int>& board) {
    int side = sectorSize * sectorSize;
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            out << " " << board[i * side + j];
        }
        out << std::endl;
     }
    out << std::endl;
}

/**
 * Gets the open locations from one sector of the original board.
 * Will throw an error if sector is less than 0 or greater than or
 * equal to the sector size squared.
 *
 * For example, if the sector size is 3 (typical of a 9x9 Sudoku grid),
 * then the sector must be a value from 0 to 8.
 *
 * @param sector A sector. The first sector is 0. Typically this value is 0 to 8.
 * @oaram sectorSize The width of one sector. Typically 3.
 * @param original The original board vector.
 * @return the open positions of one sector of the board.
 */
std::vector<int> getOpenLocations(int sector, int sectorSize, const std::vector<int>& original) {
    throwErrorIfSectorIsIncorrect(sector, sectorSize);
    int side = sectorSize * sectorSize;
    int row = sector / sectorSize;
    int col = sector % sectorSize;
    int board_corner = row * (side * sectorSize) + col * sectorSize;

    std::vector<int> locations;
    for (int y = 0; y < sectorSize; y++) {
        for (int x = 0; x < sectorSize; x++) {
            int location = board_corner + (y * side) + x;
            if (original[location] == 0) {
                locations.push_back(location);
            }
        }
    }

    return locations;
}

/**
 * Gets the unused values from one sector of the original board.
 * Will throw an error if sector is less than 0 or greater than or
 * equal to the sector size squared.
 *
 * For example, if 1, 3, 5, and 6 are found in a 3x3 sector, then
 * this should return 2, 4, 7, 8, and 9.
 *
 * @param sector A sector. The first sector is 0. Typically this value is 0 to 8.
 * @oaram sectorSize The width of one sector. Typically 3.
 * @param original The original board vector.
 * @return the unused values of one sector of the board.
 */
std::vector<int> getUnusedValues(int sector, int sectorSize, const std::vector<int>& original) {
    throwErrorIfSectorIsIncorrect(sector, sectorSize);
    int side = sectorSize * sectorSize;
    int row = sector / sectorSize;
    int col = sector % sectorSize;
    int board_corner = row * (side * sectorSize) + col * sectorSize;

    std::vector<bool> seen;
    for (int i = 0; i <= side * side; i++) {
        seen.push_back(false);
    }

    for (int y = 0; y < sectorSize; y++) {
        for (int x = 0; x < sectorSize; x++) {
            int location = board_corner + (y * side) + x;
            if (original[location] > 0) {
                seen[ original[location] ] = true;
            }
        }
    }

    std::vector<int> unused;
    for (int i = 1; i <= side; i++) {
        if (seen[i] == false) {
            unused.push_back(i);
        }
    }

    return unused;
}


/**
 * Gets a positive random integer from 0 to n-1.
 * This will not check if you supply it with a negative number,
 * but you should be smart enough not to do that.
 *
 * For example, if this is supplied a 3, this may return 0, 1, or 2.
 *
 * @param n the upper bound of the random value (exclusive)
 * @return a value from 0 to n-1
 */
int getRandomNumber(int n) {
    return rand()%n;
}

/**
 * Loads a sector of `board` with initial values.
 * Will throw an error if sector is less than 0 or greater than or
 * equal to the sector size squared. This will change the board
 * variable afterward.
 * 
 * @param sector the sector to load.
 * @param sectorSize the width of the sector in this puzzle.
 * @param board the board which we are trying to solve.
 */
void loadSectorWithInitialValues(int sector, int sectorSize, std::vector<int>& board) {
    throwErrorIfSectorIsIncorrect(sector, sectorSize);
    // Call getOpenLocations to get the open locations
    // Call getUnusedValues to get the unused values
    std::vector<int> openLocations = getOpenLocations(sector,sectorSize,board);
    std::vector<int> unusedValues = getUnusedValues(sector,sectorSize,board);
    
    // Note: the length of the locations should be the same
    // as the length of the unused values.

    // Take one unused value and one random location.
    //    while the random location has already been used,
    //       pick another random location
    //    write that unused value to the board specified by the location.
    for(int i = 0; i < unusedValues.size(); i++){
        int location  = openLocations[getRandomNumber(openLocations.size())];
        while(board[location] != 0){
            location = openLocations[rand()%openLocations.size()];
        }
        board[location] = unusedValues[i];
    }
}

/**
 * Loads each sector of `board` with initial values.
 *
 * @param sector the sector to load.
 * @param sectorSize the width of the sector in this puzzle.
 * @param board the board which we are trying to solve.
 * @param original the original board. Necessary for finding openings.
 */
void loadAllSectorsWithInitialValues(int sectorSize, std::vector<int>& board) {
    // Call loadSectorWithInitialValues once per sector
    for(int i = 0; i < sectorSize * sectorSize; i++){
        loadSectorWithInitialValues(i,sectorSize,board);
    }
}

/**
 * Computes the energy of a board. This is the sum of all
 * of the duplicate values seen in each row and again in
 * each column.
 *
 * @param sectorSize the width of the sector in this puzzle.
 * @param board the board which we are trying to solve, completely filled.
 */
int computeEnergy(int sectorSize, const std::vector<int>& board) {
    // Set energy to 0 
    int energy = 0;
    // Set side to be sectorSize * sectorSize
    // Note: this tells you how many rows and columns there are.
    int side = sectorSize * sectorSize;
    // Create a bool vector named `seen` that is `side` elements long PLUS ONE.
    std::vector<bool> seen(side+1);
    // Count the duplicates seen per row.
    // For each row
    //    Set all elements in `seen` to false.
    //    for each column
    //      Compute the location.
    //      if the value of board's location in the seen array is true
    //        (This means seen[board[location]] == true)
    //        then increment energy by 1
    //      set the value of the board's location in the seen array to true

    for(int y = 0; y < side; y++){
        seen.assign(seen.size(), false);
        for(int x = 0; x < side; x++){
            int location = y * side + x;
            if(seen[board[location]] == true){
                energy += 1;
            }
            else{
                seen[board[location]] = true;
            }
        }
    }
    // Count the duplicates seen per col.
    // (This was nearly identical as rows.)
    for(int y = 0; y < side; y++){
        seen.assign(seen.size(), false);
        for(int x = 0; x < side; x++){
            int location = x * side + y;
            if(seen[board[location]] == true){
                energy += 1;
            }
            else{
                seen[board[location]] = true;
            }
        }
    }
    // Return energy
    return energy;
}

/**
 * Get Two locations which are returned by refernce.
 *
 * @param sector the sector to modify.
 * @param sectorSize the width of the sector in this puzzle.
 * @param original the original board. Necessary for finding openings.
 * @param a first location (which is returned by reference)
 * @param b second location (which is returned by reference)
 */
void getTwoCellsInASector(int sector, int sectorSize, const std::vector<int>& original, int& a, int& b) {
    // Call getOpenLocations to get the open locations for a sector.
    std::vector<int> locations = getOpenLocations(sector, sectorSize, original);
    // Set a to be a random value from the locations array.
    // Set b to be a random value from the locations array.
    a = locations[rand() % locations.size()];
    b = locations[rand() % locations.size()];
    // Note: You might be wondering if it's okay if a == b.
    // It isn't great, but it saves us from having an infinite loop
    // if there is only one movable square in the puzzle.
}

/**
 * Given two locations and the board, swaps the values within the
 * board at those locations. 
 *
 * @param a the first location
 * @param b the second locaation
 * @param board the board we are trying to solve
 */
void swapTwoLocations(int a, int b, std::vector<int>& board) {
    // Swap the values at location's a and location's b
    int temp = board[a];
    board[a] = board[b];
    board[b] = temp;
    //std:: cout << "Swapped" << a << " and " << b << std::endl;
}

/**
 * Gets a random sector
 *
 * @param sectorSize the sector size of our puzzle.
 * @return a random sector
 */
int getRandomSector(int sectorSize) {
    return getRandomNumber(sectorSize * sectorSize);
}

/**
 * Performs the Monte Carlo Solution and returns the solved puzzle.
 *
 * @param sectorSize the width of the sector in this puzzle.
 * @param original the original board. Necessary for finding openings.
 * @returns the solved board
 */
std::vector<int> performMonteCarlo(int sectorSize, const std::vector<int>& original) {
    // Copy the original into a new vector named `board`.
    // Load all sectors with initial values within the new `board`.
    // Compute the energy of the board. Save this to `energy`.
    std::vector<int> board(original.size());
    for(int i = 0; i < original.size(); i ++){
        board[i] = original[i];
    }
    loadAllSectorsWithInitialValues(sectorSize, board);
    int energy = computeEnergy(sectorSize, board);
    // while energy is greater than 0
        // Select a random sector
        // Get two location from that sector which are swapable
        // Swap those two locations
        // Compute the new energy. Save this to `newenergy`

        // If the new energy is better or equal to energy, save that to energy.
        // Else
        //    Get a random number less than 100
        //    If this random number is zero, save the new energy
        //         to energy and pretend that the score did get better.
        //    Else,
        //        Undo the swapped locations.
        //FIXME:
    while(energy > 0){
        int sector = getRandomSector(sectorSize);
        int a, b;
        getTwoCellsInASector(sector, sectorSize, original, a, b);
        swapTwoLocations(a, b, board);
        int newEnergy = computeEnergy(sectorSize, board);
        //std::cout << newEnergy << std::endl;
        if(newEnergy < energy){
            energy = newEnergy;
        }
        else{
            int num = rand() % 100;
            if(num == 0){
                energy = newEnergy;
            }
            else{
                swapTwoLocations(a, b, board);
            }
        }
    }
    // Return the board (change this)
    return board;
}

/**
 * Gets the sector size from an input stream
 *
 * @param an input stream
 * @return the sector size
 */
int getSectorSize(std::istream& in) {
    int sectorSize;
    std::cin >> sectorSize;
    return sectorSize;
}

/**
 * Reads in a board from an input stream
 *
 * @param in the input stream
 * @param sectorSize the sector size of our board
 * @return the board
 */
std::vector<int> readInBoard(std::istream& in, int sectorSize) {
    std::vector<int> board;

    int side = sectorSize * sectorSize;
    int cells = side * side;
    for (int i = 0; i < cells; i++) {
        int x;
        in >> x;
        board.push_back(x);
    }

    return board;
}

int main() {
    srand(time(0));
    int sectorSize = 3;
    std::vector<int> board = readInBoard(std::cin, sectorSize);

    displayBoard(std::cout, sectorSize, board);

    board = performMonteCarlo(sectorSize, board);

    displayBoard(std::cout, sectorSize, board);

    return 0;
}
