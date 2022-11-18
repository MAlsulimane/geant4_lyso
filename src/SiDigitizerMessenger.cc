
#include "SiDigitizerMessenger.hh"
#include "SiDigitizer.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

SiDigitizerMessenger::SiDigitizerMessenger(SiDigitizer* digitizer) :
	digi(digitizer)
{
	digiDir = new G4UIdirectory("/det/digi/");
	digiDir->SetGuidance("commands related to the digitization process");

	pedestalCmd = new G4UIcmdWithADouble("/det/digi/pedestal",this);
	pedestalCmd->SetGuidance("Set pedestal value (in elementary charge units)");
	pedestalCmd->SetDefaultValue(5000);
	pedestalCmd->AvailableForStates(G4State_Idle);

	noiseCmd = new G4UIcmdWithADouble("/det/digi/noise",this);
	noiseCmd->SetGuidance("Define standard deviation of strip gaussian electronic noise (in elementary charge units).");
	noiseCmd->SetDefaultValue(1000);
	noiseCmd->AvailableForStates(G4State_Idle);
/*
	crosstalkCmd = new G4UIcmdWithADouble("/det/digi/crosstalk",this);
	crosstalkCmd->SetGuidance("Define the cross talk fraction between strips");
	crosstalkCmd->SetDefaultValue(0.05);
	crosstalkCmd->AvailableForStates(G4State_Idle);
*/
    diffusionCmd = new G4UIcmdWithADouble("/det/digi/bias",this);
    diffusionCmd->SetGuidance("Define the bias voltage which governs the electric filed and charge diffusion between strips");
    diffusionCmd->SetDefaultValue(50);  // volts not available as unit
    diffusionCmd->AvailableForStates(G4State_Idle);
    
	conversionCmd = new G4UIcmdWithADoubleAndUnit("/det/digi/conversionFactor",this);
	conversionCmd->SetGuidance("Define the conversion Energy/charge conversion factor.");
	conversionCmd->SetGuidance("For example a factor of 3.6*eV means that 1 electron is created every 3.6 eV of deposited energy.");
	conversionCmd->SetDefaultValue(3.6*eV);//TODO: Do I have to specify here units?
	conversionCmd->SetDefaultUnit("eV");
	conversionCmd->SetUnitCategory("Energy");
	conversionCmd->AvailableForStates(G4State_Idle);

}


SiDigitizerMessenger::~SiDigitizerMessenger()
{
	delete pedestalCmd;
	delete noiseCmd;
	//delete crosstalkCmd;
    delete diffusionCmd;
	delete conversionCmd;
	delete digiDir;
}

void SiDigitizerMessenger::SetNewValue(G4UIcommand* cmd,G4String newValue)
{
	if ( cmd == pedestalCmd )
		digi->SetPedestal( pedestalCmd->GetNewDoubleValue(newValue) );

	if ( cmd == noiseCmd )
		digi->SetNoise( noiseCmd->GetNewDoubleValue(newValue) );

	//if ( cmd == crosstalkCmd )
	//	digi->SetCrosstalk( crosstalkCmd->GetNewDoubleValue(newValue) );
    
    if ( cmd == diffusionCmd )
    	digi->SetDiffusion( diffusionCmd->GetNewDoubleValue(newValue) );

	if ( cmd == conversionCmd ) {
		//note that digitizer requires Q/MeV
		G4double value = 1./conversionCmd->GetNewDoubleValue( newValue );
		digi->SetConversionFactor( value );
	}
}
