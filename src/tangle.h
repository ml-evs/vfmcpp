#ifndef GUARD_TANGLE_H
#define GUARD_TANGLE_H

#include "filament.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

class Tangle{
public:
	vector <Filament> 	mTangle;
	Tangle(){};
	Tangle(Filament Ring1, Filament Ring2){
		mTangle.push_back(Ring1);
		mTangle.push_back(Ring2);
	}
	~Tangle(){};
	void CalcVelocityNL_OF();
};

#endif