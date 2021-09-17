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
        void calculateGradient(vector<double> &yd);
        void updateWeight();
    private:
        vector<double> activationFunction(vector<double> &a);
        vector<double> activationFunctionDerivative(vector<double> &a);
        double sigmoid(double a);
        double tan_h(double a);
        vector<vector<double>> deltaweight;
        double eta = 0.1; double alpha = 0.1;
};

DenseLayer::DenseLayer(int numNeurons, int numNeuronsNextLayer)
{
    for(int i=0;i<numNeuronsNextLayer;++i)
    {
        weights.push_back(vector<double>());
        deltaweight.push_back(vector<double>());
        for(int j=0;j<=numNeurons;++j)
        {
            weights.back().push_back(rand()/(double)RAND_MAX);
            deltaweight.back().push_back(0.0f);
        }
    }
}
double DenseLayer::sigmoid(double a)
{
    return 1.0/double(1+exp(-a));
}

vector<double> DenseLayer::activationFunction(vector<double> &a)
{
    vector<double> c;
    for(int i=0;i<a.size();++i)
        c.push_back(sigmoid(a[i]));

    return c;
}

vector<double> DenseLayer::activationFunctionDerivative(vector<double> &b)
{
    vector<double> c;
    for(int i=0;i<b.size();++i)
        c.push_back(b[i]*(1-b[i]));

    return c;
}

void DenseLayer::feedforward(Layer *prevLayer)
{
    setInput(prevLayer->output);
    input.push_back(1.0f);
    vector<double> c = util::innerProduct(weights,input);
    setOutput(activationFunction(c));
}

void DenseLayer::calculateGradient(Layer *nextLayer)
{
    auto temp = util::innerProduct(nextLayer->delta , nextLayer->weights);
    temp.assign(temp.begin(),temp.end()-1);
    vector<double> c = activationFunctionDerivative(output);
    delta = util::hadamardProduct(temp,c);

    vector<vector<double>> new_delta = util::innerProduct(delta,input);
    util::product(eta,new_delta);
    util::product(alpha,deltaweight);
    util::add(deltaweight,new_delta);
}

void DenseLayer::calculateGradient(vector<double> &yd)
{
    vector<double> c = activationFunctionDerivative(output);
    delta.clear();
    for(int i=0;i<output.size();++i)
    {
        delta.push_back((yd[i]-output[i])*c[i]);
    }
    
    vector<vector<double>> new_delta = util::innerProduct(delta,input);
    util::product(eta,new_delta);
    util::product(alpha,deltaweight);
    util::add(deltaweight,new_delta);
}

void DenseLayer::updateWeight()
{
    util::add(weights,deltaweight);
}
