#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <cmath>
#include <limits>
#include <queue>

#define ERROR 0.00001

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

queue <string> fastestPaths(double** map, double** in, int r, int c) {
	stack <string> path;
	queue <string> ans;
	for (int j = 0; j < c; j++){
		int loc = j;
		for(int i = r-1; i > 0; i--){
			if(loc > 0 && abs(map[i][loc] - (map[i-1][loc-1] + 1.4 * in[i][loc])) < ERROR){
				path.push(" SE ");
				loc--;
			} else if(loc < c-1 && abs(map[i][loc] - (map[i-1][loc+1] + 1.4 * in[i][loc])) < ERROR){
				path.push(" SW ");
				loc++;
			} else
				path.push(" S ");
		}
		ostringstream str1;
		str1 << loc;
		string loc_string = str1.str();

		path.push(loc_string);
		path.push("\n");
		while(!path.empty()){
			ans.push(path.top()); //Have to check in C4 lab if this works. IDE says to cast to static_cast<basic_string<char> &&>, fails when compiling on cmd line.
			path.pop();
		}
	}
	return ans;
}

int main()
{
	ifstream input;
    input.open("input-small.txt");
	if (!input) {
		cout << "Error opening input.txt for reading." << endl;
		return 1;
	}

	int r , c;
	input >>  r;
    input >> c;
	double **in;
	initializeMatrix(&in, r, c);
    int x;

	for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j)
            input >> in[i][j];

	}

    input.close();

	double **costMap;
	initializeMatrix(&costMap, r, c);
	costMap = iterDPMap(in, r, c);

	queue <string> answer = fastestPaths(costMap, in, r, c);

	ofstream output("output.txt", fstream::out);
	if (!output) {
		cout << "Error opening output.txt for writing." << endl;
		return 1;
	}

	for (int i = 0; i < c; ++i)
	{
		for(int j = 0; j <= r; ++j){
			output << answer.front();
			answer.pop();
		}
	}

}

