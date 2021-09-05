#include "util.h"
#include<iostream>
#include<stdio.h>
#include<vector>

using namespace std;

constexpr int M=12;
constexpr int N=12;
constexpr int P=10;

int main()
{
    vector<float> C;
    vector<vector<float>> A;
    vector<float> B;
    
    for(int i=0;i<M;++i)
    {
        A.push_back(vector<float>());
        for(int j=0;j<N;++j) 
            A.back().push_back(1.0f);
    }
    for(int i=0;i<N;++i)
    {
        //B.push_back(vector<float>());
        //for(int j=0;j<N;++j)
            B.push_back(i+1.0f);
    }
    C = util::innerProduct(B,A);
    for(int i=0;i<C.size();++i)
    {
        //for(int j=0;j<C[0].size();++j)
            cout << C[i] << " ";
        cout << std::endl;
    }

}
