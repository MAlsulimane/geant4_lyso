
#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction * det):detector(det)
{
  detDir = new G4UIdirectory("/det/");
  detDir->SetGuidance("detector construction commands");

  //**************** GLOBAL DET. PARAMETERS **********************************//
    
  // Turn on/off build of strip detectors
  build_strip_detCmd = new G4UIcmdWithABool("/det/strip_build",this);
  build_strip_detCmd->SetGuidance("Select setup true to have strip detectors built");
  build_strip_detCmd->AvailableForStates(G4State_Idle);
  
  // Turn on/off build of pixel detectors
  build_pixel_detCmd = new G4UIcmdWithABool("/det/pixel_build",this);
  build_pixel_detCmd->SetGuidance("Select setup true to have pixel detectors built");
  build_pixel_detCmd->AvailableForStates(G4State_Idle);
  
  set_nb_of_stripsCmd = new G4UIcmdWithAnInteger("/det/n_strips",this);
  set_nb_of_stripsCmd->SetGuidance("Select no. of strips in each detector plane");
  set_nb_of_stripsCmd->SetParameterName("n_strips",true);
  set_nb_of_stripsCmd->SetDefaultValue(1024.);
  set_nb_of_stripsCmd->AvailableForStates(G4State_Idle);
  
  set_strip_pitchCmd = new G4UIcmdWithADoubleAndUnit("/det/strip_pitch",this);
  set_strip_pitchCmd->SetGuidance("Select pitch of strips in each detector plane");
  set_strip_pitchCmd->SetParameterName("strip_pitch",true);
  set_strip_pitchCmd->SetDefaultValue(0.0908);
  set_strip_pitchCmd->SetUnitCategory("Length");
  set_strip_pitchCmd->SetDefaultUnit("mm");
  set_strip_pitchCmd->AvailableForStates(G4State_Idle);
  
  set_strip_lengthCmd = new G4UIcmdWithADoubleAndUnit("/det/strip_length",this);
  set_strip_lengthCmd->SetGuidance("Select length of strips in each detector plane");
  set_strip_lengthCmd->SetParameterName("strip_length",true);
  set_strip_lengthCmd->SetDefaultValue(96.0);
  set_strip_lengthCmd->SetUnitCategory("Length");
  set_strip_lengthCmd->SetDefaultUnit("mm");
  set_strip_lengthCmd->AvailableForStates(G4State_Idle);
  
  set_det_thicknessCmd = new G4UIcmdWithADoubleAndUnit("/det/thickness",this);
  set_det_thicknessCmd->SetGuidance("Select thickness of strips in each detector plane");
  set_det_thicknessCmd->SetParameterName("thickness",true);
  set_det_thicknessCmd->SetDefaultValue(0.15);
  set_det_thicknessCmd->SetUnitCategory("Length");
  set_det_thicknessCmd->SetDefaultUnit("mm");
  set_det_thicknessCmd->AvailableForStates(G4State_Idle);
  
  //**************** FIRST TRACKER MODULE **********************************//
    
  //**************** Commands for x1 sensor **********************************//
	
  x1_SensorDir = new G4UIdirectory("/det/x1_Sensor/");
  x1_SensorDir->SetGuidance("commands related to the x1 sensor plane");
    
  // set sensor as DUT to enable strip geometry and readout
  set_x1_SensorDUTsetupCmd = new G4UIcmdWithABool("/det/x1_Sensor/DUTsetup",this);
  set_x1_SensorDUTsetupCmd->SetGuidance("Select setup true to have DUT (Device Under Test) setup: x1 Si plane replaced by DUT");
  set_x1_SensorDUTsetupCmd->AvailableForStates(G4State_Idle);

  // positions and stereo angle
  x_x1_SensorCmd = new G4UIcmdWithADoubleAndUnit("/det/x1_Sensor/x_pos",this);
  x_x1_SensorCmd->SetGuidance("Define x position of x1 sensor plane");
  x_x1_SensorCmd->SetParameterName("x_pos",true);
  x_x1_SensorCmd->SetDefaultValue(0.);
  x_x1_SensorCmd->SetUnitCategory("Length");
  x_x1_SensorCmd->SetDefaultUnit("mm");
  
  y_x1_SensorCmd = new G4UIcmdWithADoubleAndUnit("/det/x1_Sensor/y_pos",this);
  y_x1_SensorCmd->SetGuidance("Define y position of x1 sensor plane");
  y_x1_SensorCmd->SetParameterName("y_pos",true);
  y_x1_SensorCmd->SetDefaultValue(0.);
  y_x1_SensorCmd->SetUnitCategory("Length");
  y_x1_SensorCmd->SetDefaultUnit("mm");
  
  stereo_x1_SensorCmd = new G4UIcmdWithADoubleAndUnit ("/det/x1_Sensor/stereo_angle", this);
  stereo_x1_SensorCmd->SetGuidance("Define stereo angle of x1 sensor plane around beam axis");
  stereo_x1_SensorCmd->SetParameterName("stereo_x1",true);
  stereo_x1_SensorCmd->SetUnitCategory("Angle");
  stereo_x1_SensorCmd->SetDefaultUnit("deg");
  
  // lateral positions and stereo angle offsets (used to simulate mechanical misalignment)
  xShift_x1_SensorCmd = new G4UIcmdWithADoubleAndUnit("/det/x1_Sensor/xShift",this);
  xShift_x1_SensorCmd->SetGuidance("Define x position offset of x1 sensor plane");
  xShift_x1_SensorCmd->SetParameterName("xShift",true);
  xShift_x1_SensorCmd->SetDefaultValue(0.);
  xShift_x1_SensorCmd->SetUnitCategory("Length");
  xShift_x1_SensorCmd->SetDefaultUnit("mm");
  
  yShift_x1_SensorCmd = new G4UIcmdWithADoubleAndUnit("/det/x1_Sensor/yShift",this);
  yShift_x1_SensorCmd->SetGuidance("Define y-shift of x1 sensor plane");
  yShift_x1_SensorCmd->SetParameterName("yShift",true);
  yShift_x1_SensorCmd->SetDefaultValue(0.);
  yShift_x1_SensorCmd->SetUnitCategory("Length");
  yShift_x1_SensorCmd->SetDefaultUnit("mm");
  
  stereoShift_x1_SensorCmd = new G4UIcmdWithADoubleAndUnit ("/det/x1_Sensor/stereoShift", this);
  stereoShift_x1_SensorCmd->SetGuidance("Define stereo angle offset of x1 sensor plane around beam axis");
  stereoShift_x1_SensorCmd->SetParameterName("stereoShift_x1",true);
  stereoShift_x1_SensorCmd->SetUnitCategory("Angle");
  stereoShift_x1_SensorCmd->SetDefaultUnit("deg");
  
  //**************** Commands for u1 sensor **********************************//
	
  u1_SensorDir = new G4UIdirectory("/det/u1_Sensor/");
  u1_SensorDir->SetGuidance("commands related to the u1 sensor plane");
  
  // set sensor as DUT to enable strip geometry and readout
  set_u1_SensorDUTsetupCmd = new G4UIcmdWithABool("/det/u1_Sensor/DUTsetup",this);
  set_u1_SensorDUTsetupCmd->SetGuidance("Select setup true to have DUT (Device Under Test) setup: u1 Si plane replaced by DUT");
  set_u1_SensorDUTsetupCmd->AvailableForStates(G4State_Idle);
  
  // positions and stereo angle
  x_u1_SensorCmd = new G4UIcmdWithADoubleAndUnit("/det/u1_Sensor/x_pos",this);
  x_u1_SensorCmd->SetGuidance("Define x position of u1 sensor plane");
  x_u1_SensorCmd->SetParameterName("x_pos",true);
  x_u1_SensorCmd->SetDefaultValue(0.);
  x_u1_SensorCmd->SetUnitCategory("Length");
  x_u1_SensorCmd->SetDefaultUnit("mm");
  
  y_u1_SensorCmd = new G4UIcmdWithADoubleAndUnit("/det/u1_Sensor/y_pos",this);
  y_u1_SensorCmd->SetGuidance("Define y position of u1 sensor plane");
  y_u1_SensorCmd->SetParameterName("y_pos",true);
  y_u1_SensorCmd->SetDefaultValue(0.);
  y_u1_SensorCmd->SetUnitCategory("Length");
  y_u1_SensorCmd->SetDefaultUnit("mm");
  
  stereo_u1_SensorCmd = new G4UIcmdWithADoubleAndUnit ("/det/u1_Sensor/stereo_angle", this);
  stereo_u1_SensorCmd->SetGuidance("Define stereo angle of u1 sensor plane around beam axis");
  stereo_u1_SensorCmd->SetParameterName("stereo_u1",true);
  stereo_u1_SensorCmd->SetUnitCategory("Angle");
  stereo_u1_SensorCmd->SetDefaultUnit("deg");
  
  // lateral positions and stereo angle offsets (used to simulate mechanical misalignment)
  xShift_u1_SensorCmd = new G4UIcmdWithADoubleAndUnit("/det/u1_Sensor/xShift",this);
  xShift_u1_SensorCmd->SetGuidance("Define x position offset of u1 sensor plane");
  xShift_u1_SensorCmd->SetParameterName("xShift",true);
  xShift_u1_SensorCmd->SetDefaultValue(0.);
  xShift_u1_SensorCmd->SetUnitCategory("Length");
  xShift_u1_SensorCmd->SetDefaultUnit("mm");
  
  yShift_u1_SensorCmd = new G4UIcmdWithADoubleAndUnit("/det/u1_Sensor/yShift",this);
  yShift_u1_SensorCmd->SetGuidance("Define y-shift of u1 sensor plane");
  yShift_u1_SensorCmd->SetParameterName("yShift",true);
  yShift_u1_SensorCmd->SetDefaultValue(0.);
  yShift_u1_SensorCmd->SetUnitCategory("Length");
  yShift_u1_SensorCmd->SetDefaultUnit("mm");
  
  stereoShift_u1_SensorCmd = new G4UIcmdWithADoubleAndUnit ("/det/u1_Sensor/stereoShift", this);
  stereoShift_u1_SensorCmd->SetGuidance("Define stereo angle offset of u1 sensor plane around beam axis");
  stereoShift_u1_SensorCmd->SetParameterName("stereoShift_u1",true);
  stereoShift_u1_SensorCmd->SetUnitCategory("Angle");
  stereoShift_u1_SensorCmd->SetDefaultUnit("deg");
  
  //**************** Commands for v1 sensor **********************************//
  
  v1_SensorDir = new G4UIdirectory("/det/v1_Sensor/");
  v1_SensorDir->SetGuidance("commands related to the v1 sensor plane");
	
  // set sensor as DUT to enable strip geometry and readout
  set_v1_SensorDUTsetupCmd = new G4UIcmdWithABool("/det/v1_Sensor/DUTsetup",this);
  set_v1_SensorDUTsetupCmd->SetGuidance("Select setup true to have DUT (Device Under Test) setup: v1 Si plane replaced by DUT");
  set_v1_SensorDUTsetupCmd->AvailableForStates(G4State_Idle);
  
  // positions and stereo angle
  x_v1_SensorCmd = new G4UIcmdWithADoubleAndUnit("/det/v1_Sensor/x_pos",this);
  x_v1_SensorCmd->SetGuidance("Define x position of v1 sensor plane");
  x_v1_SensorCmd->SetParameterName("x_pos",true);
  x_v1_SensorCmd->SetDefaultValue(0.);
  x_v1_SensorCmd->SetUnitCategory("Length");
  x_v1_SensorCmd->SetDefaultUnit("mm");
  
  y_v1_SensorCmd = new G4UIcmdWithADoubleAndUnit("/det/v1_Sensor/y_pos",this);
  y_v1_SensorCmd->SetGuidance("Define y position of v1 sensor plane");
  y_v1_SensorCmd->SetParameterName("y_pos",true);
  y_v1_SensorCmd->SetDefaultValue(0.);
  y_v1_SensorCmd->SetUnitCategory("Length");
  y_v1_SensorCmd->SetDefaultUnit("mm");
  
  stereo_v1_SensorCmd = new G4UIcmdWithADoubleAndUnit ("/det/v1_Sensor/stereo_angle", this);
  stereo_v1_SensorCmd->SetGuidance("Define stereo angle of v1 sensor plane around beam axis");
  stereo_v1_SensorCmd->SetParameterName("stereo_v1",true);
  stereo_v1_SensorCmd->SetUnitCategory("Angle");
  stereo_v1_SensorCmd->SetDefaultUnit("deg");
  
  // lateral positions and stereo angle offsets (used to simulate mechanical misalignment)
  xShift_v1_SensorCmd = new G4UIcmdWithADoubleAndUnit("/det/v1_Sensor/xShift",this);
  xShift_v1_SensorCmd->SetGuidance("Define x position offset of v1 sensor plane");
  xShift_v1_SensorCmd->SetParameterName("xShift",true);
  xShift_v1_SensorCmd->SetDefaultValue(0.);
  xShift_v1_SensorCmd->SetUnitCategory("Length");
  xShift_v1_SensorCmd->SetDefaultUnit("mm");
  
  yShift_v1_SensorCmd = new G4UIcmdWithADoubleAndUnit("/det/v1_Sensor/yShift",this);
  yShift_v1_SensorCmd->SetGuidance("Define y-shift of v1 sensor plane");
  yShift_v1_SensorCmd->SetParameterName("yShift",true);
  yShift_v1_SensorCmd->SetDefaultValue(0.);
  yShift_v1_SensorCmd->SetUnitCategory("Length");
  yShift_v1_SensorCmd->SetDefaultUnit("mm");
    
  stereoShift_v1_SensorCmd = new G4UIcmdWithADoubleAndUnit ("/det/v1_Sensor/stereoShift", this);
  stereoShift_v1_SensorCmd->SetGuidance("Define stereo angle offset of v1 sensor plane around beam axis");
  stereoShift_v1_SensorCmd->SetParameterName("stereoShift_v1",true);
  stereoShift_v1_SensorCmd->SetUnitCategory("Angle");
  stereoShift_v1_SensorCmd->SetDefaultUnit("deg");
    
    
  //**************** Commands for pix1 sensor **********************************//
    
  pix1_SensorDir = new G4UIdirectory("/det/pix1_Sensor/");
  pix1_SensorDir->SetGuidance("commands related to the pix1 sensor plane");
    
  // set sensor as DUT to enable strip geometry and readout
  set_pix1_SensorDUTsetupCmd = new G4UIcmdWithABool("/det/pix1_Sensor/DUTsetup",this);
  set_pix1_SensorDUTsetupCmd->SetGuidance("Select setup true to have DUT (Device Under Test) setup: pix1 Si plane replaced by DUT");
  set_pix1_SensorDUTsetupCmd->AvailableForStates(G4State_Idle);
  
    
  //**************** Commands for pix2 sensor **********************************//
  
  pix2_SensorDir = new G4UIdirectory("/det/pix2_Sensor/");
  pix2_SensorDir->SetGuidance("commands related to the pix2 sensor plane");
  
  // set sensor as DUT to enable strip geometry and readout
  set_pix2_SensorDUTsetupCmd = new G4UIcmdWithABool("/det/pix2_Sensor/DUTsetup",this);
  set_pix2_SensorDUTsetupCmd->SetGuidance("Select setup true to have DUT (Device Under Test) setup: pix2 Si plane replaced by DUT");
  set_pix2_SensorDUTsetupCmd->AvailableForStates(G4State_Idle);
  
  
  //**************** Commands for pix3 sensor **********************************//
  
  pix3_SensorDir = new G4UIdirectory("/det/pix3_Sensor/");
  pix3_SensorDir->SetGuidance("commands related to the pix3 sensor plane");
  
  // set sensor as DUT to enable strip geometry and readout
  set_pix3_SensorDUTsetupCmd = new G4UIcmdWithABool("/det/pix3_Sensor/DUTsetup",this);
  set_pix3_SensorDUTsetupCmd->SetGuidance("Select setup true to have DUT (Device Under Test) setup: pix3 Si plane replaced by DUT");
  set_pix3_SensorDUTsetupCmd->AvailableForStates(G4State_Idle);
  
  //**************** Commands for pix4 sensor **********************************//
  
  pix4_SensorDir = new G4UIdirectory("/det/pix4_Sensor/");
  pix4_SensorDir->SetGuidance("commands related to the pix4 sensor plane");
  
  // set sensor as DUT to enable strip geometry and readout
  set_pix4_SensorDUTsetupCmd = new G4UIcmdWithABool("/det/pix4_Sensor/DUTsetup",this);
  set_pix4_SensorDUTsetupCmd->SetGuidance("Select setup true to have DUT (Device Under Test) setup: pix4 Si plane replaced by DUT");
  set_pix4_SensorDUTsetupCmd->AvailableForStates(G4State_Idle);
  
  //**************************** PHANTOM POSITIONING AND ROTATION *******************************//
  
  phantomDir = new G4UIdirectory("/phantom/");
  phantomDir->SetGuidance("phantom rotation commands");
  
  xShift_PhantomCmd = new G4UIcmdWithADoubleAndUnit("/phantom/xShift",this);
  xShift_PhantomCmd->SetGuidance("Define x-shift of phantom");
  xShift_PhantomCmd->SetParameterName("xShift",true);
  xShift_PhantomCmd->SetDefaultValue(0.);
  xShift_PhantomCmd->SetUnitCategory("Length");
  xShift_PhantomCmd->SetDefaultUnit("mm");
  
  yShift_PhantomCmd = new G4UIcmdWithADoubleAndUnit("/phantom/yShift",this);
  yShift_PhantomCmd->SetGuidance("Define y-shift of phantom");
  yShift_PhantomCmd->SetParameterName("yShift",true);
  yShift_PhantomCmd->SetDefaultValue(0.);
  yShift_PhantomCmd->SetUnitCategory("Length");
  yShift_PhantomCmd->SetDefaultUnit("mm");
  
  zShift_PhantomCmd = new G4UIcmdWithADoubleAndUnit("/phantom/zShift",this);
  zShift_PhantomCmd->SetGuidance("Define z position of phantom centre (all detector geometry wrt this");
  zShift_PhantomCmd->SetParameterName("zShift",true);
  zShift_PhantomCmd->SetDefaultValue(0.0);
  zShift_PhantomCmd->SetUnitCategory("Length");
  zShift_PhantomCmd->SetDefaultUnit("mm");
  
  theta_PhantomCmd = new G4UIcmdWithADoubleAndUnit ("/phantom/theta", this);
  theta_PhantomCmd->SetGuidance("Select rotation angle of phantom around beam axis");
  theta_PhantomCmd->SetParameterName("thetaPhantom",true);
  theta_PhantomCmd->SetUnitCategory("Angle");
  theta_PhantomCmd->SetDefaultUnit("deg");
  //********************************************************************************************************************

  //**************************** FILM POSITIONING AND ROTATION *******************************//
  
  filmDir = new G4UIdirectory("/film/");
  filmDir->SetGuidance("film rotation commands");
  
  xShift_FilmCmd = new G4UIcmdWithADoubleAndUnit("/film/xShift",this);
  xShift_FilmCmd->SetGuidance("Define x-shift of film");
  xShift_FilmCmd->SetParameterName("xShift",true);
  xShift_FilmCmd->SetDefaultValue(0.);
  xShift_FilmCmd->SetUnitCategory("Length");
  xShift_FilmCmd->SetDefaultUnit("mm");
  
  yShift_FilmCmd = new G4UIcmdWithADoubleAndUnit("/film/yShift",this);
  yShift_FilmCmd->SetGuidance("Define y-shift of film");
  yShift_FilmCmd->SetParameterName("yShift",true);
  yShift_FilmCmd->SetDefaultValue(0.);
  yShift_FilmCmd->SetUnitCategory("Length");
  yShift_FilmCmd->SetDefaultUnit("mm");
  
  zShift_FilmCmd = new G4UIcmdWithADoubleAndUnit("/film/zShift",this);
  zShift_FilmCmd->SetGuidance("Define z position of film centre (all detector geometry wrt this");
  zShift_FilmCmd->SetParameterName("zShift",true);
  zShift_FilmCmd->SetDefaultValue(0.0);
  zShift_FilmCmd->SetUnitCategory("Length");
  zShift_FilmCmd->SetDefaultUnit("mm");
  
  theta_FilmCmd = new G4UIcmdWithADoubleAndUnit ("/film/theta", this);
  theta_FilmCmd->SetGuidance("Select rotation angle of film around beam axis");
  theta_FilmCmd->SetParameterName("thetaFilm",true);
  theta_FilmCmd->SetUnitCategory("Angle");
  theta_FilmCmd->SetDefaultUnit("deg");

 //**************************** CONTACT1 POSITIONING AND ROTATION *******************************//
  
    contact1Dir = new G4UIdirectory("/contact1/");
  contact1Dir->SetGuidance("contact1 rotation commands");
  
  xShift_Contact1Cmd = new G4UIcmdWithADoubleAndUnit("/contact1/xShift",this);
  xShift_Contact1Cmd->SetGuidance("Define x-shift of contact1");
  xShift_Contact1Cmd->SetParameterName("xShift",true);
  xShift_Contact1Cmd->SetDefaultValue(0.);
  xShift_Contact1Cmd->SetUnitCategory("Length");
  xShift_Contact1Cmd->SetDefaultUnit("mm");
  
  yShift_Contact1Cmd = new G4UIcmdWithADoubleAndUnit("/contact1/yShift",this);
  yShift_Contact1Cmd->SetGuidance("Define y-shift of contact1");
  yShift_Contact1Cmd->SetParameterName("yShift",true);
  yShift_Contact1Cmd->SetDefaultValue(0.);
  yShift_Contact1Cmd->SetUnitCategory("Length");
  yShift_Contact1Cmd->SetDefaultUnit("mm");
  
  zShift_Contact1Cmd = new G4UIcmdWithADoubleAndUnit("/contact1/zShift",this);
  zShift_Contact1Cmd->SetGuidance("Define z position of contact1 centre (all detector geometry wrt this");
  zShift_Contact1Cmd->SetParameterName("zShift",true);
  zShift_Contact1Cmd->SetDefaultValue(0.0);
  zShift_Contact1Cmd->SetUnitCategory("Length");
  zShift_Contact1Cmd->SetDefaultUnit("mm");
  
  theta_Contact1Cmd = new G4UIcmdWithADoubleAndUnit ("/contact1/theta", this);
  theta_Contact1Cmd->SetGuidance("Select rotation angle of contact1 around beam axis");
  theta_Contact1Cmd->SetParameterName("thetaContact1",true);
  theta_Contact1Cmd->SetUnitCategory("Angle");
  theta_Contact1Cmd->SetDefaultUnit("deg");

 //**************************** CONTACT2 POSITIONING AND ROTATION *******************************//
  
    contact2Dir = new G4UIdirectory("/contact2/");
  contact2Dir->SetGuidance("contact2 rotation commands");
  
  xShift_Contact2Cmd = new G4UIcmdWithADoubleAndUnit("/contact2/xShift",this);
  xShift_Contact2Cmd->SetGuidance("Define x-shift of contact2");
  xShift_Contact2Cmd->SetParameterName("xShift",true);
  xShift_Contact2Cmd->SetDefaultValue(0.);
  xShift_Contact2Cmd->SetUnitCategory("Length");
  xShift_Contact2Cmd->SetDefaultUnit("mm");
  
  yShift_Contact2Cmd = new G4UIcmdWithADoubleAndUnit("/contact2/yShift",this);
  yShift_Contact2Cmd->SetGuidance("Define y-shift of contact2");
  yShift_Contact2Cmd->SetParameterName("yShift",true);
  yShift_Contact2Cmd->SetDefaultValue(0.);
  yShift_Contact2Cmd->SetUnitCategory("Length");
  yShift_Contact2Cmd->SetDefaultUnit("mm");
  
  zShift_Contact2Cmd = new G4UIcmdWithADoubleAndUnit("/contact2/zShift",this);
  zShift_Contact2Cmd->SetGuidance("Define z position of contact2 centre (all detector geometry wrt this");
  zShift_Contact2Cmd->SetParameterName("zShift",true);
  zShift_Contact2Cmd->SetDefaultValue(0.0);
  zShift_Contact2Cmd->SetUnitCategory("Length");
  zShift_Contact2Cmd->SetDefaultUnit("mm");
  
  theta_Contact2Cmd = new G4UIcmdWithADoubleAndUnit ("/contact2/theta", this);
  theta_Contact2Cmd->SetGuidance("Select rotation angle of contact2 around beam axis");
  theta_Contact2Cmd->SetParameterName("thetaContact2",true);
  theta_Contact2Cmd->SetUnitCategory("Angle");
  theta_Contact2Cmd->SetDefaultUnit("deg");
  //*******************************************************************************************************************
  
  //**************************** GEOMETRY POSITIONING PARAMETERS *******************************//
  
  geomDir = new G4UIdirectory("/geom/");
  geomDir->SetGuidance("parameters for positioning of all geometry");
  
  set_inter_plane_distCmd = new G4UIcmdWithADoubleAndUnit("/geom/plane_dist",this);
  set_inter_plane_distCmd->SetGuidance("Define distance between individual detector planes");
  set_inter_plane_distCmd->SetParameterName("plane_dist",true);
  set_inter_plane_distCmd->SetDefaultValue(10.);
  set_inter_plane_distCmd->SetUnitCategory("Length");
  set_inter_plane_distCmd->SetDefaultUnit("mm");
  
  set_inter_module_distCmd = new G4UIcmdWithADoubleAndUnit("/geom/module_dist",this);
  set_inter_module_distCmd->SetGuidance("Define distance between (x,u,v) detector modules");
  set_inter_module_distCmd->SetParameterName("module_dist",true);
  set_inter_module_distCmd->SetDefaultValue(20.);
  set_inter_module_distCmd->SetUnitCategory("Length");
  set_inter_module_distCmd->SetDefaultUnit("mm");
  
  set_phantom_gapCmd = new G4UIcmdWithADoubleAndUnit("/geom/phantom_gap",this);
  set_phantom_gapCmd->SetGuidance("Define gap between front and back of tracker into which phantom will fit");
  set_phantom_gapCmd->SetParameterName("phantom_gap",true);
  set_phantom_gapCmd->SetDefaultValue(85.);
  set_phantom_gapCmd->SetUnitCategory("Length");
  set_phantom_gapCmd->SetDefaultUnit("mm");
  
  set_halfPhantomSizeZCmd = new G4UIcmdWithADoubleAndUnit("/geom/halfPhantomSizeZ",this);
  set_halfPhantomSizeZCmd->SetGuidance("Define phantom radius");
  set_halfPhantomSizeZCmd->SetParameterName("halfPhantomSizeZ",true);
  set_halfPhantomSizeZCmd->SetDefaultValue(37.5);
  set_halfPhantomSizeZCmd->SetUnitCategory("Length");
  set_halfPhantomSizeZCmd->SetDefaultUnit("mm");
  //*************************************************************************************************************
 
  set_film_gapCmd = new G4UIcmdWithADoubleAndUnit("/geom/film_gap",this);
  set_film_gapCmd->SetGuidance("Define gap between front and back of tracker into which film will fit");
  set_film_gapCmd->SetParameterName("film_gap",true);
  set_film_gapCmd->SetDefaultValue(85.);
  set_film_gapCmd->SetUnitCategory("Length");
  set_film_gapCmd->SetDefaultUnit("mm");
  
  set_halfFilmSizeZCmd = new G4UIcmdWithADoubleAndUnit("/geom/halfFilmSizeZ",this);
  set_halfFilmSizeZCmd->SetGuidance("Define film radius");
  set_halfFilmSizeZCmd->SetParameterName("halfFilmSizeZ",true);
  set_halfFilmSizeZCmd->SetDefaultValue(37.5);
  set_halfFilmSizeZCmd->SetUnitCategory("Length");
  set_halfFilmSizeZCmd->SetDefaultUnit("mm");

  //***************************************************
   set_contact1_gapCmd = new G4UIcmdWithADoubleAndUnit("/geom/contact1_gap",this);
  set_contact1_gapCmd->SetGuidance("Define gap between front and back of tracker into which contact1 will fit");
  set_contact1_gapCmd->SetParameterName("contact1_gap",true);
  set_contact1_gapCmd->SetDefaultValue(85.);
  set_contact1_gapCmd->SetUnitCategory("Length");
  set_contact1_gapCmd->SetDefaultUnit("mm");
  
  set_halfContact1SizeZCmd = new G4UIcmdWithADoubleAndUnit("/geom/halfContact1SizeZ",this);
  set_halfContact1SizeZCmd->SetGuidance("Define contact1 radius");
  set_halfContact1SizeZCmd->SetParameterName("halfContact1SizeZ",true);
  set_halfContact1SizeZCmd->SetDefaultValue(37.5);
  set_halfContact1SizeZCmd->SetUnitCategory("Length");
  set_halfContact1SizeZCmd->SetDefaultUnit("mm");
  //*****************************************************************
 set_contact2_gapCmd = new G4UIcmdWithADoubleAndUnit("/geom/contact2_gap",this);
  set_contact2_gapCmd->SetGuidance("Define gap between front and back of tracker into which contact2 will fit");
  set_contact2_gapCmd->SetParameterName("contact2_gap",true);
  set_contact2_gapCmd->SetDefaultValue(85.);
  set_contact2_gapCmd->SetUnitCategory("Length");
  set_contact2_gapCmd->SetDefaultUnit("mm");
  
  set_halfContact2SizeZCmd = new G4UIcmdWithADoubleAndUnit("/geom/halfContact2SizeZ",this);
  set_halfContact2SizeZCmd->SetGuidance("Define contact2 radius");
  set_halfContact2SizeZCmd->SetParameterName("halfContact2SizeZ",true);
  set_halfContact2SizeZCmd->SetDefaultValue(37.5);
  set_halfContact2SizeZCmd->SetUnitCategory("Length");
  set_halfContact2SizeZCmd->SetDefaultUnit("mm");
  //************************************************************************************************************
  
  set_zShift_pixel_trackerCmd = new G4UIcmdWithADoubleAndUnit("/geom/pix_tracker_z_pos",this);
  set_zShift_pixel_trackerCmd->SetGuidance("Define distance of pixel along the z axis");
  set_zShift_pixel_trackerCmd->SetParameterName("pix_tracker_z_pos",true);
  set_zShift_pixel_trackerCmd->SetDefaultValue(10.);
  set_zShift_pixel_trackerCmd->SetUnitCategory("Length");
  set_zShift_pixel_trackerCmd->SetDefaultUnit("mm");
  set_zShift_pixel_trackerCmd->AvailableForStates(G4State_Idle);
  
  //**************************** MATERIALS OF DETECTOR AND TRACKER COMPONENTS *******************************//
  
  matDir = new G4UIdirectory("/mat/");
  matDir->SetGuidance("define materials of all components in the tracker simulation");
    
  set_world_materialCmd = new G4UIcmdWithAString("/mat/world_material", this);
  set_world_materialCmd->SetGuidance("Change the world material");
  set_world_materialCmd->SetParameterName("world_material", false);
  set_world_materialCmd->SetDefaultValue("G4_AIR");
  set_world_materialCmd->AvailableForStates(G4State_Idle);
  
  set_detector_materialCmd = new G4UIcmdWithAString("/mat/detector_material", this);
  set_detector_materialCmd->SetGuidance("Change the detector material");
  set_detector_materialCmd->SetParameterName("detector_material", false);
  set_detector_materialCmd->SetDefaultValue("G4_Si");
  set_detector_materialCmd->AvailableForStates(G4State_Idle);
  
  set_phantom_materialCmd = new G4UIcmdWithAString("/mat/phantom_material", this);
  set_phantom_materialCmd->SetGuidance("Change the phantom material");
  set_phantom_materialCmd->SetParameterName("phantom_material", false);
  set_phantom_materialCmd->SetDefaultValue("G4_PLEXIGLASS");
  set_phantom_materialCmd->AvailableForStates(G4State_Idle);
  //****************************************************************************************************************
 set_film_materialCmd = new G4UIcmdWithAString("/mat/film_material", this);
  set_film_materialCmd->SetGuidance("Change the film material");
  set_film_materialCmd->SetParameterName("film_material", false);
  set_film_materialCmd->SetDefaultValue("G4_PLEXIGLASS");
  set_film_materialCmd->AvailableForStates(G4State_Idle);
  //*****************************************************
  set_contact1_materialCmd = new G4UIcmdWithAString("/mat/contact1_material", this);
  set_contact1_materialCmd->SetGuidance("Change the contact1 material");
  set_contact1_materialCmd->SetParameterName("contact1_material", false);
  set_contact1_materialCmd->SetDefaultValue("G4_PLEXIGLASS");
  set_contact1_materialCmd->AvailableForStates(G4State_Idle);
  //*********************************************************
 set_contact2_materialCmd = new G4UIcmdWithAString("/mat/contact2_material", this);
  set_contact2_materialCmd->SetGuidance("Change the contact2 material");
  set_contact2_materialCmd->SetParameterName("contact2_material", false);
  set_contact2_materialCmd->SetDefaultValue("G4_PLEXIGLASS");
  set_contact2_materialCmd->AvailableForStates(G4State_Idle);
  //**********************************************************************************************************************
  
  //*********************************************************************************************************//
    
  updateCmd = new G4UIcmdWithoutParameter("/det/update",this);
  updateCmd->SetGuidance("force to recompute geometry.");
  updateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  updateCmd->SetGuidance("if you changed geometrical value(s).");
  updateCmd->AvailableForStates(G4State_Idle);
  
}

