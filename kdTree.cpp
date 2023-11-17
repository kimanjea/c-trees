//
// Implementation for the kdTree class
// Author: Max Benson
// Date: 10/27/2021
// Modified by:
// Modification date:
//

#include <cassert>
#include <cfloat>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
using std::cout;
using std::endl;
using std::setw;
using std::string;
using std::stringstream;
using std::ostream;

#include "VariableArrayList.h"
#include "Queue.h"
#include "Picture.h"
#include "kdTree.h"

/**
 * Default constructor
 * Creates an "null tree"
 */
kdTree::kdTree() {
    _root = nullptr;
}

/**
 * Destructor
 * Frees the dynamic memory allocated for the tree
 */
kdTree::~kdTree() {
    delete _root;
}

/**
 * Insert a point into the kdTree
 * @param p a point
 */
void kdTree::Insert(const Point& p) {
   _root=Insert(this->_root,p,true);

}

/**
 * Find the set of points lying within a rectangular region
 * @param range rectangle to search
 * @return set of points within that rectangle
 */
Set<Point> kdTree::RangeSearch(const Rectangle& range) const {
	Set<Point> spots;
	if(nullptr!=_root){
	    RangeSearch(_root,range,spots,true);

	}
	return spots;
}

/**
 * Find the nearest neighbor to a point in the kdTree
 * Will always find a neighbor except for an empty tree
 * @param p a point to find the nearest neighbor of
 * @param NN upon return will contain the nearest neighbor(s)
 * @return true except when the tree is empty, then returns false
 */
bool kdTree::NearestNeighbors(const Point& p, Set<Point>& NN) const {
    if (nullptr == _root) {
        return false;
    }
    else {
        float SqrDist = -1;
        NearestNeighbors(_root, p, NN, SqrDist,true);
        return true;
    }
}

/**
 * Find the set of points lying within a rectangular region
 * using an inefficient method that visits every tree node
 * @param range rectangle to search
 * @return set of points within that rectangle
 */
Set<Point> kdTree::SlowRangeSearch(const Rectangle& range) const {
    Set<Point> points;

    if (nullptr != _root) {
        SlowRangeSearch(_root, range, points);
    }
    return points;
}

/**
 * Find the nearest neighbor to a point in the kdTree in
 * an inefficient way by visiting EVERY tree node
 * Will always find a neighbor except for an empty tree
 * @param p a point to find the nearest neighbor of
 * @param NN the set of nearest neighbors
 * @return true except when the tree is empty, then returns false
 */
bool kdTree::SlowNearestNeighbors(const Point& p, Set<Point>& NN) const {
    if (nullptr == _root) {
        return false;
    }
    else {
        float NNSqrDist = FLT_MAX;

        SlowNearestNeighbors(_root, p, NN, NNSqrDist);
        return true;
    }
}

/**
 * Get bounding rectangle for points in tree
 * @return rectangle
 * @return
 */
Rectangle kdTree::GetBoundingRectangle() const {
    Rectangle bounds = {0, 0, 0, 0};

    if (nullptr != _root) {
        GetBoundingRectangle(_root, bounds);
    }
    return bounds;
}

/**
 * Produce a string representation of the kdTree
 * @return string
 */
string kdTree::DisplayTree() const {
    string s;

    if (_root != nullptr) {
        int levelCur = 1;
        int height = Height(_root);
        string blanks(5*((int)pow(2, height-levelCur)-1), ' ');
        struct LevelNode {
            TreeNode *tree;
            int level;
        };
        LevelNode levelNode = {_root, levelCur};
        Queue<LevelNode> queue;

        // Level order traversal
        queue.Enqueue(levelNode);
        while (!queue.IsEmpty()) {
            LevelNode levelNode = queue.Dequeue();
            int level = levelNode.level;
            TreeNode *tree = levelNode.tree;

            // When we reach a new level emit end of line
            if (levelCur < level) {
                s += "\n";
                levelCur++;
                blanks = string(5*((int)pow(2, height-levelCur)-1), ' ');
            }

            // If there is actually a node here display it, else insert blanks
            s += blanks;
            if (tree != nullptr) {
                stringstream ss;

                ss << "(" << setw(3) << tree->X();
                ss << "," << setw(3) << tree->Y();
                ss << ")";
                s += ss.str();
            } else {
                s += string(9, ' ');
            }
            s += blanks + " ";

            // As long as we haven't reached our depth, enquue next level
            if (levelCur < height) {
                LevelNode levelNodeLeft = {nullptr, level + 1};
                LevelNode levelNodeRight = { nullptr, level + 1 };
                if (tree != nullptr) {
                    levelNodeLeft.tree = tree->Left();
                    levelNodeRight.tree = tree->Right();
                }
                queue.Enqueue(levelNodeLeft);
                queue.Enqueue(levelNodeRight);
            }
        }
    }
    s += "\n";
    return s;
}

