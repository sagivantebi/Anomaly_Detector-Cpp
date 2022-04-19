

#ifndef ANOMALYDETECTOR_H_
#define ANOMALYDETECTOR_H_

#include <string>
#include <vector>
#include "timeseries.h"
#include "math.h"


class AnomalyReport{
public:
	const std::string description;
	const long timeStep;
	AnomalyReport(std::string description, long timeStep):description(description),timeStep(timeStep){}
};

class TimeSeriesAnomalyDetector {
public:
	virtual void learnNormal(const TimeSeries& ts)=0;
	virtual std::vector<AnomalyReport> detect(const TimeSeries& ts)=0;
	virtual ~TimeSeriesAnomalyDetector(){}
};

#endif /* ANOMALYDETECTOR_H_ */
