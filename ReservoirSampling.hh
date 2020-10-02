#ifndef RESERVOIRSAMPLING_HH
#define RESERVOIRSAMPLING_HH

#include "TVector3.h"
#include "TString.h"

#include <vector>

class TFile;
class TTree;
class TRandom3;

class ReservoirSampling{

public:
	ReservoirSampling(int, TString);
	~ReservoirSampling();

	void SampleEvent(double, TVector3, TVector3);
	
	void FillTree();
	void WriteTree();

private:

	void InitializeTree();

	std::vector<double> R;
	std::vector<TVector3> PE;
	std::vector<TVector3> PL;
	std::vector<TVector3> PR;

	double pe[3];
	double pn[3];

	TRandom3* fRand;

	TString fFilename;
	TFile* fFile;
	TTree* fTree;
	
	int fK;
	double minR; 
	int minIndex;
	double jump; 
	
};	

#endif

