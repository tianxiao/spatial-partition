#include "StdAfx.h"
#include "xtQuadTree.h"
#include <assert.h>
#include <math.h>
#include <queue>
#include <stack>

#include <stdlib.h>
#include <stdio.h>

xtQuadTree::xtQuadTree(int center[], /*double length, */int depth)
{
	mCenter.x = center[0];
	mCenter.y = center[1];

	mLargestDepth = depth;
	
	assert(depth>0);

	int width = 1<<(depth);
	xtPnt lb, rt;
	lb.x = -width;
	lb.y = -width;
	rt.x = width;
	rt.y = width;

	mRoot = new xtQuadTreeNode;
	mRoot->SetNode(NULL,lb,rt,0);
}


xtQuadTree::~xtQuadTree(void)
{
}

void xtQuadTree::InsertSegment(double start[], double end[],xtQuadTreeNode *root)
{
	if ( IsSegmentSquareOverlay(start, end, root) ) {
		if ( root->mDepth == this->mLargestDepth ) {
			root->mState = PARTIAL;
		} else {
			// if not allocated,
			if ( !root->mChild ) {
				root->AddChild(this->mLargestDepth);
			}

			for ( int i=0; i<4; ++i ) {
				InsertSegment(start,end,&(root->mChild[i]));
			}
		}
	}
}

// use the stackoverflow
// http://stackoverflow.com/questions/99353/how-to-test-if-a-line-segment-intersects-an-axis-aligned-rectange-in-2d
bool SegmentIntersectRectangle(double a_rectangleMinX,
                                 double a_rectangleMinY,
                                 double a_rectangleMaxX,
                                 double a_rectangleMaxY,
                                 double a_p1x,
                                 double a_p1y,
                                 double a_p2x,
                                 double a_p2y)
{
    // Find min and max X for the segment

    double minX = a_p1x;
    double maxX = a_p2x;

    if(a_p1x > a_p2x)
    {
      minX = a_p2x;
      maxX = a_p1x;
    }

    // Find the intersection of the segment's and rectangle's x-projections

    if(maxX > a_rectangleMaxX)
    {
      maxX = a_rectangleMaxX;
    }

    if(minX < a_rectangleMinX)
    {
      minX = a_rectangleMinX;
    }

    if(minX > maxX) // If their projections do not intersect return false
    {
      return false;
    }

    // Find corresponding min and max Y for min and max X we found before

    double minY = a_p1y;
    double maxY = a_p2y;

    double dx = a_p2x - a_p1x;

    if(abs(dx) > 0.0000001)
    {
      double a = (a_p2y - a_p1y) / dx;
      double b = a_p1y - a * a_p1x;
      minY = a * minX + b;
      maxY = a * maxX + b;
    }

    if(minY > maxY)
    {
      double tmp = maxY;
      maxY = minY;
      minY = tmp;
    }

    // Find the intersection of the segment's and rectangle's y-projections

    if(maxY > a_rectangleMaxY)
    {
      maxY = a_rectangleMaxY;
    }

    if(minY < a_rectangleMinY)
    {
      minY = a_rectangleMinY;
    }

    if(minY > maxY) // If Y-projections do not intersect return false
    {
      return false;
    }

    return true;
}

