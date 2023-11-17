//
// Lab 6 Main program
// Author: Max Benson
// Date: 10/27/2021
//

#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::cin;
using std::stringstream;
using std::ifstream;
using std::endl;

#include "kdTree.h"

void PrintHelp();
bool IsNumber(const string& token);
bool TestNNSearch(const kdTree& kd, const Point& p, Set<Point>& NN);
bool TestRangeSearch(const kdTree& kd, const Rectangle& range, Set<Point>& points);

int main() {
    kdTree kd;

    PrintHelp();
    while (true) {
        string line;
        string command;
        int key;

        cout << "Enter Command (H for help): ";
        getline(cin, line);
        stringstream ss(line);
        ss >> command;
        if (command == "I") {
            string numberString;
            Point point;

            // X coordinate
            ss >> numberString;
            if (!IsNumber(numberString)) {
                cout << "Expecting number for x-coordinate" << endl;
                continue;
            }
            point.x = stoi(numberString);

            // Y coordinate
            ss >> numberString;
            if (!IsNumber(numberString)) {
                cout << "Expecting number for y-coordinate" << endl;
                continue;
            }
            point.y = stoi(numberString);

            kd.Insert(point);
        }
        else if (command == "L") {
            ifstream fs;
            string fileName;
            bool fileError = false;

            ss >> fileName;
            fs.open(fileName);
            if (!fs.is_open()) {
                cout << "Can't open file" << endl;
                continue;
            }
            while (fs) {
                Point point;

                fs >> point.x;
                fs >> point.y;
                kd.Insert(point);
            }
            fs.close();
        }
        else if (command == "N") {
            string numberString;
            Point point;
            Set<Point> nn;

            // X coordinate
            ss >> numberString;
            if (!IsNumber(numberString)) {
                cout << "Expecting number for x-coordinate" << endl;
                continue;
            }
            point.x = stoi(numberString);

            // Y coordinate
            ss >> numberString;
            if (!IsNumber(numberString)) {
                cout << "Expecting number for y-coordinate" << endl;
                continue;
            }
            point.y = stoi(numberString);

            if (TestNNSearch(kd, point, nn)) {
                cout << "Nearest Neighbor = " << nn << endl;
            }
        }
        else if (command == "R") {
            string numberString;
            Rectangle range;
            Set<Point> points;

            // xLow
            ss >> numberString;
            if (!IsNumber(numberString)) {
                cout << "Expecting number for xLow" << endl;
                continue;
            }
            range.xLow = stoi(numberString);

            // xHigh
            ss >> numberString;
            if (!IsNumber(numberString)) {
                cout << "Expecting number for xHigh" << endl;
                continue;
            }
            range.xHigh = stoi(numberString);

            // yLow
            ss >> numberString;
            if (!IsNumber(numberString)) {
                cout << "Expecting number for yLow" << endl;
                continue;
            }
            range.yLow = stoi(numberString);

            // yHigh
            ss >> numberString;
            if (!IsNumber(numberString)) {
                cout << "Expecting number for xHigh" << endl;
                continue;
            }
            range.yHigh = stoi(numberString);

            if (TestRangeSearch(kd, range, points)) {
                cout << "Range Search returns " << points << endl;
            }
        }
        else if (command == "D") {
            cout << kd.DisplayPlanarDecomposition();
        }
        else if (command == "P") {
            cout << kd.DisplayTree();
        }
        else if (command == "V") {
            Rectangle bounds = kd.GetBoundingRectangle();
            bool NNSucceeds = true;
            bool RSSucceeds = true;

            for (int x = bounds.xLow-10; x <= bounds.xHigh+10; x ++) {
                for (int y = bounds.yLow - 10; y <=  bounds.yHigh + 1; y++) {
                    Point point = {x, y};
                    Set<Point> nn;

                    if (!TestNNSearch(kd, point, nn)) {
                        NNSucceeds = false;
                        break;
                    }
                }
            }
            if (NNSucceeds) {
                cout << "Validation for Nearest Neighbor Search succeeds" << endl;
            }
            else {
                cout << "Validation for Nearest Neighbor Search fails" << endl;
            }
            for (int xLow = bounds.xLow-10; xLow <= bounds.xHigh+10; xLow ++) {
                for (int xHigh = xLow; xHigh <= bounds.xHigh + 10; xHigh++) {
                    for (int yLow = bounds.yLow - 10; yLow <= bounds.yHigh + 10; yLow++) {
                        for (int yHigh = yLow; yHigh <= bounds.yHigh + 10; yHigh++) {
                            Rectangle range = {xLow, xHigh, yLow, yHigh};
                            Set<Point> points;

                            if (!TestRangeSearch(kd, range, points)) {
                                RSSucceeds = false;
                                break;
                            }
                        }
                    }
                }
            }
            if (RSSucceeds) {
                cout << "Validation for Range Search succeeds" << endl;
            }
            else {
                cout << "Validation for Range Search fails" << endl;
            }
        }
        else if (command == "H") {
            PrintHelp();
        }
        else if (command == "Q") {
            break;
        }
        else {
            cout << "Unrecognized Command" << endl;
        }
    }
    return 0;
}

void PrintHelp() {
    cout << "Commands:" << endl;
    cout << "- I x y                        insert the point (x,y) into the tree" << endl;
    cout << "- L <filename>                 load points from file" << endl;
    cout << "- P                            print the tree" << endl;
    cout << "- D                            display planar decomposition" << endl;
    cout << "- R xLow xHigh yLow yHigh      search for points in range" << endl;
    cout << "- N x y                        find nearest neighbor to (x, y) in tree" << endl;
    cout << "- V                            validate nearest neighbor and range search" << endl;
    cout << "- H                            print this help" << endl;
    cout << "- Q                            quit"  << endl;
}

bool IsNumber(const string& token) {
    size_t i = 0;

    if (token.length() == 0) {
        return false;
    }
    if (token[0] == '-' && token.length() > 1) {
        i++;
    }
    for (; i < token.length(); i ++) {
        if (!isdigit(token[i]) ) {
            return false;
        }
    }
    return true;
}

bool TestNNSearch(const kdTree& kd, const Point& p, Set<Point>& NN) {
    Set<Point> SlowNN;

    if (!kd.NearestNeighbors(p, NN) ) {
        cout << "NearestNeighbors fails for P = " << p << endl;
        return false;
    }
    if (!kd.SlowNearestNeighbors(p, SlowNN) ) {
        cout << "SlowNearestNeighbors fails for P = " << p << endl;
        return false;
    }
    if (NN != SlowNN) {
        cout << "NearestNeighbors and SlowNearestHeighbors return different points for P = " << p << endl;
        cout << "NearestNeighbors returned " << NN << endl;
        cout << "SlowNearestNeighbors returned " << SlowNN << endl;
        return false;
    }
    return true;
}

bool TestRangeSearch(const kdTree& kd, const Rectangle& range, Set<Point>& points) {
    Set<Point> pointsSlow;

    points = kd.RangeSearch(range);
    pointsSlow = kd.SlowRangeSearch(range);
    if (points != pointsSlow) {
        cout << "RangeSearch and SlowRangeSearch return different sets for range = " << range << endl;
        cout << "RangeSearcj returned " << points << endl;
        cout << "SlowRangeSearch returned " << pointsSlow << endl;
        return false;
    }
    return true;
}
