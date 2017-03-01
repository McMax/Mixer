#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>

#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"

using namespace std;

void findPrimes(int entries, vector<int> &primeNumbers)
{
	bool prime;

	for(int i=2; i<=entries; i++)
	{
		prime = true;

		for(int j=2; j<= sqrt(i); j++)
		{
			if((i%j)==0)
			{
				prime = false;
				break;
			}
		}

		if(prime)
			primeNumbers.push_back(i);
	}

	cout << primeNumbers.size() << endl;

	return;
}

void findFactors(int entries, vector<int> &primeNumbers, vector<int> &primeFactors)
{
	int current;

	for(int i=0; i<primeNumbers.size(); i++)
	{
		current = primeNumbers[i];
		if((entries%current)==0)
		{
			primeFactors.push_back(current);
			findFactors(entries/current,primeNumbers,primeFactors);	//To understand recurrence, you must firstly understand recurrence
			break;
		}
	}
}

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
		return 1;
	}
	TFile *newfile;
	TTree *new_input_tree;
	TString temp;

	TTree* input_tree = (TTree*)file_to_split->Get("events");
	Int_t nentries = input_tree->GetEntries();

	if(nentries==0)
	{
		cout << "Particle tree has no events. Stopping..." << endl;
		return 2;
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

	return 0;
}

void create_modulo_table(int entries)
{
	int start = 0, end = 70;
	cout << "Table of modulo divisions" << endl;

	cout << "#\t";
	for(int j=0; j<10; j++)
		cout << j << "\t";
	cout << endl;

	for(int i=start; i<=end; i+=10)
	{
		cout << i << "\t";

		for(int j=0; j<10; j++)
		{
			if((i==0) && (j<=1))
				cout << "-\t";
			else
				cout << (entries%(i+j)) << "\t";
		}

		cout << endl;
	}

	return;
}

int optimal_split(const TString path)
{
	vector<int> primeNumbers;
	vector<int> primeFactors;

	int divider;

	TFile *file_to_split = new TFile(path);
	if(file_to_split->IsZombie())
	{
		cout << "File " << file_to_split << " is not opened" << endl;
		return -1;
	}
	TTree* input_tree = (TTree*)file_to_split->Get("events");
	int nentries = input_tree->GetEntries();
	file_to_split->Close();

	cout << "Number of events: " << nentries << endl;
	create_modulo_table(nentries);

	//Finding prime numbers

	cout << "Calculating prime numbers" << endl;
	findPrimes(nentries, primeNumbers);

	//Finding prime factors
	cout << "Finding prime factors" << endl;
	findFactors(nentries, primeNumbers, primeFactors);

	cout << "Prime factor" << endl;
	for(int i=0; i<primeFactors.size(); i++)
		cout << primeFactors[i] << endl;

	cout << endl << "Enter divider: ";
	cin >> divider;
	cout << endl;

	return split(path, divider);
}

//Splitter divides big ParticleTree file into a given number of files to let them mix in parallel.
//If one divides the number of all events in wrong way, there will be some rest events that will not get into the smaller ParticleTrees.
//That is why an "optimal_split" function was written to choose the divider which minimize or get rid of rest events.
int main(int argc, char* argv[])
{
	int signal = -1;

	if(argc == 3)
	{
		TApplication app("app",&argc,argv);
		signal = split(app.Argv(1), atoi(app.Argv(2)));
	}
	else if(argc == 2)
	{
		TApplication app("app",&argc,argv);
		signal = optimal_split(app.Argv(1));
	}
	else
	{
		cout << "Usage: splitter <BASE_FILE> <NUMBER_OF_FILES>\nor" << endl;
		cout << "splitter <BASE_FILE> - will calculate modulo table and let you choose the divider" << endl;
		return -1;
	}

	return signal;
}

