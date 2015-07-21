/* Ring self-reconnection algorithm using Type I/II in Baggaley, JLT 168:18-30 (2012).
	with no dependence on line length following reconnection (yet). */

#include "tangle.h"
#include "point.h"
#include <fstream>
#include <sstream>

using namespace std;

void Tangle::SelfReconnect(int P, int Q, int k, int l){
	mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tattempting self-reconnection" << endl;
	/* reassign pointers to separate new ring  */
	/* copy old ring to tangle */
	mTangle.push_back(new Ring());
	double mindist2(123456);
	int dist_flag(0);

    int k_prev_cache(123456);
    for(int j(0); j<mTangle[P]->mN; j++){
    	if(mTangle[P]->mPoints[j] == mTangle[P]->mPoints[k]->mPrev){
    		k_prev_cache = j;
    	}
    }

/*	double dist_i_1(mTangle[P]->mPoints[k]->Disp2(mTangle[Q]->mPoints[l]->mPrev));
	dist_i_1 += mTangle[P]->mPoints[k]->mPrev->Disp2(mTangle[Q]->mPoints[l]);
	if(dist_i_1<mindist2){mindist2=dist_i_1; dist_flag = 1;}
	
	double dist_i_2(mTangle[P]->mPoints[k]->Disp2(mTangle[Q]->mPoints[l]->mNext));
	dist_i_2 += mTangle[P]->mPoints[k]->mNext->Disp2(mTangle[Q]->mPoints[l]);
	if(dist_i_2<mindist2){mindist2=dist_i_2; dist_flag = 2;}*/
	
	double dist_ii_1(mTangle[P]->mPoints[k]->mNext->Disp2(mTangle[Q]->mPoints[l]->mPrev));
	dist_ii_1 += mTangle[P]->mPoints[k]->Disp2(mTangle[Q]->mPoints[l]);
	if(dist_ii_1<mindist2){mindist2=dist_ii_1; dist_flag = 3;}
	
	double dist_ii_2(mTangle[P]->mPoints[k]->mPrev->Disp2(mTangle[Q]->mPoints[l]->mNext));
	dist_ii_2 += mTangle[P]->mPoints[k]->Disp2(mTangle[Q]->mPoints[l]);
	if(dist_ii_2<mindist2){mindist2=dist_ii_2; dist_flag = 4;}
	int i(0);
	Point* pStart;
	Point* pNew;
	switch(dist_flag){
		case 3: {
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tcase 3 line reconnection" << endl;
			mTangle[P]->mPoints[k]->mNext->mPrev = mTangle[Q]->mPoints[l]->mPrev;  
			mTangle[Q]->mPoints[l]->mPrev->mNext = mTangle[P]->mPoints[k]->mNext;
			int k_next_cache(123456);
    		for(int j(0); j<mTangle[P]->mN; j++){
    			if(mTangle[P]->mPoints[j] == mTangle[P]->mPoints[k]->mNext){
    				k_next_cache = j;
    			}
    		}
			pNew = mTangle[P]->mPoints[k_next_cache];
			pStart = pNew;
			/* reassign pointers to separate new ring */
			mTangle[P]->mPoints[k]->mNext = mTangle[Q]->mPoints[l];
			mTangle[Q]->mPoints[l]->mPrev = mTangle[P]->mPoints[k];		
			i = 0;
			do{
				mTangle.back()->mPoints.push_back(new Point(pNew));
				mTangle.back()->mN++;
				pNew = pNew->mNext;
				i++;
			}while(pNew!=pStart);
			break;
		}
		case 4: {
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tcase 4 line reconnection" << endl;
			mTangle[P]->mPoints[k]->mPrev->mNext = mTangle[Q]->mPoints[l]->mNext;  
			mTangle[Q]->mPoints[l]->mNext->mPrev = mTangle[P]->mPoints[k]->mPrev;
			pNew = mTangle[P]->mPoints[k];
			pStart = pNew;
			/* reassign pointers to separate new ring */
			mTangle[P]->mPoints[k]->mPrev = mTangle[Q]->mPoints[l];
			mTangle[Q]->mPoints[l]->mNext = mTangle[P]->mPoints[k];
			i = 0;
			do{
				mTangle.back()->mPoints.push_back(new Point(pNew));
				mTangle.back()->mN++;
				pNew = pNew->mNext;
				i++;
			}while(pNew!=pStart);
			break;
		}
		default: {
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tOdd kind of reconnection encountered - need to reconsider input" << endl;
		}
	}
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

	pNew = mTangle[P]->mPoints[k_prev_cache];
	pStart = pNew;
	/* create new ring in tangle */
	mTangle.push_back(new Ring());
	do{
		/* push back position and velocities of new points to tangle */
		mTangle.back()->mPoints.push_back(new Point(pNew));
		mTangle.back()->mN++;
		pNew = pNew->mNext;
	}while(pNew!=pStart);	
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