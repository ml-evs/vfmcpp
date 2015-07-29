/* 'Smart' reconnection algorithm that chooses whether to use Type I/II in Baggaley, JLT 168:18-30 (2012).
	depending on line length following reconnection. */

#include "tangle.h"
#include "point.h"
#include <fstream>
#include <sstream>

using namespace std;

void Tangle::Reconnect(int P, int Q, int k, int l){
	mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tattempting reconnection" << endl;
	if(mTangle[Q]->mFlagType == 1){int swap(Q); Q = P; P = swap; swap = l; l = k; k = swap;}
	double mindist2(123456);
	int dist_flag(0);

/*	double dist_i_1(mTangle[P]->mPoints[k]->Disp2(mTangle[Q]->mPoints[l]->mPrev));
	dist_i_1 += mTangle[P]->mPoints[k]->mPrev->Disp2(mTangle[Q]->mPoints[l]);
	if(dist_i_1<mindist2){mindist2=dist_i_1; dist_flag = 1;}
	
	double dist_i_2(mTangle[P]->mPoints[k]->Disp2(mTangle[Q]->mPoints[l]->mNext));
	dist_i_2 += mTangle[P]->mPoints[k]->mNext->Disp2(mTangle[Q]->mPoints[l]);
	if(dist_i_2<mindist2){mindist2=dist_i_2; dist_flag = 2;}
*/
	double dist_ii_1(mTangle[P]->mPoints[k]->mNext->Disp2(mTangle[Q]->mPoints[l]->mPrev));
	dist_ii_1 += mTangle[P]->mPoints[k]->Disp2(mTangle[Q]->mPoints[l]);
	if(dist_ii_1<mindist2){mindist2=dist_ii_1; dist_flag = 3;}
	
	double dist_ii_2(mTangle[P]->mPoints[k]->mPrev->Disp2(mTangle[Q]->mPoints[l]->mNext));
	dist_ii_2 += mTangle[P]->mPoints[k]->Disp2(mTangle[Q]->mPoints[l]);
	if(dist_ii_2<mindist2){mindist2=dist_ii_2; dist_flag = 4;}
	switch(dist_flag){
		case 3: {
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tcase 3 reconnection" << endl;
			int l_prev_cache(123456);
			for(int j(0); j<mTangle[Q]->mN; j++){
				if(mTangle[Q]->mPoints[j] == mTangle[Q]->mPoints[l]->mPrev){
					l_prev_cache = j;
				}
			}
			mTangle[P]->mPoints[k]->mNext->mPrev = mTangle[Q]->mPoints[l]->mPrev;  
			mTangle[Q]->mPoints[l]->mPrev->mNext = mTangle[P]->mPoints[k]->mNext;
			/* copy points from the other filament to the current filament and delete */
			Point* occ;
			occ = mTangle[Q]->mPoints[l_prev_cache];
			int i(0);
			while(i<mTangle[Q]->mN){
				mTangle[P]->mPoints.push_back(new Point(occ));
				occ = occ->mPrev;
				i++;
			}
			/* reassign pointers */
			for(unsigned int j(mTangle[P]->mN+1); j!= mTangle[P]->mPoints.size()-1; j++){
				mTangle[P]->mPoints[j]->mNext = mTangle[P]->mPoints[j-1];
				mTangle[P]->mPoints[j]->mPrev = mTangle[P]->mPoints[j+1];
			}
			mTangle[P]->mPoints[k]->mNext->mPrev = mTangle[P]->mPoints[mTangle[P]->mN];
			mTangle[P]->mPoints[mTangle[P]->mN]->mPrev = mTangle[P]->mPoints[mTangle[P]->mN+1];
			mTangle[P]->mPoints[mTangle[P]->mN]->mNext = mTangle[P]->mPoints[k]->mNext;
			mTangle[P]->mPoints.back()->mPrev = mTangle[P]->mPoints[k];
			mTangle[P]->mPoints.back()->mNext = mTangle[P]->mPoints[mTangle[P]->mPoints.size()-2];	
			mTangle[P]->mPoints[k]->mNext = mTangle[P]->mPoints[mTangle[P]->mPoints.size()-1];
			mTangle[P]->mN = mTangle[P]->mPoints.size();
			break;
		}
		case 4: {
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tcase 4 reconnection" << endl;
			mTangle[P]->mPoints[k]->mPrev->mNext = mTangle[Q]->mPoints[l]->mNext;  
			mTangle[Q]->mPoints[l]->mNext->mPrev = mTangle[P]->mPoints[k]->mPrev;
			/* copy points from the other filament to the current filament and delete */
			Point* occ;
			occ = mTangle[Q]->mPoints[l];
			int i(0);
			while(i<mTangle[Q]->mN){
				mTangle[P]->mPoints.push_back(new Point(occ));
				occ = occ->mPrev; //perhaps change in future for contiguous storage
				i++;
			}
			/* reassign pointers */
			for(unsigned int j(mTangle[P]->mN+1); j!= mTangle[P]->mPoints.size()-1; j++){
				mTangle[P]->mPoints[j]->mNext = mTangle[P]->mPoints[j-1];
				mTangle[P]->mPoints[j]->mPrev = mTangle[P]->mPoints[j+1];
			}
			mTangle[P]->mPoints[k]->mPrev->mNext = mTangle[P]->mPoints[mTangle[P]->mPoints.size()-1];
			mTangle[P]->mPoints[mTangle[P]->mN]->mPrev = mTangle[P]->mPoints[mTangle[P]->mN+1];
			mTangle[P]->mPoints[mTangle[P]->mN]->mNext = mTangle[P]->mPoints[k];
			mTangle[P]->mPoints.back()->mPrev = mTangle[P]->mPoints[k]->mPrev;
			mTangle[P]->mPoints.back()->mNext = mTangle[P]->mPoints[mTangle[P]->mPoints.size()-2];	
			mTangle[P]->mPoints[k]->mPrev = mTangle[P]->mPoints[mTangle[P]->mN];
			mTangle[P]->mN = mTangle[P]->mPoints.size();
			break;
		}
		default: {
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tOdd kind of reconnection encountered - need to reconsider input" << endl;
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tCase " << dist_flag << " reconnection attempted." << endl;
		}
		
	}
	/* delete old ring */
	for(unsigned int q(0); q<mTangle[Q]->mPoints.size(); q++){
		delete mTangle[Q]->mPoints[q];
	}
	delete mTangle[Q];
	mTangle.erase(mTangle.begin()+Q);
	mN_recon++;
	mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tsuccessful reconnection" << endl;
}
