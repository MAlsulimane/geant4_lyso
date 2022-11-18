
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

#include "G4Material.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
//class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    
  // Constructor
  DetectorConstruction();
    
  // Destructor
  ~DetectorConstruction();
    
public:
    
  // Construct geometry of the setup
  G4VPhysicalVolume* Construct();

  // Update geometry
  void UpdateGeometry();

  G4ThreeVector Get_x1_SensorPosition() const { return pos_x1_Sensor; }
  G4ThreeVector Get_u1_SensorPosition() const { return pos_u1_Sensor; }
  G4ThreeVector Get_v1_SensorPosition() const { return pos_v1_Sensor; }
    
  // Get position offsets (set by user in macro), kept separate from position vectors so tracking can
  // be carried out with/without knowing the offset to see effect. position of sensor centre calculated
  // by GEANT4 by adding position vector to offset vector.
  G4ThreeVector Get_x1_SensorPositionOffset() const { return pos_x1_SensorOffset; }
  G4ThreeVector Get_u1_SensorPositionOffset() const { return pos_u1_SensorOffset; }
  G4ThreeVector Get_v1_SensorPositionOffset() const { return pos_v1_SensorOffset; }

  G4ThreeVector Set_x1_SensorPosition(const G4ThreeVector & pos) { return pos_x1_Sensor=pos; }
  G4ThreeVector Set_u1_SensorPosition(const G4ThreeVector & pos) { return pos_u1_Sensor=pos; }
  G4ThreeVector Set_v1_SensorPosition(const G4ThreeVector & pos) { return pos_v1_Sensor=pos; }
    
  // Set position offsets (set by user in macro), kept separate from position vectors so tracking can
  // be carried out with/without knowing the offset to see effect. position of sensor centre calculated
  // by GEANT4 by adding position vector to offset vector.
  G4ThreeVector Set_x1_SensorPositionOffset(const G4ThreeVector & pos) { return pos_x1_SensorOffset=pos; }
  G4ThreeVector Set_u1_SensorPositionOffset(const G4ThreeVector & pos) { return pos_u1_SensorOffset=pos; }
  G4ThreeVector Set_v1_SensorPositionOffset(const G4ThreeVector & pos) { return pos_v1_SensorOffset=pos; }
    
  G4bool   Set_x1_SensorDUTSetup( const G4bool& flag ) { return is_x1_PlaneDUT=flag; }
  G4bool   Set_u1_SensorDUTSetup( const G4bool& flag ) { return is_u1_PlaneDUT=flag; }
  G4bool   Set_v1_SensorDUTSetup( const G4bool& flag ) { return is_v1_PlaneDUT=flag; }
    
  G4ThreeVector Set_PhantomPosition(const G4ThreeVector & pos)  { return pos_phantom=pos; }
  G4double Set_PhantomAngle(const G4double theta)               { return phantomTheta=theta; }
  //*********************************************************************************************************************
 G4ThreeVector Set_FilmPosition(const G4ThreeVector & pos)  { return pos_film=pos; }
  G4double Set_FilmAngle(const G4double theta)               { return filmTheta=theta; }

  // G4double Set_zShift_film_tracker(const G4double dist){return film_tracker_z_pos=dist;}
  // G4double Get_zShift_film_tracker()  {return film_tracker_z_pos;}
  //*************************************************

  G4ThreeVector Set_Contact1Position(const G4ThreeVector & pos)  { return pos_contact1=pos; }
  G4double Set_Contact1Angle(const G4double theta)               { return contact1Theta=theta; }
  //************************************************

  G4ThreeVector Set_Contact2Position(const G4ThreeVector & pos)  { return pos_contact2=pos; }
  G4double Set_Contact2Angle(const G4double theta)               { return contact2Theta=theta; }
  //**********************************************************************************************************************
    
  G4double Set_inter_plane_dist(const G4double dist)     {return inter_plane_dist=dist;}
  G4double Set_inter_module_dist(const G4double dist)    {return inter_module_dist=dist;}
  G4double Set_phantom_gap(const G4double dist)          {return phantom_gap=dist;}
  G4double Set_halfPhantomSizeZ(const G4double dist)     {return halfPhantomSizeZ=dist;}
 

 // Add the shilding****************************************
  G4ThreeVector Set_ShieldPosition(const G4ThreeVector & pos)  { return pos_shield=pos; }
  G4double Set_ShieldAngle(const G4double theta)               { return shieldTheta=theta; }
  G4double Set_halfShieldSizeZ(const G4double dist)     {return halfShieldSizeZ=dist;}
  //*************************************************
  //**************************************************************************************************************************
 G4double Set_film_gap(const G4double dist)          {return film_gap=dist;}
  G4double Set_halfFilmSizeZ(const G4double dist)     {return halfFilmSizeZ=dist;}
  //******************************************************
   G4double Set_contact1_gap(const G4double dist)          {return contact1_gap=dist;}
   G4double Set_halfContact1SizeZ(const G4double dist)     {return halfContact1SizeZ=dist;}
  //*********************************************************
  G4double Set_contact2_gap(const G4double dist)          {return contact2_gap=dist;}
   G4double Set_halfContact2SizeZ(const G4double dist)     {return halfContact2SizeZ=dist;}
  //*****************************************************************************************************************************
    
  G4double Set_zShift_pixel_tracker(const G4double dist){return pixel_tracker_z_pos=dist;}
   G4double Get_zShift_pixel_tracker()  {return pixel_tracker_z_pos;}
    
  G4double Set_strip_length(const G4double length){return sensorStripLength=length;}
  G4double Set_strip_pitch(const G4double pitch){return teleStripPitch=pitch;}
  G4int Set_nb_of_strips(const G4int strips){return noOfSensorStrips=strips;}
  //G4int Set_nb_of_planes(const G4int){return noOfSensorPlanes=;} //Not used
  G4double Set_sensor_thickness(const G4double thickness){return sensorThickness=thickness;}
    
  G4double Get_x1_SensorAngle() const { return x1_SensorTheta; }
  G4double Get_u1_SensorAngle() const { return u1_SensorTheta; }
  G4double Get_v1_SensorAngle() const { return v1_SensorTheta; }

  G4ThreeVector Get_PhantomPosition() const { return pos_phantom; }
  G4double Get_PhantomAngle() const { return phantomTheta; }

  G4ThreeVector Get_ShieldPosition() const { return pos_shield; } //*******Shielding add
  G4double Get_ShieldAngle() const { return shieldTheta; } //****************Shielding add
  //***************************************************************************************************************
 G4ThreeVector Get_FilmPosition() const { return pos_film; }
  G4double Get_FilmAngle() const { return filmTheta; }
  //*******************************
  G4ThreeVector Get_Contact1Position() const { return pos_contact1; }
  G4double Get_Contact1Angle() const { return contact1Theta; }
  //*****************************************************************
 G4ThreeVector Get_Contact2Position() const { return pos_contact2; }
  G4double Get_Contact2Angle() const { return contact2Theta; }
  //****************************************************************************************************************
    
    G4bool   Set_strip_det_build( const G4bool& flag ) { return build_strip_detectors=flag; }
    G4bool   Set_pixel_det_build( const G4bool& flag ) { return build_pixel_detectors=flag; }
    
    // Pixel detector
    G4bool   Set_pix1_SensorDUTSetup( const G4bool& flag ) { return is_pix1_PlaneDUT=flag; }
    G4bool   Set_pix2_SensorDUTSetup( const G4bool& flag ) { return is_pix2_PlaneDUT=flag; }
    G4bool   Set_pix3_SensorDUTSetup( const G4bool& flag ) { return is_pix3_PlaneDUT=flag; }
    G4bool   Set_pix4_SensorDUTSetup( const G4bool& flag ) { return is_pix4_PlaneDUT=flag; }
    
  G4bool   Set_filmDUTSetup( const G4bool& flag ) { return is_filmDUT=flag; } //********************************************************************

  G4bool   Set_contact1DUTSetup( const G4bool& flag ) { return is_contact1DUT=flag; } //********************************************************************

  G4bool   Set_contact2DUTSetup( const G4bool& flag ) { return is_contact2DUT=flag; } //********************************************************************

    
  // Get stero angle offsets (set by user in macro), kept separate from stereo angle so tracking can
  // be carried out with/without knowing the offset to see effect. stereo angle of sensor calculated
  // by GEANT4 by adding stereo angle to stereo angle offset.
  G4double Get_x1_SensorAngleOffset() const   { return x1_SensorThetaOffset; }
  G4double Get_u1_SensorAngleOffset() const   { return u1_SensorThetaOffset; }
  G4double Get_v1_SensorAngleOffset() const   { return v1_SensorThetaOffset; }
    
  G4double Set_x1_SensorAngle(const G4double theta)  { return x1_SensorTheta=theta; }
  G4double Set_u1_SensorAngle(const G4double theta)  { return u1_SensorTheta=theta; }
  G4double Set_v1_SensorAngle(const G4double theta)  { return v1_SensorTheta=theta; }
    
  // Set stero angle offsets (set by user in macro), kept separate from stereo angle so tracking can
  // be carried out with/without knowing the offset to see effect. stereo angle of sensor calculated
  // by GEANT4 by adding stereo angle to stereo angle offset.
  G4double Set_x1_SensorAngleOffset(const G4double theta)  { return x1_SensorThetaOffset=theta; }
  G4double Set_u1_SensorAngleOffset(const G4double theta)  { return u1_SensorThetaOffset=theta; }
  G4double Set_v1_SensorAngleOffset(const G4double theta)  { return v1_SensorThetaOffset=theta; }
    

  G4double Get_inter_plane_dist()      {return inter_plane_dist;}
  G4double Get_inter_module_dist()     {return inter_module_dist;}
  G4double Get_phantom_gap()           {return phantom_gap;}
  G4double Get_halfPhantomSizeZ()      {return halfPhantomSizeZ;}

  G4double Get_halfShieldSizeZ()      {return halfShieldSizeZ;} //*******************

  //********************************************************************************************************************************
 G4double Get_film_gap()           {return film_gap;}
  G4double Get_halfFilmSizeZ()      {return halfFilmSizeZ;}

  //*****************************
   G4double Get_contact1_gap()           {return contact1_gap;}
   G4double Get_halfContact1SizeZ()      {return halfContact1SizeZ;}
  //*******************************************************************
   G4double Get_contact2_gap()           {return contact2_gap;}
   G4double Get_halfContact2SizeZ()      {return halfContact2SizeZ;}
  //********************************************************************************************************************************

  G4double Get_strip_length()          {return sensorStripLength;}
  G4double Get_strip_pitch()           {return teleStripPitch;}
  G4int Get_nb_of_strips()             {return noOfSensorStrips;}
  G4int Get_nb_of_planes()             {return noOfSensorPlanes;}
  G4double Get_sensor_thickness()      {return sensorThickness;}
    
    G4double Get_pixel_plane_length()           {return PixelsensorLength;}
    G4double Get_pixel_pitch()                  {return telePixelPitch;}
    G4int Get_nb_of_pixels()                    {return noOfSensorPixels;}
    G4int Get_nb_of_pix_planes()                {return noOfPixelSensorPlanes;}
    G4double Get_pix_sensor_thickness()         {return PixelsensorThickness;}
    
    
  G4bool  SetWorldMaterial(G4String material);
  G4bool  SetDetectorMaterial(G4String material);
  G4bool  SetPhantomMaterial(G4String material);
  //**********************************************************************************************************************************
  G4bool  SetShieldMaterial(G4String material); //*****************************
  

 G4bool  SetFilmMaterial(G4String material);

    G4bool  SetContact1Material(G4String material);

    G4bool  SetContact2Material(G4String material);

  
 
  G4bool Get_build_strip_detectors() const {return build_strip_detectors;}
  G4bool Get_build_pixel_detectors() const {return build_pixel_detectors;}
    
  G4Material* GetWorldMaterial()          {return world_material;}
  G4Material* GetDetectorMaterial()       {return detector_material;}
  G4Material* GetPhantomMaterial()        {return phantom_material;}
    G4Material* GetFilmMaterial()        {return film_material;}
    G4Material* GetContact1Material()        {return contact1_material;}
  G4Material* GetShieldMaterial()        {return shield_material;}//*******************

    G4Material* GetContact2Material()        {return contact2_material;}

  //*******************************************************************************************************************************************
  
    
