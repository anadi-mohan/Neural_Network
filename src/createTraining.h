#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<stdlib.h>

using namespace std;

class TrainingData
{
public:
    TrainingData(const string filename);
    bool isEof(void) { return m_trainingDataFile.eof(); }
    void getTopology(vector<unsigned>& topology);

    unsigned getNextInputs(vector<double>& inputVals);
    unsigned getTargetOutputs(vector<double>& targetOutputVals);

private:
    ifstream m_trainingDataFile;
};

void TrainingData::getTopology(vector<unsigned>& topology)
{
    string line;
    string label;

    getline(m_trainingDataFile, line);
    stringstream ss(line);
    ss >> label;
    cout << label;
    if (this->isEof() || label.compare("topology:") != 0) {
        cout<<"Wrong topology";
        //abort();
    }

    while (!ss.eof()) {
        unsigned  n;
        ss >> n;
        topology.push_back(n);
    }
    return;
}

TrainingData::TrainingData(const string filename)
{
    m_trainingDataFile.open(filename.c_str());
}

unsigned TrainingData::getNextInputs(vector<double>& inputVals)
{
//    inputVals.clear();
//    string line;
//    getline(m_trainingDataFile, line);
//    stringstream ss(line);
//
//    string label;
//    ss >> label;
//    if (label.compare("in:") == 0) {
//        double oneValue;
//        while (ss >> oneValue) {
//            inputVals.push_back(oneValue);
//        }
//    }
//    return inputVals.size();
    inputVals.clear();
    string line;
    getline(m_trainingDataFile, line);
    stringstream ss(line);
    double val;
    ss>>val;
    while(val!=-1.0)
    {
        inputVals.push_back(val/254.0);
        getline(m_trainingDataFile, line);
        stringstream ss(line);
        ss>>val;
    }
    return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(vector<double>& targetOutputVals)
{
//    targetOutputVals.clear();
//
//    string line;
//    getline(m_trainingDataFile, line);
//    stringstream ss(line);
//
//    string label;
//    ss >> label;
//    if (label.compare("out:") == 0) {
//        double oneValue;
//        while (ss >> oneValue) {
//            targetOutputVals.push_back(oneValue);
//        }
//    }
//    return targetOutputVals.size();
    targetOutputVals.clear();
    
    string line;
    getline(m_trainingDataFile, line);
    stringstream ss(line);
    double val;
    ss>>val;
    //targetOutputVals.push_back(val);
    int i=0;
    while(i<10)
    {
        if (i==int(val)) targetOutputVals.push_back(1);
        else targetOutputVals.push_back(0.0);
        ++i;
    }
    return targetOutputVals.size();
}
