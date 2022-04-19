/* Authors:
 * Ben Ganon - 318731007
 * Sagiv Antebi - 318159282
 */

#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"

/**
 * simple constructor allocating memory to the cf array
 */
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    this->cf = new std::vector<correlatedFeatures>;

}

/**
 * simple destructor to erase the cf array
 */
SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    for (correlatedFeatures curr: *this->cf) {
        curr.feature1.clear();
        curr.feature2.clear();

    }
    this->cf->clear();
    delete cf;
}

/**
 * learnNormal calculates correlated columns in the given timeseries vie pearson and fills up the cf array
 * @param ts - timeseries with normal data for the detector to learn
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts, float threshold) {
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
        //if we found any correlation above the given pearson threshhold (PEARSON) we enter the columns into cf
        if (-1 != index) {
            if (m >= threshold) {
                simpleLearner(ts, m, i, index, this->cf);
            }
        }
    }
}
/**
 * simpleLearner - the sub function that do the calculations when the threshold is bigger than 0.9
 * insert the correlatedFeature into the given correlatedFeatures
 * @param ts - time series - the columns
 * @param m - max pearson
 * @param i - num of the main column
 * @param index - num of the correlated column
 */
void SimpleAnomalyDetector::simpleLearner(const TimeSeries &ts, float m, int i, int index, std::vector<correlatedFeatures>* featureArray) {
    //remember delete this
    correlatedFeatures correlatedFeature;
    correlatedFeature.corrlation = m;
    //need to change the function operation in timeSeries - need to return std::vector with floats
    correlatedFeature.feature1 = ts.getNameOfRaw(i);
    correlatedFeature.feature2 = ts.getNameOfRaw(index);
    correlatedFeature.col1 = i;
    correlatedFeature.col2 = index;
    //need to add to those two:
    correlatedFeature.lin_reg = linear_reg(ts.getColumn(i), ts.getColumn(index), ts.getColumn(i).size());
    correlatedFeature.threshold = maxDev(ts.getColumn(i), ts.getColumn(index), ts.getColumn(i).size(),
                                         correlatedFeature.lin_reg) * THRESHOLD;
    correlatedFeature.isHybrid = false;
    //correlatedFeature
   featureArray->push_back(correlatedFeature);
}

/**
 * detect iterates over the correlatedd features created in learnNormal,
 * then measures each point's distance from the linear regression line of the two cf's,
 * if a point is farther away than the threshold, detect adds an anomaly report to the std::vector,
 * then returns the std::vector.
 * @param ts - the desired timeseries to find anomalies in
 * @return
 */
std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    std::vector<AnomalyReport> anomalies;
    //iteration over cf's
    for (correlatedFeatures currCf: *cf) {
        //the two columns from the timeseries
        std::vector<float> col1 = ts.getColumn(currCf.col1);
        std::vector<float> col2 = ts.getColumn(currCf.col2);
        //iterating over each line in the columns
        for (int i = 0; i < col1.size(); i++) {
            Point *currP = new Point(col1.at(i), col2.at(i));
            simpleDetection(currCf.threshold, *currP, currCf, i, &anomalies);
            delete currP;
        }
    }
    return anomalies;
}

void SimpleAnomalyDetector::simpleDetection(float threshold, Point p1, correlatedFeatures cf, int timeStep, std::vector<AnomalyReport>* anomalies) {
    //checking if the point is too far away from lin_reg
    if(dev(p1, cf.lin_reg) > threshold) {
        //creating a new anomaly report
        std::string desc = cf.feature1 + "-" + cf.feature2;
        AnomalyReport anomaly = AnomalyReport(desc, timeStep+ 1);
        //pushing into given anomaly vector
        anomalies->push_back(anomaly);
    }
}

/**
 * adds a new correlated feature to the anomaly detector.
 * @param features1
 */
void SimpleAnomalyDetector::addCf(correlatedFeatures features1) {
    this->cf->push_back(features1);
}

/**
 * returns the correlated feature array
 * @return
 */
std::vector<correlatedFeatures> SimpleAnomalyDetector::getNormalModel() {
    return *this->cf;
}

/**
 * calculates the maximum deviation from the linear regression of two given data sets
 * @param std::vector1 - data set 1
 * @param std::vector2 - data set 2
 * @param size - size of the arrays
 * @param l - line to calculate deviation from
 * @return
 */
float SimpleAnomalyDetector::maxDev(std::vector<float> vector1, std::vector<float> vector2, unsigned int size, Line l) {
    float tempMax;
    float maxNum = 0;
    for (int i = 0; i < size; ++i) {
        //the maximum distance from the line
        tempMax = dev(Point(vector1.at(i), vector2.at(i)), l);
        if (maxNum < tempMax) {
            maxNum = tempMax;
        }
    }
    return maxNum;
}

