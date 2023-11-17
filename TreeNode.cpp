//
// Implements the TreeNode Class
// Author: Max Benson
// Date: 10/27/2021
//

#include <assert.h>
#include "TreeNode.h"

/**
 * Constructor
 * @param x x-coordinate of point
 * @param y y-coordinate of point
 */
TreeNode::TreeNode(int x, int y) {
    _x = x;
    _y = y;
    _left = nullptr;
    _right = nullptr;
}

/**
 * Destructor
 * Frees allocated memory
 */
TreeNode::~TreeNode() {
    delete _left;
    delete _right;
}


