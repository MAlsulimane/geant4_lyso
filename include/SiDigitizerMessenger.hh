
#ifndef DIGITIZERMESSENGER_HH_
#define DIGITIZERMESSENGER_HH_

#include "globals.hh"
#include "G4UImessenger.hh"

class SiDigitizer;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;

class SiDigitizerMessenger : public G4UImessenger
{
public:
	// Constructor
	SiDigitizerMessenger(SiDigitizer*);
	// Destructor
	virtual ~SiDigitizerMessenger();
	// handle user commands
	void SetNewValue(G4UIcommand*,G4String);
private:
	SiDigitizer*				digi;

	G4UIdirectory*				digiDir;
	G4UIcmdWithADouble*			pedestalCmd;
	G4UIcmdWithADouble*			noiseCmd;
	//G4UIcmdWithADouble*		  crosstalkCmd;
    G4UIcmdWithADouble*			diffusionCmd;
	G4UIcmdWithADoubleAndUnit*	conversionCmd;
};

#endif /* DIGITIZERMESSENGER_HH_ */
