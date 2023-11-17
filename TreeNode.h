//
// Interface Definition for the TreeNode Class
// Author: Max Benson
// Date: 10/27/2021
//
#ifndef TREENODE_H
#define TREENODE_H

#include <iostream>
using std::ostream;
using std::string;
using std::to_string;

class TreeNode {
public:
    TreeNode(int x, int y);
    ~TreeNode();

    int X() const { return _x; };
    int Y() const { return _y; };
    TreeNode *Left() const { return _left; };
    TreeNode *Right() const { return _right; };

    void SetLeft(TreeNode* left) {_left = left;};
    void SetRight(TreeNode* right) {_right = right;};

private:
    int _x;
    int _y;
    TreeNode* _left;
    TreeNode* _right;
};

#endif //TREENODE_H
