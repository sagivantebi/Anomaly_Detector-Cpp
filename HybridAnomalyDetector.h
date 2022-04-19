

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include <vector>

class HybridAnomalyDetector : public SimpleAnomalyDetector {
private:
    float threshold;
public:
    HybridAnomalyDetector();

    void setCorrelationThreshold(float threshold);

    void HybridLearner(const TimeSeries &ts, float m, int i, int index);

    void learnNormal(const TimeSeries &ts) override;

    std::vector<AnomalyReport> detect(const TimeSeries &ts) override;

    std::vector<Point> getPoints(std::vector<float> v1, std::vector<float> v2);

    void hybridDetection(float threshold, Point p, correlatedFeatures cf, int timeStep,
                         std::vector<AnomalyReport> *anomalies);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
