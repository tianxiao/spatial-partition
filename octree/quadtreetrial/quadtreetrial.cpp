// quadtreetrial.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "xtQuadTree.h"
#include <math.h>

double center[2] = {2.5,2.5};
double extend[2] = {2.5,2.5};
double segment[16][2] = {
	{1,1},{1,3},
	{1,3},{2,4},
	{2,4},{3,4},
	{3,4},{4,3},
	{4,3},{4,1},
	{1,3},{2,2},
	{2,2},{3,2},
	{3,2},{4,3},
};

int centerI[2];
int segmentsI[32];

const int depth = 6;
double scale = 1<<depth;

void ScaleDataX2(double segmentlist[][2], unsigned int numSegs, 
	double center[], double extend[], double resolution, int &depth) 
{
	const double llength = extend[0]>extend[1]?extend[0]:extend[1];
	const double numGrid = llength/resolution;
	depth = (int) ( log(numGrid)/log(2.)+1 ); 

	const double scalefactor = (1<<depth)/llength;

	for ( unsigned int i=0; i<numSegs; ++i ) {
		for ( int j=0; j<2; ++j ) {
			segmentlist[i][j] = center[j]+(segmentlist[i][j]-center[j])*scalefactor;	
		}
	}

}


// scale data
void ScaleData()
{
	center[0] = center[0]*scale;
	center[1] = center[1]*scale;
	for ( int i=0; i<16; ++i ) {
		segment[i][0] *=scale;
		segment[i][1] *=scale;

		segmentsI[i*2] = (int)segment[i][0];
		segmentsI[i*2+1] = (int)segment[i][1];
	}

	centerI[0] = (int)center[0];
	centerI[1] = (int)center[1];

	
};


int _tmain(int argc, _TCHAR* argv[])
{
	//ScaleData();
	int inputdepth;
	ScaleDataX2(segment,16,center,extend,0.001,inputdepth);
	centerI[0] = int(center[0]);
	centerI[1] = int(center[1]);

	xtQuadTree quadtree(centerI,inputdepth);

	for ( int i=0; i<8; ++i ) {
		quadtree.InsertSegment(segment[i*2],segment[i*2+1]);
	}

	std::vector<xtQuadTreeNode *> levelnodelist;
	const int dumplevel = inputdepth-0;
	quadtree.DumpLevel(dumplevel,levelnodelist);
	quadtree.DumpLevelToPPM("level8.ppm",dumplevel,levelnodelist);


	return 0;
}

