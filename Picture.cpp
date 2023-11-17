//
// Implementation for the Picture class used to
// format the planar decomposition
// Author: Max Benson
// Date: 11/03/2021
//

#include <cassert>
#include "Picture.h"

/**
 * Constructor for Picture class
 * Creates string of characters that when displayed will be a rectangle
 * of the size (xHigh-xLow) x (yHigh - yLow) according to information rect
 * Initially it's all blanks except for * except for newline characters at
*  the end of each row.
 * @param rect this is the rectangle specifying the desired size
 */

Picture::Picture(const Rectangle& rect) {
    size_t ncols;
    size_t nrows;

    _rectangle = rect;
    nrows = _rectangle.yHigh - _rectangle.yLow + 1;
    ncols = _rectangle.xHigh - _rectangle.xLow + 1;
    _canvas = string(nrows*(ncols+1), ' ');
    for (int row = _rectangle.yLow; row <= _rectangle.yHigh; row++) {
        SetChar(_rectangle.xHigh+1, row, '\n');
    }
}

/**
 * Using - characters draw a horizontal line from x = xStart to x = xEnd
 * at height y
 * @param xStart - starting coordinate 
 * @param xEnd - ending coordinate
 * @param y - coordinate of row
 */
void Picture::DrawHLine(int xStart, int xEnd, int y) {
    assert(xStart <= xEnd);
    assert(xStart >= _rectangle.xLow && xStart <= _rectangle.xHigh);
    assert(xEnd >= _rectangle.xLow && xEnd <= _rectangle.xHigh);
    assert(y >= _rectangle.yLow && y <= _rectangle.yHigh);

    for (int x = xStart; x <= xEnd; x ++ ) {
        if (IsBlank(x, y)) {
            SetChar(x, y, '-');
        }
    }
}

/**
 * Using | characters draw a verticalk line from y = yStart down to y = yEnd
 * at column x
 * @param yStart - starting coordinate
 * @param yEnd - ending coordinate
 * @param x - coordinate of column
 */
void Picture::DrawVLine(int yStart, int yEnd, int x) {
    assert(yStart <= yEnd);
    assert(yStart >= _rectangle.yLow && yStart <= _rectangle.yHigh);
    assert(yEnd >= _rectangle.yLow && yEnd <= _rectangle.yHigh);
    assert(x >= _rectangle.xLow && x <= _rectangle.xHigh);

    for (int y = yStart; y <= yEnd; y ++ ) {
        if (IsBlank(x, y)) {
            SetChar(x, y,  '|');
        }
    }
}

/**
 * Using a * character mark the location of a point
 * @param x coordinate of column point is located in
 * @param y coordinate of row point is located in
 */
void Picture::MarkPoint(int x, int y) {
    assert(x >= _rectangle.xLow && x <= _rectangle.xHigh);
    assert(y >= _rectangle.yLow && y <= _rectangle.yHigh);
    SetChar(x, y,  '*');
}

/**
 * Put specified character at specified location
 * @param x coordinate of row
 * @param y coordinate of column
 * @param ch character
 */
void Picture::SetChar(int x, int y, char ch) {
    size_t ncols;

    ncols = _rectangle.xHigh - _rectangle.xLow + 2;
    _canvas[ncols*(_rectangle.yHigh-y)+(x-_rectangle.xLow)] = ch;
}

/**
 * Check if specified location is already occupied or just a blank
 * @param x coordinate of row
 * @param y coordinate of column
 * @return true if blank, false if another character present
 */
bool Picture::IsBlank(int x, int y) const {
    size_t ncols;

    ncols = _rectangle.xHigh - _rectangle.xLow + 2;
    return _canvas[ncols*(_rectangle.yHigh-y)+(x-_rectangle.xLow)] == ' ';
}