/**
 * Produce a string representation of the planar
 * decomposition produced by the kdTree
 * @return string
 */
string kdTree::DisplayPlanarDecomposition() const {
    if (nullptr == _root) {
        string empty;

        return empty;
    } else {
        Rectangle bounds = {_root->X(), _root->X(), _root->Y(), _root->Y()};
        GetBoundingRectangle(_root, bounds);
        Picture picture(bounds);
        DisplayPlanarDecomposition(picture, _root, bounds.xLow, bounds.xHigh, bounds.yLow, bounds.yHigh, true);
        return picture.ToString();
    }
}

/**
 * Recursively walk down tree to find place to insert the point
 * @param  tree pointer to root of subtree where insertion occurs
 * @param  p point to insert
 * @param  xDirection if true compare in the x-direction, else compare in y direciton
 * @return updated root of this subtree
 */
TreeNode* kdTree::Insert(TreeNode* tree, const Point& p, bool xDirection) {
	if(tree==nullptr){
	    TreeNode* nueNode=new TreeNode(p.x,p.y);
	    return nueNode;
	}
	Point peeNode={tree->X(),tree->Y()};
	if(p==peeNode){
	    return tree;
	}
	if(xDirection){
	    if(tree->X()>=p.x){
	        tree->SetLeft(Insert(tree->Left(),p,!xDirection));
	    } else if ( tree->X()<p.x){
	        tree->SetRight(Insert(tree->Right(),p,!xDirection));
	    }
	} else {
	    if(tree->Y()>=p.y){
	        tree->SetLeft(Insert(tree->Left(),p,!xDirection));
	    } else if(tree->Y()<p.y){
	        tree->SetRight(Insert(tree->Right(),p,!xDirection));
	    }
	}
	return tree;
}

/**
 * Recursively walk down tree to perform range search
 * @param  tree pointer to root of subtree to range search
 * @param range rectangle to search
 * @param points set of points, any found in this subtree will be added to set
 * @param  xDirection if true compare in the x-direction, else compare in y direciton
 */
void kdTree::RangeSearch(TreeNode* tree, const Rectangle& range, Set<Point>& points, bool xDirection) const {
	if(tree!= nullptr){
	    Point R ={tree->X(),tree->Y()};
	    if(range.Contains(R)){
	        points.Add(R);
	    }
        RangeSearch(tree->Left(),range,points,!xDirection);
        RangeSearch(tree->Right(),range,points,!xDirection);

	}
}

/**
 * Recursively walk down tree to find nearest neighbor to a point
 * @param  tree pointer to root of subtree to explore
 * @param  p point to find nearest neighbor of
 * @param  NN if better neighbor(s) are found on this traversal will be added to set
 * @param  NNSqrDist updated with square of the distance to closest neighboring point found so far
 * @param  xDirection if true compare in the x-direction, else compare in y direciton
 */
void kdTree::NearestNeighbors(TreeNode* tree, const Point& p, Set<Point>& NN, float& NNSqrDist, bool xDirection) const {
    if (tree != nullptr) {
        float xDistance = (float) (tree->X() - p.x);
        float yDistance = (float) (tree->Y() - p.y);
        float sqrDist = xDistance * xDistance + yDistance * yDistance;
        if (sqrDist <= NNSqrDist || NNSqrDist < 0) {
            Point closePoint;
            closePoint.x = tree->X();
            closePoint.y = tree->Y();
            if (sqrDist < NNSqrDist || NNSqrDist < 0) {
                NN.Clear();
                NNSqrDist = sqrDist;
            }
            NN.Add(closePoint);
        }
        if (xDirection) {
            if (tree->X() <= p.x) {
                NearestNeighbors(tree->Left(), p, NN, NNSqrDist, !xDirection);
            }
            if (sqrDist < NNSqrDist) {
                NearestNeighbors(tree->Right(), p, NN, NNSqrDist, !xDirection);
            } else {
                NearestNeighbors(tree->Right(), p, NN, NNSqrDist, !xDirection);
            }
            if (sqrDist < NNSqrDist) {
                NearestNeighbors(tree->Left(), p, NN, NNSqrDist, !xDirection);
            } else if (tree->Y() <= p.y) {
                NearestNeighbors(tree->Left(), p, NN, NNSqrDist, !xDirection);
            }
            if (sqrDist < NNSqrDist) {
                NearestNeighbors(tree->Right(), p, NN, NNSqrDist, !xDirection);
            } else {
                NearestNeighbors(tree->Right(), p, NN, NNSqrDist, !xDirection);
            }
            if (sqrDist < NNSqrDist) {
                NearestNeighbors(tree->Left(), p, NN, NNSqrDist, !xDirection);
            }
        }
        NearestNeighbors(tree->Left(), p, NN, NNSqrDist, !xDirection);
        NearestNeighbors(tree->Right(), p, NN, NNSqrDist, !xDirection);
    }
}


