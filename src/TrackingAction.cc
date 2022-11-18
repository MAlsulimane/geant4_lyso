
#include "TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "T01TrackInformation.hh"

TrackingAction::TrackingAction()
{;}

TrackingAction::~TrackingAction()
{;}

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)//****
{
  if(aTrack->GetParentID()==0 && aTrack->GetUserInformation()==0) //****
   {
     T01TrackInformation* anInfo = new T01TrackInformation(aTrack); //****
   G4Track* theTrack = (G4Track*)aTrack; //****
   theTrack->SetUserInformation(anInfo);
  }
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack) //****
{
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
    if(secondaries)
   {
     T01TrackInformation* info = (T01TrackInformation*)(aTrack->GetUserInformation()); //*******
    size_t nSeco = secondaries->size();//***********
     if(nSeco>0)
    {
      for(size_t i=0;i<nSeco;i++)
      { 
        T01TrackInformation* infoNew = new T01TrackInformation(info);
        (*secondaries)[i]->SetUserInformation(infoNew);
	}
    }
  }
  }

//********************************************************************
// G4VTrajectory* Trajectory = TrackingManager->GimmeTrajectory();
// if(Trajectory)
// {
//   T01TrackInformation* info = (T01TrackInformation*)(aTrack->GetUserInformation()); //*******
     /*  size_t nSeco = Trajectory->size();
     if(nSeco>0)
    {
      for(size_t i=0;i<nSeco;i++)
      { 
        T01TrackInformation* infoNew = new T01TrackInformation(info);
        (*Trajectory)[i]->SetUserInformation(infoNew);
	}
    }
    }*/
//}
