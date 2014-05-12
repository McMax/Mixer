#include <iostream>
#include <cstdlib>

#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"

using namespace std;

int split(const TString path, Int_t divider)
{
	TString filename = path;
	UInt_t pozycja1 = path.Last('/');
	if(pozycja1 > 200)
		pozycja1 = 0;
	TString file = filename.Remove(filename.Index("."));
	file.Remove(0,pozycja1);

	TFile *file_to_split = new TFile(path);
	if(file_to_split->IsZombie())
	{
		cout << "File " << file_to_split << " is not opened" << endl;
		return -1;
	}
	TFile *newfile;
	TTree *new_input_tree;
	TString temp;

	TTree* input_tree = (TTree*)file_to_split->Get("events");
	Int_t nentries = input_tree->GetEntries();

	if(nentries==0)
	{
		cout << "Particle tree has no events. Stopping..." << endl;
		return 1;
	}

	Int_t nentries_divider = nentries/divider;

	for(int i=0; i<divider; i++)
	{
		cout << "File " << i << " starting from " << i*nentries_divider << " and copying " << nentries_divider << endl;
		temp = file;
		temp += i;
		temp += ".root";

		newfile = new TFile(temp,"recreate");
		new_input_tree = input_tree->CopyTree("","",nentries_divider,i*nentries_divider);
		newfile->cd();
		new_input_tree->Write();
		newfile->Close();
	}

	file_to_split->Close();

	return 1;
}

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		cout << "Usage: splitter <BASE_FILE> <NUMBER_OF_FILES>" << endl;
		return 0;
	}
	TApplication app("app",&argc,argv);
	int signal = split(app.Argv(1), atoi(app.Argv(2)));

	return signal;
}

