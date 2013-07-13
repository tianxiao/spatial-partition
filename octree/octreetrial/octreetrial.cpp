// octreetrial.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "xtOctree.h"
#include "xtOctreeAdapter.h"

const int numPnt = 3;
double points[numPnt*3] = {
	0.,0.,0.,
	1.,0.,0.,
	2.,2.,2.,
};

xtOctreeAdapter octreedata;
int depth;
void InitialData() 
{
	octreedata.ReserveSize(numPnt);
	for ( int i=0; i<numPnt; ++i ) {
		octreedata.AddPoint(&(points[3*i]));
	}
	depth = octreedata.AdaptDataForOctree(0.01);
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	InitialData();
	xtPnt3Adapter<double> dcenter = octreedata.GetCenter();
	xtPnt3Adapter<double> dextend = octreedata.GetExtend();
	std::vector<xtPnt3Adapter<double> > &points = octreedata.GetPnts();

	int center[3] = { (int)dcenter.x, (int)dcenter.y, (int)dcenter.z };
	xtOctree<int> xoctree( center, depth );

	for ( int i=0; i<numPnt; ++i ) {
		xoctree.InsertPoint(&(points[i].x));
	}

	std::vector<xtOctreeNode<int> *> levelnodelist; 
	xoctree.DumpLevel(5,levelnodelist);

	return 0;
}

