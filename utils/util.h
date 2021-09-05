#include<stdio.h>
#include<iostream>
#include<vector>
#include<cassert>
#include <limits>
#include <CL/sycl.hpp>
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
    for(int i=0;i<a.size();++i)
    {
        c.push_back(vector<T>());
        for(int j=0;j<a[0].size();++j)
            c.back().push_back(0.0f);
    }
    
    try
    {
        queue q(gpu_selector{}, dpc_common::exception_handler);
        cout << "GPU Device: " << q.get_device().get_info<info::device::name>() << "\n";
        for(int i=0;i<a.size();++i)
        {
            buffer a_buf(a[i]);
            buffer b_buf(b[i]);
            buffer c_buf(c[i]);

            q.submit([&](auto &h){
                    accessor A(a_buf,h,read_only);
                    accessor B(b_buf,h,read_only);
                    accessor C(c_buf,h,write_only);
                    h.parallel_for(range(a[0].size()),[=](auto index){
                            C[index] = A[index]+B[index];
                            });
                    });
        }
    }
    catch (sycl::exception const &e) {
        cout << "An exception is caught while multiplying matrices.\n";
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
