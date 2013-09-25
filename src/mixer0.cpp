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
	Particle *particle, new_particle;
	input_tree->SetBranchAddress("event",&event);

	const Long64_t treeNentries = input_tree->GetEntries();
	Long64_t ev;
	UInt_t part;
	UInt_t Npart;

	vector<UInt_t> events_vect;
	vector<UInt_t> particles_vect;
	vector<UInt_t> mult_vect;
	map<UInt_t,unsigned> events_map;

	TRandom2 randgen(time(NULL));

	for(ev=0; ev<treeNentries; ++ev)
	{
		input_tree->GetEntry(ev);
		events_map.insert(pair<UInt_t,unsigned>(event->GetEid(),ev));
		
		Npart = event->GetNpa();
		mult_vect.push_back(Npart);
		for(part = 0; part<Npart; ++part)
		{
			particle = event->GetParticle(part);
			if((TMath::Abs(particle->GetBx()) > 4) || (TMath::Abs(particle->GetBy()) > 2))
				continue;
			events_vect.push_back(event->GetEid());
			particles_vect.push_back(particle->GetPid());
		}
		if(!(ev%50000))
			cout << "Event: " << ev << endl;
	}

	cout << "ParticleTree loaded. " << treeNentries << " events (" << ev << ")" <<  endl;
	cout << "First EID: " << events_vect[0] << " | Last EID: " << events_vect[events_vect.size()-1] << endl;


	const UInt_t firstPID = particles_vect[0];
	const UInt_t lastPID = particles_vect[particles_vect.size()-1];
	UInt_t list_size = lastPID - firstPID;

	cout << "First PID: " << firstPID << " | last PID: " << lastPID << " | size: " << list_size << endl;

	ParticleTree output_tree(outputfile);
	set<int> unique_events_set;
	unsigned control=0;
	int rand_evid, rand_part;
	unsigned int Nentries = events_map.size();
	unsigned long 	switch_counter=0,
					switch_limit = Nentries*Nentries;

	bool event_cancelled = false;

	for(ev = 0; ev<Nentries; ++ev)
	{
		unique_events_set.clear();
		control = 0;
		input_tree->GetEntry(ev);
		output_tree.BeginEvent(event->GetEid());

		switch_counter = 0;
		event_cancelled = false;

		//cout << ev << ": multiplicity: " << mult_vect[ev] << endl;
		for(int j=0; j<mult_vect[ev]; ) //number of part/event = MULTIP[i]
		{
			rand_part = randgen.Rndm()*particles_vect.size();
			//cout << "rand_part = " << rand_part << " = " << particles_vect[rand_part] << endl;

			rand_evid = events_vect[rand_part];
			//cout << "rand_evid = " << rand_evid << endl;

			unique_events_set.insert(rand_evid);
			//cout << "If" << endl;
			if(unique_events_set.size()>control)
			{
				//cout << "Getting event from tree" << endl;
				input_tree->GetEntry(events_map[rand_evid]);
				
				/*
				cout << "Event in tree: " << event->GetEid() << endl;
				cout << "Particles: " << endl;
				for(int i=0; i<event->GetNpa(); i++)
					cout << i << ": " << ((Particle*)event->GetParticle(i))->GetPid() << endl;

				cout << "First particle in event: " << event->GetFirstParticle() << endl;
				cout << "Getting particle: " << ((particles_vect[rand_part] - event->GetFirstParticle())) << endl;
				*/
				particle = event->GetParticle(particles_vect[rand_part] - event->GetFirstParticle());
				/*
				cout << "#Ev: " << ev << ", EVID: " << rand_evid << ", rand_part: " << particles_vect[rand_part] << endl;
					cout << "Adding particle" << endl;
					cout << "Pid: " << particles_vect[rand_part];
					cout << " px: " << particle->GetPx() << endl;
				*/
				output_tree.AddParticle(*particle, particles_vect[rand_part]);

				//cout << "Erasing particle with PID: " << particles_vect[rand_part] << endl;
				events_vect.erase(events_vect.begin() + rand_part);
				particles_vect.erase(particles_vect.begin() + rand_part);
				++control;
				j=j+1;
			}
			else
			{
				if(switch_counter > switch_limit)
				{
					event_cancelled = true;
					break;
				}

				if(!(++switch_counter%Nentries))
					cout << "Rejected. Event: " << ev << " | from old event: " << rand_evid << " ctr:" << switch_counter << "/" << switch_limit << endl;
				continue;
			}

		}  //track loop

		if(event_cancelled)
			output_tree.CancelEvent();
		else
			output_tree.EndEvent();

		//cout << " | from different events: " << unique_events_set.size() << endl;
		//if(!(ev%100))
			cout << ev << "/" << treeNentries << " multiplicity: "<< mult_vect[ev] << ", from diff. events: " << unique_events_set.size()<< endl;
	}

	cout << "Closing" << endl;
	output_tree.Close();
	input_root_file->Close();

	return 0;
}
