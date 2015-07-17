/* Ring self-reconnection algorithm using Type I/II in Baggaley, JLT 168:18-30 (2012).
	with no dependence on line length following reconnection (yet). */

#include "tangle.h"
#include "point.h"
#include <fstream>
#include <sstream>

using namespace std;

void Tangle::SelfReconnect(int P, int Q, int k, int l){
	mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tattempting self-reconnection" << endl;
	mN_f = 1; mN_slow = 0;
	/* reassign pointers to separate new ring  */
	mTangle[P]->mPoints[k]->mNext->mPrev = mTangle[Q]->mPoints[l]->mPrev;  
	mTangle[Q]->mPoints[l]->mPrev->mNext = mTangle[P]->mPoints[k]->mNext;
	Point* pNew = mTangle[P]->mPoints[l];
	/* copy old ring to tangle */
	mTangle.push_back(new Ring());
	do{
		/* push back position and velocities of new points to tangle */
		mTangle.back()->mPoints.push_back(new Point(pNew));
		mTangle.back()->mN++;
		pNew = pNew->mNext;
	}while(pNew!=mTangle[Q]->mPoints[k]->mNext);	
	
	/* count number of points on new ring then assign their pointers in order*/
	int N_new = mTangle.back()->mN;
	for(int d(1); d!=N_new-1; d++){
		mTangle.back()->mPoints[d]->mPrev = mTangle.back()->mPoints[d-1];
		mTangle.back()->mPoints[d]->mNext = mTangle.back()->mPoints[d+1];
	}	
	mTangle.back()->mPoints[N_new-1]->mPrev = mTangle.back()->mPoints[N_new-2];
	mTangle.back()->mPoints[N_new-1]->mNext = mTangle.back()->mPoints[0];
	mTangle.back()->mPoints[0]->mNext = mTangle.back()->mPoints[1];
	mTangle.back()->mPoints[0]->mPrev = mTangle.back()->mPoints[N_new-1]; 

	pNew = mTangle[Q]->mPoints[k]->mNext;
	/* create new ring in tangle */
	mTangle.push_back(new Ring());
	do{
		/* push back position and velocities of new points to tangle */
		mTangle.back()->mPoints.push_back(new Point(pNew));
		mTangle.back()->mN++;
		pNew = pNew->mNext;
	}while(pNew!=mTangle[Q]->mPoints[k]->mNext);	
	N_new = mTangle.back()->mN;
	/* reassign pointers */ 
	for(int d(1); d!=N_new-1; d++){
		mTangle.back()->mPoints[d]->mPrev = mTangle.back()->mPoints[d-1];
		mTangle.back()->mPoints[d]->mNext = mTangle.back()->mPoints[d+1];
	}
	mTangle.back()->mPoints[N_new-1]->mPrev = mTangle.back()->mPoints[N_new-2];
	mTangle.back()->mPoints[N_new-1]->mNext = mTangle.back()->mPoints[0];
	mTangle.back()->mPoints[0]->mNext = mTangle.back()->mPoints[1];
	mTangle.back()->mPoints[0]->mPrev = mTangle.back()->mPoints[N_new-1]; 
	
	for(unsigned int q(0); q<mTangle[P]->mPoints.size(); q++){
		delete mTangle[P]->mPoints[q];
	}
	mTangle.erase(mTangle.begin()+P);
	mN_recon++;
	mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tsuccessful self-reconnection" << endl;
}