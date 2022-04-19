/* Authors:
 * Ben Ganon - 318731007
 * Sagiv Antebi - 318159282
 */

#include <math.h>
#include <cmath>
#include "anomaly_detection_util.h"
#include <vector>


/**
 * avg - calculate the avg of the x array numbers
 * @param x  - the float array
 * @param size  - the size of the array
 * @return the avg
 */
float avg(float *x, int size) {
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += x[i];
    }
    return sum / size;
}

/**
 * multiply array x by array y, one cell at a time-
 * x1*y1, x2*y2, etc.
 * @param x - float array
 * @param y - float array
 * @param size - size of both arrays
 * @return - new array z that contains the multiplication of each cell
 */
float *multiplyArray(float *x, float *y, int size) {
    float *multi;
    multi = new float[size];
    for (int i = 0; i < size; i++) {
        multi[i] = x[i] * y[i];
    }
    return multi;

}
// returns the variance of X and Y
/**
 * var -  returns the variance of X and Y
 * @param x  - the float array
 * @param size  - the size of the array
 * @return - the variance of X and Y
 */
float var(float *x, int size) {
    //the mean of the group
    float mean = avg(x, size);
    //the sum of all the SquaredDifference
    float sumDif = 0;
    for (int i = 0; i < size; ++i) {
        sumDif += pow((x[i] - mean), 2);
    }
    //return the variance
    return sumDif / size;
}
/**
 * returns the covariance of X and Y
 * @param x
 * @param y
 * @param size
 * @return - float representing the covariance of x and y
 */
float cov(float *x, float *y, int size) {
    //average of array x
    float avgX = avg(x, size);
    //average of array y
    float avgY = avg(y, size);
    //average of the two arrays multiplied
    float *multi = multiplyArray(x, y, size);
    float avgXY = avg(multi, size);
    delete[]multi;
    //subtraction to give the final result
    return avgXY - avgX * avgY;
}

/**
 * returns the Pearson correlation coefficient of X and Y
 * @param x - float array
 * @param y - float array
 * @param size - size of both arrays
 * @return - returns (float) pearson correlation coefficient
 */
float pearson(float *x, float *y, int size) {
    return cov(x, y, size) / (sqrt(var(x, size)) * sqrt(var(y, size)));
}

/**
 * linear_reg- performs a linear regression and returns the line equation
 * @param points - array of points
 * @param size - size of the array of points
 * @return Line - returns the line equation from the linear regression operation on the given points
 */
Line linear_reg(Point **points, int size) {
    //all the x,y points in arrays
    float xPoints[size];
    float yPoints[size];
    //for loop to insert the points value into the array accordingly
    for (int i = 0; i < size; ++i) {
        xPoints[i] = points[i]->x;
        yPoints[i] = points[i]->y;
    }
    //𝑎=𝐶𝑂𝑉(𝑥,𝑦)/𝑉𝐴𝑅(𝑥)
    float a = cov(xPoints, yPoints, size) / var(xPoints, size);
    //𝑏=𝑦̅−𝑎𝑥̅ (𝑥̅ and 𝑦̅ are the avg of x and y)
    float b = avg(yPoints, size) - a * avg(xPoints, size);
    return Line(a, b);
}

/**
 * linear_reg- performs a linear regression and returns the line equation
 * @return Line - returns the line equation from the linear regression operation on the given points
 */
Line linear_reg(std::vector<float> v1, std::vector<float> v2, int size){
    float* feature1 = v1.data();
    float* feature2= v2.data();
    //𝑎=𝐶𝑂𝑉(𝑥,𝑦)/𝑉𝐴𝑅(𝑥)
    float a = cov(feature1, feature2, size) / var(feature1, size);
    //𝑏=𝑦̅−𝑎𝑥̅ (𝑥̅ and 𝑦̅ are the avg of x and y)
    float b = avg(feature2, size) - a * avg(feature1, size);
    return Line(a, b);

}
/**
 * dev - returns the deviation between point p and the line equation of the points
 * @param p - point to measure the distance to
 * @param points - array of points to create the line to measure the distance from the point
 * @param size - num of the points
 * @return the deviation between point p and the line equation of the points
 */
float dev(Point p, Point **points, int size) {
    Line l = linear_reg(points, size);
    //calculate the y value of the line l
    float lineYValue = l.f(p.x);
    //return the absolute value of their distance difference
    return std::abs(p.y - lineYValue);
}
/**
 * dev - returns the deviation between point p and the line equation of the points
 * @param p - point to measure the distance to the line
 * @param l - line to measure the distance from the point
 * @return the deviation between point p and the line equation of the points
 */
float dev(Point p, Line l) {
    //calculate the y value of the line l
    float lineYValue = l.f(p.x);
    //return the absolute value of their distance difference
    return std::abs(p.y - lineYValue);
}


