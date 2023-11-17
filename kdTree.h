//
// Interface definition for the kdTree class
// Author: Max Benson
// Date: 10/27/2021
//

#ifndef KDTREE_H
#define KDTREE_H

#include "Set.h"
#include "TreeNode.h"
#include "Picture.h"

class kdTree {
public:
    kdTree();
    ~kdTree();

    void Insert(const Point& point);
    Set<Point> RangeSearch(const Rectangle& range) const;
    bool NearestNeighbors(const Point& point, Set<Point>& NN) const;

    string DisplayTree() const;
    string DisplayPlanarDecomposition() const;

    // For validation
    Set<Point> SlowRangeSearch(const Rectangle& range) const;
    bool SlowNearestNeighbors(const Point& point, Set<Point>& NN) const;
    Rectangle GetBoundingRectangle() const;

private:
    TreeNode* Insert(TreeNode* tree, const Point& p, bool xDirection);
    void RangeSearch(TreeNode* tree, const Rectangle& range, Set<Point>& points, bool xDirection) const;
    void NearestNeighbors(TreeNode* tree, const Point& p, Set<Point>& NN, float& NNSqrDist, bool xDirection) const;
    void SlowRangeSearch(TreeNode* tree, const Rectangle& range, Set<Point>& points) const;
    void SlowNearestNeighbors(TreeNode* tree, const Point& p, Set<Point>& NN, float& NNSqrDist) const;
    void DisplayPlanarDecomposition(Picture& picture, TreeNode* tree, int xLow, int xHigh, int yLow, int yHigh, bool xDirection) const;
    void GetBoundingRectangle(TreeNode* tree, Rectangle& bounds) const;
    int Height(TreeNode* tree) const;

    TreeNode* _root;
};

#endif //KDTREE_H
