/* 	Adds or removes points to keep the resolution within a fixed range
Details given by Baggaley and Barenghi, PRB 83 (2011) */

#include "filament.h"

using namespace std;

void Filament::MeshAdjust(double dr){
	double res = dr;
	bool Remeshed = false;
	
	for (int k(0); k<mN; k++){
		if(Remeshed==true){break;}
		/* maintain reasonable local curvature */
		double R(0); // 1/|s''| at new point
		for(int j(0);j!=3;j++){
			R += pow((mPoints[k]->mNext->mS2Prime[j] + mPoints[k]->mPrev->mS2Prime[j])/2,2);
		}
		R = sqrt(R);
		R = 1/R;
		/* point deletion for smoothing and proximity*/
		if((1/R)>1.9/dr || mPoints[k]->mSegLength < 0.5*dr){
			if((1/R)>1.9/dr){cout << "Deleting point " << k << " / " << mPoints.size() << " for smoothing." << endl;}
			if(mPoints[k]->mSegLength < 0.5*dr){cout << "Deleting point " << k << " / " << mPoints.size() << " for proximity." << endl;}
			/* reassign next and last pointers for point to be deleted */
			mPoints[k]->mNext->mPrev = mPoints[k]->mPrev;
			mPoints[k]->mPrev->mNext = mPoints[k]->mNext;
			/* erase point from mPoints and decrement mN */
			mN--;
			delete mPoints[k];
			mPoints.erase(mPoints.begin()+k);
			CalcMeshLengths(); CalcSPrime(); CalcS2Prime();
			Remeshed = true;
			break;
		}
		/* point addition */
		else if (mPoints[k]->mSegLength > dr){
		cout << "Adding point at " << k << " / " <<  mPoints.size() << endl;
			/* increment mN */
			mN++;
			/* create new point and reassign pointers */
			mPoints.push_back(new Point());
			mPoints.back()->mPrev = mPoints[k]->mPrev;
			mPoints.back()->mNext = mPoints[k];
			mPoints[k]->mPrev->mNext = mPoints.back();
			mPoints[k]->mPrev = mPoints.back();
			/* calculate position of new point a la PRB 2011, Baggaley & Barenghi */
			for(int j(0);j!=3;j++){
				mPoints.back()->mS2Prime[j] = (mPoints.back()->mNext->mS2Prime[j] + mPoints.back()->mPrev->mS2Prime[j])/2;
				mPoints.back()->mPos[j] = (mPoints.back()->mS2Prime[j]) * R * (sqrt( pow(R,2) - 0.25*pow(mPoints.back()->mNext->mSegLength,2)) - R);
				mPoints.back()->mPos[j] += 0.5*(mPoints.back()->mNext->mPos[j] + mPoints.back()->mPrev->mPos[j]);
			}
			CalcMeshLengths(); CalcSPrime(); CalcS2Prime();
			Remeshed = true;
			break;
		}
	}
	if(Remeshed == true){this->MeshAdjust(res);}
}
