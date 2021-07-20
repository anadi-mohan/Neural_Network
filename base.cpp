#include<iostream>
#include<stdlib.h>
#include<vector>

using namespace std;

class base
{
    public:
        void setA(int n){a=n;}
        void setB(int n){b=n;}
        virtual int add(){}
        virtual int sub(){}
        virtual int mul(){}
        virtual int div(){}
    protected:
        int a,b;
};

class inheri1 : public base
{
    public:
        int add(){return a+b;}
        int sub(){return a-b;}
};

class inheri2 : public base
{
    public:
        int add(){return a*b;}
        int div(){return a/b;}
};

int main()
{
    vector<base *> test;
    test.push_back(new inheri1());
    test.back()->setA(4);
    test.back()->setB(5);
    test.push_back(new inheri2());
    test.back()->setA(7);
    test.back()->setB(8);

    cout << test[0]->add() << " " << test[1]->add() << endl;
}
