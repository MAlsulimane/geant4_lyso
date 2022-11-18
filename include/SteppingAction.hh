
#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class G4VUserDetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(/*G4VUserDetectorConstruction* */);
   ~SteppingAction(){};

  //  void UserSteppingAction( const G4Step* );
	
private:
    //G4VUserDetectorConstruction* myDetector;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
