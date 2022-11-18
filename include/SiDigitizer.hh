
#ifndef SIDIGITIZER_HH_
#define SIDIGITIZER_HH_


#include "G4VDigitizerModule.hh"
#include "SiDigi.hh"
#include "NoiseGenerator.hh"
#include "MeV2ChargeConverter.hh"
#include "CrosstalkGenerator.hh"
#include "SiDigitizerMessenger.hh"

#include "DiffusionGenerator.hh"
#include "TrackerGeometry.hh"

/*
 Simulation of the digitization process
 This class is responsible for the simulation of the ditis i.e.
 it is responsible for simulating the read-out of the detector.
 Geant4 provides hits in the sensitive detector (i.e. energy depositions 
 and positions in elements of the detector that have been declared sensitive),
 this class is responsible to convert these hits in the electronic read-out
 format SiDigi.
 
 Digitization consists of the following steps:
    -# converting the energy deposit into charge
    -# simulating the strip cross talk
    -# for each strip add a pedestal
    -# smear the collected charge with electronic noise
    -# add cross talk
    -# add charge sharing (diffusion)
 
 All relevant methods are virtual, you can inherit from this base 
 class to overwrite behaviour. These classes uses two support classes
 to simulate noise and convert energy deposit (in MeV) to the appropriate
 units using MeV2ChargeConverter and NoiseGenerator.
 */

class SiDigitizer : public G4VDigitizerModule
{
public:
    
   /* constructor
   Creates a digitization module
   param aName The name of the module
   */
    
  SiDigitizer(G4String aName, G4int strips, G4int planes);

  // Empty destructor
  virtual ~SiDigitizer() {};

   /* Perform digitization
      This method is declared pure virtual in the base class
      and thus must be implemented */
    
  virtual void Digitize();
protected:
  // simulate electronics
  //
  /* Simulate crosstalk between strips
   *
   * The charge collected by one strip "leaks" to the adjacent strips
   * Thus the charge collected by the strip that has been "hit"
   * is reduced and part of this goes to the adjacent strips
   * digitsMap : the digits collection digitsMap[ planeNumber ][ stripNumber ]
   * Important: crosstalk should be simulated before noise and pedestal
   * is added. Digitize
   */
  //virtual void MakeCrosstalk(std::vector< std::vector< SiDigi* > >& digitsMap);
   virtual void MakeDiffusion(std::vector< std::vector< SiDigi* > >& digitsMap );

public:
  // some simple set & get functions
  //
  //TODO: Add setters for the other noise parameters?
  //TODO: make a messanger to set parameters?
  inline void     SetPedestal( const G4double& aValue )         { pedestal = aValue; }
  inline void	  SetNoise( const G4double& aValue )            { noise = NoiseGenerator(aValue); }
  //inline void	  SetCrosstalk( const G4double& aValue )        { crosstalk = CrosstalkGenerator(aValue,1024); }
  inline void	  SetDiffusion( const G4double& aValue )        { diffusion = DiffusionGenerator(aValue); }
    
  inline void	  SetConversionFactor( const G4double& aValue ) { convert = MeV2ChargeConverter(aValue); }
  inline void     SetCollectionName( const G4String& aName )    { digiCollectionName = aName; }
    
  inline void     SetHitsCollectionName_x1( const G4String& aName ){ hitsCollName_x1 = aName; }
  inline void     SetHitsCollectionName_u1( const G4String& aName ){ hitsCollName_u1 = aName; }
  inline void     SetHitsCollectionName_v1( const G4String& aName ){ hitsCollName_v1 = aName; }
    
  inline void     SetHitsCollectionName_x2( const G4String& aName ){ hitsCollName_x2 = aName; }
  inline void     SetHitsCollectionName_u2( const G4String& aName ){ hitsCollName_u2 = aName; }
  inline void     SetHitsCollectionName_v2( const G4String& aName ){ hitsCollName_v2 = aName; }
    
  inline void     SetHitsCollectionName_TP1( const G4String& aName ){ hitsCollName_TP1 = aName; }
  inline void     SetHitsCollectionName_TP2( const G4String& aName ){ hitsCollName_TP2 = aName; }
    
  inline void     SetHitsCollectionName_TP3( const G4String& aName ){ hitsCollName_TP3 = aName; }
  inline void     SetHitsCollectionName_TP4( const G4String& aName ){ hitsCollName_TP4 = aName; }
    
  inline void     SetHitsCollectionName_x3( const G4String& aName ){ hitsCollName_x3 = aName; }
  inline void     SetHitsCollectionName_u3( const G4String& aName ){ hitsCollName_u3 = aName; }
  inline void     SetHitsCollectionName_v3( const G4String& aName ){ hitsCollName_v3 = aName; }

  inline void     SetHitsCollectionName_x4( const G4String& aName ){ hitsCollName_x4 = aName; }
  inline void     SetHitsCollectionName_u4( const G4String& aName ){ hitsCollName_u4 = aName; }
  inline void     SetHitsCollectionName_v4( const G4String& aName ){ hitsCollName_v4 = aName; }
    
  inline void     SetHitsCollectionName_TP5( const G4String& aName ){ hitsCollName_TP5 = aName; }
    
  //Used for re-setting the no. of planes and strips in case they have been changed from the default
  //vaules set in DetectorConstruction.cc by the .mac file. Necessary because DetectorConsturcion object with
  //default parameters is passed to EventAction before the .mac file is processed. EventAction then uses the
  //DetectorConstruction object to construct root file with array elements = to the defined no. of strips.
  //This then creates an error if the .mac file alters these parameters and recompiles the DetectorConstruction object.
  inline void     ReSetDigiCollectionStrips( const G4int& aValue ){ digiCollectionStrips = aValue; }
  inline void     ReSetDigiCollectionPlanes( const G4int& aValue ){ digiCollectionPlanes = aValue; }

private:
    
  //Name of the digits collection
  G4String digiCollectionName;

  //Name of the hits collection. Should match what has been used in SensitiveDetector
  G4String hitsCollName_x1;
  G4String hitsCollName_u1;
  G4String hitsCollName_v1;
    
  G4String hitsCollName_x2;
  G4String hitsCollName_u2;
  G4String hitsCollName_v2;
 
  G4String hitsCollName_TP1;
  G4String hitsCollName_TP2;
    
  G4String hitsCollName_TP3;
  G4String hitsCollName_TP4;
    
  G4String hitsCollName_x3;
  G4String hitsCollName_u3;
  G4String hitsCollName_v3;
    
  G4String hitsCollName_x4;
  G4String hitsCollName_u4;
  G4String hitsCollName_v4;
    
  G4String hitsCollName_TP5;

  G4int digiCollectionStrips;
  G4int digiCollectionPlanes;    
    
  //Pedestal level
  G4double pedestal;
    
  //The object responsible to generate the electronic noise
  NoiseGenerator noise;
    
  //The object that converts the energy deposit in collected charge
  MeV2ChargeConverter convert;
    
  //The object that handles cross talk
  //CrosstalkGenerator crosstalk;
    
  //The object that handles the charge diffusion
  //And is used by the MakeDiffusion() function.
  DiffusionGenerator diffusion;
  //The object that handles the tracker geometry used in charge diffusion
  //And is used by the MakeDiffusion() function.
  TrackerGeometry tracker;

  //Messenger to implement some UI commands
  //SiDigitizerMessenger messenger;
    
};

#endif /* SIDIGITIZER_HH_ */
