#include<stdlib.h>
#include<vector>

using namespace std;

class Layer
{
    public:
        void setInput(vector<double> Input){input = Input;}
        void setOutput(vector<double> Output){output = Output;}
        virtual void feedforward(Layer *){}
        virtual void calculateGradient(Layer *){}
        virtual void calculateGradient(vector<double> &){}
        virtual void updateWeight(){}
        vector<double> input;
        vector<double> output;
        vector<double> delta;
        vector<vector<double>> weights;
};
