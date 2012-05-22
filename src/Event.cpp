#include "TObject.h"
#include "TClonesArray.h"

#include "Particle.h"
#include "Event.h"

Event::Event()
{
	fEid = 0;
	fParticles = new TClonesArray("Particle",15);
	fNpa = 0;
}

Event::Event(UInt_t eid)
{
	fEid = eid;
	fParticles = new TClonesArray("Particle",15);
	fNpa = 0;
}

Event::~Event()
{
	Clear();
	delete fParticles;
}

Particle* Event::GetParticle(UInt_t index) const
{
	if((index < 0) || (index >= fNpa))
		return NULL;

	return ((Particle*) fParticles->At(index));
}

void Event::AddParticle(UInt_t pid, Short_t charge, Float_t bx, Float_t by, Float_t px, Float_t py, Float_t pz)
{
	new ((*fParticles) [fNpa]) Particle(pid, charge, bx, by, px, py, pz);
	fNpa++;
}

void Event::AddParticle(const Particle& particle)
{
	new ((*fParticles) [fNpa]) Particle(particle);
	fNpa++;
}

void Event::Clear()
{
	fParticles->Clear();
	fNpa = 0;
}

void Event::RemoveAt(Int_t index)
{
	fParticles->RemoveAt(index);
	fParticles->Compress();
}

ClassImp(Event);
