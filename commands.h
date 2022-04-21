/* Authors:
 * Ben Ganon
 * Sagiv Antebi 
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "timeseries.h"
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}

    void readAndFile(string fileName) {
        ofstream out(fileName);
        string s = "";
        while ((s = read()) != "done\n") {
            out << s << endl;
        }
        out.close();
    }


};


struct blockReport {
    int start;
    int end;
    string description;
    bool tp;
};

struct currentState {
    float threshold;
    vector<AnomalyReport> report;
    vector<blockReport> blkReports;
    int rowNum;

    currentState() {
        threshold = 0.9;
        rowNum = 0;
    }
};


class Command {
protected:
    DefaultIO *io;

public:
    const string description;

    Command(DefaultIO *io, const string description) : io(io), description(description) {}

    virtual void execute(currentState *currentState) = 0;

    virtual ~Command() {}
};


class UploadCSV : public Command {
public:
    UploadCSV(DefaultIO *io) : Command(io, "upload a time series csv file") {}

    virtual void execute(currentState *currState) {
        io->write("Please upload your local train CSV file.\n");
        io->readAndFile("anomalyTrain.csv");
        io->write("Upload complete.\n");
        io->write("Please upload your local test CSV file.\n");
        io->readAndFile("anomalyTest.csv");
        io->write("Upload complete.\n");
    }
};

class Settings : public Command {
public:
    Settings(DefaultIO *io) : Command(io, "algorithm settings") {}

    virtual void execute(currentState *currState) {

        float thresh;
        bool choiceFlag = true;
        while (choiceFlag) {
            io->write("The current correlation threshold is ");
            io->write(currState->threshold);
            io->write("\n");
            io->write("Type a new threshold\n");
            io->read(&thresh);
            if (0 > thresh || thresh > 1) {
                io->write("please choose a value between 0 and 1.\n");
            } else {
                choiceFlag = false;
                currState->threshold = thresh;
            }
        }
    }
};

class Detect : public Command {
public:
    Detect(DefaultIO *io) : Command(io, "detect anomalies") {}

    virtual void execute(currentState *currState) {
        TimeSeries train("anomalyTrain.csv");
        TimeSeries test("anomalyTest.csv");
        HybridAnomalyDetector had;
        currState->rowNum = test.getRowSize();
        had.setCorrelationThreshold(currState->threshold);
        had.learnNormal(train);
        currState->report = had.detect(test);
        blockReport fReport;
        int timeStepTemp = currState->report.at(0).timeStep;
        for (AnomalyReport anom: currState->report) {
            //the first report
            if (timeStepTemp == anom.timeStep) {
                fReport.start = anom.timeStep;
                fReport.end = anom.timeStep;
                fReport.description = anom.description;
                fReport.tp = false;
            }
                //additional report - another block
            else if (timeStepTemp != anom.timeStep - 1) {
                fReport.end = timeStepTemp;
                currState->blkReports.push_back(fReport);
                fReport.start = anom.timeStep;
                fReport.description = anom.description;
                fReport.tp = false;
            }
            timeStepTemp = anom.timeStep;
        }
        fReport.end = timeStepTemp;
        currState->blkReports.push_back(fReport);
        io->write("anomaly detection complete.\n");

    }
};

class Results : public Command {
public:
    Results(DefaultIO *io) : Command(io, "display results") {}

    virtual void execute(currentState *currState) {
        for_each(currState->report.begin(), currState->report.end(), [this](AnomalyReport &anom) {
            io->write(anom.timeStep);
            io->write("\t " + anom.description + "\n");
        });
        io->write("Done.\n");
    }
};


class AnalyzeAnomalies : public Command {
public:
    AnalyzeAnomalies(DefaultIO *io) : Command(io, "upload anomalies and analyze results") {}

    bool isFalseNega(unsigned long start, unsigned long end, currentState *currState) {
        bool fpFlag = true;
        int i = 0;
        for (blockReport report: currState->blkReports) {
            int startBlock = report.start;
            int endbBlock = report.end;
            if (((startBlock <= start && start <= endbBlock) || (startBlock <= end && end <= endbBlock))
                || (start < startBlock && endbBlock < end)) {
                //
                currState->blkReports.at(i).tp = true;
                fpFlag = false;
            }
            i++;
        }
        return fpFlag;
    }
    bool isTruePos(int repStart, int repEnd, int start, int end) {
        return !(repStart > end || repEnd < start);
    }
    /**
     * the return vector is : vector[0] = Positive,  vector[1] = False Positive
     * @param currState
     * @return
     */
    vector<int> PosiCount(currentState *currState) {
        vector<int> sums;
        int sumPosi = 0;
        int sumFalsePosi = 0;
        for (blockReport report: currState->blkReports) {
            if (!report.tp)
                sumFalsePosi++;
            sumPosi++;
        }
        sums.push_back(sumPosi);
        sums.push_back(sumFalsePosi);
        return sums;
    }

    virtual void execute(currentState *currState) {
        io->write("Please upload your local anomalies file.\n");
        string s = "";
        int tpNum = 0;
        int fpNum = 0;
        int N;
        int rowSum = 0;
        float TruePosiRate, FalseAlarmRate;
        vector<blockReport> anomalies;
        while ((s = io->read()) != "done\n") {
            unsigned long dividerIndex = s.find(',');
            string startString = s.substr(0, dividerIndex);
            string endString = s.substr(dividerIndex + 1,s.length() -1);
            unsigned long start = stoi(startString);
            unsigned long end = stoi(endString);
            blockReport newAnom;
            newAnom.start = start;
            newAnom.end = end;
            anomalies.push_back(newAnom);
            rowSum += end - start + 1;
        }

        for(blockReport report : currState->blkReports) {
            for(blockReport exception : anomalies) {
                if(isTruePos(report.start, report.end, exception.start, exception.end)) {
                    tpNum++;
                    break;
                }
            }
            //
        }
        fpNum = currState->blkReports.size() - tpNum;
        io->write("Upload complete.\n");
        N = currState->rowNum - rowSum;
        TruePosiRate = ((int) (1000.0 * tpNum / anomalies.size())) / 1000.0f;
        FalseAlarmRate = ((int) (1000.0 * fpNum / N)) / 1000.0f;
        io->write("True Positive Rate: ");
        io->write(TruePosiRate);
        io->write("\nFalse Positive Rate: ");
        io->write(FalseAlarmRate);
        io->write("\n");
    }
};

class Finish : public Command {
public:
    Finish(DefaultIO *dio) : Command(dio, "exit") {}

    virtual void execute(currentState *currState) {
    }
};


#endif
