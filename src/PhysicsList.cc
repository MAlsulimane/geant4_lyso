
#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronHElasticPhysics.hh"
#include "G4HadronDElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
//#include "G4HadronElasticPhysicsLHEP.hh"
//#include "G4HadronQElasticPhysics.hh"
#include "G4ChargeExchangePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4NeutronCrossSectionXS.hh"
#include "G4StoppingPhysics.hh"
#include "G4StoppingPhysics.hh"
//#include "G4LHEPStoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
//#include "G4IonLHEPPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4EmExtraPhysics.hh"
//#include "G4EmProcessOptions.hh"
#include "G4HadronInelasticQBBC.hh"
//#include "HadronPhysicsQGSP_BIC.hh"

#include "G4LossTableManager.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"

#include "G4SystemOfUnits.hh"

#include "G4IonFluctuations.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4UniversalFluctuation.hh"

#include "G4BraggIonGasModel.hh"
#include "G4BetheBlochIonGasModel.hh"

#include "G4PhysicalConstants.hh"

//#include "G4ProtonInelasticProcess.hh"
#include "G4CascadeInterface.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4RegionStore.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  G4LossTableManager::Instance();
  defaultCutValue = 10.*um;
  //fCutForGamma     = defaultCutValue;
  //fCutForElectron  = defaultCutValue;
  //fCutForPositron  = defaultCutValue;
  SetVerboseLevel(1);
    
  fHelIsRegisted  = false;
  fBicIsRegisted  = false;
  fBiciIsRegisted = false;

  fStepMaxProcess  = 0;

  //fMessenger = new PhysicsListMessenger(this);

  // EM physics
  fEmName = G4String("emstandard_opt4");
  fEmPhysicsList = new G4EmStandardPhysics_option4(verboseLevel);
    
  //fEmName = G4String("emlivermore");
  //fEmPhysicsList = new G4EmLivermorePhysics(verboseLevel);

  // Decay physics and all particles
  fDecPhysicsList = new G4DecayPhysics(verboseLevel);
    
  //Electro-nuclear reactions
  fHadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  //Elastic Scattering
  fHadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel));
  //Inelastic Scattering 
  fHadronPhys.push_back( new G4HadronInelasticQBBC(verboseLevel));  //Bertini
  //fHadronPhys.push_back(   new HadronPhysicsQGSP_BIC(verboseLevel));  //LowE
    
  //Neutrons & Ions
  fHadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
  fHadronPhys.push_back( new G4IonBinaryCascadePhysics(verboseLevel));
    
  fHadronPhys.push_back( new G4StoppingPhysics(verboseLevel));
    
    //G4CascadeInterface* bertiniModel = new G4CascadeInterface();
    //G4ProtonInelasticProcess* inelProcess = new G4ProtonInelasticProcess();
    //inelProcess->RegisterMe(bertiniModel);
    //G4ProcessManager* processManager = new G4ProcessManager();
    //processManager->AddDiscreteProcess(inelProcess);
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{
  //delete fMessenger;
  delete fEmPhysicsList;
  delete fDecPhysicsList;
  for(size_t i=0; i<fHadronPhys.size(); i++) {delete fHadronPhys[i];}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  fDecPhysicsList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  // transportation
  AddTransportation();
  
  // electromagnetic physics list
  fEmPhysicsList->ConstructProcess();

  // decay physics list
  fDecPhysicsList->ConstructProcess();
  
  // hadronic physics lists
  for(size_t i=0; i<fHadronPhys.size(); i++) {
    fHadronPhys[i]->ConstructProcess();
  }
  
  // step limitation (as a full process)
  //AddStepMax();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::AddPhysicsList(const G4String& name)
{
  if (verboseLevel>1) {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
  }

  if (name == fEmName) return;

  if /*(name == "local") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new PhysListEmStandard(name);

  } else if*/ (name == "emstandard_opt0") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics(1);

  } else if (name == "emstandard_opt1") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option1();

  } else if (name == "emstandard_opt2") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option2();
    
  } else if (name == "emstandard_opt3") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option3();    
    
  } else if (name == "emstandard_opt4") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option4();    

  } /*else if (name == "standardSS") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new PhysListEmStandardSS(name);

  }*/ else if (name == "ionGasModels") {

    AddPhysicsList("emstandard_opt0");
    fEmName = name;
    //AddIonGasModels();

  }/* else if (name == "standardNR") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new PhysListEmStandardNR(name);

      
  }*/ else if (name == "emlivermore") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmLivermorePhysics();

  } else if (name == "empenelope") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmPenelopePhysics();

  } else if (name == "elastic" && !fHelIsRegisted) {
    fHadronPhys.push_back( new G4HadronElasticPhysics());
    fHelIsRegisted = true;

  } else if (name == "DElastic" && !fHelIsRegisted) {
    fHadronPhys.push_back( new G4HadronDElasticPhysics());
    fHelIsRegisted = true;

  } else if (name == "HElastic" && !fHelIsRegisted) {
    fHadronPhys.push_back( new G4HadronHElasticPhysics());
    fHelIsRegisted = true;

  } else if (name == "binary" && !fBicIsRegisted) {
    fHadronPhys.push_back(new G4HadronInelasticQBBC());
    fBicIsRegisted = true;

  } else if (name == "binary_ion" && !fBiciIsRegisted) {
    fHadronPhys.push_back(new G4IonPhysics());
    fBiciIsRegisted = true;

  } else {

    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
           << " is not defined"
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void PhysicsList::AddStepMax()
{
  // Step limitation seen as a process
  fStepMaxProcess = new StepMax();

  theParticleIterator->reset();
  while ((*theParticleIterator)()){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (fStepMaxProcess->IsApplicable(*particle) && pmanager)
      {
        pmanager ->AddDiscreteProcess(fStepMaxProcess);
      }
  }
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
    
    // default production thresholds for the world volume
    SetCutsWithDefault();
    
    if (verboseLevel>0) DumpCutValuesTable();
}

/*
void PhysicsList::SetCuts()
{
  if (verboseLevel >0) {
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(fCutForGamma, "gamma");
  SetCutValue(fCutForElectron, "e-");
  SetCutValue(fCutForPositron, "e+");

  if (verboseLevel>0) DumpCutValuesTable();
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void PhysicsList::SetCutForGamma(G4double cut)
{
  fCutForGamma = cut;
  SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void PhysicsList::SetCutForElectron(G4double cut)
{
  fCutForElectron = cut;
  SetParticleCuts(fCutForElectron, G4Electron::Electron());
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void PhysicsList::SetCutForPositron(G4double cut)
{
  fCutForPositron = cut;
  SetParticleCuts(fCutForPositron, G4Positron::Positron());
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void PhysicsList::AddIonGasModels()
{
  G4EmConfigurator* em_config = G4LossTableManager::Instance()->EmConfigurator();
  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4String partname = particle->GetParticleName();
    if(partname == "alpha" || partname == "He3" || partname == "GenericIon") {
      G4BraggIonGasModel* mod1 = new G4BraggIonGasModel();
      G4BetheBlochIonGasModel* mod2 = new G4BetheBlochIonGasModel();
      G4double eth = 2.*MeV*particle->GetPDGMass()/proton_mass_c2;
      em_config->SetExtraEmModel(partname,"ionIoni",mod1,"",0.0,eth,
                                 new G4IonFluctuations());
      em_config->SetExtraEmModel(partname,"ionIoni",mod2,"",eth,100*TeV,
                                 new G4UniversalFluctuation());

    }
  }
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

