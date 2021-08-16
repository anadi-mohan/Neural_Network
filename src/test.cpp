#include<stdlib.h>
#include<iostream>
#include<vector>
#include "DenseLayer.h"

using namespace std;

int main()
{
    Layer *layer = new Layer();
    vector<float> input{1,1};
    layer->setInput(input);
    layer->setOutput(input);

    DenseLayer *test = new DenseLayer(2,4);
    vector<Layer *> network;
    network.push_back(test);
    test = new DenseLayer(4,1);
    network.push_back(test);
    vector<float> yd{0.5};
    
    for(int i=0;i<100;++i)
    {
        network[0]->feedforward(layer);
        network[1]->feedforward(network[0]);
    
        cout << endl;
    
        float cost_function=0;
        cout << "Output";
        for(int i=0;i<yd.size();++i)
        {
            cout << " " << network[1]->output[i];
            cost_function += pow(network[1]->output[i]-yd[i],2.0)/2;
        }
        cout << endl;
    
        cout << "Average error -> " << cost_function << endl;
        network[1]->calculateOutputGradient(yd);
        network[0]->calculateLocalGradient(network[1]);
    
        network[0]->updateWeight();
        network[1]->updateWeight();
        cout << endl;
    }
}
