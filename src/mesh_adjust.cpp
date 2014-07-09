/* 	Adds or removes points to keep the resolution within a fixed range
Details given by Baggaley and Barenghi, PRB 83 (2011) */

#include "filament.h"

using namespace std;

void Filament::AdjMeshLengths(double dr){

	cout << "Adjusting..." << '\n' << endl;

	for (int k(0); k!=mN; k++){
		if (mPoints[k]->mSegLength < 0.5*dr){
			cout << "Segment " << k << " is too small." << endl;
			/* reassign next and last pointers for point to be deleted */
			mPoints[k]->mNext->mPrev = mPoints[k]->mPrev;
			mPoints[k]->mPrev->mNext = mPoints[k]->mNext;
			/* erase point from mPoints and decrement mN */
			mN--;
			mPoints.erase(mPoints.begin()+k);
		}
		else if (mPoints[k]->mSegLength > 1.01*dr){
			double R(0); // 1/|s''| at new point
			cout << "Segment " << k << " is too large." << endl;
			mN++;

			/* create new point and reassign pointers */
			mPoints[mN-1]->mPrev = mPoints[k]->mPrev;
			mPoints[mN-1]->mNext = mPoints[k];
			mPoints[k]->mPrev->mNext = mPoints[mN-1];
			mPoints[k]->mPrev = mPoints[mN-1];

			/* maintain local curvature */
			for(int j(0);j!=3;j++){
				mPoints[mN-1]->mS2Prime[j] = (mPoints[mN-1]->mNext->mS2Prime[j] + mPoints[mN-1]->mPrev->mS2Prime[j])/2;
				R += pow(mPoints[mN-1]->mS2Prime[j],2);
				R = 1/R;
			}
			/* calculate position of new point a la PRB 2011, Baggaley & Barenghi */
			for(int j(0);j!=3;j++){
				mPoints[mN-1]->mPos[j] = ( (mPoints[mN-1]->mS2Prime[j]) * (sqrt(pow(R,2) + 0.25 * mPoints[mN-1]->mNext->mSegLength) - R) ) / R;
				mPoints[mN-1]->mPos[j] += 0.5*(mPoints[mN-1]->mNext->mPos[j] + mPoints[mN-1]->mPrev->mPos[j]);
			}
		}
		CalcMeshLengths();
	}
}