
#include "tangle.h"

using namespace std;

void Tangle::LoopKill(){

	vector <Filament>::iterator begin, current, end;
	begin = mTangle.begin(); end = mTangle.end();
	for (current = begin; current != end; current++){			
		if (current->mN < 6){
			//cout << "Terminating..." << endl;
			mTangle.erase(current);
		}
	}
}