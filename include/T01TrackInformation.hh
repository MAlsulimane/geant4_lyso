
#ifndef T01TrackInformation_h
#define T01TrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class T01TrackInformation : public G4VUserTrackInformation 
{
  public:
    T01TrackInformation();
  T01TrackInformation(const G4Track* aTrack); 
  T01TrackInformation(const T01TrackInformation* aTrackInfo); 
    virtual ~T01TrackInformation();

    inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);
    inline int operator ==(const T01TrackInformation& right) const
  {return (this==&right);}

    void Print() const;
  G4String             particleName;
  G4String             parentName;

  //private: I comment out the private, its wrong and dangereus, but if I dont do this I cant set any if conditions in SD Detector_pix class. Its nesassery step.
    G4int                 originalTrackID;
    G4int                 parent_ID;

   G4ParticleDefinition* particleDefinition;
  //G4ParticleDefinition* thisDef;
  //  G4ParticleDefinition* thisDef = aTrack->GetDefinition();
   G4ThreeVector         originalPosition;
   G4ThreeVector         originalMomentum;
   G4double              originalEnergy;
  G4double              kineticEnergy; //***********************add this to define the k.E

   G4double              originalTime;
  // G4String             Name;


  public:
    inline G4int GetOriginalTrackID() const {return originalTrackID;}
    inline G4ParticleDefinition* GetOriginalParticle() const {return particleDefinition;}
   inline G4ThreeVector GetOriginalPosition() const {return originalPosition;}
   inline G4ThreeVector GetOriginalMomentum() const {return originalMomentum;}
   inline G4double GetOriginalEnergy() const {return originalEnergy;}
   inline G4double GetOriginalTime() const {return originalTime;}
  inline G4double GetkineticEnergy() const {return kineticEnergy;} //**************************************

  // inline G4String GetDynamicParticle() const {return DynamicParticle;}

};

extern G4Allocator<T01TrackInformation> aTrackInformationAllocator; 

inline void* T01TrackInformation::operator new(size_t)
{ void* aTrackInfo; 
  aTrackInfo = (void*)aTrackInformationAllocator.MallocSingle(); 
  return aTrackInfo; 
}

inline void T01TrackInformation::operator delete(void *aTrackInfo) 
{ aTrackInformationAllocator.FreeSingle((T01TrackInformation*)aTrackInfo);} 

#endif
