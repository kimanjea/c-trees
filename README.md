# lab6-2d-search-tree-f21

In this assignment you will work with 2d-search trees storing points in the coordinate plane.  You will write code for inserting points and for performing range and nearest neighbor searches.

A 2d-search tree is represented by a _kdTree_ object.  When you first create a _kdTree_ object, the tree will be empty.  The _ _root_ member variable of empty trees is equal to  _nullptr_.

## Step 1: Implement Insert

You need to get insertion working first.  The public method to insert a point into a _kdTree_ object is:

```
void Insert(const Point& point);
```

Implementing this method takes no more than one line.  It starts the recursive insert process by calling the private method

```
TreeNode* Insert(TreeNode* tree, const Point& p, bool xDirection);
```

Here is a sketch of what the private _Insert_ method does:

1. If  the tree is empty, then return a new node containing the point
1. Else 
    1.  If BOTH x- and y- coordinates of the point p and the point in the node are equal, your point is already in the tree, do nothing
    1.  Else compare the coordinate of your point with the point stored in the root 
        1.  If the coordinate of p is <=  the coordinate of the point in the node, then insert in left subtree 
        1.  If coordinate of p is >  the coordinate of the point in the node, then insert in right subtree 
    1.  Return a pointer to the existing root

Once you have _Insert_ implemented you can use the main program to test whether the trees make sense.  For instance, you can insert points one by one and then print out either the tree or its planar decomposition.

