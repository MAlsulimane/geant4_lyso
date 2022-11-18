
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QGSP_BIC_HP.hh"       
#include "Shielding.hh" 
#include "G4Version.hh"

#include "G4VisExecutive.hh"
#if  G4VERSION_NUMBER>=930
#include "G4UIExecutive.hh"
#else
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#endif

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "PhysicsList.hh"
#include "QGSP_BERT.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include "SteppingAction.hh"

#include "TSystem.h"
#include "TStopwatch.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
    
  TStopwatch timer;
  timer.Start();
  Double_t rt = 0, /*r_et = 0,*/ cpt = 0/*, cp_et = 0*/;
    
  //Comment out for non-random (deterministic) events
  time_t systime = time(NULL);
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  CLHEP::HepRandom::setTheSeed(systime*G4UniformRand());
    
  //Run manager
  G4RunManager * runManager = new G4RunManager();

  //mandatory Initialization classes
  DetectorConstruction* detector = new DetectorConstruction();
  runManager->SetUserInitialization(detector);

  //G4VUserPhysicsList* physics = new PhysicsList();
  G4VUserPhysicsList* physics = new QGSP_BIC_HP(); // This is the best physics list for Neutron simulations
    //G4VUserPhysicsList* physics = new Shielding();

  runManager->SetUserInitialization(physics);
   
  //mandatory User Action classes
  //G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
  PrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);

  //Optional User Action classes
  //Event action (handles for beginning / end of event)
  EventAction* event_action = new EventAction(detector);
  //Run action (handles for beginning / end of event)
  //This particular class needs a pointer to the event action
  RunAction* run_action = new RunAction( gen_action, event_action, detector );
    
  SteppingAction* step_action = new SteppingAction(/*detector*/);
  runManager->SetUserAction( step_action );
  runManager->SetUserAction( event_action );
  runManager->SetUserAction( run_action );

  // Initialize G4 kernel
  runManager->Initialize();
      
  //Initilize the visualization manager
  G4VisManager* visManager = new G4VisExecutive();
  visManager->Initialize();
     
  // Get the pointer to the User Interface manager
  //
  G4UImanager * UImanager = G4UImanager::GetUIpointer();  

  if (argc!=1)
  {  // batch mode
	  //command line contains name of the macro to execute
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
  }
  else  // interactive mode : define UI session
  {
	  G4UIExecutive * ui = new G4UIExecutive(argc,argv);
	  //If UI has graphics execute special macro: opens OpenGL Qt driver
	  if (ui->IsGUI())
		  UImanager->ApplyCommand("/control/execute main.mac");
	  else
		  UImanager->ApplyCommand("/control/execute main.mac");
	  ui->SessionStart();
	  delete ui;
  }

  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

  delete runManager;
    
    timer.Stop();
	rt = timer.RealTime();
	cpt = timer.CpuTime();
	//r_et = E_nb/timer.RealTime();
	//cp_et = E_nb/timer.CpuTime();
	printf("\nTotal time taken for simulation + writing: RT = %6.2fs, Cpu = %6.2fs\n\n",rt,cpt);

  return 0;
}
