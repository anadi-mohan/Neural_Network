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
        queue q(gpu_selector{}, dpc_common::exception_handler);
        for(int i=0;i<a.size();++i)
        {
            buffer C{c[i]};
            buffer A{a[i]};
            buffer B{b[i]};
    
           
            q.submit([&](auto &h){
                    accessor A1(A, h, read_only);
                    accessor B1(B, h, read_only);
                    accessor C1(C, h, write_only);
    
                    h.parallel_for(range(b[0].size()),[=](auto index){
                            C1[index] = A1[index] + B1[index];
                            });
                    });
        }
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
        c.push_back(0.0f);

    vector<vector<T>> temp;
    for(int i=0;i<a[0].size();++i)
    {
        temp.push_back(vector<T>());
        for(int j=0;j<a.size();++j)
            temp.back().push_back(a[j][i]);
    }

    try
    {
        queue q(gpu_selector{}, dpc_common::exception_handler);

        buffer B{b};
        buffer C{c};

        for(int i=0;i<a[0].size();++i)
        {
            buffer A{temp[i]};
            q.submit([&](auto &h){
                    accessor A1(A, h, read_only);
                    accessor B1(B, h, read_only);
                    accessor C1(C, h, write_only);
                    int width = b.size();
    
                    h.parallel_for(range(a.size()), [=](auto index){
                            C1[index] +=(A1[index]*B1[i])/width;
                            });
                    });
            host_accessor C1(C);
            for(int j=0;j<a.size();++j)
                cout <<C1[j] << " ";
            cout << std::endl;
        }
    }
    catch (sycl::exception const &e) {
        cout << "An exception is caught while adding matrices.\n";
        terminate();
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
            c.back().push_back(0.0f);
    }

    try
    {
        queue q(gpu_selector{}, dpc_common::exception_handler);

        buffer A{a};
        buffer B{b};
        for(int i=0;i<a.size();++i)
        {
            buffer C{c[i]};

            q.submit([&](auto &h){
                    accessor A1(A, h, read_only);
                    accessor B1(B, h, read_only);
                    accessor C1(C, h, write_only);

                    h.parallel_for(range(b.size()), [=](auto index){
                            C1[index] = A1[i] * B1[index];
                            });
                    });
        }
    }
    catch (sycl::exception const &e) {
        cout << "An exception is caught while inner Product of matrices.\n";
        terminate();
    }

    return c;
}


template<typename T> 
vector<T> util::innerProduct(vector<T> &a, vector<vector<T>> &b)
{
    vector<T> c;
    
    assert(a.size()==b.size());
    for(int i=0;i<b[0].size();++i)
        c.push_back(0.0f);
    try
    {
        queue q(gpu_selector{}, dpc_common::exception_handler);
        cout << "GPU device: " << q.get_device().get_info<info::device::name>() << "\n";
        buffer A{a};
        buffer C{c};

        for(int i=0;i<b.size();++i)
        {
            buffer B{b[i]};
            
            q.submit([&](auto &h){
                    accessor A1(A, h, read_only);
                    accessor B1(B, h, read_only);
                    accessor C1(C, h, write_only);
                    int width = b[0].size();

                    h.parallel_for(range(b[0].size()), [=](auto index){
                            C1[index] += (A1[i]*B1[index])/width;
                            });
                    });
        }
    }
    catch (sycl::exception const &e) {
        cout << "An exception is caught while inner Product of matrices.\n";
        terminate();
    }

    return c;
}

template<typename T>
vector<T> util::hadamardProduct(vector<T> &a, vector<T> &b)
{
    vector<T> c;

    assert(a.size()==b.size());
    for(int i=0;i<b.size();++i)
    {
            c.push_back(1.0f);
    }

    try
    {
        queue q(gpu_selector{}, dpc_common::exception_handler);

            buffer C{c};
            buffer A{a};
            buffer B{b};


            q.submit([&](auto &h){
                    accessor A1(A, h, read_only);
                    accessor B1(B, h, read_only);
                    accessor C1(C, h, write_only);

                    h.parallel_for(range(b.size()),[=](auto index){
                            C1[index] = A1[index]*B1[index];
                            });
                    });
        }
    catch (sycl::exception const &e) {
        cout << "An exception is caught while Hadamard Product of vectors.\n";
        terminate();
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
            c.back().push_back(0.0f);
    }

    try
    {
        queue q(gpu_selector{}, dpc_common::exception_handler);

        for(int i=0;i<a.size();++i)
        {
            buffer A{a[i]};
            buffer C{c[i]};

            q.submit([&](auto &h){
                    accessor A1(A, h, read_only);
                    accessor C1(C, h, write_only);

                    h.parallel_for(range(a[0].size()), [=](auto index){
                            C1[index] = n*A1[index];
                            });
                    });
        }
    }
    catch (sycl::exception const &e) {
        cout << "An exception is caught while scalar Product matrices.\n";
        terminate();
    }

    return c;
}
