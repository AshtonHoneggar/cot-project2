#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <cmath>
#include <queue>

#define ERROR 0.00001 // Used to compare doubles

using namespace std;

// Initializes a dynamic 2D array -> arr[r][c]
inline void initializeMatrix(double*** arr, int r, int c) {
	*arr = new double*[r];			// Creates an array of double pointers size r
	for (int i = 0; i < r; ++i)		// For each double pointer in arr,
        (*arr)[i] = new double[c];	// Set as an array of size c
}

//==========================================================
// Iteratively creates and returns a 2D map of costs.
// Each "tile" contains the minimum cost required
// to reach that tile starting from the top row.
//----------------------------------------------------------
// in: a 2D double array which is the input map.
// r: the number of rows
// c: the number of columns
//==========================================================
double** iterDPMap(double** in, int r, int c) {
	double min;		// Stores the value of the minimum cost tile
	double** ans;	// Stores the cost map to be returned
	initializeMatrix(&ans, r, c); // Initializes ans to size r x c

	for (int i = 0; i < r; ++i) { // Starting at the top row, and for each row
		for (int j = 0; j < c; ++j){ // Starting at the leftmost column, and for each column
			
			if (i == 0) // If location is in the first row
				ans[0][j] =	in[0][j]; // Then the lowest cost is just the cost of that tile
			else // If the location is in any other row
			{
                // Set min to be the cost of the current tile plus minCost of the North tile
				min = ans[i-1][j] + in[i][j];

				// Check if the NW tile exists and if the cost is lower than the North tile
                if(j > 0 && ans[i-1][j-1] + 1.4 * in[i][j] < min)
                    min = ans[i-1][j-1] + 1.4 * in[i][j];
				
				// Check if the NE tile exists and if the cost is lower than the North or NW tiles
                if(j < c-1 && ans[i-1][j+1] + 1.4 * in[i][j] < min)
                    min = ans[i-1][j+1] + 1.4 * in[i][j];

				ans[i][j] = min; // Set the tile to be the minimum value of the three options
			}
		}
	}
	return ans; // Return the 2D cost map array
}

//==========================================================
// Calculates and returns the fastest path to each of the
// tiles in the bottom border in a queue<string>.
// Iterates through each bottom tile and moves up, finding
// the fastest way to the top based on the cost map.
//----------------------------------------------------------
// map: the cost map calculated by iterDPMap
// in: the initial input map from input.txt
// r: the number of rows in each array
// c: the number of columns in each array
//==========================================================
queue <string> fastestPaths(double** map, double** in, int r, int c) {
	stack <string> path;	// Stack to store the fastest path
	queue <string> ans;		// Queue to reverse the stack for output
	
	for (int j = 0; j < c; j++){ // Starting at column 0 and for each column
		int loc = j; // Keeps track of the column so the starting column is known
		for(int i = r-1; i > 0; i--){ // Starting at the bottom row, and moving to the top
		
			// If minCost of tile NW + 1.4*cost of current tile = current tile minCost 
			if(loc > 0 && abs(map[i][loc] - (map[i-1][loc-1] + 1.4 * in[i][loc])) < ERROR){
				path.push(" SE ");	// Push SE direction
				loc--;				// Decrement column index
				
			// If minCost of tile NE + 1.4*cost of current tile = current tile minCost 
			} else if(loc < c-1 && abs(map[i][loc] - (map[i-1][loc+1] + 1.4 * in[i][loc])) < ERROR){
				path.push(" SW ");	// Push SW direction
				loc++;				// Increment column index 
				
			// If minCost of tile N + cost of current tile = current tile minCost
			} else
				path.push(" S ");	// Push S direction
		}
		
		// Convert column number into a string
		ostringstream str1;
		str1 << loc;
		string loc_string = str1.str();

		
		path.push(loc_string);	// Push column number onto stack
		path.push("\n");		// Push a newline onto stack for output formatting
		
		// Reverse the stack contents into a queue for output formatting
		while(!path.empty()){
			ans.push(path.top()); //Have to check in C4 lab if this works. IDE says to cast to static_cast<basic_string<char> &&>, fails when compiling on cmd line.
			path.pop();
		}
	}
	return ans; // Return queue with shortest paths
}

//==========================================================
// Gets the input from input.txt, calls other functions to
// get the shortest paths. Writes the shortest path into
// output.txt
//==========================================================
int main()
{
	// Input file for reading terrain array
	ifstream input;
    input.open("input-small.txt");
	if (!input) { // Print error and exit if cannot open for read
		cout << "Error opening input.txt for reading." << endl;
		return 1;
	}

	int r , c;		// r = # of rows, c = # of columns
	input >> r;		// Get r from file
    input >> c;		// Get c from file
	double **in;	// Declare a 2D array pointer
	initializeMatrix(&in, r, c); // Initialize the array to be of size r x c

	// For each tile in the terrain map, insert into the "in" array
	for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j)
            input >> in[i][j];
	}

    input.close(); // Close input file

	double **costMap;					// Create a 2D array for storing a cost map
	initializeMatrix(&costMap, r, c);	// Initialize to size r x c
	costMap = iterDPMap(in, r, c);		// Set equal to output from iterDPMap

	// Create a queue to hold the return from call to fastestPaths
	queue <string> answer = fastestPaths(costMap, in, r, c);

	// Open output.txt for writing
	ofstream output("output.txt", fstream::out);
	if (!output) { // Print error and exit if cannot open for write
		cout << "Error opening output.txt for writing." << endl;
		return 1;
	}

	// Print results into output file
	for (int i = 0; i < c; ++i)
	{
		for(int j = 0; j <= r; ++j){
			output << answer.front();
			answer.pop();
		}
	}

}

