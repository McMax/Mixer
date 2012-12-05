#include <iostream>
#include "ParticleTree.h"

ParticleTree::ParticleTree(TString filename)
{
	file = new TFile(filename, "recreate");
	tree = new TTree("events","events");
	event = new Event();
	tree->Branch("event","Event",&event);
	part_id = ev_id = 0;
}

void ParticleTree::SetEvid(UInt_t eid)
{
	ev_id = eid;
}

void ParticleTree::BeginEvent()
{
	event->SetEid(++ev_id);
}

void ParticleTree::BeginEvent(UInt_t eid)
{
	event->SetEid(eid);
}

UInt_t ParticleTree::Check()
{
	return event->GetNpa();
}		

void ParticleTree::EndEvent()
{
	using namespace std;
	
	tree->Fill();
	event->Clear();
}

void ParticleTree::CancelEvent()
{
	event->Clear();
}

void ParticleTree::AddParticle(Short_t charge, Float_t bx, Float_t by,  Float_t px, Float_t py, Float_t pz, Float_t dedx)
{
	event->AddParticle(++part_id, charge, bx, by, px, py, pz, dedx);
}

void ParticleTree::AddParticle(UInt_t pid, Short_t charge, Float_t bx, Float_t by,  Float_t px, Float_t py, Float_t pz, Float_t dedx)
{
	event->AddParticle(pid, charge, bx, by, px, py, pz, dedx);
}

void ParticleTree::Close()
{
	tree->AutoSave("overwrite");
	delete tree;
	file->Close();
	delete file;
}

ParticleTree::~ParticleTree()
{

}
