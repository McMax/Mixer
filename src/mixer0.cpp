#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime> 

#include "TFile.h"
#include "TTree.h"
#include "TRandom2.h"
#include "Particle.h"
#include "Event.h"
#include "ParticleTree.h"

using namespace std;

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		cout << "Usage: mixer0 <filenumber>" << endl;
		return -1;
	}

	UInt_t filenumber = atoi(argv[1]);

	TString filename = "ParticleTree";
	TString outputfile = "MixedParticleTree";
	filename += filenumber;
	outputfile += filenumber;
	filename += ".root";
	outputfile += ".root";
	TFile *input_root_file = new TFile(filename);
	TTree* input_tree = (TTree*)input_root_file->Get("events");

	Event *event = new Event();
	Particle *particle;
	input_tree->SetBranchAddress("event",&event);

	const Long64_t treeNentries = input_tree->GetEntries();
	Long64_t ev;
	UInt_t Npart;

	vector<Long64_t> events_vect;
	vector<UInt_t> mult_vect;

	TRandom2 randgen(time(NULL));

	for(ev=0; ev<treeNentries; ++ev)
	{
		input_tree->GetEntry(ev);
		events_vect.push_back(ev);
		
		Npart = event->GetNpa();
		mult_vect.push_back(Npart);
		if(!(ev%50000))
			cout << "Event: " << ev << endl;
	}

	cout << "ParticleTree loaded. " << treeNentries << " events" << endl;

	ParticleTree output_tree(outputfile);
	set<int> unique_events_set;
	unsigned control=0;
	int rand_evid, rand_pid;

	for(ev = 0; ev<events_vect.size(); ++ev)
	{
		unique_events_set.clear();
		control = 0;
		output_tree.BeginEvent();

		//cout << ev << ": multiplicity: " << mult_vect[ev] << endl;
		for(int j=0; j<mult_vect[ev]; ) //number of part/event = MULTIP[i]
		{
			rand_evid = randgen.Rndm()*events_vect.size();
			//cout << "rand_evid: " << rand_evid << endl;
			unique_events_set.insert(events_vect[rand_evid]);
			if(unique_events_set.size()>control)
			{
			//	cout << "Getting event from tree" << endl;
				input_tree->GetEntry(events_vect[rand_evid]);
				rand_pid = randgen.Rndm()*mult_vect[rand_evid];
			//	cout << "rand_pid: " << rand_pid << " getting particle" << endl;
				particle = event->GetParticle(rand_pid);
			//	cout << "Adding particle" << endl;
			//	cout << "Pid: " << particle->GetPid() << " px: " << particle->GetPx() << endl;
				output_tree.AddParticle(particle->GetPid(), particle->GetCharge(), particle->GetBx(), particle->GetBy(), particle->GetPx(), particle->GetPy(), particle->GetPz());

				events_vect.erase(events_vect.begin()+rand_evid);
				mult_vect.erase(mult_vect.begin()+rand_evid);

				++control;
				j=j+1;
			}
			else
			{
				cout << "Rejected" << endl;
				continue;
			}

		}  //track loop

		output_tree.EndEvent();

		//cout << " | from different events: " << unique_events_set.size() << endl;
		if(!(ev%50))
			cout << ev << "/" << treeNentries << " multiplicity: "<< mult_vect[ev] << ", from diff. events: " << unique_events_set.size()<< endl;
	}

	output_tree.Close();
	input_root_file->Close();
}
