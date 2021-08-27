#include<stdio.h>
#include<iostream>
#include<vector>
#include<cassert>
#include <CL/sycl.hpp>
#include <limits>
#include "dpc_common.hpp"

using namespace std;
using namespace sycl;

class util
{
    public:
        template<typename T> static vector<vector<T>> add(vector<vector<T>> &a, vector<vector<T>> &b);
        template<typename T> static vector<T> innerProduct(vector<vector<T>> &a, vector<T> &b);
        template<typename T> static vector<T> innerProduct(vector<T> &a, vector<vector<T>> &b);
        template<typename T> static vector<vector<T>> innerProduct(vector<T> &a, vector<T> &b);
        template<typename T> static vector<T> hadamardProduct(vector<T> &a, vector<T> &b);
        template<typename T, typename U> static vector<vector<T>> product(U n, vector<vector<T>> &a);
};

template<typename T>
vector<vector<T>> util::add(vector<vector<T>> &a,vector<vector<T>> &b)
{
    vector<vector<T>> c;

    assert(a.size()==b.size());
    assert(a[0].size()==b[0].size());
    for(int i=0;i<b.size();++i)
    {
        c.push_back(vector<T>());
        for(int j=0;j<b[0].size();++j)
            c.back().push_back(0.0f);
    }

    try
    {
        queue q(cpu_selector{}, dpc_common::exception_handler);
        cout << "CPU Device: " << q.get_device().get_info<info::device::name>() << std::endl;
        buffer C{c};
        buffer A{a};
        buffer B{b};
       
        q.submit([&](auto &h){
                accessor A1(A, h, read_only);
                accessor B1(B, h, read_only);
                accessor C1(C, h, write_only);

                h.parallel_for(range(b.size(),b[0].size()),[=](auto index){
                        C1[index[0]][index[1]] = A1[index[0]][index[1]] + B1[index[0]][index[1]];
                        });
                });

    }
    catch (sycl::exception const &e) {
        cout << "An exception is caught while adding matrices.\n";
        terminate();
    }

    return c;
}

template<typename T>
vector<T> util::innerProduct(vector<vector<T>> &a, vector<T> &b)
{
    vector<T> c;
    
    assert(a[0].size()==b.size());
    for(int i=0;i<a.size();++i)
    {
        T sum = 0;
        for(int j=0;j<b.size();++j)
        {
            sum += a[i][j]*b[j];
        }
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
         T sum=0;
         for(int j=0;j<b.size();++j)
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
    {
        c.push_back(a[i]*b[i]);
    }

    return c;
}

template<typename T,typename U>
vector<vector<T>> util::product(U n, vector<vector<T>> &a)
{
    vector<vector<T>> c;

    for(int i=0;i<a.size();++i)
    {
        c.push_back(vector<T>());
        for(int j=0;j<a[0].size();++j)
            c.back().push_back(n*a[i][j]);
    }

    return c;
}
