
#include "T01TrackInformation.hh"
#include "G4ios.hh"

G4Allocator<T01TrackInformation> aTrackInformationAllocator;



T01TrackInformation::T01TrackInformation()
{
    originalTrackID = 0;
    particleDefinition = 0;
      originalPosition = G4ThreeVector(0.,0.,0.);
     originalMomentum = G4ThreeVector(0.,0.,0.);
     originalEnergy = 0.;
     kineticEnergy = 0.; //**********implement the K.E for the particles
     originalTime = 0.;
     parent_ID = 0;
}

T01TrackInformation::T01TrackInformation(const G4Track* aTrack)
{
   originalTrackID = aTrack->GetTrackID();
    particleDefinition = aTrack->GetDefinition();
     originalPosition = aTrack->GetPosition();
     originalMomentum = aTrack->GetMomentum();
     kineticEnergy = aTrack->GetKineticEnergy();//**********************
     originalEnergy = aTrack->GetTotalEnergy();
     originalTime = aTrack->GetGlobalTime();
        particleName = particleDefinition -> GetParticleName(); 
     //particleName = DynamicParticle -> GetParticleName(); 

       parent_ID = aTrack->GetParentID();
       //G4ParticleDefinition* thisDef = particleDefinition ->ParentName();
	 parentName = particleDefinition -> GetParticleSubType();
	 // GetParticleType();  This give you for example either lepton e- or neutrino OR bayon proton, neutron 


}

T01TrackInformation::T01TrackInformation(const T01TrackInformation* aTrackInfo)
{
  originalTrackID = aTrackInfo->originalTrackID;
    particleDefinition = aTrackInfo->particleDefinition;
     originalPosition = aTrackInfo->originalPosition;
     originalMomentum = aTrackInfo->originalMomentum;
    originalEnergy = aTrackInfo->originalEnergy;
    kineticEnergy = aTrackInfo->kineticEnergy; //*********************
    originalTime = aTrackInfo->originalTime;
     parent_ID = aTrackInfo->parent_ID;
       particleName = particleDefinition-> GetParticleName(); 
       // particleName = DynamicParticle-> GetParticleName(); 

}

T01TrackInformation::~T01TrackInformation(){;}

void T01TrackInformation::Print() const
{
    G4cout 

      //  << "Original track ID " << originalTrackID << " Particle_Name " << particleName << " ParentID  " << parent_ID  << "  Parent  " << parentName << " at " << originalPosition << " Energy = " << originalEnergy  << G4endl; 
    //<< "Original ID " <<   originalMomentum
      /*<< " at " << originalPosition*/  
 
    //G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName();
      << "Original Position " << originalPosition << "kineticEnergy" << kineticEnergy <<  G4endl;
}
