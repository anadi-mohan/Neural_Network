#include<stdlib.h>
#include<vector>

using namespace std;

class Layer
{
    public:
        void setInput(vector<float> Input){input = Input;}
        void setOutput(vector<float> Output){output = Output;}
        virtual void feedforward(Layer *){}
        virtual void calculateGradient(Layer *){}
        virtual void calculateGradient(vector<float> &){}
        virtual void updateWeight(){}
        vector<float> input;
        vector<float> output;
        vector<float> delta;
        vector<vector<float>> weights;
};
