
#include "SensitiveDetector.hh"

#include "G4Step.hh"
#include "Randomize.hh"

#include "G4HCofThisEvent.hh"

#include "G4HCtable.hh"
#include "G4SDManager.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4VProcess.hh"

SensitiveDetector::SensitiveDetector(G4String SDname)
  : G4VSensitiveDetector(SDname)
{
  // 'collectionName' is a protected data member of base class G4VSensitiveDetector.
  // Here we declare the name of the collection we will be using.
  collectionName.insert(SDname);
    
    //G4cout << "\nA new SDcollection was added: " << SDname << "\n" << G4endl;
    
  //collectionName.insert("SiHitCollection");
 
  // Note that we may add as many collection names we would wish: ie
  // a sensitive detector can have many collections.
  HCID = -1;
}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  // step is guaranteed to be in pixel volume : no need to check for volume
  
  G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
    
  // total energy deposit in this step
  G4double edep = step->GetTotalEnergyDeposit();
  //G4cout << "SensitiveDetector edep = " << edep << G4endl;
    
  // non-ionising component to energy loss. allows ionising (edep - ni_edep)/mass and
  // non ionising dose (ni_edep/mass) to be calculated at a later stage
  G4double ni_edep = step->GetNonIonizingEnergyDeposit();
    
  //check if step is due to primary particle: it has track ID 1 and parent 0
  // The primary is the track with ID 1 and with no parent
  G4bool isPrimary = ( step->GetTrack()->GetTrackID() == 1 && step->GetTrack()->GetParentID() == 0 ) ? true : false;

  //if (edep <= 0.) return false;   //Gets rid of neutral particles

  // get step points in world coordinate system
  G4ThreeVector point1 = step->GetPreStepPoint()->GetPosition();
  G4ThreeVector point2 = step->GetPostStepPoint()->GetPosition();
  
  // randomize point of energy deposition to get hit pos for this step
  G4ThreeVector pointE = point1 + G4UniformRand()*(point2 - point1);
  //G4ThreeVector pointE = point2;
	
  //Sets kinetic energy of hit
  G4double kin_e = step->GetTrack()->GetKineticEnergy();
    
  // get step points in world coordinate system
  G4double t1 = step->GetPreStepPoint()->GetLocalTime();
  G4double t2 = step->GetPostStepPoint()->GetLocalTime();
	
  // randomize point of energy deposition to get hit time for this step
  G4double htime = t1 + G4UniformRand()*(t2 - t1);							  

  G4int stripCopyNo = touchable->GetReplicaNumber();
  G4int planeCopyNo = touchable->GetReplicaNumber(1);
  G4int track = step->GetTrack()->GetTrackID();
  //G4int Z = step->GetTrack()->GetDefinition()->GetPDGCharge();
  
  SiHit* hit = new SiHit(stripCopyNo,planeCopyNo,isPrimary,track);
  hitCollection->insert(hit);

  G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName();
  //G4cout << "particleName = " << particleName << G4endl;
    
  // Use to get physics process used to create particle
  // not used as isPrimary tells us if it is a secondary or not.
  //const G4VProcess* creatorProcess = step->GetTrack()->GetCreatorProcess();
  //G4String processName = creatorProcess->GetProcessName();
    

  // store kinetic energy
  hit->SetKE(kin_e);
    
  // store energy deposition
  hit->AddEdep(edep);
    
  // store non-ionising energy deposition
  hit->AddNonIonisingEdep(ni_edep);
    
  // store hit time
  hit->SetHitTime(htime);
				
  // store position of energy deposition
  hit->SetPosition(pointE);
    
  // store particle name
  hit->SetParticleName(particleName);
    
  // store process name (physics that generated hit)
  //hit->SetProcessName(processName);
    
  return true;
}

void SensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{

  // ------------------------------
  // -- Creation of the collection
  // ------------------------------
  // -- collectionName[0] is "SiHitCollection", as declared in constructor
  //hitCollection = new SiHitCollection(GetName(), collectionName[0]);
  hitCollection = new SiHitCollection(SensitiveDetectorName, collectionName[0]);

    //G4cout << "HitCollection generated for strip sensitive detector: " << SensitiveDetectorName;
    //G4cout << " and collectionName[0] = " << collectionName[0] << G4endl;
    
  // ----------------------------------------------------------------------------
  // -- and attachment of this collection to the "Hits Collection of this Event":
  // ----------------------------------------------------------------------------
  // -- To insert the collection, we need to get an index for it. This index
  // -- is unique to the collection. It is provided by the GetCollectionID(...)
  // -- method (which calls what is needed in the kernel to get this index).
  //static G4int HCID = -1;
    
  if (HCID<0) HCID = GetCollectionID(0); // <<-- this is to get an ID for collectionName[0]
  HCE->AddHitsCollection(HCID, hitCollection);
 
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  /*
  // test output of hits
  G4cout << "EndOfEvent method of SD `" << GetName() << "' called." << G4endl;
  for (size_t i = 0; i < hitCollection->GetSize(); ++i ) {
    G4cout<<i<<G4endl;
	(*hitCollection[i])->Print();
  }
  */

  // -- we could have attached the collection to the G4HCofThisEvent in this
  // -- method as well (instead of Initialize).
}
