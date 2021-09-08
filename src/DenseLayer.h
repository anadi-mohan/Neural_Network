#include<stdlib.h>
#include "../utils/util.h"
#include "Layer.h"
#include<vector>
#include<cmath>
#include<iostream>

using namespace std;

class DenseLayer : public Layer
{
    public:
        DenseLayer(int numNeurons,int numNeuronsNextLayer);
        void feedforward(Layer *prevLayer);
        void calculateGradient(Layer *nextLayer);
        void calculateGradient(vector<float> &yd);
        void updateWeight();
    private:
        vector<float> activationFunction(vector<float> &a);
        vector<float> activationFunctionDerivative(vector<float> &a);
        float sigmoid(float a);
        float tan_h(float a);
        vector<vector<float>> deltaweight;
        float eta = 0.1; float alpha = 0.1;
};

DenseLayer::DenseLayer(int numNeurons, int numNeuronsNextLayer)
{
    for(int i=0;i<numNeuronsNextLayer;++i)
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

vector<float> DenseLayer::activationFunction(vector<float> &a)
{
    vector<float> c;
    for(int i=0;i<a.size();++i)
        c.push_back(tanh(a[i]));

    return c;
}

vector<float> DenseLayer::activationFunctionDerivative(vector<float> &b)
{
    vector<float> c;
    for(int i=0;i<b.size();++i)
        c.push_back(1-pow(b[i],2));

    return c;
}

void DenseLayer::feedforward(Layer *prevLayer)
{
    setInput(prevLayer->output);
    input.push_back(1.0f);
    vector<float> c = util::innerProduct(weights,input);
    setOutput(activationFunction(c));
}

void DenseLayer::calculateGradient(Layer *nextLayer)
{
    auto temp = util::innerProduct(nextLayer->delta , nextLayer->weights);
    temp.assign(temp.begin(),temp.end()-1);
    vector<float> c = activationFunctionDerivative(output);
    delta = util::hadamardProduct(temp,c);

    vector<vector<float>> new_delta = util::innerProduct(delta,input);
    util::product(eta,new_delta);
    util::product(alpha,deltaweight);
    util::add(deltaweight,new_delta);
}

void DenseLayer::calculateGradient(vector<float> &yd)
{
    vector<float> c = activationFunctionDerivative(output);
    delta.clear();
    for(int i=0;i<output.size();++i)
    {
        delta.push_back((yd[i]-output[i])*c[i]);
    }
    
    vector<vector<float>> new_delta = util::innerProduct(delta,input);
    util::product(eta,new_delta);
    util::product(alpha,deltaweight);
    util::add(deltaweight,new_delta);
}

void DenseLayer::updateWeight()
{
    util::add(weights,deltaweight);
}
