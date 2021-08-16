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
    vector<float> B;
    vector<float> C;
    
    for(int i=0;i<M;++i)
    {
        A.push_back(vector<float>());
        for(int j=0;j<N;++j) 
            A.back().push_back(1.0f);
    }
    for(int i=0;i<N;++i)
    {
        B.push_back(i+1.0f);
    }
    C = util::innerProduct(A, B);
    /*C.assign(C.begin(),C.end()-1);*/
    for(int i=0;i<C.size();++i)
    {
        cout << C[i] << std::endl;
    }

}
