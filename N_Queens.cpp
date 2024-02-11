#include <iostream>
#include <vector>
#include <fstream>
#include <omp.h>

using namespace std;

bool isSafe(int row, int col, const vector<int>& placement) {
    for (int i = 0; i < row; ++i) {
        if (placement[i] == col || abs(i - row) == abs(placement[i] - col)) {
            return false;
        }
    }
    return true;
}

void solveNQueens(int n, int row, vector<int>& placement, int& count, ofstream& outfile) {
    if (row == n) {
        #pragma omp critical
        {
            ++count; // Increment solution count

            // Print the solution to file
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (placement[i] == j) outfile << "Q ";
                    else outfile << ". ";
                }
                outfile << endl;
            }
            outfile << endl;
        }
        return;
    }

    #pragma omp parallel for
    for (int col = 0; col < n; ++col) {
        if (isSafe(row, col, placement)) {
            placement[row] = col;
            solveNQueens(n, row + 1, placement, count, outfile);
        }
    }
}

int main() {
    int n;
    int num_threads;

    cout << "Enter the size of the chessboard (n): ";
    cin >> n;

    cout << "Enter the number of threads: ";
    cin >> num_threads;

    if (n <= 0 || num_threads <= 0) {
        cout << "Invalid input. Please enter positive values for n and number of threads.\n";
        return 1;
    }

    vector<int> placement(n, 0);
    int count = 0;

    ofstream outfile("solutions.txt");
    if (!outfile.is_open()) {
        cout << "Failed to open file for writing solutions." << endl;
        return 1;
    }

    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp single
        solveNQueens(n, 0, placement, count, outfile);
    }

    double end_time = omp_get_wtime();
    double execution_time = end_time - start_time;

    outfile.close();

    cout << "Total number of solutions: " << count << endl;
    cout << "Execution time: " << execution_time << " seconds" << endl;

    return 0;
}