/**
 * Recursively walk down tree to perform range search
 * inefficiently visiting every node of the kdTree
 * @param  tree pointer to root of subtree to range search
 * @param range rectangle to search
 * @param points set of points, any found in this subtree will be added to set
 */
void kdTree::SlowRangeSearch(TreeNode* tree, const Rectangle& range, Set<Point>& points) const {
    if (tree != nullptr) {
        // Check point stored in node
        Point P = { tree->X(), tree->Y() };
        if (range.Contains(P)) {
            points.Add(P);
        }

        // Check subtrees
        SlowRangeSearch(tree->Left(), range, points);
        SlowRangeSearch(tree->Right(), range, points);
    }
}

/**
 * Recursively walk down tree to find nearest neighbor to a point
 * @param  tree pointer to root of subtree to explore
 * @param  p point to find nearest neighbor of
 * @param  NN if better neighbor(s) are found on this traversal will be added to set
 * @param  NNSqrDist updated with square of the distance to closest neighboring point found so far
 */
void kdTree::SlowNearestNeighbors(TreeNode* tree, const Point& p, Set<Point>& NN, float& NNSqrDist) const {
    if (tree != nullptr) {
        // Check if this point is the nearest neighbor
        float xDist = (float)(tree->X() - p.x);
        float yDist = (float)(tree->Y() - p.y);
        float sqrDist =  xDist*xDist + yDist*yDist;
        if (sqrDist <= NNSqrDist) {
            Point nearPoint;

            nearPoint.x = tree->X();
            nearPoint.y = tree->Y();
            if (sqrDist < NNSqrDist) {
                NN.Clear();
                NNSqrDist  = sqrDist;
            }
            NN.Add(nearPoint);
        }

        // Check subtreds
        SlowNearestNeighbors(tree->Left(), p, NN, NNSqrDist);
        SlowNearestNeighbors(tree->Right(), p, NN, NNSqrDist);
    }
}

/**
 * Plot a picture of the kdTree recursively
 * @param picture - picture object
 * @param tree - pointer to tree node
 * @param xLow - left side of box
 * @param xHigh - right side of box
 * @param yLow - bottom side of box
 * @param yHigh - top side of box
 * @param xDirection - whether current node is querying in x direction or not
 */
void kdTree::DisplayPlanarDecomposition(Picture& picture, TreeNode* tree, int xLow, int xHigh, int yLow, int yHigh, bool xDirection) const {
    if (tree != nullptr) {
        if (xDirection) {
            picture.DrawVLine(yLow, yHigh, tree->X());
            picture.MarkPoint(tree->X(), tree->Y());
            DisplayPlanarDecomposition(picture, tree->Left(), xLow, tree->X(), yLow, yHigh, !xDirection);
            DisplayPlanarDecomposition(picture, tree->Right(), tree->X(), xHigh, yLow, yHigh, !xDirection);
        } else {
            picture.DrawHLine(xLow, xHigh, tree->Y());
            picture.MarkPoint(tree->X(), tree->Y());
            DisplayPlanarDecomposition(picture, tree->Left(), xLow, xHigh, yLow, tree->Y(), !xDirection);
            DisplayPlanarDecomposition(picture, tree->Right(), xLow, xHigh, tree->Y(), yHigh, !xDirection);
        }
    }
}

/**
 * Recursive find bounding rectangle
 * @param tree - pointer to tree node
 * @param bounds - rectangle the limits of this are updated as we walk down tree
 */
void kdTree::GetBoundingRectangle(TreeNode *tree, Rectangle& bounds) const {
    assert(tree != nullptr);

    if (tree->X() < bounds.xLow) {
        bounds.xLow= tree->X();
    }
    else if (tree->X() > bounds.xHigh) {
        bounds.xHigh = tree->X();
    }
    if (tree->Y() < bounds.yLow) {
        bounds.yLow = tree->Y();
    }
    else if (tree->Y() > bounds.yHigh) {
        bounds.yHigh = tree->Y();
    }
    if (nullptr != tree->Left()) {
        GetBoundingRectangle(tree->Left(), bounds);
    }
    if (nullptr != tree->Right()) {
        GetBoundingRectangle(tree->Right(), bounds);
    }
}

/**
 * Recursively calculate height of tree
 * @param tree - pointer to tree node
 */
int kdTree::Height(TreeNode *tree) const {
    int leftHeight;
    int rightHeight;

    if (nullptr == tree) {
        return 0;
    }
    leftHeight = Height(tree->Left());
    rightHeight = Height(tree->Right());
    if (leftHeight >= rightHeight) {
        return leftHeight+1;
    }
    else {
        return rightHeight+1;
    }
}


