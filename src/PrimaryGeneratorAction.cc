
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
  : outfile(0)
{
  gun = InitializeGPS();
  
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
  gun->GeneratePrimaryVertex(anEvent);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete gun;
}

G4VPrimaryGenerator* PrimaryGeneratorAction::InitializeGPS()
{
  /*G4GeneralParticleSource */ gps = new G4GeneralParticleSource();
    
  // setup details easier via UI commands see main.mac

  // particle type
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  // G4ParticleDefinition* ion = particleTable->FindParticle("Carbon");
  // gps->GetCurrentSource()->SetParticleDefinition(Carbon);

  // set energy distribution
  G4SPSEneDistribution *eneDist = gps->GetCurrentSource()->GetEneDist() ;
  eneDist->SetEnergyDisType("Mono"); // or gauss
  eneDist->SetMonoEnergy(190.8*MeV);

  // set position distribution
  G4SPSPosDistribution *posDist = gps->GetCurrentSource()->GetPosDist();
  posDist->SetPosDisType("Beam");  // or Point,Plane,Volume,Beam
  posDist->SetCentreCoords(G4ThreeVector(0.0*cm,0.0*cm,0.0*cm));
  //posDist->SetBeamSigmaInX(0.1*mm);
  //posDist->SetBeamSigmaInY(0.1*mm);

  // set angular distribution
  G4SPSAngDistribution *angDist = gps->GetCurrentSource()->GetAngDist();
  angDist->SetParticleMomentumDirection( G4ThreeVector(0., 0., 1.) );
  angDist->SetAngDistType("beam2d");
  
  //Test for tracking with zero incoming angle and large energy
  //comment out energy selection in main.mac to use
  //eneDist->SetMonoEnergy(190.8*GeV);
  //angDist->SetBeamSigmaInAngX(0.0*mrad);
  //angDist->SetBeamSigmaInAngY(0.0*mrad);
    
  // 240mmm range, 190.8 MeV protons
  angDist->SetBeamSigmaInAngX(7.3*mrad);
  angDist->SetBeamSigmaInAngY(7.3*mrad);
    
  angDist->DefineAngRefAxes("angref1",G4ThreeVector(-1.,0.,0.));
    
  return gps;
}

