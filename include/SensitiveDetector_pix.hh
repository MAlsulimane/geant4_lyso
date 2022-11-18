
#ifndef SensitiveDetector_pix_h
#define SensitiveDetector_pix_h 1

#include "G4VSensitiveDetector.hh"
class DetectorConstruction;
class RunAction;

#include "SiHit_pix.hh"              // <<- the hit "format" we define
class G4HCofThisEvent;           // <<- means "H(it) C(ollections) of This Event"


/*
 * Defines sensitve part of detector geometry.
 *
 * Stores Hits with 
 *  * deposited energy
 *  * position
 * in <i>Hit Collections of This Event</i>
 *
 * ProcessHits()
 */
class SensitiveDetector_pix : public G4VSensitiveDetector
{
public:
  // Constructor
  SensitiveDetector_pix(G4String SDname);
  // Destructor
  ~SensitiveDetector_pix();

public:
  // methods from base class G4VSensitiveDetector
  // Mandatory base class method : it must to be overloaded:
  G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);

  // (optional) method of base class G4VSensitiveDetector
  void Initialize(G4HCofThisEvent* HCE);
  // (optional) method of base class G4VSensitiveDetector
  void EndOfEvent(G4HCofThisEvent* HCE);


private:
  SiHit_pixCollection*      hitCollection;
  G4int                 HCID;       //JT
};

#endif
