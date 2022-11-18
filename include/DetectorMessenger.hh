
#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*
This class provides the user interface to DetectorConstruction

It allows for
 - change of detector position
 - rotation of the tracker detectors around the z-axis
 - rotation of the phantom
 - changing detector geometry
 - changing materials
*/

class DetectorMessenger: public G4UImessenger
{
public:
  // Constructor
  DetectorMessenger(DetectorConstruction* );
  // Destructor
  ~DetectorMessenger();
    
  // handle user commands
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  
  DetectorConstruction*      detector;
  G4UIcmdWithoutParameter*   updateCmd;
    
  G4UIdirectory*             detDir;
    
  G4UIdirectory*             phantomDir;
  G4UIdirectory*             shieldDir; //*************************

  G4UIdirectory*             filmDir;  //*****************************************************************************************************************************************
   G4UIdirectory*             contact1Dir;  //*****************************************************************************************************************************************
   G4UIdirectory*             contact2Dir;  //*****************************************************************************************************************************************


  G4UIdirectory*             geomDir;

  G4UIdirectory*             x1_SensorDir;
  G4UIdirectory*             u1_SensorDir;
  G4UIdirectory*             v1_SensorDir;

  G4UIdirectory*             matDir;
    
    
    G4UIcmdWithABool*			build_strip_detCmd;
    
  G4UIcmdWithAnInteger* set_nb_of_stripsCmd;
  G4UIcmdWithADoubleAndUnit* set_strip_pitchCmd;
  G4UIcmdWithADoubleAndUnit* set_strip_lengthCmd;
  G4UIcmdWithADoubleAndUnit* set_det_thicknessCmd;
    
  G4UIcmdWithADoubleAndUnit* set_inter_plane_distCmd;
  G4UIcmdWithADoubleAndUnit* set_inter_module_distCmd;
  G4UIcmdWithADoubleAndUnit* set_phantom_gapCmd;
  G4UIcmdWithADoubleAndUnit* set_halfPhantomSizeZCmd;

  G4UIcmdWithADoubleAndUnit* set_halfShieldSizeZCmd;//******************************


  G4UIcmdWithADoubleAndUnit* set_film_gapCmd;          //************************************************************************************************************************
  G4UIcmdWithADoubleAndUnit* set_halfFilmSizeZCmd;

   G4UIcmdWithADoubleAndUnit* set_contact1_gapCmd;          //************************************************************************************************************************
  G4UIcmdWithADoubleAndUnit* set_halfContact1SizeZCmd;

 G4UIcmdWithADoubleAndUnit* set_contact2_gapCmd;          //************************************************************************************************************************
  G4UIcmdWithADoubleAndUnit* set_halfContact2SizeZCmd;
    
  G4UIcmdWithADoubleAndUnit* set_zShift_pixel_trackerCmd;
    
  G4UIcmdWithADoubleAndUnit* xShift_PhantomCmd;
  G4UIcmdWithADoubleAndUnit* yShift_PhantomCmd;
  G4UIcmdWithADoubleAndUnit* zShift_PhantomCmd;
  G4UIcmdWithADoubleAndUnit* theta_PhantomCmd;


  //**************************************
    
  G4UIcmdWithADoubleAndUnit* xShift_ShieldCmd;
  G4UIcmdWithADoubleAndUnit* yShift_ShieldCmd;
  G4UIcmdWithADoubleAndUnit* zShift_ShieldCmd;
  G4UIcmdWithADoubleAndUnit* theta_ShieldCmd;
  //*************************************
  //*************************************************************************************************************************************
   
  G4UIcmdWithADoubleAndUnit* xShift_FilmCmd;
  G4UIcmdWithADoubleAndUnit* yShift_FilmCmd;
  G4UIcmdWithADoubleAndUnit* zShift_FilmCmd;
  G4UIcmdWithADoubleAndUnit* theta_FilmCmd;
  //*******************************************
   
    G4UIcmdWithADoubleAndUnit* xShift_Contact1Cmd;
  G4UIcmdWithADoubleAndUnit* yShift_Contact1Cmd;
  G4UIcmdWithADoubleAndUnit* zShift_Contact1Cmd;
  G4UIcmdWithADoubleAndUnit* theta_Contact1Cmd;
  //*******************************************
   
    G4UIcmdWithADoubleAndUnit* xShift_Contact2Cmd;
  G4UIcmdWithADoubleAndUnit* yShift_Contact2Cmd;
  G4UIcmdWithADoubleAndUnit* zShift_Contact2Cmd;
  G4UIcmdWithADoubleAndUnit* theta_Contact2Cmd;
  //************************************************************************************************************************************

  G4UIcmdWithADoubleAndUnit* x_x1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* y_x1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* stereo_x1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* xShift_x1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* yShift_x1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* stereoShift_x1_SensorCmd;
	
  G4UIcmdWithADoubleAndUnit* x_u1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* y_u1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* stereo_u1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* xShift_u1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* yShift_u1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* stereoShift_u1_SensorCmd;
	
  G4UIcmdWithADoubleAndUnit* x_v1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* y_v1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* stereo_v1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* xShift_v1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* yShift_v1_SensorCmd;
  G4UIcmdWithADoubleAndUnit* stereoShift_v1_SensorCmd;

  G4UIcmdWithABool*			 set_x1_SensorDUTsetupCmd;
  G4UIcmdWithABool*			 set_u1_SensorDUTsetupCmd;
  G4UIcmdWithABool*			 set_v1_SensorDUTsetupCmd;
    
  G4UIcmdWithAString*        set_world_materialCmd;
  G4UIcmdWithAString*        set_detector_materialCmd;
  G4UIcmdWithAString*        set_phantom_materialCmd;
  G4UIcmdWithAString*        set_shield_materialCmd; //********************

  G4UIcmdWithAString*        set_film_materialCmd; //**********************************************************************************
  G4UIcmdWithAString*        set_contact1_materialCmd; //**********************************************************************************
  G4UIcmdWithAString*        set_contact2_materialCmd; //**********************************************************************************


    
    
    // Pixel detector
    
    G4UIcmdWithABool*			build_pixel_detCmd;
    
    G4UIdirectory*              pix1_SensorDir;
    G4UIdirectory*              pix2_SensorDir;
    G4UIdirectory*              pix3_SensorDir;
    G4UIdirectory*              pix4_SensorDir;
    
    G4UIcmdWithABool*           set_pix1_SensorDUTsetupCmd;
    G4UIcmdWithABool*			set_pix2_SensorDUTsetupCmd;
    G4UIcmdWithABool*			set_pix3_SensorDUTsetupCmd;
    G4UIcmdWithABool*			set_pix4_SensorDUTsetupCmd;
   
    
};
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

