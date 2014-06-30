/* 	Calculate non-local velocity contributions
 	Adapted from CalcNonLocalVel_OtherFilament.m by Paul Walmsley */

#ifndef GUARD_VELNL_OF_H
#define GUARD_VELNL_OF_H

#include <vector>

using namespace std;

vector <vector <double> > VelocityNL_OF(vector <vector <double> > Ring, vector <vector <double> > Ring2);

#endif