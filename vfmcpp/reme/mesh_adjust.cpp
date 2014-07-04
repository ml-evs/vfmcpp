#include "filament.h"

void Filament::AdjMeshLengths(double dr){
	cout << "Adjusting..." << '\n' << endl;
	//mSegLengths[3] = 1e-10; // Test point
	for (int k = 0; k < mN; k++){
		cout << "Beginning " << k << endl;
		if (mSegLengths[k] < dr / 2){
			cout << "Segment " << k << " is too small." << endl;
			for (int l = k + 1; l < mN; l++){
				cout << "Pushing back from " << l << endl;
				mPos[l - 1] = mPos[l];
				cout << "mPos[" << l - 1 << "][3] is now " << mPos[l - 1][3] << endl;
			}
			mN = mN - 1;
			cout << "Number of points: " << mN;
			mPos.resize(mN); cout << "	mPos.size(): " << mPos.size() << endl;
		}
		cout << "Finished " << k << '\n' << endl;
	}
}