#include "util.h"
#include<iostream>
#include<stdio.h>
#include<vector>

using namespace std;

constexpr int M=12;
constexpr int N=10;
constexpr int P=10;

int main()
{
    vector<vector<float>> A;
    vector<vector<float>> B;
    vector<vector<float>> C;
    
    for(int i=0;i<M;++i)
    {
        A.push_back(vector<float>());
        for(int j=0;j<N;++j) 
            A.back().push_back(1.0f);
    }
    for(int i=0;i<M;++i)
    {
        B.push_back(vector<float>());
        for(int j=0;j<N;++j)
            B.back().push_back(1.0f);
    }
    C = util::add(A, B);
    /*C.assign(C.begin(),C.end()-1);*/
    for(int i=0;i<C.size();++i)
    {
        for(int j=0;j<C[0].size();++j)
            cout << C[i][j] << " ";
        cout << std::endl;
    }

}
