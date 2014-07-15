/* 	Adds or removes points to keep the resolution within a fixed range
Details given by Baggaley and Barenghi, PRB 83 (2011) */

#include "filament.h"

using namespace std;

void Filament::MeshAdjust(double dr){
	for (int k(0); k<mN; k++){
		/* maintain reasonable local curvature */
		double R(0); // 1/|s''| at new point
		for(int j(0);j!=3;j++){
			R += pow((mPoints[mN-1]->mNext->mS2Prime[j] + mPoints[mN-1]->mPrev->mS2Prime[j])/2,2);
		}
		R = sqrt(R);
		R = 1/R;
		/* point deletion for smoothing and proximity*/
		if((1/R)>1.9/dr || mPoints[k]->mSegLength < 0.5*dr){
			if((1/R)>1.9/dr){cout << "Deleting point for smoothing." << endl;}
			if(mPoints[k]->mSegLength < 0.5*dr){cout << "Deleting point " << k << " for proximity." << endl;}
			/* reassign next and last pointers for point to be deleted */
			mPoints[k]->mNext->mPrev = mPoints[k]->mPrev;
			mPoints[k]->mPrev->mNext = mPoints[k]->mNext;
			/* erase point from mPoints and decrement mN */
			mN--;
			delete mPoints[k];
			mPoints.erase(mPoints.begin()+k);
			CalcMeshLengths(); CalcSPrime(); CalcS2Prime();
		}
		/* point addition */
		else if (mPoints[k]->mSegLength > dr){
			/* increment mN */
			mN++;
			/* create new point and reassign pointers */
			mPoints.push_back(new Point());
			mPoints[mN-1]->mPrev = mPoints[k]->mPrev;
			mPoints[mN-1]->mNext = mPoints[k];
			mPoints[k]->mPrev->mNext = mPoints[mN-1];
			mPoints[k]->mPrev = mPoints[mN-1];
			/* calculate position of new point a la PRB 2011, Baggaley & Barenghi */
			for(int j(0);j!=3;j++){
				mPoints[mN-1]->mPos[j] = (mPoints[mN-1]->mNext->mS2Prime[j] + mPoints[mN-1]->mPrev->mS2Prime[j])/2;
				mPoints[mN-1]->mPos[j] = (mPoints[mN-1]->mS2Prime[j]) * R * (sqrt( pow(R,2) - 0.25*pow(mPoints[mN-1]->mNext->mSegLength,2)) - R);
				mPoints[mN-1]->mPos[j] += 0.5*(mPoints[mN-1]->mNext->mPos[j] + mPoints[mN-1]->mPrev->mPos[j]);
			}
			CalcMeshLengths(); CalcSPrime(); CalcS2Prime();
			cout << "Added point at index " << k << "." << endl;
		}
	}
}
