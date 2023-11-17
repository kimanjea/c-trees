//
// Interface definition for the Picture class used to
// format the planar decomposition
// Author: Max Benson
// Date: 11/03/2021
//
#ifndef PICTURE_H
#define PICTURE_H

#include <istream>
using std::ostream;
using std::string;
using std::to_string;

struct Point {
    int x;
    int y;

    friend ostream& operator<<(ostream& os, const Point& point) {
        return os << "(" + to_string(point.x) + ", " + to_string(point.y) + ")";
    }
    friend bool operator==(const Point& lhs, const Point& rhs){ return lhs.x == rhs.x && lhs.y == rhs.y; }
    friend bool operator!=(const Point& lhs, const Point& rhs){ return !(lhs == rhs); }
    friend bool operator>(const Point& lhs, const Point& rhs){ return lhs.x > rhs.x || (lhs.x == rhs.x && lhs.y > rhs.y); }
};

struct Rectangle {
    int xLow;
    int xHigh;
    int yLow;
    int yHigh;

    bool Contains (const Point& point) const { return point.x >= xLow && point.x <= xHigh && point.y >= yLow && point.y <= yHigh; }

    friend ostream& operator<<(ostream& os, const Rectangle& rect) {
        os << "[xLow=" + to_string(rect.xLow) + ", xHigh=" + to_string(rect.xHigh);
        return os << ", yLow=" + to_string(rect.yLow) + ", yHigh=" + to_string(rect.yHigh) << "]";
    }
    friend bool operator==(const Rectangle& lhs, const Rectangle& rhs){ return lhs.xLow == rhs.xLow && lhs.xHigh == rhs.xHigh && lhs.yLow == rhs.yLow && lhs.yHigh ==rhs.yHigh; }
    friend bool operator!=(const Rectangle& lhs, const Rectangle& rhs){ return !(lhs == rhs); }
};

class Picture {
public:
    Picture(const Rectangle& rect);

    string ToString() const { return _canvas; }

    void DrawHLine(int xStart, int xEnd, int y);
    void DrawVLine(int yStart, int yEnd, int x);
    void MarkPoint(int x, int y);

private:
    void SetChar(int x, int y, char ch);
    bool IsBlank(int x, int y) const;
    Rectangle _rectangle;
    string _canvas;
};


#endif //PICTURE_H
