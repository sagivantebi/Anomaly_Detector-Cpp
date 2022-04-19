
#include "HybridAnomalyDetector.h"
#include "minCircle.h"

#define HYBRID_PEARSON 0.5
#define PEARSON 0.9


HybridAnomalyDetector::HybridAnomalyDetector() {
    this->threshold = 0.9;
}

//TODO:: need to change this function name
void HybridAnomalyDetector::setCorrelationThreshold(float newThresh){
    this->threshold = newThresh;
}

void HybridAnomalyDetector::HybridLearner(const TimeSeries &ts, float m, int i, int index){
    correlatedFeatures correlatedFeature;
    correlatedFeature.corrlation = m;
    //need to change the function operation in timeSeries - need to return std::vector with floats
    correlatedFeature.feature1 = ts.getNameOfRaw(i);
    correlatedFeature.feature2 = ts.getNameOfRaw(index);
    correlatedFeature.col1 = i;
    correlatedFeature.col2 = index;
    std::vector<float> column1 = ts.getColumn(i);
    std::vector<float> column2 = ts.getColumn(index);
    correlatedFeature.C  = callMinCircle(getPoints(column1, column2));
    correlatedFeature.threshold = correlatedFeature.C.radius * THRESHOLD;
    correlatedFeature.isHybrid = true;
    this->cf->push_back(correlatedFeature);
}


void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    float m, p;
    int index;
    //iterating over the columns
    for (int i = 0; i < ts.numColumns(); i++) {
        //current max pearson coefficient
        m = 0;
        //current index of the coefficient column
        index = -1;
        //iterating over the rest of the columns
        for (int j = i + 1; j < ts.numColumns(); j++) {
            //calculating pearson of i and j
            p = pearson(ts.getColumn(i).data(), ts.getColumn(j).data(), ts.getColumn(i).size());
            if (p > m) {
                m = p;
                index = j;
            }
        }
            if (-1 != index) {
                if (m >= this->threshold) {
                    HybridAnomalyDetector::simpleLearner(ts, m, i, index, this->cf);
                }
                else if (m >= HYBRID_PEARSON) {
                    HybridAnomalyDetector::HybridLearner(ts, m, i, index);
                }
            }
        }
    }


std::vector<Point> HybridAnomalyDetector::getPoints(std::vector<float> v1, std:: vector<float> v2) {
    std::vector<Point> PointArray;
    for (int i = 0; i < v1.size(); ++i) {
        Point currPoint = Point(v1.at(i), v2.at(i));
        PointArray.push_back(currPoint);
    }
    return PointArray;
}

std::vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
    std::vector<AnomalyReport> anomalies;
    //iteration over cf's
    for (correlatedFeatures currCf: *this->cf) {
        //the two columns from the timeseries
        std::vector<float> col1 = ts.getColumn(currCf.col1);
        std::vector<float> col2 = ts.getColumn(currCf.col2);
        std::vector<Point> points = getPoints(col1, col2);
        //iterating over each line in the columns
        int i = 0;
        for(Point currP : points) {
            if(currCf.isHybrid) {
                HybridAnomalyDetector::hybridDetection(currCf.threshold, currP, currCf, i, &anomalies);
            }
            else {
                SimpleAnomalyDetector::simpleDetection(currCf.threshold, currP, currCf, i, &anomalies);
            }
            i++;
        }
    }
    return anomalies;
}

void HybridAnomalyDetector::hybridDetection(float threshold, Point p, correlatedFeatures cf, int timeStep, std::vector<AnomalyReport>* anomalies) {
    Point cfCircleCenter = cf.C.center;
    if(dist(p, cfCircleCenter) > threshold) {
        std::string desc = cf.feature1 + "-" + cf.feature2;
        AnomalyReport anomaly = AnomalyReport(desc, timeStep+ 1);
        //pushing into given anomaly vector
        anomalies->push_back(anomaly);
    }
}


