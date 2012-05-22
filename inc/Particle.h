#ifndef PARTICLE_H
#define PARTICLE_H

#include "TObject.h"

class Particle : public TObject
{
	UInt_t fPid;		//Particle index
	Short_t fCharge;	//Particle absolute charge
	Float_t fBx;		//Particle bx parameter
	Float_t fBy;		//Particle by parameter
	Float_t fPx;		//Particle momentum on X-axis
	Float_t fPy;		//Particle momentum on Y-axis
	Float_t fPz;		//Particle momentum on Z-axis

public:
	Particle();
	Particle(UInt_t, Short_t, Float_t, Float_t, Float_t, Float_t, Float_t);
	virtual ~Particle();

	inline UInt_t GetPid() const { return fPid;}
	inline Short_t GetCharge() const { return fCharge;}
	inline Float_t GetBx() const { return fBx;}
	inline Float_t GetBy() const { return fBy;}
	inline Float_t GetPx() const { return fPx;}
	inline Float_t GetPy() const { return fPy;}
	inline Float_t GetPz() const { return fPz;}
	
	inline void SetPid(UInt_t pid) { fPid = pid;}
	inline void SetCharge(Short_t charge) { fCharge = charge;}
	inline void SetBx(Float_t bx) { fBx = bx;}
	inline void SetBy(Float_t by) { fBy = by;}
	inline void SetPx(Float_t px) { fPx = px;}
	inline void SetPy(Float_t py) { fPy = py;}
	inline void SetPz(Float_t pz) { fPz = pz;}

	ClassDef(Particle,1);
};

#endif
