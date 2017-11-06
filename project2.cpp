#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stack>
using namespace std;

int main()
{
	string str, dimensions;
	ifstream input("input.txt", fstream::in);
	if (!input) {
		cout << "Error opening input.txt for reading." << endl;
		return 1;
	}
	ifstream output("output.txt", fstream::out);
	if (!output) {
		cout << "Error opening output.txt for writing." << endl;
		return 1;
	}

	int r, c;
	input >> r >> c;
	int i, j;
	double **in;
	initializeMatrix(in, r, c);

	for (i = 0; i < r; ++i) {
		for (j = 0; j < c; ++j)
			input >> in[i][j];
	}

	double **costMap;
	initializeMatrix(costMap, r, c);
	costMap = iterDPMap(in, r, c);
	/*for (i = 0; i < c; ++i)
	{
		
	}*/

}

inline void initializeMatrix(double** arr, int r, int c) {
	int i;
	arr = new double*[r];
	for (i = 0; i < r; ++i)
		arr[i] = new double[c];
}

double** iterDPMap(double** in, int r, int c) {
	int i, j;
	double** ans;
	initializeMatrix(ans, r, c);

	for (i = 0; i < r; ++i) {
		for (j = 0; j < c; ++j) {
			if (i == 0) {
				ans[0][j] = min(
					j > 0 ? 1.4 * in[0][j - 1] : INFINITY,
					in[0][j],
					j < c - 1 ? 1.4 * in[0][j + 1] : INFINITY);
			}
			else
			{
				ans[i][j] = min(
					j > 0 ? in[i - 1][j - 1] + 1.4 * in[i][j - 1] : INFINITY,
					ans[i - 1][j] + in[i][j],
					j < c - 1 ? ans[i - 1][j + 1] + 1.4 * in[i][j + 1] : INFINITY);
			}
		}
	}
	return ans;
}