private:
    
  // define needed materials
  void DefineMaterials();
    
  // initialize geometry parameters
  void ComputeParameters();

private:

    // Boolean
    G4bool build_strip_detectors;
    G4bool build_pixel_detectors;
    
    //Materials

    G4Material* world_material;
    G4Material* detector_material;
    G4Material* phantom_material;
  G4Material* shield_material; //*****************************


  G4Material* film_material;//*************************************************************************************
   G4Material* contact1_material;//*************************************************************************************
   G4Material* contact2_material;//*************************************************************************************


    
    // Geometry

    // global mother volume
    G4LogicalVolume * logicWorld;

    // 1st x plane
    G4VPhysicalVolume* physi_x1_Sensor;
    // 1st u plane
    G4VPhysicalVolume* physi_u1_Sensor;
    // 1st v plane
    G4VPhysicalVolume* physi_v1_Sensor;


    G4VPhysicalVolume * physi_x1_SensorStrip;
    G4VPhysicalVolume * physi_u1_SensorStrip;
    G4VPhysicalVolume * physi_v1_SensorStrip;

    G4VPhysicalVolume * physi_phantom;
  G4VPhysicalVolume * physi_shield; //***************

  G4VPhysicalVolume * physi_film; //***********************************************************************************
   G4VPhysicalVolume * physi_contact1; //***********************************************************************************
   G4VPhysicalVolume * physi_contact2; //***********************************************************************************



    G4VPhysicalVolume* physi_pix1_Sensor;
    G4VPhysicalVolume* physi_pix2_Sensor;
    G4VPhysicalVolume* physi_pix3_Sensor;
    G4VPhysicalVolume* physi_pix4_Sensor;
    
    G4VPhysicalVolume * physi_pix1_SensorRow;
    G4VPhysicalVolume * physi_pix2_SensorRow;
    G4VPhysicalVolume * physi_pix3_SensorRow;
    G4VPhysicalVolume * physi_pix4_SensorRow;
    
    G4VPhysicalVolume * physi_pix1_SensorPixel;
    G4VPhysicalVolume * physi_pix2_SensorPixel;
    G4VPhysicalVolume * physi_pix3_SensorPixel;
    G4VPhysicalVolume * physi_pix4_SensorPixel;
    
    // Dimensions
    G4double halfWorldLengthXY;
    G4double halfWorldLengthZ;
    G4double halfPhantomSizeXY;
    G4double halfPhantomSizeZ;
  //*************************************************************************************************
  G4double halfShieldSizeXY; //******************************
  G4double halfShieldSizeZ; //*****************************
    G4double halfFilmSizeXY;
    G4double halfFilmSizeZ;


   G4double halfContact1SizeXY;
   G4double halfContact1SizeZ;


   G4double halfContact2SizeXY;
   G4double halfContact2SizeZ;

    G4double inter_plane_dist;
    G4double inter_module_dist;
    G4double phantom_gap;

    G4double film_gap;
   G4double contact1_gap;

   G4double contact2_gap;

  //************************************************************************************************

    G4double halfSensorSizeX;
    G4double halfSensorSizeY;
    G4double halfSensorSizeZ;

    G4int noOfSensorStrips;
    G4int noOfSensorPlanes;
    G4double sensorStripLength;
    G4double sensorThickness;
    G4double teleStripPitch;

    G4ThreeVector pos_x1_Sensor;
    G4ThreeVector pos_u1_Sensor;
    G4ThreeVector pos_v1_Sensor;

    G4ThreeVector pos_x1_SensorOffset;
    G4ThreeVector pos_u1_SensorOffset;
    G4ThreeVector pos_v1_SensorOffset;

    G4ThreeVector pos_phantom;
    G4double phantomTheta;
  //*******************************************************************************************************
  G4ThreeVector pos_shield; //*****************************
  G4double shieldTheta;//*******************************

 G4ThreeVector pos_film;
    G4double filmTheta;

 G4ThreeVector pos_contact1;
    G4double contact1Theta;
 G4ThreeVector pos_contact2;
    G4double contact2Theta;

    G4double x1_SensorTheta;
    G4double u1_SensorTheta;
    G4double v1_SensorTheta;

    G4double x1_SensorThetaOffset;
    G4double u1_SensorThetaOffset;
    G4double v1_SensorThetaOffset;
    
    G4bool is_x1_PlaneDUT;
    G4bool is_u1_PlaneDUT;
    G4bool is_v1_PlaneDUT;

    // Pixels
    G4double halfPixSensorSizeX;
    G4double halfPixSensorSizeY;
    G4double halfPixSensorSizeZ;
    
    G4double noOfSensorPixels;
    G4double noOfPixelSensorPlanes;
    G4double telePixelPitch;
    G4double PixelsensorLength;
    G4double PixelsensorThickness;
    
    G4ThreeVector pos_pix1_Sensor;
    G4ThreeVector pos_pix2_Sensor;
    G4ThreeVector pos_pix3_Sensor;
    G4ThreeVector pos_pix4_Sensor;
    
    G4double pix1_SensorTheta;
    G4double pix2_SensorTheta;
    G4double pix3_SensorTheta;
    G4double pix4_SensorTheta;
    
  G4bool is_filmDUT;   //**********************************************************************
   G4bool is_contact1DUT;   //**********************************************************************
   G4bool is_contact2DUT;   //**********************************************************************

    G4bool is_pix1_PlaneDUT;
    G4bool is_pix2_PlaneDUT;
    G4bool is_pix3_PlaneDUT;
    G4bool is_pix4_PlaneDUT;
    
    G4double pixel_tracker_z_pos;
    

  //UI Messenger
  DetectorMessenger * messenger;
	
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
