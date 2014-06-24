/* 	Calculate mesh lengths
 	Adapted from CalcMeshLenghs.m by Paul Walmsley, which was 
 	based on work by Baggaley and Barenghi, JLTP 166,3 (2011) */

#ifndef GUARD_MESH_LENGTHS_H
#define GUARD_MESH_LENGTHS_H

#include <vector>
#include <cmath>

using namespace std;

vector <double> MeshLengths(vector <vector <double> > Ring);

#endif