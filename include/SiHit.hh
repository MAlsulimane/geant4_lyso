
#ifndef SiHit_h
#define SiHit_h 1

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
 *  - strip and plane number
 *  - deposited energy
 *  - position information
 */

class SiHit : public G4VHit {
public:
  // Constructor
  SiHit(const G4int strip, const G4int plane, const G4bool isPrimary, G4int track);
  // Destructor
  ~SiHit();
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
  void          AddNonIonisingEdep(const double ni_e)       { ni_eDep += ni_e; }
  void          SetHitTime(const double t)                  { hit_time = t; }
  void          SetPosition(const G4ThreeVector & pos)      { position = pos; }
  void          SetParticleName(const G4String p_name)      { ParticleName = p_name; }
//void          SetProcessName(const G4String pr_name)      { ProcessName = pr_name; }
    
  G4double      GetKE()                const  { return K_E;}
  G4double      GetEdep()              const  { return eDep;}
  G4double      GetNonIonisingEdep()   const  { return ni_eDep;}
  G4double      GetHitTime()           const  { return hit_time;}
  G4ThreeVector GetPosition()          const  { return position; }
  G4int         GetStripNumber()       const  { return stripNumber; }
  G4int         GetPlaneNumber()       const  { return planeNumber; }
  G4int         GetTrackNumber()       const  { return trackNumber; }
  G4bool	    GetIsPrimary()         const  { return isPrimary; }
  G4String      GetParticleName()      const  { return ParticleName; }
//G4String      GetProcessName()       const  { return ProcessName; }
    
private:
  const G4int   stripNumber, planeNumber, trackNumber;
  const G4bool  isPrimary;
  G4double      K_E;
  G4double      eDep;
  G4double      ni_eDep;
  G4double      hit_time;
  G4ThreeVector position;
  G4String      ParticleName;
//G4String      ProcessName;
    
};

// Define the "hit collection" using the template class G4THitsCollection:
typedef G4THitsCollection<SiHit> SiHitCollection;


// -- new and delete overloaded operators:
extern G4Allocator<SiHit> SiHitAllocator;

inline void* SiHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) SiHitAllocator.MallocSingle();
  return aHit;
}
inline void SiHit::operator delete(void *aHit)
{
  SiHitAllocator.FreeSingle((SiHit*) aHit);
}

#endif
