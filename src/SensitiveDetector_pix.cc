
#include "SensitiveDetector_pix.hh"

#include "G4Step.hh"
#include "Randomize.hh"

#include "G4HCofThisEvent.hh"

#include "G4HCtable.hh"
#include "G4SDManager.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4VProcess.hh"
#include "T01TrackInformation.hh"
#include "TrackingAction.hh"


SensitiveDetector_pix::SensitiveDetector_pix(G4String SDname)
  : G4VSensitiveDetector(SDname)
{
  // 'collectionName' is a protected data member of base class G4VSensitiveDetector.
  // Here we declare the name of the collection we will be using.
  collectionName.insert(SDname);
  //collectionName.insert("SiHit_pixCollection");
 
    //G4cout << "\nA new SDcollection was added: " << SDname << "\n" << G4endl;
    
  // Note that we may add as many collection names we would wish: ie
  // a sensitive detector can have many collections.
  HCID = -1;
}

SensitiveDetector_pix::~SensitiveDetector_pix()
{}

//G4bool SensitiveDetector_pix::ProcessHits(G4Step*Step,G4TouchableHistory*ROhist)
//{
// T01TrackInformation* info = (T01TrackInformation*)(Step->GetTrack()->GetUserInformation());
// G4cout << " Original Track ID " << info->GetOriginalTrackID() << G4endl;

  
//}
//****************************************************************************************************************************************
G4bool SensitiveDetector_pix::ProcessHits(G4Step *step, G4TouchableHistory *)//*********************
{
 

  T01TrackInformation* info = new T01TrackInformation(step->GetTrack()); // you could write atrack its fine
  //  T01TrackInformation* info = (T01TrackInformation*)(step->GetTrack()->GetUserInformation());
  // G4cout << " OriginalTrackID " << info->GetOriginalTrackID() << G4endl;   // Now the code its crash, but if you cooment out this command it will work {{DONT CHANGE ITS WORK}} 

  //  G4cout << " Original Position " << info->GetOriginalPosition() << G4endl;
  //G4cout << " particleDefinition " << info->GetparticleDefinition() << G4endl;
  // G4cout << " Original Track ID " << info->GetTrackID() << G4endl;

  // if(/*info->originalTrackID > 1 &&*/ info->Name != "e-") {info->Print();}
  
  //if(info->originalTrackID == 1 && info->Name != "e-") {info->Print();}



  //  if
  // (aTrackInfo->originalTrackID > 1);

  //  if(info->"proton"==Name);
      
  //  info->Print();
  // G4cout <<  originalPosition = aTrackInfo->originalPosition << G4endl;
  G4double truth_KE = info->GetkineticEnergy(); //********************************************** it may cause error
  G4ThreeVector point = info->GetOriginalPosition(); //**********************************************************  originalPosition = aTrack->GetPosition(); from the track information class
  //G4cout point;
  //G4cout truth_KE; 
  //G4cout << "Position " << point  << " K.E "  << truth_KE << G4endl;


//**********************************************************************************************************************************************
   //G4cout << "\nWe are now executing SensitiveDetector_pix::ProcessHits()\n" << G4endl;
    
  // step is guaranteed to be in Strip volume : no need to check for volume
   //T01TrackInformation* info = new T01TrackInformation(step->GetTrack()); //************************************************************************************
  G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
    
  // total energy deposit in this step
  G4double edep = step->GetTotalEnergyDeposit();
  //  G4cout << "SensitiveDetector_pix edep = " << edep << G4endl;

  //Get the Kinetic Energy of the particles from the G4track NOT from the G4Step
  //G4double truth_KE = info->GetkineticEnergy(); //********************************************** it may cause error

    
  // non-ionising component to energy loss. allows ionising (edep - ni_edep)/mass and
  // non ionising dose (ni_edep/mass) to be calculated at a later stage
  G4double ni_edep = step->GetNonIonizingEnergyDeposit();
    
  //check if step is due to primary particle: it has track ID 1 and parent 0
  // The primary is the track with ID 1 and with no parent
  G4bool isPrimary = (step->GetTrack()->GetTrackID() == 1 && step->GetTrack()->GetParentID() == 0 ) ? true : false;
//  G4bool isPrimary = step->GetTrack()->GetTrackID() > 0; // && step->GetTrack()->GetParentID() == 0 ) ? true : false; this work by this conditions *************************************
  //  G4cout << "isPrimary = " << isPrimary  << " at "  << G4endl;


  //if (edep <= 0.) return false;   //Gets rid of neutral particles

  // get step points in world coordinate system
  G4ThreeVector point1 = step->GetPreStepPoint()->GetPosition();
  G4ThreeVector point2 = step->GetPostStepPoint()->GetPosition();

  // G4ThreeVector point = info->GetOriginalPosition(); //**********************************************************  originalPosition = aTrack->GetPosition(); from the track information class
 
  //***********************************************************************************************
  G4ThreeVector momentum = step->GetPreStepPoint()->GetMomentum();
  // G4cout << "SensitiveDetector_pix momentum = " << momentum << G4endl;
 G4String Process_Name = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();                                                 // I include this
 //  G4cout << "ProcessName = " << Process_Name   << G4endl; // This will give you the process name on the current step


  //**********************************************************************************************
  // randomize point of energy deposition to get hit pos for this step
  G4ThreeVector pointE = point1 + G4UniformRand()*(point2 - point1);
  //G4ThreeVector pointE = point2;
	
  //Sets kinetic energy of hit
  G4double kin_e = step->GetTrack()->GetKineticEnergy();
                                               //G4cout << "PID = " << GetKineticEnergy() << G4endl;

  // get step points in world coordinate system
  G4double t1 = step->GetPreStepPoint()->GetLocalTime();
  G4double t2 = step->GetPostStepPoint()->GetLocalTime();
	
  // randomize point of energy deposition to get hit time for this step
  G4double htime = t1 + G4UniformRand()*(t2 - t1);							  

  G4int pixelCopyNo = touchable->GetReplicaNumber();
  G4int planeCopyNo = touchable->GetReplicaNumber(1);
  G4int track = step->GetTrack()->GetTrackID();
  //G4int Z = step->GetTrack()->GetDefinition()->GetPDGCharge();
  
  SiHit_pix* hit = new SiHit_pix(pixelCopyNo,planeCopyNo,isPrimary,track);
  hitCollection->insert(hit);

  G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName();
  // G4String particleParent = step->GetTrack()->GetDefinition()->GetParticleName();                                                 // I include this

  // G4cout << "particleName = " << particleName   << G4endl;


  //	for(particleName;)

		  // if ("proton"==particleName;)
	    
  // if(particleName == "gamma") {


  // G4cout << "particleName = " << particleName  << " at " << edep << G4endl;
  // }
  // G4cout << "ParentID = " << KineticEnergy << G4endl;

    
  // Use to get physics process used to create particle
  // not used as isPrimary tells us if it is a secondary or not.
  //const G4VProcess* creatorProcess = step->GetTrack()->GetCreatorProcess();
  //G4String processName = creatorProcess->GetProcessName();
    

  // store kinetic energy
  hit->SetKE(kin_e);
    
  // store energy deposition
  hit->AddEdep(edep);
    
  hit->AddTruth_KE(truth_KE); //******************************************************


  // store non-ionising energy deposition
  hit->AddNonIonisingEdep(ni_edep);
    
  // store hit time
  hit->SetHitTime(htime);

  // hit->SetOriginalTrackID          //********************************************************************************************
				
  // store position of energy deposition
  hit->SetPosition(pointE);
   
  hit->SetTruth_Position(point);//-************************************************************************ 
  // store particle name
    hit->SetParticleName(particleName);
  
  // store particle parent name //****************************************************************************
    //  hit->SetParticleParent(particleParent);                                                                                                   // i add this
 
  // store particle process name //****************************************************************************
     hit->SetProcessName(Process_Name);  

  // store process name (physics that generated hit)
  //hit->SetProcessName(processName);
    
  return true;
}

