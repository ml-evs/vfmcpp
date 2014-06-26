/* 	Calculate velocities
 	Adapted from CalcVelMaster.m by Paul Walmsley */

#ifndef GUARD_VEL_H
#define GUARD_VEL_H

#include <vector>

using namespace std;

vector <vector <double> > CalcVelocity(vector <vector <double> > Ring1, vector <vector <double> > Ring2);
vector <double> FindFlag(vector <vector <double> > Ring, int f);
vector <vector <double> > CalcSPrime(vector <vector <double> > Ring, vector <double> L);
vector <vector <double> > CalcS2Prime(vector <vector <double> > Ring, vector <double> L);

#endif