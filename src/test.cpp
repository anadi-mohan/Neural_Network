#include<stdlib.h>
#include<iostream>
#include<vector>
#include "createTraining.h"
#include "DenseLayer.h"

using namespace std;

int main()
{
    TrainingData trainData("./Training.txt");
    vector<unsigned> topology;
    trainData.getTopology(topology);

    DenseLayer *test = new DenseLayer(2,4);
    vector<Layer *> network;
    network.push_back(test);
    test = new DenseLayer(4,1);
    network.push_back(test);

    Layer *layer = new Layer();
    vector<float> input;
    vector<float> yd;
    
    int trainingPass = 0;
    while(trainingPass!=2000)
    {
        ++trainingPass;
        cout << endl << "EPOCH " << trainingPass;

        trainData.getNextInputs(input);
        layer->setOutput(input);
        cout << endl << "Input";
        for(int i=0;i<input.size();++i)
        {
            cout << " " << input[i];
        }

        network[0]->feedforward(layer);
        network[1]->feedforward(network[0]);

        trainData.getTargetOutputs(yd);
        cout << endl;

        float cost_function=0;
        cout << "Output";
        for(int i=0;i<yd.size();++i)
        {
            cout << " " << network[1]->output[i] << endl << "Desired Output " << yd[i];
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
