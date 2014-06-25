/* 	Calculate velocities
 	Adapted from CalcVelMaster.m by Paul Walmsley */

#ifndef GUARD_VEL_H
#define GUARD_VEL_H

#include <vector>

using namespace std;

vector <vector <double> > Velocity(vector <vector <double> > Ring1, vector <vector <double> > Ring2);
vector <vector <double> > CalcSPrime(vector <vector <double> > Ring);
vector <double> FindFlag(vector <vector <double> > Ring, int f);
vector <vector <double> > CalcAS(vector <vector <double> > Ring, vector <vector <double> > L);

#endif