#include <iostream>
#include <vector>

using namespace std;

double fact(int N)
{
    if(N < 0)
        return 0;
    if (N == 0) 
        return 1; 
    else 
        return N * fact(N - 1);
}

void print(vector<vector<double>> vec)
{
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        for(unsigned int j = 0;j < vec[0].size();j++){
            cout << vec[i][j] << " ";
        }
        cout << endl;
    }
}

double q_interation(unsigned int i, double q){
    double res = q;
    for(unsigned int j = 1;j<i+1;j++){
        res *= q - j;
    }
    return res;
}

double q2_interation(unsigned int i, double q){
    double res = q;
    for(unsigned int j = 1;j<i+1;j++){
        res *= q + j;
    }
    return res;
}

int main()
{
    vector<vector<double>> table = {{1, 1.5, 2, 2.5}, {1, 1.2247, 1.4142, 1.5811}};
    double x = 1.69;
    double h = table[0][1] - table [0][0];
    double q = (x - table[0][0])/h;


    for (unsigned int i = 1; i < table[0].size(); i++)
    {
        vector<double> next_vector;
        for (unsigned int j = 1; j < table[i].size(); j++)
        {
            next_vector.push_back(table[i][j] - table[i][j - 1]);
        }
        table.push_back(next_vector);
    }
    print(table);

    //first
    double result = table[1][0];
    for(unsigned int i = 0;i < table.size()-2;i++){
        result += (table[i+2][0]/fact(i+1))*q_interation(i,q);//todo q
    }
    cout << result << endl;

    //second
    double result2 = table[1][table[1].size()-1];
    q = (x - table[0][table[0].size()-1])/h;
    for(unsigned int i = 0;i < table.size()-2;i++){
        result2 += (table[i+2][table.size() - 3 - i]/fact(i+1))*q2_interation(i,q);//todo q
    }
    cout << result2 << endl;
    return 0;
}

