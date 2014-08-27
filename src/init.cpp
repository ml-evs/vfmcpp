#include "tangle.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

string Tangle::Initialise(string runfile){
	
  if(runfile == "NULL"){
  	runfile = "../run.in";
  	}
  ifstream infile(runfile);
	string line;
	stringstream input, convert;
	string put;
	string filename;
	double t_total;
	int N;
	vector <double> param(5);
	cout << "\t - - - - - - -    INITIALISING TANGLE    - - - - - - - -\n\n";
	cout << "\t    init path: " << runfile << endl;
	while(infile){
		getline(infile, line);
		if(line.substr(0,4) == "path"){
			input.clear();
			input << line.substr(5);
			input >> filename;
			filename = filename + "/data_";
			filename = "../" + filename;
			cout << "\t    data path: " << filename << endl;
		}
		if(line.substr(0,4) == "time"){
			input.clear();
			input << line.substr(5);
			input >> t_total;
			mTotalTime = t_total;
			cout << "\t    simulation time = " << t_total << " s" << endl;
		}
		else if(line.substr(0,4) == "Eext"){
			put.clear();
			input.clear();
			convert.clear();
			cout << "\t    electric field: ";
			input << line.substr(5);
			for(int i(0);i<5;i++){
				convert.clear();
				getline(input, put, ' ');
				convert << put;
				convert >> param[i];
				if(input.eof()) break;
			}
			mEFieldAmp = param[0]; mEFieldDuration = param[1];
			mEFieldDirection = int(param[2]);
			cout << " amplitude = " << mEFieldAmp << " V/m, duration = " << mEFieldDuration << " s, in ";
			if(mEFieldDirection==0) cout << "x direction." << endl;
			if(mEFieldDirection==1) cout << "y direction." << endl;
			if(mEFieldDirection==2) cout << "z direction." << endl;
		}

		else if(line.substr(0,4) == "N_pt"){
			input.clear();
			input << line.substr(5);
			input >> N;
		}
		else if(line.substr(0,4) == "ring"){
			put.clear();
			input.clear();
			convert.clear();
			cout << "\t    ring";
			input << line.substr(5);
			for(int i(0);i<5;i++){
				convert.clear();
				getline(input, put, ' ');
				convert << put;
				
				convert >> param[i];
				if(input.eof()) break;
			}
			mTangle.push_back(new Ring(N, param[0], param[1], param[2], param[3], param[4]));
			cout << " r = " << param[0] << " m, p = (" << param[1];
			cout << ", " << param[2] << ", " << param[3] << ") m. " << endl;
			for(unsigned int i(0);i<param.size(); i++) param[i] = 0;
		}
		else if(line.substr(0,4) == "line"){
			put.clear();
			input.clear();
			convert.clear();
			cout << "\t    string";
			input << line.substr(5);
			for(int i(0);i<4;i++){
				convert.clear();
				getline(input, put, ' ');
				convert << put;
				convert >> param[i];
				if(input.eof()) break;
			}
			mTangle.push_back(new String(N, param[0], param[1], param[2], param[3]));
			cout << " L = " << param[0] << " m, p = (" << param[1];
			cout << ", " << param[2] << ", " << param[3] << ") m. " << endl;
			for(unsigned int i(0);i<param.size(); i++) param[i] = 0;
		}
		else if(line.substr(0,4) == "q_pt"){
			put.clear();
			input.clear();
			convert.clear();
			cout << "\t    charged segment on filament ";
			input << line.substr(5);
			for(int i(0);i<4;i++){
				convert.clear();
				getline(input, put, ' ');
				convert << put;
				convert >> param[i];
				if(input.eof()) break;
			}
			mTangle[param[0]]->mPoints[int(param[1])]->mCharge = param[2]; mTangle[param[0]]->mCarriesCharge = true;
			cout << param[0] << " with size " << param[2] << " C, at mesh point " << param[1] << ". " << endl;
		}
	}
	infile.close();
	return filename;
}