#pragma once
#include <assert.h>

struct xtPnt
{
	int x, y;
};

enum IntersectState {
	PARTIAL,
	FULL,
	EMPTY,
};

class xtQuadTreeNode
{
public:
	xtQuadTreeNode(void);
	~xtQuadTreeNode(void);

	
	//void SetNodeAsInitializedRoot(double length, unsigned int depth);
	void SetNode(xtQuadTreeNode *parent, xtPnt &lb, xtPnt &rt, int depth) {
		mParent = parent;
		mChild = NULL;
		pv = NULL;
		mLB = lb;
		mRT = rt;
		mDepth = depth;
		mState = EMPTY;
	};

	void SetNode(xtQuadTreeNode *parent, int depth) {
		mParent = parent;
		mChild = NULL;
		pv = NULL;
		mDepth = depth;
		mState = EMPTY;
	};

	void SetNode(xtPnt &lb, xtPnt &rt) {
		mLB = lb;
		mRT = rt;
	}

	/*
	  +------------p3-----------rt
	  |            |             |
	  |     C3     |      C2     |
	  |            |             |
	  p4-----------p0-----------p2
	  |            |             |
	  |     C0     |      C1     |
	  |            |             |
	  lb-----------p1------------+
	*/

	void AddChild(const unsigned int LD) {
		assert(LD>mDepth);
		mChild = new xtQuadTreeNode[4];
		for ( int i=0; i<4; ++i ) {
			mChild[i].SetNode(this,this->mDepth+1);
		}
		xtPnt p0, p1, p2, p3, p4;
		p0.x = ( mLB.x + mRT.x )/2;
		p0.y = ( mLB.y + mRT.y )/2;
		p1.x = p0.x;
		p1.y = mLB.y;
		p2.x = mRT.x;
		p2.y = p0.y;
		p3.x = p0.x;
		p3.y = mRT.y;
		p4.x = mLB.x;
		p4.y = p0.y;
		mChild[0].SetNode(mLB,p0);
		mChild[1].SetNode(p1,p2);
		mChild[2].SetNode(p0,mRT);
		mChild[3].SetNode(p4,p3);
	};


	xtQuadTreeNode *mParent;
	xtQuadTreeNode *mChild;
	unsigned int mDepth;
	IntersectState mState;
	void *pv;

	xtPnt mLB;   // left bottom
	xtPnt mRT;   // right top
};