DetectorMessenger::~DetectorMessenger()
{
  delete build_strip_detCmd;
  delete build_pixel_detCmd;
  
  delete set_nb_of_stripsCmd;
  delete set_strip_pitchCmd;
  delete set_strip_lengthCmd;
  delete set_det_thicknessCmd;
  
  delete set_inter_plane_distCmd;
  delete set_inter_module_distCmd;
  delete set_phantom_gapCmd;
  delete set_halfPhantomSizeZCmd;
  //*******************************************************
 delete set_film_gapCmd;
  delete set_halfFilmSizeZCmd;
  //*************************
  delete set_contact1_gapCmd;
  delete set_halfContact1SizeZCmd;

  delete set_contact2_gapCmd;
  delete set_halfContact2SizeZCmd;

  //****************************************************
  
  delete phantomDir;
  delete xShift_PhantomCmd;
  delete yShift_PhantomCmd;
  delete zShift_PhantomCmd;
  delete theta_PhantomCmd;
  //***********************************************************
 delete filmDir;
  delete xShift_FilmCmd;
  delete yShift_FilmCmd;
  delete zShift_FilmCmd;
  delete theta_FilmCmd;
  //***************************
  delete contact1Dir;
  delete xShift_Contact1Cmd;
  delete yShift_Contact1Cmd;
  delete zShift_Contact1Cmd;
  delete theta_Contact1Cmd;

 delete contact2Dir;
  delete xShift_Contact2Cmd;
  delete yShift_Contact2Cmd;
  delete zShift_Contact2Cmd;
  delete theta_Contact2Cmd;
  //*************************************************************
    
  delete x_x1_SensorCmd;
  delete y_x1_SensorCmd;
  delete stereo_x1_SensorCmd;
  delete xShift_x1_SensorCmd;
  delete yShift_x1_SensorCmd;
  delete stereoShift_x1_SensorCmd;
  delete set_x1_SensorDUTsetupCmd;
  delete x1_SensorDir;
  
  delete x_u1_SensorCmd;
  delete y_u1_SensorCmd;
  delete stereo_u1_SensorCmd;
  delete xShift_u1_SensorCmd;
  delete yShift_u1_SensorCmd;
  delete stereoShift_u1_SensorCmd;
  delete set_u1_SensorDUTsetupCmd;
  delete u1_SensorDir;
  
  delete x_v1_SensorCmd;
  delete y_v1_SensorCmd;
  delete stereo_v1_SensorCmd;
  delete xShift_v1_SensorCmd;
  delete yShift_v1_SensorCmd;
  delete stereoShift_v1_SensorCmd;
  delete set_v1_SensorDUTsetupCmd;
  delete v1_SensorDir;
  
  delete set_world_materialCmd;
  delete set_detector_materialCmd;
  delete set_phantom_materialCmd;
  //*************************************************
  delete set_film_materialCmd;

  delete set_contact1_materialCmd;

  delete set_contact2_materialCmd;

  //****************************************************

  delete matDir;
  
  delete updateCmd;
  delete detDir;
  
  delete pix1_SensorDir;
  delete pix2_SensorDir;
  delete pix3_SensorDir;
  delete pix4_SensorDir;
  delete set_pix1_SensorDUTsetupCmd;
  delete set_pix2_SensorDUTsetupCmd;
  delete set_pix3_SensorDUTsetupCmd;
  delete set_pix4_SensorDUTsetupCmd;
  delete set_zShift_pixel_trackerCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if ( command == build_strip_detCmd )
    detector->Set_strip_det_build( build_strip_detCmd->GetNewBoolValue(newValue) );
    
  if ( command == build_pixel_detCmd )
    detector->Set_pixel_det_build( build_pixel_detCmd->GetNewBoolValue(newValue) );
    
  if ( command == updateCmd ) detector->UpdateGeometry();
    
  if ( command == set_nb_of_stripsCmd )
    {detector->Set_nb_of_strips(set_nb_of_stripsCmd->GetNewIntValue(newValue));}
    
  if ( command == set_strip_pitchCmd )
    {detector->Set_strip_pitch(set_strip_pitchCmd->GetNewDoubleValue(newValue));}
  
  if ( command == set_strip_lengthCmd )
    {detector->Set_strip_length(set_strip_lengthCmd->GetNewDoubleValue(newValue));}
    
  if ( command == set_det_thicknessCmd )
    {detector->Set_sensor_thickness(set_det_thicknessCmd->GetNewDoubleValue(newValue));}
    
  //********************************* GEOM **************************************//
    
  if ( command == set_inter_plane_distCmd )
    {detector->Set_inter_plane_dist(set_inter_plane_distCmd->GetNewDoubleValue(newValue));}
    
  if ( command == set_inter_module_distCmd )
    {detector->Set_inter_module_dist(set_inter_module_distCmd->GetNewDoubleValue(newValue));}
    
  if ( command == set_phantom_gapCmd )
    {detector->Set_phantom_gap(set_phantom_gapCmd->GetNewDoubleValue(newValue));}
  
  if ( command == set_halfPhantomSizeZCmd )
    {detector->Set_halfPhantomSizeZ(set_halfPhantomSizeZCmd->GetNewDoubleValue(newValue));}
  //**************************************************************************************************************
   
  if ( command == set_film_gapCmd )
    {detector->Set_film_gap(set_film_gapCmd->GetNewDoubleValue(newValue));}
  
  if ( command == set_halfFilmSizeZCmd )
    {detector->Set_halfFilmSizeZ(set_halfFilmSizeZCmd->GetNewDoubleValue(newValue));}

  //***********************************
   
   if ( command == set_contact1_gapCmd )
   {detector->Set_contact1_gap(set_contact1_gapCmd->GetNewDoubleValue(newValue));}
  
  if ( command == set_halfContact1SizeZCmd )
   {detector->Set_halfContact1SizeZ(set_halfContact1SizeZCmd->GetNewDoubleValue(newValue));}
  //****************************************************

   if ( command == set_contact2_gapCmd )
   {detector->Set_contact2_gap(set_contact2_gapCmd->GetNewDoubleValue(newValue));}
  
  if ( command == set_halfContact2SizeZCmd )
   {detector->Set_halfContact2SizeZ(set_halfContact2SizeZCmd->GetNewDoubleValue(newValue));}

  //****************************************************************************************************************
  
  if ( command == set_zShift_pixel_trackerCmd )
    {detector->Set_zShift_pixel_tracker(set_zShift_pixel_trackerCmd->GetNewDoubleValue(newValue));}
    
  //******************************** PHANTOM *************************************//
    
  if ( command == xShift_PhantomCmd ) {
    G4ThreeVector pos= detector->Get_PhantomPosition();
    pos.setX( xShift_PhantomCmd->GetNewDoubleValue(newValue) );
    detector->Set_PhantomPosition(pos);
  }
	
  if ( command == yShift_PhantomCmd ) {
    G4ThreeVector pos= detector->Get_PhantomPosition();
    pos.setY( yShift_PhantomCmd->GetNewDoubleValue(newValue) );
    detector->Set_PhantomPosition(pos);
  }
    
  if ( command == zShift_PhantomCmd ) {
    G4ThreeVector pos= detector->Get_PhantomPosition();
    pos.setZ( zShift_PhantomCmd->GetNewDoubleValue(newValue) );
    detector->Set_PhantomPosition(pos);
  }
	
  if ( command == theta_PhantomCmd )
    detector->Set_PhantomAngle(theta_PhantomCmd->GetNewDoubleValue(newValue));
  //*********************************************************************************************************
 //******************************** FILM *************************************//
    
  if ( command == xShift_FilmCmd ) {
    G4ThreeVector pos= detector->Get_FilmPosition();
    pos.setX( xShift_FilmCmd->GetNewDoubleValue(newValue) );
    detector->Set_FilmPosition(pos);
  }
	
  if ( command == yShift_FilmCmd ) {
    G4ThreeVector pos= detector->Get_FilmPosition();
    pos.setY( yShift_FilmCmd->GetNewDoubleValue(newValue) );
    detector->Set_FilmPosition(pos);
  }
    
  if ( command == zShift_FilmCmd ) {
    G4ThreeVector pos= detector->Get_FilmPosition();
    pos.setZ( zShift_FilmCmd->GetNewDoubleValue(newValue) );
    detector->Set_FilmPosition(pos);
  }
	
  if ( command == theta_FilmCmd )
    detector->Set_FilmAngle(theta_FilmCmd->GetNewDoubleValue(newValue));
//******************************** CONTACT1 *************************************//
    
   if ( command == xShift_Contact1Cmd ) {
   G4ThreeVector pos= detector->Get_Contact1Position();
   pos.setX( xShift_Contact1Cmd->GetNewDoubleValue(newValue) );
   detector->Set_Contact1Position(pos);
   }
	
  if ( command == yShift_Contact1Cmd ) {
   G4ThreeVector pos= detector->Get_Contact1Position();
   pos.setY( yShift_Contact1Cmd->GetNewDoubleValue(newValue) );
   detector->Set_Contact1Position(pos);
   }
    
  if ( command == zShift_Contact1Cmd ) {
   G4ThreeVector pos= detector->Get_Contact1Position();
   pos.setZ( zShift_Contact1Cmd->GetNewDoubleValue(newValue) );
   detector->Set_Contact1Position(pos);
  }
	
   if ( command == theta_Contact1Cmd )
   detector->Set_Contact1Angle(theta_Contact1Cmd->GetNewDoubleValue(newValue));

   //******************************** CONTACT2 *************************************//
    
   if ( command == xShift_Contact2Cmd ) {
   G4ThreeVector pos= detector->Get_Contact2Position();
   pos.setX( xShift_Contact2Cmd->GetNewDoubleValue(newValue) );
   detector->Set_Contact2Position(pos);
   }
	
  if ( command == yShift_Contact2Cmd ) {
   G4ThreeVector pos= detector->Get_Contact2Position();
   pos.setY( yShift_Contact2Cmd->GetNewDoubleValue(newValue) );
   detector->Set_Contact2Position(pos);
   }
    
  if ( command == zShift_Contact2Cmd ) {
   G4ThreeVector pos= detector->Get_Contact2Position();
   pos.setZ( zShift_Contact2Cmd->GetNewDoubleValue(newValue) );
   detector->Set_Contact2Position(pos);
  }
	
   if ( command == theta_Contact2Cmd )
   detector->Set_Contact2Angle(theta_Contact2Cmd->GetNewDoubleValue(newValue));
  //**********************************************************************************************************
  
  //********************** FIRST TRACKER MODULE **********************************//
  
  //**************** Commands for x1 sensor **********************************//

  if ( command == set_x1_SensorDUTsetupCmd )
    detector->Set_x1_SensorDUTSetup( set_x1_SensorDUTsetupCmd->GetNewBoolValue(newValue) );
  
    
  if ( command == x_x1_SensorCmd ) {
        G4ThreeVector pos = detector->Get_x1_SensorPosition();
        pos.setX( x_x1_SensorCmd->GetNewDoubleValue(newValue) );
        detector->Set_x1_SensorPosition(pos);
  }
    
  if ( command == y_x1_SensorCmd ) {
    G4ThreeVector pos = detector->Get_x1_SensorPosition();
    pos.setY( y_x1_SensorCmd->GetNewDoubleValue(newValue) );
    detector->Set_x1_SensorPosition(pos);
  }
    
  if ( command == stereo_x1_SensorCmd )
    detector->Set_x1_SensorAngle( stereo_x1_SensorCmd->GetNewDoubleValue(newValue) );
  
  
  if ( command == xShift_x1_SensorCmd ) {
    G4ThreeVector pos = detector->Get_x1_SensorPositionOffset();
    pos.setX( xShift_x1_SensorCmd->GetNewDoubleValue(newValue) );
    detector->Set_x1_SensorPositionOffset(pos);
  }
	
  if ( command == yShift_x1_SensorCmd ) {
    G4ThreeVector pos = detector->Get_x1_SensorPositionOffset();
    pos.setY( yShift_x1_SensorCmd->GetNewDoubleValue(newValue) );
    detector->Set_x1_SensorPositionOffset(pos);
  }
  
  if ( command == stereoShift_x1_SensorCmd )
    detector->Set_x1_SensorAngleOffset( stereoShift_x1_SensorCmd->GetNewDoubleValue(newValue) );
  
  //**************** Commands for u1 sensor **********************************//
	
  if ( command == set_u1_SensorDUTsetupCmd )
    detector->Set_u1_SensorDUTSetup( set_u1_SensorDUTsetupCmd->GetNewBoolValue(newValue) );


  if ( command == x_u1_SensorCmd ) {
    G4ThreeVector pos = detector->Get_u1_SensorPosition();
    pos.setX( x_u1_SensorCmd->GetNewDoubleValue(newValue) );
    detector->Set_u1_SensorPosition(pos);
  }
  
  if ( command == y_u1_SensorCmd ) {
    G4ThreeVector pos = detector->Get_u1_SensorPosition();
    pos.setY( y_u1_SensorCmd->GetNewDoubleValue(newValue) );
    detector->Set_u1_SensorPosition(pos);
  }
  
  if ( command == stereo_u1_SensorCmd )
    detector->Set_u1_SensorAngle( stereo_u1_SensorCmd->GetNewDoubleValue(newValue) );
  
  
  if ( command == xShift_u1_SensorCmd ) {
    G4ThreeVector pos = detector->Get_u1_SensorPositionOffset();
    pos.setX( xShift_u1_SensorCmd->GetNewDoubleValue(newValue) );
    detector->Set_u1_SensorPositionOffset(pos);
  }
  
  if ( command == yShift_u1_SensorCmd ) {
    G4ThreeVector pos = detector->Get_u1_SensorPositionOffset();
    pos.setY( yShift_u1_SensorCmd->GetNewDoubleValue(newValue) );
    detector->Set_u1_SensorPositionOffset(pos);
  }

  if ( command == stereoShift_u1_SensorCmd )
    detector->Set_u1_SensorAngleOffset( stereoShift_u1_SensorCmd->GetNewDoubleValue(newValue) );
  
  //**************** Commands for v1 sensor **********************************//
  
  if ( command == set_v1_SensorDUTsetupCmd )
    detector->Set_v1_SensorDUTSetup( set_v1_SensorDUTsetupCmd->GetNewBoolValue(newValue) );
  
  
  if ( command == x_v1_SensorCmd ) {
    G4ThreeVector pos = detector->Get_v1_SensorPosition();
    pos.setX( x_v1_SensorCmd->GetNewDoubleValue(newValue) );
    detector->Set_v1_SensorPosition(pos);
  }
  
  if ( command == y_v1_SensorCmd ) {
    G4ThreeVector pos = detector->Get_v1_SensorPosition();
    pos.setY( y_v1_SensorCmd->GetNewDoubleValue(newValue) );
    detector->Set_v1_SensorPosition(pos);
  }
  
  if ( command == stereo_v1_SensorCmd )
    detector->Set_v1_SensorAngle( stereo_v1_SensorCmd->GetNewDoubleValue(newValue) );
  
  
  if ( command == xShift_v1_SensorCmd ) {
    G4ThreeVector pos = detector->Get_v1_SensorPositionOffset();
    pos.setX( xShift_v1_SensorCmd->GetNewDoubleValue(newValue) );
    detector->Set_v1_SensorPositionOffset(pos);
  }
  
  if ( command == yShift_v1_SensorCmd ) {
    G4ThreeVector pos = detector->Get_v1_SensorPositionOffset();
    pos.setY( yShift_v1_SensorCmd->GetNewDoubleValue(newValue) );
    detector->Set_v1_SensorPositionOffset(pos);
  }
  
  if ( command == stereoShift_v1_SensorCmd )
    detector->Set_v1_SensorAngleOffset( stereoShift_v1_SensorCmd->GetNewDoubleValue(newValue) );
  
  //**************** Commands for pix1 sensor **********************************//
    
  if ( command == set_pix1_SensorDUTsetupCmd )
    detector->Set_pix1_SensorDUTSetup( set_pix1_SensorDUTsetupCmd->GetNewBoolValue(newValue) );
  
  //**************** Commands for pix2 sensor **********************************//
  
  if ( command == set_pix2_SensorDUTsetupCmd )
    detector->Set_pix2_SensorDUTSetup( set_pix2_SensorDUTsetupCmd->GetNewBoolValue(newValue) );
  
  //**************** Commands for pix3 sensor **********************************//
  
  if ( command == set_pix3_SensorDUTsetupCmd )
    detector->Set_pix3_SensorDUTSetup( set_pix3_SensorDUTsetupCmd->GetNewBoolValue(newValue) );
  
  //**************** Commands for pix4 sensor **********************************//
  
  if ( command == set_pix4_SensorDUTsetupCmd )
    detector->Set_pix4_SensorDUTSetup( set_pix4_SensorDUTsetupCmd->GetNewBoolValue(newValue) );
  
  //******************** Define Materials *********************************//
  
  if (command == set_world_materialCmd)       {detector->SetWorldMaterial(newValue);}
  if (command == set_detector_materialCmd)    {detector->SetDetectorMaterial(newValue);}
  if (command == set_phantom_materialCmd)     {detector->SetPhantomMaterial(newValue);}
  //********************************************************************************************
  if (command == set_film_materialCmd)     {detector->SetFilmMaterial(newValue);}

   if (command == set_contact1_materialCmd)     {detector->SetContact1Material(newValue);}

   if (command == set_contact2_materialCmd)     {detector->SetContact2Material(newValue);}


  //********************************************************************************************
  
  //******************************************************************************//
  
}

