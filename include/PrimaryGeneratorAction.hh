// PrimaryGeneratorAction.hh

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

// Defines mandatory user class PrimaryGeneratorAction

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"

#include <fstream>


class G4VPrimaryGenerator;
 
// This mandatory user class provides the primary particle generator
//
// Geant4 provides a number of predefined primary particle generator, to be utilised by the user.
//  - G4ParticleGun
//  - G4GeneralParticleSource
//
// GeneratePrimaries()

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  // constructor
  PrimaryGeneratorAction();
  // destructor
  ~PrimaryGeneratorAction();
  // defines primary particles (mandatory)
  void GeneratePrimaries(G4Event*);
    G4GeneralParticleSource * gps;
private:  
  G4VPrimaryGenerator* InitializeGPS();
private:
  G4VPrimaryGenerator* gun;
  std::ofstream * outfile;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
