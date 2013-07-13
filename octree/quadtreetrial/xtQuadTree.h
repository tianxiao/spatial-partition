#pragma once
#include "xtQuadTreeNode.h"
#include <vector>



class xtQuadTree
{
public:
	xtQuadTree(int center[],/* double length,*/ int depth);
	~xtQuadTree(void);

	void InsertSegment(double start[], double end[]) {
		InsertSegment(start,  end, mRoot);
	}

	void DumpLevel(unsigned int level, std::vector<xtQuadTreeNode *> &nodelist);
	void DumpLevelToPPM(const char *filename, unsigned int level, std::vector<xtQuadTreeNode *> &nodelist);

private:
	bool IsSegmentSquareOverlay(double start[], double end[],xtQuadTreeNode *root);

private:
	void InsertSegment(double start[], double end[],xtQuadTreeNode *root);

private:
	xtQuadTreeNode *mRoot;
	xtPnt mCenter;
	/*double mLength;*/
	unsigned int mLargestDepth;
};