void SensitiveDetector_pix::Initialize(G4HCofThisEvent* HCE)
{

     //G4cout << "\nWe are now executing SensitiveDetector_pix::Initialize()\n" << G4endl;
    
  // ------------------------------
  // -- Creation of the collection
  // ------------------------------
  // -- collectionName[0] is "SiHit_pixCollection", as declared in constructor
  //hitCollection = new SiHit_pixCollection(GetName(), collectionName[0]);
  hitCollection = new SiHit_pixCollection(SensitiveDetectorName, collectionName[0]);

    //G4cout << "HitCollection generated for pixel sensitive detector: " << SensitiveDetectorName;
    //G4cout << " and collectionName[0] = " << collectionName[0] << G4endl;
    
  // ----------------------------------------------------------------------------
  // -- and attachment of this collection to the "Hits Collection of this Event":
  // ----------------------------------------------------------------------------
  // -- To insert the collection, we need to get an index for it. This index
  // -- is unique to the collection. It is provided by the GetCollectionID(...)
  // -- method (which calls what is needed in the kernel to get this index).
  //static G4int HCID = -1;
    
    if ( HCID < 0 ) {HCID = GetCollectionID(0);} // <<-- this is to get an ID for collectionName[0]
    HCE->AddHitsCollection(HCID, hitCollection);
 
}

void SensitiveDetector_pix::EndOfEvent(G4HCofThisEvent*)
{
    //G4cout << "\nWe are now executing SensitiveDetector_pix::EndOfEvent()\n" << G4endl;
    
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
