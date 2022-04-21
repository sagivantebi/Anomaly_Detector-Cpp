/* Authors:
 * Ben Ganon
 * Sagiv Antebi
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include "minCircle.h"
#define THRESHOLD 1.1
#define PEARSON 0.9
struct correlatedFeatures {
    std::string feature1, feature2;// names of the correlated features
    int col1, col2;
    float corrlation;
    Line lin_reg;
    float threshold;
    Circle C;
    bool isHybrid;
};

class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
    //this vector is a dynamic array of correlated features.
protected:
    std::vector<correlatedFeatures>* cf;
public:
    SimpleAnomalyDetector();

    void addCf(correlatedFeatures cf);

    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries &ts, float threshold);

    virtual std::vector<AnomalyReport> detect(const TimeSeries &ts);

    virtual std::vector<correlatedFeatures> getNormalModel();

    float maxDev(std::vector<float> vector1, std::vector<float> vector2, unsigned int size,Line l);

    virtual void simpleLearner(const TimeSeries &ts, float m, int i, int index, std::vector<correlatedFeatures>* featureArray);

    virtual void simpleDetection(float threshold, Point p1, correlatedFeatures cf, int timeStep, std::vector<AnomalyReport>* anomalies);
};


#endif /* SIMPLEANOMALYDETECTOR_H_ */



