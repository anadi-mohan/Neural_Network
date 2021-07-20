#include<stdlib.h>
#include "../utils/util.h"
#include "Layer.h"
#include<vector>
#include<cmath>

using namespace std;

class DenseLayer : public Layer
{
    public:
        DenseLayer(int numNeurons,int numNeuronsNextLayer);
        void feedforward(Layer *prevLayer);
        void calculateLocalGradient(Layer *nextLayer);
        void updateWeight();
    private:
        vector<float> & activationFunction(vector<float> &a);
        vector<float> & activationFunctionDerivative(vector<float> &a);
        float sigmoid(float a);
        vector<vector<float>> weights;
        vector<vector<float>> deltaweight;
        float eta = 0.01; float alpha = 0.1;
};

DenseLayer::DenseLayer(int numNeurons, int numNeuronsNextLayer)
{
    for(int i=0;i<=numNeuronsNextLayer;++i)
    {
        weights.push_back(vector<float>());
        deltaweight.push_back(vector<float>());
        for(int j=0;j<=numNeurons;++j)
        {
            weights.back().push_back(rand()/(float)RAND_MAX);
            deltaweight.back().push_back(0.0f);
        }
    }
}

float DenseLayer::sigmoid(float a)
{
    return 1.0/float(1+exp(-a));
}

vector<float> & DenseLayer::activationFunction(vector<float> &a)
{
    vector<float> c;
    for(int i=0;i<a.size();++i)
        c.push_back(sigmoid(a[i]));

    return c;
}

vector<float> & DenseLayer::activationFunctionDerivative(vector<float> &b)
{
    vector<float> c;
    for(int i=0;i<b.size();++i)
        c.push_back(b[i]*(1-b[i]));

    return c;
}

void DenseLayer::feedforward(Layer *prevLayer)
{
    setInput(prevLayer->output);
    input.push_back(1.0f);
    setOutput(activationFunction(util::innerProduct(weights,input)));
}

void DenseLayer::calculateLocalGradient(Layer *nextLayer)
{
    auto temp = util::innerProduct(nextLayer->delta , weights);
    temp.assign(temp.begin(),temp.end()-1);
    delta = util::hadamardProduct(temp,activationFunctionDerivative(output));

    deltaweight = util::add(util::product(eta,util::innerProduct(delta,input)),util::product(alpha,deltaweight));
}

void DenseLayer::updateWeight()
{
    weights = util::add(weights,deltaweight);
}
