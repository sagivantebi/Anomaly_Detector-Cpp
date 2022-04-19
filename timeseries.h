/* Authors:
 * Ben Ganon - 318731007
 * Sagiv Antebi - 318159282
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <string>
#include <iostream>


class TimeSeries {
private:
    std::vector<std::vector<float> *> *table;
    std::vector<std::string> *columnNames;
public:
    TimeSeries(const char *CSVfileName);

    ~TimeSeries();

    void printTable();

    void addColumn(std::vector<float> *column, std::string name);

    void addRow(std::vector<float> values);

    std::vector<float> getColumn(int column) const;

    std::vector<float> getRow(int row);

    float getCell(int row, int column) const;

    void setCell(int row, int column, float val);

    std::vector<float> *copyVector(std::vector<float> *v);

    int numColumns() const;

    std::string getNameOfRaw(int i) const;

    std::vector<std::string> * getNames() const;

    int getRowSize();
};


#endif /* TIMESERIES_H_ */