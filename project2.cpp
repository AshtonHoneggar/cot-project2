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

inline void initializeMatrix(double*** arr, int r, int c) {
	*arr = new double*[r];
	for (int i = 0; i < r; ++i)
        (*arr)[i] = new double[c];
}

double** iterDPMap(double** in, int r, int c) {
	double min;
	double** ans;
	double max = numeric_limits<double>::infinity();
	initializeMatrix(&ans, r, c);

	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j){
			if (i == 0)
				ans[0][j] =	in[0][j];
			else
			{
                min = ans[i-1][j] + in[i][j];

                if(j > 0 && ans[i-1][j-1] + 1.4 * in[i][j] < min)
                    min = ans[i-1][j-1] + 1.4 * in[i][j];
                if(j < c-1 && ans[i-1][j+1] + 1.4 * in[i][j] < min)
                    min = ans[i-1][j+1] + 1.4 * in[i][j];

				ans[i][j] = min;
			}
		}
	}
	return ans;
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

