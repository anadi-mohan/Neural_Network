#include<stdlib.h>
#include<vector>

using namespace std;

class Layer
{
    public:
        void setInput(vector<float> Input){input = Input;}
        void setOutput(vector<float> Output){output = Output;}
        virtual void feedforward(Layer *){}
        virtual void calculateLocalGradient(Layer *){}
        virtual void updateWeight(float){}
        vector<float> input;
        vector<float> output;
        vector<float> delta;
};
