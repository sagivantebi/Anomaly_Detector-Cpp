/* Authors:
 * Ben Ganon - 318731007
 * Sagiv Antebi - 318159282
 */
#include "timeseries.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

TimeSeries::TimeSeries(const char *CSVfileName) {
    std::ifstream file;
    //openning the file given
    file.open(CSVfileName);
    //a line string to hold the current line in the file as we iterate
    std::string line;
    //word string to hold the current word
    std::string word;
    //insert the fist line
    getline(file, line);
    std::stringstream stream(line);
    //the first raw , the categories
    std::vector<std::string> firstLine;

    columnNames = new std::vector<std::string>;
    //while loop goes through each word in the line and insert it into columnNames
    while (getline(stream, word, ',')) {
        columnNames->push_back(word);
    }
    table = new std::vector<std::vector<float>*>;
    for(int i = 0; i < columnNames->size(); i++) {
        auto *p = new std::vector<float>;
        table->push_back(p);
    }
    //insert the values
    while (getline(file, line)) {
        std::stringstream floatStream(line);
        int place = 0;
        while (getline(floatStream, word, ',')) {
           this->table->at(place)->push_back(stof(word));
           place++;
        }
    }
    file.close();
}
//test
TimeSeries::~TimeSeries(){
    for(std::string s : *this->columnNames) {
        s.clear();
    }
    this->columnNames->clear();
    delete this->columnNames;
    for(std::vector<float> * v : *this->table) {
        v->clear();
        delete v;
    }
    this->table->clear();
    delete this->table;
}

/**
 * prints the timeseries.
 */
void TimeSeries::printTable() {
    for (std::string s: *columnNames) {
        std::cout << s;
    }
    std::cout << std::endl;
    for (int i = 0; i < this->table->at(0)->size(); i++) {
        for (std::vector<float> *v: *this->table) {
            std::cout << v->at(i) << "    ";
        }
    std::cout << std::endl;
    }
}
/**
 * adds a column to the table - each row will receive a new member at the back
 * @param column - the column vector given to add
 * @param name - name of the column to add to the columnNames vector
 */
void TimeSeries::addColumn(std::vector<float>* column, std::string name) {
    //adding the name to the column vector
    columnNames->push_back(name);
    std::vector<float>* inCol = copyVector(column);
    this->table->push_back(inCol);

}

/**
 * adds a new row to the timeseries.
 * @param values
 */
void TimeSeries::addRow(std::vector<float> values) {
    if (values.size() > this->table->size() || values.size() < this->table->size()) {
        std::cout << "number of columns is mismatched!";
        exit(1);
    }
    for (int i = 0; i < this->table->size(); i++) {
        this->table->at(i)->push_back(values[i]);
    }
}

/**
 * retreives a desired column from the timeseries
 * @param column
 * @return
 */
std::vector<float> TimeSeries::getColumn(int column) const {
    return *this->table->at(column);
}

/**
 * retreives a desired row from the timeseries
 * @param row
 * @return
 */
std::vector<float> TimeSeries::getRow(int row) {
    std::vector<float> f;
    for (std::vector<float>* v : *this->table) {
        f.push_back(v->at(row));
    }
    return f;
}

int TimeSeries::getRowSize() {
    return this->table->at(0)->size() - 1;
}
/**
 * returns the desired cell's value
 * @param row
 * @param column
 * @return
 */
float TimeSeries::getCell(int row, int column) const{
    return this->table->at(column)->at(row);
}

/**
 * sets the desired cell's value to val
 * @param row
 * @param column
 * @param val
 */
void TimeSeries::setCell(int row, int column, float val) {
    this->table->at(column)->at(row) = val;
}

/**
 * copies a given vector (deep copy) and returns it.
 * @param v
 * @return
 */
std::vector<float>* TimeSeries::copyVector(std::vector<float>* v) {
    auto* newV = new std::vector<float>;
    for(int i = 0; i < v->size(); i++) {
        newV->push_back(v->at(i));
    }
    return newV;
}

/**
 * returns the number of columns in the timeseries.
 * @return
 */
int TimeSeries::numColumns() const {
    return this->columnNames->size();
}

std::string TimeSeries::getNameOfRaw(int i) const{
    return this->columnNames->at(i);
}
std::vector<std::string>* TimeSeries::getNames() const {
    return this->columnNames;
}