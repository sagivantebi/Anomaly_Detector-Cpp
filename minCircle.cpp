/* Authors:
 * Ben Ganon
 * Sagiv Antebi
 */
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "anomaly_detection_util.h"
#include "minCircle.h"

using namespace std;

// Defining infinity
const float INF = 1e18;




/**
 * dist - return the distance between two points
 * @param pointA
 * @param pointB
 * @return return the euclidean distance between the two points
 */
float dist(const Point &pointA, const Point &pointB) {
    return sqrt(pow(pointA.x - pointB.x, 2)
                + pow(pointA.y - pointB.y, 2));
}

/**
 * point_in_circle -  checks if the point is inside the circle
 * @param c - circle
 * @param p - point
 * @return true if the point is inside the circle | false otherwise
 */
bool point_in_circle(const Circle &c, const Point &p) {
    return dist(c.center, p) <= c.radius;
}

/**
 * get_circle_center - return the circle center
 * @param bx
 * @param by
 * @param cx
 * @param cy
 * @return point indicates the circle center
 */
Point get_circle_center(float bx, float by,
                        float cx, float cy) {
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    return {(cy * B - by * C) / (2 * D),
            (bx * C - cx * B) / (2 * D)};
}

/**
 * circle_from - a unique circle that intersects three points
 * @param pointA - first point
 * @param pointB - second point
 * @param pointC - third point
 * @return circle that intersects three points
 */
Circle circle_from(const Point &pointA, const Point &pointB,
                   const Point &pointC) {
    Point I = get_circle_center(pointB.x - pointA.x, pointB.y - pointA.y,
                                pointC.x - pointA.x, pointC.y - pointA.y);

    I.x += pointA.x;
    I.y += pointA.y;
    return {I, dist(I, pointA)};
}


/**
 * circle_from - return the smallest circle that intersects 2 points
 * @param pointA - first point
 * @param pointB - second point
 * @return the smallest circle that intersects 2 points
 */
Circle circle_from(const Point &pointA, const Point &pointB) {
    Point C = {static_cast<float>((pointA.x + pointB.x) / 2.0), static_cast<float>((pointA.y + pointB.y) / 2.0)};
    return {C, static_cast<float>(dist(pointA, pointB) / 2.0)};
}

/**
 * is_valid_circle - check whether a circle encloses the given points
 * @param c - circle
 * @param P - point
 * @return true if the circle encloses the given points
 */
bool is_valid_circle(const Circle &c,
                     const vector<Point> &P) {
    for (const Point &p: P)
        if (!point_in_circle(c, p))
            return false;
    return true;
}

/**
 * minCircleTrivial
 * @param P  - vector of points
 * @return the minimum enclosing
 */
Circle minCircleTrivial(vector<Point> &P) {
    assert(P.size() <= 3);
    if (P.empty()) {
        return {{0, 0}, 0};
    } else if (P.size() == 1) {
        return {P[0], 0};
    } else if (P.size() == 2) {
        return circle_from(P[0], P[1]);
    }
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle c = circle_from(P[i], P[j]);
            if (is_valid_circle(c, P))
                return c;
        }
    }
    return circle_from(P[0], P[1], P[2]);
}

/**
 * minCircleHelper - Takes a set of input points P and a set R points on the circle boundary.
 * n represents the number of points in P that are not yet processed.
 * @param P - vector of points
 * @param R - vector of points
 * @param n
 * @return Returns the MEC using Welzl's algorithm
 */
Circle minCircleHelper(vector<Point> &P,
                       vector<Point> R, int n) {
    if (n == 0 || R.size() == 3) {
        return minCircleTrivial(R);
    }
    int idx = rand() % n;
    Point p = P[idx];
    swap(P[idx], P[n - 1]);
    Circle d = minCircleHelper(P, R, n - 1);
    if (point_in_circle(d, p)) {
        return d;
    }
    R.push_back(p);
    return minCircleHelper(P, R, n - 1);
}

/**
 * callMinCircle - organize the things for minCircleHelper and call it
 * @param P - vector of points
 * @return the min circle
 */
Circle callMinCircle(const vector<Point> &P) {
    vector<Point> P_copy = P;
    random_shuffle(P_copy.begin(), P_copy.end());
    return minCircleHelper(P_copy, {}, P_copy.size());
}

/**
 * findMinCircle - main function to call to get the min circle
 * @param points - array of points
 * @param size - size of array
 * @return the min circle of the points
 */
Circle findMinCircle(Point **points, std::size_t size) {
    const vector<Point> P = PointArrayToVector(points, size);
    return callMinCircle(P);
}

const std::vector<Point> PointArrayToVector(Point** points, size_t size) {
    std::vector<Point> vectorPoints;
    vectorPoints.reserve(size);
    for (int i = 0; i < size; ++i) {
        vectorPoints.push_back(*points[i]);
    }
    return vectorPoints;
}
