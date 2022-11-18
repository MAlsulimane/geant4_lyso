
#ifndef SiHit_pix_h
#define SiHit_pix_h 1

/*
 * Define user class SiHit.
 *
 * We define "our" hit format : it is caracterized by its plane and strip
 * numbers, and an energy value, the accumulated energy in this strip
 */

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"

/*
 * This class stores information of a hit.
 *
 * It contains
 *  - PIXEL and plane number
 *  - deposited energy
 *  - position information
 */

class SiHit_pix : public G4VHit {
public:
  // Constructor
  SiHit_pix(const G4int pixel, const G4int plane, const G4bool isPrimary, G4int track);
  // Destructor
  ~SiHit_pix();
  // Print on screen a Hit
  void Print();
  
public:
  // The new and delete operators are overloaded for performances reasons:
  inline void *operator    new(size_t);
  inline void  operator delete(void *aHit);

public:
  // simple set and get methods
  void          SetKE(const double ke)                      { K_E = ke; }
  void          AddEdep(const double e)                     { eDep += e; }
  void          AddTruth_KE(const double truth_e)                     { truth_KE = truth_e; } //************************************may it error

  void          AddNonIonisingEdep(const double ni_e)       { ni_eDep += ni_e; }
  void          SetHitTime(const double t)                  { hit_time = t; }
  void          SetPosition(const G4ThreeVector & pos)      { position = pos; }
  void          SetTruth_Position(const G4ThreeVector & pos)      { truth_position = pos; } //*************************************
  void          SetParticleName(const G4String p_name)      { ParticleName = p_name; }
  void          SetParticleParent(const G4String p1_name)      { ParticleParent = p1_name; } //*******************************************************
  void          SetProcessName(const G4String pr_name)      { ProcessName = pr_name; } //*******************************************************


//void          SetProcessName(const G4String pr_name)      { ProcessName = pr_name; }
    
  G4double      GetKE()                const  { return K_E;}
  G4double      GetEdep()              const  { return eDep;}
 
  G4double      GetTruth_KE()              const  { return truth_KE;} //************************************ may cause error

  G4double      GetNonIonisingEdep()   const  { return ni_eDep;}
  G4double      GetHitTime()           const  { return hit_time;}
  G4ThreeVector GetPosition()          const  { return position; }
  G4ThreeVector GetTruth_Position()          const  { return truth_position; } //***********************
  G4int         GetPixelNumber()       const  { return pixelNumber; }
  G4int         GetPlaneNumber()       const  { return planeNumber; }
  G4int         GetTrackNumber()       const  { return trackNumber; }
  G4bool	    GetIsPrimary()         const  { return isPrimary; }
  G4String      GetParticleName()      const  { return ParticleName; }
  G4String      GetParticleParent()      const  { return ParticleParent; } //*************************************************************************
  G4String      GetProcessName()      const  { return ProcessName; } //*************************************************************************

//G4String      GetProcessName()       const  { return ProcessName; }
    
private:
  const G4int   pixelNumber, planeNumber, trackNumber;
  const G4bool  isPrimary;
  G4double      K_E;
  G4double      eDep;
 
  G4double      truth_KE; //**************************************** may cause error

  G4double      ni_eDep;
  G4double      hit_time;
  G4ThreeVector position;
  G4ThreeVector truth_position; //******************************
  G4String      ParticleName;
  G4String      ParticleParent; //*******************************************************************
  G4String      ProcessName; //*******************************************************************

//G4String      ProcessName;
    
};

// Define the "hit collection" using the template class G4THitsCollection:
typedef G4THitsCollection<SiHit_pix> SiHit_pixCollection;


// -- new and delete overloaded operators:
extern G4Allocator<SiHit_pix> SiHit_pixAllocator;

inline void* SiHit_pix::operator new(size_t)
{
  void *aHit;
  aHit = (void *) SiHit_pixAllocator.MallocSingle();
  return aHit;
}
inline void SiHit_pix::operator delete(void *aHit)
{
  SiHit_pixAllocator.FreeSingle((SiHit_pix*) aHit);
}

#endif
