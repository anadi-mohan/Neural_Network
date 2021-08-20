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

    unsigned getNextInputs(vector<float>& inputVals);
    unsigned getTargetOutputs(vector<float>& targetOutputVals);

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
        abort();
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

unsigned TrainingData::getNextInputs(vector<float>& inputVals)
{
    inputVals.clear();
    string line;
    getline(m_trainingDataFile, line);
    stringstream ss(line);

    string label;
    ss >> label;
    if (label.compare("in:") == 0) {
        float oneValue;
        while (ss >> oneValue) {
            inputVals.push_back(oneValue);
        }
    }
    return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(vector<float>& targetOutputVals)
{
    targetOutputVals.clear();

    string line;
    getline(m_trainingDataFile, line);
    stringstream ss(line);

    string label;
    ss >> label;
    if (label.compare("out:") == 0) {
        float oneValue;
        while (ss >> oneValue) {
            targetOutputVals.push_back(oneValue);
        }
    }
    return targetOutputVals.size();
}
