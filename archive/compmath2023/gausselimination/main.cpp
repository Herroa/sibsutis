#include <iostream>
#include <vector>

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

void gaussElimination(vector<vector<double>> &matrix)
{
    int rows = matrix.size();
    int cols = matrix[0].size() - 1;

    for (int i = 0; i < rows; i++)
    {
        int pivotRow = i;
        for (int k = i + 1; k < rows; k++)
        {
            if (abs(matrix[k][i]) > abs(matrix[pivotRow][i]))
            {
                pivotRow = k;
            }
        }
        swap(matrix[i], matrix[pivotRow]);

        double pivot = matrix[i][i];
        for (int j = i; j <= cols; j++)
        {
            matrix[i][j] /= pivot;
        }

        for (int k = 0; k < rows; k++)
        {
            if (k != i)
            {
                double factor = matrix[k][i];
                for (int j = i; j <= cols; j++)
                {
                    matrix[k][j] -= factor * matrix[i][j];
                }
            }
        }
    }
}

int main()
{
    vector<vector<double>> matrix1 = {{1, -2, 1, 0},
                                      {2, 2, -1, 3},
                                      {3, -1, 1, 5}};
    gaussElimination(matrix1);
    cout << "\nsolution:" << endl;
    printMatrix(matrix1);

    vector<vector<double>> matrix2 = {{3, 4, 10}, {5, -7, 3}};
    gaussElimination(matrix2);
    cout << "\nsolution:" << endl;
    printMatrix(matrix2);

    vector<vector<double>> matrix3 = {{1, 2, -3, -4},
                                      {2, 5, -4, 0},
                                      {-3, 1, 3, 5}};
    gaussElimination(matrix3);
    cout << "\nsolution:" << endl;
    printMatrix(matrix3);

    vector<vector<double>> matrix4 = {{1, 2, 3, -2, 6},
                                      {2, 4, -2, -3, 18},
                                      {3, 2, -1, 2, 4},
                                      {2, -3, 2, 1, 8}};
    gaussElimination(matrix4);
    cout << "\nsolution:" << endl;
    printMatrix(matrix4);

    return 0;
}