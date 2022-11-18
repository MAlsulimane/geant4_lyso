
#ifndef EVENTACTION_HH_
#define EVENTACTION_HH_


#include "G4UserEventAction.hh"
#include "G4String.hh"
#include "NoiseGenerator.hh"
#include "CrosstalkGenerator.hh"
#include "MeV2ChargeConverter.hh"

class G4Event;
class RootSaver;
class DetectorConstruction;

/*
 * User's EventAction class
 * This class implements the Event Action.
 * The relevant method is EndOfEventAction where the
 * digitization (SiDigitizer class) and the storage of hits and digits
 * in a ROOT TTree are perfomed (RootSaver class).
 */

class EventAction : public G4UserEventAction
{
public:
	//Default constructor
	EventAction(DetectorConstruction* myDet);
	//Default destructor
	virtual ~EventAction() {};
	//Beginning of event
	void BeginOfEventAction(const G4Event* anEvent);
	//Digitize hits and store information
	void EndOfEventAction(const G4Event* anEvent);
	//Set the RootSaver
	inline void SetRootSaver( RootSaver* saver ) { rootSaver = saver; }
private:
	//pointer to saver object
	RootSaver* rootSaver;
    
	//hits collection name
	G4String hitsCollName_x1;
	G4String hitsCollName_u1;
	G4String hitsCollName_v1;
    
    //hits collection name
    G4String hitsCollName_pix1;
    G4String hitsCollName_pix2;
    G4String hitsCollName_pix3;
    G4String hitsCollName_pix4;
    
	//digits collection name
	G4String digitsCollName;
    G4String digitsCollName_pix;
    DetectorConstruction* myDetector;
};

#endif /* EVENTACTION_HH_ */
