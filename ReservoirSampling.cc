#include "ReservoirSampling.hh"

#include "TTree.h"
#include "TFile.h"
#include "TRandom3.h"

using namespace std;

ReservoirSampling::ReservoirSampling(int K, TString filename) {

	fK = K;

	fFileName = filename;

	fRand = new TRandom3(0);	

}

void ReservoirSampling::SampleEvent(double weight, TVector3 electron, TVector3 neutron){

	if(R.size() < fK) {

		double r = pow(fRand->Uniform(), 1./weight);		
		
		R.push_back(r);
		PE.push_back(electron);
		PR.push_back(neutron);

		minR = *min_element(R.begin(), R.end());
		minIndex = min_element(R.begin(),R.end()) - R.begin();
		jump = log(fRand->Uniform()) / log(minR);

	} else {

		jump -= weight;

		if(jump <= 0.) {

			double t = pow(minR, weight);
			double r = pow(fRand->Uniform(t,1.), 1./weight);
						
			R[minIndex] = r;
			PE[minIndex] = electron;
			PR[minIndex] = neutron;			

			minR = *min_element(R.begin(), R.end());
			minIndex = min_element(R.begin(),R.end()) - R.begin();
			jump = log(fRand->Uniform()) / log(minR);
		
		}
	
	}

} 


void ReservoirSampling::InitializeTree() {

        fTree = new TTree("T", "BAND fast Monte Carlo");

        fTree->SetMaxTreeSize(1900000000);  // 1.9 GB

        fTree->Branch("pe", pe, "pe[3]/D");
        fTree->Branch("pn", pn, "pn[3]/D");


}

void ReservoirSampling::FillTree() {
	
	fFile = new TFile(filename, "RECREATE");	
	InitializeTree();

	for(int i = 0; i < fK; i++) {

		pe[0] = PE[i].x();
		pe[1] = PE[i].y();
		pe[2] = PE[i].z();

		pn[0] = PR[i].x();
		pn[1] = PR[i].y();
		pn[2] = PR[i].z();

		fTree->Fill();

	}

}

void ReservoirSampling::WriteTree() {

        fFile->cd();
        fTree->Write("T", TObject::kOverwrite);

        fTree->ResetBranchAddresses();
        delete fTree;
        fTree = NULL;

        fFile->Close();

        delete fFile;
        fFile = NULL;


}



