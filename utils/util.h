#include<stdio.h>
#include<iostream>
#include<vector>
#include<cassert>
#include <limits>

using namespace std;

class util
{
    public:
        template<typename T> static void add(vector<vector<T>> &a, vector<vector<T>> &b);
        template<typename T> static vector<T> innerProduct(vector<vector<T>> &a, vector<T> &b);
        template<typename T> static vector<T> innerProduct(vector<T> &a, vector<vector<T>> &b);
        template<typename T> static vector<vector<T>> innerProduct(vector<T> &a, vector<T> &b);
        template<typename T> static vector<T> hadamardProduct(vector<T> &a, vector<T> &b);
        template<typename T, typename U> static void product(U n, vector<vector<T>> &a);
};

template<typename T>
void util::add(vector<vector<T>> &a, vector<vector<T>> &b)
{

    assert(a.size()==b.size());
    assert(a[0].size()==b[0].size());
    for(int i=0;i<a.size();++i)
        for(int j=0;j<a[0].size();++j)
            a[i][j] = a[i][j]+b[i][j];
}

template<typename T>
vector<T> util::innerProduct(vector<vector<T>> &a, vector<T> &b)
{
    vector<T> c;
    
    assert(a[0].size()==b.size());

    for(int i=0;i<a.size();++i)
    {
        T sum =0;
        for(int j=0;j<a[0].size();++j)
            sum += a[i][j]*b[j];
        c.push_back(sum/b.size());
    }

    return c;
}

template<typename T>
vector<vector<T>> util::innerProduct(vector<T> &a, vector<T> &b)
{
    vector<vector<T>> c;

    for(int i=0;i<a.size();++i)
    {
        c.push_back(vector<T>());
        for(int j=0;j<b.size();++j)
            c.back().push_back(a[i]*b[j]);
    }
    return c;
}


template<typename T> 
vector<T> util::innerProduct(vector<T> &a, vector<vector<T>> &b)
{
    vector<T> c;
    
    assert(a.size()==b.size());

    for(int i=0;i<b[0].size();++i)
    {
        T sum = 0;
        for(int j=0;j<a.size();++j)
            sum += a[j]*b[j][i];
        c.push_back(sum/b.size());
    }
    return c;
}

template<typename T>
vector<T> util::hadamardProduct(vector<T> &a, vector<T> &b)
{
    vector<T> c;

    assert(a.size()==b.size());
    for(int i=0;i<a.size();++i)
        c.push_back(a[i]*b[i]);
    return c;
}

template<typename T,typename U>
void util::product(U n, vector<vector<T>> &a)
{
    for(int i=0;i<a.size();++i)
    {
        for(int j=0;j<a[0].size();++j)
            a[i][j] *= n;
    }
}
