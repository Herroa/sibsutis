#include <iostream>
#include <vector>
#include <cmath>

#define EPS 0.001
using namespace std;

void printMatrix(vector<vector<double>> &matrix)
{
    for (auto row : matrix)
    {
        for (auto element : row)
        {
            cout << element << " ";
        }
        cout << endl;
    }
}

void printVector(vector<double> &matrix)
{
    for (auto element : matrix)
    {
        cout << element << " ";
    }
}

vector<double> solveSystem(vector<vector<double>> &A)
{
    for (unsigned int i = 0; i < A.size(); i++)
    {
        double diagonal = A[i][i];
        for (unsigned int j = 0; j < A[0].size(); j++)
        {
            A[i][j] /= diagonal;
        }
        A[i][i] = 0;
    }
    vector<double> x = {0, 0, 0};
    vector<double> C;
    for (unsigned int i = 0; i < A.size(); i++)
    {
        C.push_back(A[i][A[0].size() - 1]);
    }
    vector<double> new_x;
    while(1){
    for (unsigned int i = 0; i < A.size(); i++)
    {
        double sum = 0;
        for (unsigned int j = 0; j < A[0].size() - 1; j++)
        {
            sum += x[j] * A[i][j];
        }

        new_x.push_back(C[i] - sum);
        sum = 0;
    }
    // cout << x[0] << " " << new_x[0] << endl;
    if(fabs(new_x[0]-x[0])<EPS){
        break;
    }
    x = new_x;
    for (auto element : new_x)
    {
        cout << element << " ";
    }
    cout << endl;
    new_x.clear();
    }
    return new_x;
}

int main()
{
    vector<vector<double>> A = {{5, -1, 2, 3}, {-2, -10, 3, -4}, {1, 2, 5, 12}};

    vector<double> solution = solveSystem(A);
    // printVector(solution);

    return 0;
}