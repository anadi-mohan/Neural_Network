#include<stdlib.h>
#include<iostream>
#include<vector>
#include "createTraining.h"
#include "DenseLayer.h"
#include<chrono>

using namespace std;

int main()
{
    //TrainingData trainData("/home/u79318/Neural_Network/src/Training.txt");
    TrainingData trainData("./train_images.txt");
    TrainingData trainLabel("./train_labels.txt");
    //TrainingData trainData("./Training.txt");
    //vector<unsigned> topology;
    //trainData.getTopology(topology);

    DenseLayer *test = new DenseLayer(784,512);
    vector<Layer *> network;
    network.push_back(test);
    test = new DenseLayer(512,10);
    network.push_back(test);

    Layer *layer = new Layer();
    vector<double> input;
    vector<double> yd;
  
    int trainingPass = 0;
    auto start = chrono::high_resolution_clock::now();
    while(trainingPass!=2)
    {
        ++trainingPass;

        trainData.getNextInputs(input);
        layer->setOutput(input);
//        if(trainingPass %100 == 0)
//        {
//            cout << std::endl << "Input";
//            for(int i=0;i<input.size();++i)
//            {
//                cout << " " << input[i];
//            }
//            cout << std::endl;
//        }

        network[0]->feedforward(layer);
        network[1]->feedforward(network[0]);

        trainLabel.getTargetOutputs(yd);

        if(trainingPass%1 ==0)
        {
            cout << std::endl << "EPOCH " << trainingPass;
            cout << std::endl;
            double cost_function=0;
            cout << "Desired Output:" ; 
            for(int i=0;i<yd.size();++i)
            {
                cout << " "<< yd[i];
            }
            cout << std::endl<<"Output:";
            for(int i=0;i<yd.size();++i)
            {
                cout << " " << network[1]->output[i] ;
                cost_function += pow(network[1]->output[i]-yd[i],2.0)/2;
            }
            cout << std::endl;
        
            cout << "Average error -> " << cost_function << std::endl;
        }
        network[1]->calculateGradient(yd);
        network[0]->calculateGradient(network[1]);
    
        network[0]->updateWeight();
        network[1]->updateWeight();
    }
    auto stop = chrono::high_resolution_clock::now();
//    vector<double> Test={1.0,1.0};
//    layer->setOutput(Test);
//    network[0]->feedforward(layer);
//    network[1]->feedforward(network[0]);
//    cout << "1,1 -> " << network[1]->output[0] << std::endl;
//    Test={0.0,0.0};
//    layer->setOutput(Test);
//    network[0]->feedforward(layer);
//    network[1]->feedforward(network[0]);
//    cout << "0,0 -> " << network[1]->output[0] << std::endl;
//    Test={1.0,0.0};
//    layer->setOutput(Test);
//    network[0]->feedforward(layer);
//    network[1]->feedforward(network[0]);
//    cout << "1,0 -> " << network[1]->output[0] << std::endl;
//    Test={0.0,1.0};
//    layer->setOutput(Test);
//    network[0]->feedforward(layer);
//    network[1]->feedforward(network[0]);
//    cout << "0,1 -> " << network[1]->output[0] << std::endl;
    
    auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    cout << "Total Time taken: " << duration.count() << std::endl;
}