/**
The original poster wanted to DETECT an intersection between a line segment and 
a polygon. There was no need to LOCATE the intersection, if there is one. 
If that's how you meant it, you can do less work than Liang-Barsky or 
Cohen-Sutherland:

Let the segment endpoints be p1=(x1 y1) and p2=(x2 y2). 
Let the rectangle's corners be (xBL yBL) and (xTR yTR).

Then all you have to do is

A. Check if all four corners of the rectangle are on the same side of the line. 
The implicit equation for a line through p1 and p2 is:

F(x y) = (y2-y1)x + (x1-x2)y + (x2*y1-x1*y2)

If F(x y) = 0, (x y) is ON the line. 
If F(x y) > 0, (x y) is "above" the line. 
If F(x y) < 0, (x y) is "below" the line.

Substitute all four corners into F(x y). If they're all negative or all positive, 
there is no intersection. If some are positive and some negative, go to step B.

B. Project the endpoint onto the x axis, and check if the segment's shadow intersects 
the polygon's shadow. Repeat on the y axis:

If (x1 > xTR and x2 > xTR), no intersection (line is to right of rectangle). 
If (x1 < xBL and x2 < xBL), no intersection (line is to left of rectangle). 
If (y1 > yTR and y2 > yTR), no intersection (line is above rectangle). 
If (y1 < yBL and y2 < yBL), no intersection (line is below rectangle). 
else, there is an intersection. Do Cohen-Sutherland or whatever code was mentioned 
in the other answers to your question.

Alejo
*/
bool SegmentSquareOverlapTest(double *start, double *end, double *lb, double *rt)
{
	return SegmentIntersectRectangle(lb[0],lb[1],rt[0],rt[1],start[0],start[1],end[0],end[1]);
}

// Do the transformation
bool xtQuadTree::IsSegmentSquareOverlay(double start[], double end[], xtQuadTreeNode *root)
{
	// first translate the center
	double lbxd, lbyd;
	double rtxd, rtyd;
	lbxd = root->mLB.x + mCenter.x;
	lbyd = root->mLB.y + mCenter.y;
	rtxd = root->mRT.x + mCenter.x;
	rtyd = root->mRT.y + mCenter.y;
	double lbd[2] = {lbxd, lbyd};
	double rtd[2] = {rtxd, rtyd}; 

	// overlapping check!
	bool isOverlapping = SegmentSquareOverlapTest(start,end,lbd,rtd);

	return isOverlapping;
}

void xtQuadTree::DumpLevel(unsigned int level, std::vector<xtQuadTreeNode *> &nodelist)
{
	std::queue<xtQuadTreeNode *> nodequeue;
	nodequeue.push(mRoot);

	while( !nodequeue.empty() ) {
		xtQuadTreeNode *currentnode = nodequeue.front();
		nodequeue.pop();
		if ( currentnode->mDepth == level ) {
			nodelist.push_back( currentnode );
		} else if ( currentnode->mDepth < level ){
			if ( currentnode->mChild ) {
				for ( int i=0; i<4; ++i ) {
					nodequeue.push(&(currentnode->mChild[i]));
				}
			}
		}
	}

}

void xtQuadTree::DumpLevelToPPM(const char *filename, unsigned int level, std::vector<xtQuadTreeNode *> &nodelist)
{
	//std::vector<bool> isFilllist;
	//isFilllist.reserve( nodelist.size() );
	//for ( size_t i=0; i<nodelist.size(); ++i ) {
	//	isFilllist.push_back( false );
	//}

	unsigned int width, height;
	width = height = 1<<(level-1);

	const int realwidth = 1<<(this->mLargestDepth);
	int step = 1<<(this->mLargestDepth - level+1);

	FILE *fp = fopen(filename,"w");
	// PPM headers
	fprintf(fp,"P3\n");
	fprintf(fp,"%d %d\n",width*2, width*2);
	fprintf(fp,"%d\n",15);


	for ( int i = realwidth; i>-realwidth; i-=step ) {
		for ( int j = -realwidth; j<realwidth; j+=step ) {
			int righttopx, righttopy;
			righttopx = j+step;
			righttopy = i;
			bool inCell = false;
			for ( size_t k=0; k<nodelist.size(); ++k ) {
				//if ( isFilllist[k] ) continue;
				if ( nodelist[k]->mRT.x == righttopx 
					&& nodelist[k]->mRT.y == righttopy ) {
					//isFilllist[k] = true;
					inCell = true;
				} 
			}
			if ( inCell ) {
				fprintf(fp,"15 0 15\n");
			} else {
				fprintf(fp,"0 0 0 \n");
			}
		}
	}

	fclose(fp);
}