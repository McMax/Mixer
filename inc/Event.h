#ifndef EVENT_H
#define EVENT_H

#include "TObject.h"
#include "Particle.h"

class Event : public TObject
{
	UInt_t fEid;			//Event number
	TClonesArray* fParticles;	//Array of particles
	UInt_t fNpa;

public:
	Event();
	Event(UInt_t);
	virtual ~Event();
	
	inline UInt_t GetEid() const { return fEid;}
	inline UInt_t GetNpa() const { return fNpa;}
	inline TClonesArray* GetParticles() const { return fParticles;}
	Particle* GetParticle(UInt_t) const;

	inline void SetEid(UInt_t eid) { fEid = eid;}

	void AddParticle(UInt_t, Short_t, Float_t, Float_t, Float_t, Float_t, Float_t);
	void AddParticle(const Particle&);
	void Clear();
	void RemoveAt(Int_t);

	ClassDef(Event,1);
};

#endif