![image](https://user-images.githubusercontent.com/58283777/141407881-bd3e6898-15b4-47c2-95ff-5333526cdd2f.png)
  
You can also load the points from a one of the test files provided in the repo.  You can also make your own file and use that to test your program.
 
![image](https://user-images.githubusercontent.com/58283777/141407904-424a972b-4eab-4d09-8ce7-9326d86164c5.png)

## Step 2: Implement Efficient Range Search

The _kdTree_ class already implements a  version of range search called _SlowRangeSearch_.  This method visits _every_ node of the tree to check whether the point in that node is within the search rectangle.  It finds all the points, but it doesn't take advantage of the points being stored in a 2d Search Tree to speed up the process.

Your job is to implement a more efficient range search that doesn't search into subtrees which don’t intersect with the search rectangle.

The public method that you need to implement is

```
Set<Point> RangeSearch(const Rectangle& range) const;
```

It returns the set of points found by the range search.  Its job is to set in motion a recursive range search performed by the private method

```
void RangeSearch(TreeNode* tree, const Rectangle& range, Set<Point>& points, bool xDirection) const;
```

Here is a sketch of what the private RangeSearch method does:

1. If point in the node lies within the rectangle, add to the set of points
1. If you are comparing x coordinates
	1. If the left end of the search rectangle intersects the left subtree, range search the left subtree
	1. If the right end of the search rectangle intersects the right subtree, range search the right subtree
1. Else 
	1. If the bottom end of the search rectangle intersects the left subtree, range search the left subtree
	1. If the top end of the search rectangle intersects the right subree, range search the right subtree 


The R command can be used to test your range search results.  It calls your method and also calls the slow range search method and makes sure the two results are the same.  THe screen shot below shows how this command can be used.

![image](https://user-images.githubusercontent.com/58283777/141597044-e1145a51-536f-4937-8cac-08dcc478a02e.png)

In the example above, your method would first check if (14, -16) is in the search rectangle.  It is not.  It then compares the left end of the search rectangle with the x coordinate in the node and finds that 11 < 14, so it knows it has to search the left subtree.  After searching the left subtree, it will compare the right edge with the x coordinate.  In this case it finds that  13 < 14, so there is no need to search the right subtree of (14, -16).

When it searches the left subtree of (14, -16) which has root (11, -17),  it finds that this point is in the search rectangle so it adds it to the set of points.  Then it compares the bottom edge of the search rectangle with the y coordinate in the node.  Since -20 < -17, it needs to search the left subtree.  After searching the left subtree, it will compares the top edge of the search rectangle with the y coordinate in the node.  Since -17 < 6, it also has to has to search the right subtree of (11, -17). 

When it searches the left subtree of (11, -17) which has root (7, -19), it compares the left end of the search rectangle with the x coordinate in the node and finds 11 > 7, so the left subtree does not need to be searched.  But 13 > 7 so we do need to search the right subtree.  However the right subtree is empty so this part of the search ends.

Finally, it searches the right subtree of (11, -17) which has root (10, 5).  It compares the left end of the search rectangle with the x-cordinate in the node and finds that 11 > 10, so the left subtree does not need searching.  But 13 > 10 so the right subtree does.  However the right subtree of this node is empty so there is nothing to search.

## Step 3: Implement Efficient Nearest Neighbor Search

The _kdTree_ class already implements a  version of nearest neighbor search called _SlowNearestNeighbors_.  This method visits _every_ node of the tree to to find which point(s) are closest to a point, but it doesn't take advantage of the 2d Search Tree capabilities.

Your job is to implement a more efficient nearest neighbor search.

The public method that you need to implement is

```
bool NearestNeighbors(const Point& p, Set<Point>& NN) const
```

This method simply returns _false_ if the tree is empty.  In all other cases it will perform the nearest neighbor search and add the point(s) in the tree which are closest to _p_ into the set _NN_ and return _true_.  The real work done here is by the following private method which recursively finds the nearest neighbor(s):

```
void NearestNeighbors(TreeNode* tree, const Point& p, Set<Point>& NN, float& NNSqrDist, bool xDirection) const
```

It is important to understand the purpose of the two reference parameters:

_NNSqrDist_ This is the square of the distance between the point _p_ and the nearest neighbor(s) that has been found so far.  The public method initializes this variable to _FL_MAX_ (the largest possible floating point number) since no nearest neighbors have been found so far.

_NN_ This stores the best candidate(s) for the nearest neigbbor(s) that has been found so far.  The public method will initially pass in an empty set when it calls the private method.  As the private method works it will update this set as it finds closer points.  When it finds a point closer than it has found previously (square of distance < NNSqrDist), it will clear the set and add the new point.  When it finds a point the same distance as it has found previously (square of distance == NNSqrDist) it will simply add the new point.

Here is a sketch of what this private method does:
1. Find squared distance between _p_ and the point in node, update _NN_ and _NNSqrDist_ if appropriate 
1. If comparing x coordinates
	1. If the x coordinate of _p_ <= x coordinate of the point in the node, the left subtree is more promising
		1. Search the left subtree
		1. If it is possible for a point in the right subtree to be closer than the nearest neighbor(s) we have found so far, search the right subtree
	1. Else the right subtree is more promising
		1. Search the right subtree
		1. If it is possible for a point in the left subtree to be closer than the nearest neighbor(s) we have found so far, search the left subtree
1. Else
	1. If the y coordinate of _p_ <= y coordinate of the point in the node, the left subtree is more promising
		1. Search the left subtree
		1. If it is possible for a point in the right subtree to be closer than the nearest neighbor(s) we have found so far, search the right subtree
	1. Else the right subtree is more promising
		1. Search the right subtree
		1. If it is possible for a point in the left subtree to be closer than the nearest neighbor(s) we have found so far, search the left subtree

If our point lies in one subtree, how do we know whether the nearest neighbor could be in the other subtree?  To answer this question, let's look at the case when we are comparing x-coordinates and the point is in the left subtree.  If we draw a circle of radus sqrt(NNSqrDist) around the point there are two possibilities.

Case 1: It could cross the dividing line between the left and right subtrees:

![image](https://user-images.githubusercontent.com/58283777/141600255-a3ce317b-7ae2-49f5-822a-aae71b3b9e99.png)

This is the case when the square of the difference of x-coordinates is less than then _NNSqrDist_.   In this case it is possible for a point in the right subtree to be closer.

Case 2: The dividing line lies to the right of the circle

![image](https://user-images.githubusercontent.com/58283777/141601195-e6db0a4a-ffd4-40a7-9111-33eba674f994.png)

In this case it is impossible for a point in the right subtree to be closer.

The other three cases are similar, but pay attention to the fact that points in the left subtree have x coordinates (or y coordinates depending on which direction you are comparing) <= the coordinate of the node's point, while points in the right subtree have coordinates strictly > than the node's point.

The N command can be used to test your nearest neighbor search.  It calls your method and also calls the slow nearest neighbor search method and makes sure the two results are the same.  The screen shot below shows how this command can be used.

![image](https://user-images.githubusercontent.com/58283777/141602854-f42d4a47-7880-40c5-b073-7470a2f15e45.png)

## Step 4: Testing

Here are some things you should test:

For each of the 6 test files provided in the repo:

1. Make sure you can load them succesfully
2. Check that the display appears correct
3. Use the V command to validate your range and nearest neighbor search code
4. Do a run with valgrind to make sure you are not corrupting memory some how (this is unlikely but always worth checking)




