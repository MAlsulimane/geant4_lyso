
#ifndef RUNACTION_HH_
#define RUNACTION_HH_

#include "G4UserRunAction.hh"
#include "RootSaver.hh"

class G4Run;
class EventAction;
class DetectorConstruction;
class PrimaryGeneratorAction;
//class G4VUserPrimaryGeneratorAction;

/*
 * User's RunAction class
 * This class implements the Run Action
 * The relevant method is BeginOfRunAction and EndOfRunAction
 * This class controls the saving facility (RootSaver class), since
 * the handling of ROOT TTree is done at run level (each run one TTree)
 */
class RunAction : public G4UserRunAction
{
public:
	// constructor
	RunAction( /*G4VUser*/PrimaryGeneratorAction * pgAct, EventAction* evAct, DetectorConstruction* myDet );
	// destructor
	virtual ~RunAction() {};
	// Called at the beginning of each run
	void BeginOfRunAction(const G4Run*);
	// Called at the end of each run
	void EndOfRunAction(const G4Run*);
private:
	// The ROOT TTree handler object
	RootSaver saver;
    // Pointer to the PrimaryGeneratorAction
    /*G4VUser*/PrimaryGeneratorAction * primGenAction;
	// Pointer to the EventAction
	EventAction* eventAction;
    // Pointer to the DetectorConstruction
    DetectorConstruction* myDetector;
};

#endif /* RUNACTION_HH_ */
