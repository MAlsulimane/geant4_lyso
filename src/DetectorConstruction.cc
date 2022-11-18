
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4NistManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "SensitiveDetector.hh"
#include "SensitiveDetector_pix.hh"
#include "G4SDManager.hh"

#include "G4UserLimits.hh"
#include "G4PhysicalConstants.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction()
{
	//Create a messanger (defines custom UI commands)
	messenger = new DetectorMessenger(this);

	//--------- Material definition ---------
	DefineMaterials();

	//--------- Sizes of the principal geometrical components (solids)  ---------
	ComputeParameters();
}
 
DetectorConstruction::~DetectorConstruction() { delete messenger; }
 
void DetectorConstruction::DefineMaterials() 
{
  //*************************************************************************Add this elements to define the film material as a 6LiF NOT default LiF
    G4String symbol, name;             //a=mass of a mole
   G4double a, z, density;      //z=mean number of protons
   //G4int iz, n;                 //iz=number of protons  in an isotope;   
   G4int ncomponents, natoms;                                         
   //G4double abundance, fractionmass;

   //G4NistManager* nist = G4NistManager::Instance();
 

   G4int protons=3, neutrons=3, nucleons=protons+neutrons; 
density=0.462*g/cm3; 
a=6.00*g/mole;
z=3;
name="6Li";
  G4Element* F   = new G4Element("Fluorine",symbol="F" , z= 9., a= 19*g/mole); 
   G4Isotope* li6 = new G4Isotope("Li6", protons, nucleons, a);
   G4Element* Li6  = new G4Element("Lithium6"  ,"6Li" , 1);
   Li6->AddIsotope(li6, 100*perCent);
   G4Material* Lithium6 = new G4Material("Lithium6", density, 1);
   Lithium6->AddElement(Li6, 100*perCent); // 100% means all the Li material inriched with 100% of 6Li
   G4Material* LiF = new G4Material("6LiF", density= 2.539* g/cm3, ncomponents=2); //taking into account lower density of 6Li
				       LiF->AddElement(F, natoms=1);
				        LiF->AddElement(Li6, natoms=1);
					//Add the definition of Lithium6 Carbonate 6Li2CO3, Z = 73.891 However if we consider the 6Li, it will be 72.0089
					G4Element* O   = new G4Element("Oxygen",symbol="O" , z= 8., a= 15.9994*g/mole);
					G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6, a= 12.0107*g/mole);
					G4Material* Li2CO3 = new G4Material("6Li2CO3", density= 2.11* g/cm3, ncomponents=3);
					Li2CO3->AddElement(O, natoms=3);
					Li2CO3->AddElement(Li6, natoms=2);
					Li2CO3->AddElement(C, natoms=1);
					// Add the Lyso construction materials as a sensitive detector rather than Si
					G4Element* Lu   = new G4Element("Lutetium",symbol="Lu" , z= 71, a= 174.9668*g/mole);
					G4Element* Y  = new G4Element("Yttrium"  ,symbol="Y" , z= 39, a=88.90584*g/mole);
					G4Element* Si   = new G4Element("Silicon",symbol="Si" , z= 14., a=28.085*g/mole);
					G4Element* Ce  = new G4Element("Cerium"  ,symbol="Ce" , z= 58, a= 140.116*g/mole);
					G4Material* LYSO = new G4Material("LYSO", density= 7.25* g/cm3, ncomponents=5);
					LYSO->AddElement(Lu, 73.856*perCent);
					LYSO->AddElement(Y, 1.975*perCent);
					LYSO->AddElement(Si, 6.240*perCent);
					LYSO->AddElement(O, 17.773*perCent);
					LYSO->AddElement(Ce, 0.156*perCent);
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&End of the changes&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    //################## Water Corrections ##############
    G4Element* H = new G4Element("TS_H_of_Water" , "H" , 1. , 1.0079*g/mole);
    G4Material* H2O = new G4Material("Water_ts" , 1.00*g/cm3, ncomponents=2,kStateLiquid, 593*kelvin, 150*bar);
    H2O->AddElement(H, natoms=2);
    H2O->AddElement(O, natoms=1);
    H2O->GetIonisation()->SetMeanExcitationEnergy(78.0*eV);
    G4Material* LiH = new G4Material("LiH", density= 0.708* g/cm3, ncomponents=2);
    LiH->AddElement(Li6, natoms=1);
    LiH->AddElement(H, natoms=1);
    //Add the defintion of Lithioum Hydride LiH and the correct for the Hydrogen
    //LiH atomic wheight = 7.95
    //LiH density = 0.78 g/cm3.
    
// &&&&&&&&&&&&&&&&& Correction for the H in polyethylene for thermal neutron simulation applications. This corrections used to correct the H element rather than default PE according to latest geant4 paper about neutron simulations.;
    G4Material* POLYMAT = new G4Material("POLYMAT" , 0.94*g/cm3, ncomponents=2,kStateSolid, 293.15*kelvin);
    POLYMAT->AddElement(H, natoms=4);
    POLYMAT->AddElement(C, natoms=2 );
  //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&End of the changes&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    G4Material* PolyLi = new G4Material("PolyLi" , density= 1.06* g/cm3, ncomponents=3);
    PolyLi->AddElement(H, natoms=4);
    PolyLi->AddElement(C, natoms=2 );
    PolyLi->AddElement(Li6, natoms=1);
    // Set default materials for tracker components
    // and targets/absorbers
    
    G4cout << "\nSetting the default material properties of the world and the volumes as follows...\n" << G4endl;
    
    SetWorldMaterial("G4_AIR");
    //SetDetectorMaterial("G4_Si");
     detector_material = LYSO;
    SetPhantomMaterial("G4_AIR");
    // phantom_material = H2O;
    //SetFilmMaterial("G4_LITHIUM_FLUORIDE");
        // SetFilmMaterial("G4_LITHIUM_CARBONATE");
    //SetFilmMaterial("G4_LITHIUM_HYDRIDE");
    SetContact1Material("G4_AIR"); // Not important for LYSO          
    SetContact2Material("G4_Pb"); // shielding 1 made out of Lead >>> Color RED
     //film_material = LiF;
    film_material = Li2CO3;
    //film_material = LiH;
    //film_material = PolyLi;
    shield_material = POLYMAT; //Shielding 2 made out of PE >>> Color BLUE 
    //SetShieldMaterial("G4_AIR");// Shielding 2 made out of PE 
    //shield_material = H2O;
}

G4bool DetectorConstruction::SetWorldMaterial(G4String material)
{
    if (G4Material* pMat = G4NistManager::Instance()->FindOrBuildMaterial(material, false) )
    {
        world_material = pMat;
        G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
        G4RunManager::GetRunManager() -> GeometryHasBeenModified();
        G4cout << "\nThe material of the World has been set to " << material << "\n" << G4endl;
    }
    else
    {
        G4cout << "\nWARNING: material \"" << material << "\" doesn't exist in NIST elements/materials"
        " table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
        G4cout << "Reverting to default material parameter for this volume" << G4endl;
        return false;
    }
    
    return true;
}

G4bool DetectorConstruction::SetPhantomMaterial(G4String material)
{
    if (G4Material* pMat = G4NistManager::Instance()->FindOrBuildMaterial(material, false) )
    {
        phantom_material = pMat;
        G4cout << "The material of Phantom has been set to " << material << "\n" << G4endl;
    }
    else
    {
        G4cout << "\nWARNING: material \"" << material << "\" doesn't exist in NIST elements/materials"
        " table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
        G4cout << "Reverting to default material parameter for this volume" << G4endl;
        return false;
    }
    
    return true;
}

//**********************Set the material of the shielding *******************

G4bool DetectorConstruction::SetShieldMaterial(G4String material)
{
  if (G4Material* pMat = G4NistManager::Instance()->FindOrBuildMaterial(material, false) )
    {
      shield_material = pMat;
      G4cout << "The material of Shield has been set to " << material << "\n" << G4endl;
    }
  else
    {
      G4cout << "\nWARNING: material \"" << material << "\" doesn't exist in NIST elements/materials"
        " table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
      G4cout << "Reverting to default material parameter for this volume" << G4endl;
      return false;
    }
    
  return true;
}

//******************************************************************************************************************************
G4bool DetectorConstruction::SetFilmMaterial(G4String material)
{
    if (G4Material* pMat = G4NistManager::Instance()->FindOrBuildMaterial(material, false) )
    {
        film_material = pMat;
        G4cout << "The material of film has been set to " << material << "\n" << G4endl;
    }
    else
    {

       G4cout << "\nWARNING: material \"" << material << "\" doesn't exist in NIST elements/materials"
      " table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
      G4cout << "Reverting to default material parameter for this volume" << G4endl;
       return false;
    }
    
    return true;
}

//*****************************************************************
G4bool DetectorConstruction::SetContact1Material(G4String material)
{
    if (G4Material* pMat = G4NistManager::Instance()->FindOrBuildMaterial(material, false) )
    {
        contact1_material = pMat;
        G4cout << "The material of contact1 has been set to " << material << "\n" << G4endl;
    }
    else
    {

       G4cout << "\nWARNING: material \"" << material << "\" doesn't exist in NIST elements/materials"
      " table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
      G4cout << "Reverting to default material parameter for this volume" << G4endl;
       return false;
    }
    
    return true;
}
//****************************************************************
G4bool DetectorConstruction::SetContact2Material(G4String material)
{
    if (G4Material* pMat = G4NistManager::Instance()->FindOrBuildMaterial(material, false) )
    {
        contact2_material = pMat;
        G4cout << "The material of contact2 has been set to " << material << "\n" << G4endl;
    }
    else
    {

       G4cout << "\nWARNING: material \"" << material << "\" doesn't exist in NIST elements/materials"
      " table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
      G4cout << "Reverting to default material parameter for this volume" << G4endl;
       return false;
    }
    
    return true;
}
//*******************************************************************************************************************************

G4bool DetectorConstruction::SetDetectorMaterial(G4String material)
{
    if (G4Material* pMat = G4NistManager::Instance()->FindOrBuildMaterial(material, false) )
    {
        detector_material = pMat;
        G4cout << "The material of Detector has been set to " << material << "\n" << G4endl;
    }
    else
    {
        G4cout << "\nWARNING: material \"" << material << "\" doesn't exist in NIST elements/materials"
        " table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
        G4cout << "Reverting to default material parameter for this volume" << G4endl;
        return false;
    }
    
    return true;
}

void DetectorConstruction::ComputeParameters()
{
	// This function defines the defaults
	// of the geometry construction

  halfWorldLengthXY = 0.5* m; //THE WORLD GEOMETRY HERE IS (1*1*2)m^3
	halfWorldLengthZ =  1.0* m;
	halfPhantomSizeXY = 10*cm;  // THE PHANTOM GEOMETRY HERE IS ( 20*20*40) cm^3
    halfPhantomSizeZ = 20*cm;

    halfShieldSizeXY = 2.5*cm; //**************************** Control the dimention of the shielding
    halfShieldSizeZ = 2.5*cm;//*****************************//Shielding thick is  (5cm of PE)
    //************************************************************************************  Here I change the dimentions of the film and I remove the film gap, Actually I dont now what is mean ??
	halfFilmSizeXY = 5*mm;  // THE converter length is 1.0cm
	halfFilmSizeZ = 0.5 *um;  //the converter thick is 1.0 um
	// film_gap = 0.*mm;

	halfContact1SizeXY = 5*mm; // This not important for LYSO, however it does for Si sensor due to Si sensor attached with thin layer of Al foil
           halfContact1SizeZ = 50.*nm;

           halfContact2SizeXY = 2.5*cm; // The lead shielding geometry 5cm thick of Lead 
           halfContact2SizeZ = 2.5*cm;
           //contact_gap = 0.*mm;
    //************************************************************************************
    
    noOfSensorStrips = 1024;    // NUMBER OF THE PIXEL INSIDE THE SENSOR
    teleStripPitch  = 90.8 * um;
    sensorStripLength = 96.*mm;
    sensorThickness = 150.*um; // THICKNESS OF THE SI SENSORS
    noOfSensorPlanes = 3;       // Used by RunAction and charge sharing remains fixed
    
    inter_plane_dist = 10.*mm;
    inter_module_dist = 100.*mm;
    phantom_gap = 80.*mm;

    //************************* THESE PARAMETERS (X1, U1, V1) DOSE NOT EFFECT ON THE SIMULATION, YOU NEED CARE ONLY ABOUT (PIX1, PIX2, PIX3 AND PIX4) *************
	//Device under test (DUT)
	is_x1_PlaneDUT = false;			// Flag turns Silcon planes into strip sensors,
	is_u1_PlaneDUT = false;         // set to true by macro file, can be left false
	is_v1_PlaneDUT = false;         // here.
    
	x1_SensorTheta = 0.*deg;    // Set rotation of planes 60deg for PRaVDA det
	u1_SensorTheta = 0.*deg;    // set by macro file, can be left as 0deg here.
	v1_SensorTheta = 0.*deg;
    
    phantomTheta = 0.*deg;          // Set rotation of phantom for CT mode,
                                    // set by macro file, can be left as 0deg here.

    //*******************************************************************************************************************************
filmTheta = 0.*deg;

contact1Theta = 0.*deg;
contact2Theta = 0.*deg;
//*****************************************************************************************************************************  
    
    // Initialise all offsets to zero, these can be used to specify
    // angular misalignments the effects of which can be studied
    // in the tracking routines
    
    x1_SensorThetaOffset = 0;
    u1_SensorThetaOffset = 0;
    v1_SensorThetaOffset = 0;
    
    
    // Phantom is placed by default at isocentre all other geometry is relative to this
    // any shift in z supplied in the macro will be applied at runtime and picked up
    // in the TrackerGeometry class and applied there.
    pos_phantom = G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm);
    pos_shield = G4ThreeVector(0.0*mm, 0.0*mm, 27.0*mm);//it was at 104mm at Z in order to put it after the lead shield********************* control the position of the shielding
    //*********************************************************************************************************
    pos_film = G4ThreeVector(0.0*mm, 0.0*mm, 102.7*mm);   //&&&it was at 155.9 to put it after the lead and PE shielding&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& Here I put the film in the centre of the phantom

    pos_contact1 = G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm); 
    pos_contact2 = G4ThreeVector(0.0*mm, 0.0*mm, 77.0*mm); //it was 27 at Z in order to put the shield in front of the source

    //**********************************************************************************************************

    
    // Here we initialise all volume position vectors to zero,
    // the z position is computed in the Construct() function
    // below so that the volumes are never placed on top of each other.
    // z positions are computed using parameters set in main.mac
    // or if nothing is set, the default parameter values above.

    pos_x1_Sensor = G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm);
    pos_u1_Sensor = G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm);
    pos_v1_Sensor = G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm);
    
    // Initialise all offsets to zero, these can be used to specify
    // position misalignments the effects of which can be studied
    // in the tracking routines
    
    pos_x1_SensorOffset = G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm);
    pos_u1_SensorOffset = G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm);
    pos_v1_SensorOffset = G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm);
    
    // Build/readout strips or pixels or both
    build_strip_detectors = false;
    build_pixel_detectors = true;
    // **************************** THESE PARAMETER YOU NEED TO FOCUS ON IT (PIX1, PIX2, PIX3 AND PIX4), For LYSO modelling only consider Pixel 1
    // Pixel properties
    
    is_pix1_PlaneDUT = true;
    is_pix2_PlaneDUT = true;
    is_pix3_PlaneDUT = true;
    is_pix4_PlaneDUT = true;
    
    telePixelPitch  = 50 * um;    // this to control the detector size (1.0cm) and the detector itself is segmented.
    PixelsensorLength = 10.*mm;
    
    // Sensors are 1D at the moment
    noOfSensorPixels = (PixelsensorLength / telePixelPitch);// * (PixelsensorLength / telePixelPitch); according the si diode of ahmed
    PixelsensorThickness = 500.*um;// THE THICKNESS IS 500 um for LYSO 
    noOfPixelSensorPlanes = 4;       // Used by RunAction and charge sharing remains fixed        NUMBER OF THE SI IS 4 SENSORS
    
    pix1_SensorTheta = 0*deg; // ANY CHANGE HERE WILL ROTATE AROUND THE BEAM ANGLE, THATS MEAN AROUND Z AXIS( THIS APPLY FOR ALL 4 SENSORS )
    pix2_SensorTheta = 0*deg;
    pix3_SensorTheta = 0*deg;
    pix4_SensorTheta = 0*deg;
    
    pos_pix1_Sensor = G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm); // ANY CHANGE HERE WILL SHIFT THE SENSOR POSITION FROM THE PHANTOM CENTER. E.G. IF YOU ADD [halfPhantomSizeXY+(1*mm)] THIS WILL SHIFT THE SENSOR ON THE TOP EDGE OF THE PHANTOM THEN MOVE WITH 1 mm. THIS APPLY FOR ALL SENSORS
    pos_pix2_Sensor = G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm);
    pos_pix3_Sensor = G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm);
    pos_pix4_Sensor = G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm);
        
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    
    // Define tracker parameters and planes
    // These parameters must be in here in the Construct() function
    // as these variables must be recomputed if strips,
    // pitch or strip length are changed in the .mac file at runtime.
    // via the UpdateGeometry() function called with the /det/update command in
    // the tracker_geom.mac macro
    
	halfSensorSizeX = noOfSensorStrips*teleStripPitch/2.;
	halfSensorSizeY = sensorStripLength/2.;
	halfSensorSizeZ = sensorThickness/2.;
    
    halfPixSensorSizeX = (PixelsensorLength / telePixelPitch)*telePixelPitch/2.;
    halfPixSensorSizeY = (PixelsensorLength / telePixelPitch)*telePixelPitch/2.;
    halfPixSensorSizeZ = PixelsensorThickness/2.;
    
	// This function is called by G4 when the detector is to be created
	// Definitions of Solids, Logical Volumes, Physical Volumes
  
    G4Color
    green(0.0,1.0,0.0),
    blue(0.0,0.0,1.0),
    brown(0.4,0.4,0.1),
    white(1.0,1.0,1.0),
    red(1.0,0.0,0.0),
    yellow(1.0,1.0,0.0);
    
    //************************************ Compute positions of geometry in z *********************************************//
    
    // All detectors and other geometry positioned with respect to the phantom which is at (0,0,0).
    // x/y postions are initialised to be the same as the phantom in the ComputeParameters() but positions along z (beam axis)
    // are set below using the spacing parameters defined with default values in the ComputeParameters() function or using
    // from the tracker_geom.mac. Offsets in x, y and rotation are set in the same way. Positions and stereo angles are kept
    // separate values set at runtime from their offsets so that tracking routines outside GEANT4 can be done with and without
    // offsets applied to see effect of notknowing the offset has on the tracking resolution.
    
    
    //**** Incoming tracker ****//
    
    pos_x1_Sensor.setZ( pos_phantom.getZ() - 0.5*phantom_gap - inter_plane_dist*4 - inter_module_dist );
    pos_x1_Sensor += pos_x1_SensorOffset;  // add offset to get any offset in x/y
    
	pos_u1_Sensor.setZ( pos_phantom.getZ() - 0.5*phantom_gap - inter_plane_dist*3 - inter_module_dist );
    pos_u1_Sensor += pos_u1_SensorOffset;  // add offset to get any offset in x/y
	pos_v1_Sensor.setZ( pos_phantom.getZ() - 0.5*phantom_gap - inter_plane_dist*2 - inter_module_dist );
    pos_v1_Sensor += pos_v1_SensorOffset;  // add offset to get any offset in x/y
    
    // Add any offsets in stereo angle before rotation matrices are created for each detector plane
    x1_SensorTheta += x1_SensorThetaOffset;
    u1_SensorTheta += u1_SensorThetaOffset;
    v1_SensorTheta += v1_SensorThetaOffset;
    
    
    // For now put 40 mm after last strip plane
    //pos_pix1_Sensor.setZ( pos_phantom.getZ() - 0.5*phantom_gap - inter_plane_dist*2 - inter_module_dist + 40*mm );
    G4float pixel_xy_plane_spacing = 0.*um; //SEE HERE, THE SPACE BETWEEN THE ONE SENSOR THAT CONTAIN (TWO SENSOR 1 ON X AND OTHER ON Y) IS 0 DUE TO WE CONSIDER IT 1 SENSOR COUNT AS A NET ON X,Y DIRECTIONS
  G4float air_gap = 300.*um;   
  G4float pix_det_spacing = ( air_gap) +  (  halfContact1SizeZ * 4 );
    G4float pixel_z_shift = -halfPhantomSizeZ + pixel_tracker_z_pos;
      G4float contact1_z_shift = -halfPhantomSizeZ + pixel_tracker_z_pos  +  ( PixelsensorThickness*2) +  pixel_xy_plane_spacing + ( halfContact1SizeZ * 2 ) ;
     pos_contact1.setZ( contact1_z_shift );//************************************************
     //  pos_contact2.setZ( contact2_z_shift ); TEMPORARY COMMENT OUT TO USE THE CODE FOR LYSO AND CONTACT2 AS A SHIELDING   
     pos_pix1_Sensor.setZ( pixel_z_shift );
     pos_pix2_Sensor.setZ( pos_pix1_Sensor.getZ() + pixel_xy_plane_spacing + PixelsensorThickness );
     pos_pix3_Sensor.setZ( pos_pix2_Sensor.getZ() + pix_det_spacing );
    pos_pix4_Sensor.setZ( pos_pix3_Sensor.getZ() + pixel_xy_plane_spacing + PixelsensorThickness );
    //***********************************************************************************************************************//
	//------------------------------
	// World
	//------------------------------
 
	G4GeometryManager::GetInstance()->SetWorldMaximumExtent(2.*halfWorldLengthXY);
	G4cout << "Computed tolerance = "
			<< G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
			<< " mm" << G4endl;


	G4Box * solidWorld = new G4Box("world",halfWorldLengthXY,halfWorldLengthXY,halfWorldLengthZ);
	logicWorld= new G4LogicalVolume( solidWorld, world_material, "World", 0, 0, 0);
  
	//  Must place the World Physical volume unrotated at (0,0,0).
	G4VPhysicalVolume * physiWorld = new G4PVPlacement(0,           // no rotation
			G4ThreeVector(), // at (0,0,0)
			logicWorld,      // its logical volume
			"World",         // its name
			0,               // its mother  volume
			false,           // no boolean operations
			0);              // copy number
    
    
    G4cout << "\nWorld built\n" << G4endl;
    
    //------------------------------
	// Phantom
	//------------------------------
    
    ////Box
    G4Box * solidPhantom = new G4Box("phantom",halfPhantomSizeXY,halfPhantomSizeXY,halfPhantomSizeZ);
    
    ////Cylinder
    //G4Tubs * solidPhantom = new G4Tubs("phantom", 0.*cm, halfPhantomSizeXY, halfPhantomSizeZ, 0.*deg, 360.*deg);
    
    ////Sphere
    //G4Sphere * solidPhantom = new G4Sphere("phantom", 0.*cm, halfPhantomSizeXY, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
    
    G4RotationMatrix * phantom_rm = new G4RotationMatrix;
    phantom_rm->rotateZ(phantomTheta);
	G4LogicalVolume * logicPhantom = new G4LogicalVolume( solidPhantom, phantom_material, "Phantom");
    physi_phantom = new G4PVPlacement(phantom_rm,
                                          pos_phantom,
                                          logicPhantom,
                                          "Phantom_1",
                                          logicWorld,
                                          false,
				      0);
    
    G4cout << "\nPhantom built\n" << G4endl;

    //******************************Here add the changes of the shielding **********************

// Shield
    //------------------------------
    
    //Box

G4Box * solidShield = new G4Box("shield",halfShieldSizeXY,halfShieldSizeXY,halfShieldSizeZ);
    
    ////Cylinder
    //G4Tubs * solidPhantom = new G4Tubs("phantom", 0.*cm, halfPhantomSizeXY, halfPhantomSizeZ, 0.*deg, 360.*deg);
    
    ////Sphere
    //G4Sphere * solidPhantom = new G4Sphere("phantom", 0.*cm, halfPhantomSizeXY, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
    
    G4RotationMatrix * shield_rm = new G4RotationMatrix;
    shield_rm->rotateZ(shieldTheta);
    G4LogicalVolume * logicShield = new G4LogicalVolume( solidShield, shield_material, "Shield");
    physi_shield = new G4PVPlacement(shield_rm,
                                      pos_shield,
                                      logicShield,
                                      "Shield_1",
				     logicPhantom,//World
                                      false,
                                      0);
    
    G4cout << "\nShield built\n" << G4endl;
    
    //******************************End of the changes***************************

    //**********************************************************************************************************************************************
	// Film
	//------------------------------
    
    ////Box
    G4Box * solidFilm = new G4Box("film",halfFilmSizeXY,halfFilmSizeXY,halfFilmSizeZ);
    
    ////Cylinder
    //G4Tubs * solidFilm = new G4Tubs("film", 0.*cm, halffilmSizeXY, halfFilmSizeZ, 0.*deg, 360.*deg);
    
    ////Sphere
    //G4Sphere * solidFilm = new G4Sphere("film", 0.*cm, halfFilmSizeXY, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
    
    G4RotationMatrix * film_rm = new G4RotationMatrix;
    film_rm->rotateZ(filmTheta);
	G4LogicalVolume * logicFilm = new G4LogicalVolume( solidFilm, film_material, "Film");
    physi_film = new G4PVPlacement(film_rm,
                                          pos_film,
                                          logicFilm,
                                          "Film_1",
                                           logicPhantom,
                                          false,
				   1, true);
    
    G4cout << "\nFilm built\n" << G4endl;
   //**********************************************************************************************************************************************
	// Contact1
	//------------------------------
    
    ////Box
    G4Box * solidContact1 = new G4Box("contact1",halfContact1SizeXY,halfContact1SizeXY,halfContact1SizeZ);
    
    ////Cylinder
    //G4Tubs * solidFilm = new G4Tubs("film", 0.*cm, halffilmSizeXY, halfFilmSizeZ, 0.*deg, 360.*deg);
    
    ////Sphere
    //G4Sphere * solidFilm = new G4Sphere("film", 0.*cm, halfFilmSizeXY, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
    
    G4RotationMatrix * contact1_rm = new G4RotationMatrix;
    contact1_rm->rotateZ(contact1Theta);
	G4LogicalVolume * logicContact1 = new G4LogicalVolume( solidContact1, contact1_material, "Contact1");
    physi_contact1 = new G4PVPlacement(contact1_rm,
                                          pos_contact1,
                                          logicContact1,
                                          "Contact1_1",
                                           logicPhantom,
                                          false,
				       1, true);
    
    G4cout << "\nContact1 built\n" << G4endl;
    //******************************************************************************************************************************
	// Contact2
	//------------------------------
    
    ////Box
    G4Box * solidContact2 = new G4Box("contact2",halfContact2SizeXY,halfContact2SizeXY,halfContact2SizeZ);
    
    ////Cylinder
    //G4Tubs * solidFilm = new G4Tubs("film", 0.*cm, halffilmSizeXY, halfFilmSizeZ, 0.*deg, 360.*deg);
    
    ////Sphere
    //G4Sphere * solidFilm = new G4Sphere("film", 0.*cm, halfFilmSizeXY, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
    
    G4RotationMatrix * contact2_rm = new G4RotationMatrix;
    contact2_rm->rotateZ(contact2Theta);
	G4LogicalVolume * logicContact2 = new G4LogicalVolume( solidContact2, contact2_material, "Contact2");
    physi_contact2 = new G4PVPlacement(contact2_rm,
                                          pos_contact2,
                                          logicContact2,
                                          "Contact2_1",
                                           logicWorld,
                                          false,
				       1, true);
    
    G4cout << "\nContact2 built\n" << G4endl;


    //**********************************************************************************************************************************************
    //------------------------------
    // Tracker (Pixel detector(s))
    //------------------------------

    G4Box * solidSensor_pix = new G4Box("Sensor_pix",
                                    halfPixSensorSizeX,halfPixSensorSizeY,halfPixSensorSizeZ);
    
    G4LogicalVolume * logicSensorPlane_pix =
                                            new G4LogicalVolume(solidSensor_pix,        // its solid
                                                                detector_material,      // its material
                                                                "SensorPlane_pix");     // its name
    
    // Define rotations for each sensor in x, y, or z
    // using rotation matrix, value can be set in macro files
    G4RotationMatrix * rm_pix1 = new G4RotationMatrix;
    G4RotationMatrix * rm_pix2 = new G4RotationMatrix;
    G4RotationMatrix * rm_pix3 = new G4RotationMatrix;
    G4RotationMatrix * rm_pix4 = new G4RotationMatrix;
    
    G4double halfPixSensorPixelSizeX = telePixelPitch/2.;
    G4double halfPixSensorPixelSizeY = telePixelPitch/2.;
    G4double halfPixSensorPixelSizeZ = sensorThickness/2.;
    
    
    //Device under Test - replaces Plane of Si Beam Telescope
    G4Box * solid_pix1_Sensor_pix = new G4Box("pixSensor1",halfPixSensorSizeX,halfPixSensorSizeY,halfPixSensorSizeZ);
    
    //Device under Test - replaces Plane of Si Beam Telescope
    G4Box * solid_pix2_Sensor_pix = new G4Box("pixSensor2",halfPixSensorSizeX,halfPixSensorSizeY,halfPixSensorSizeZ);
    
    //Device under Test - replaces Plane of Si Beam Telescope
    G4Box * solid_pix3_Sensor_pix = new G4Box("pixSensor3",halfPixSensorSizeX,halfPixSensorSizeY,halfPixSensorSizeZ);
    
    //Device under Test - replaces Plane of Si Beam Telescope
    G4Box * solid_pix4_Sensor_pix = new G4Box("pixSensor4",halfPixSensorSizeX,halfPixSensorSizeY,halfPixSensorSizeZ);
    
    G4LogicalVolume * logic_pix1_SensorPlane =
                                            new G4LogicalVolume(solid_pix1_Sensor_pix,  // its solid
                                                                detector_material,      // its material
                                                                "pix1_SensorPlane");    // its name
    
    G4LogicalVolume * logic_pix2_SensorPlane =
                                            new G4LogicalVolume(solid_pix2_Sensor_pix,  // its solid
                                                                detector_material,      // its material
                                                                "pix1_SensorPlane");    // its name
    
    G4LogicalVolume * logic_pix3_SensorPlane =
                                            new G4LogicalVolume(solid_pix3_Sensor_pix,  // its solid
                                                                detector_material,      // its material
                                                                "pix1_SensorPlane");    // its name
    
    G4LogicalVolume * logic_pix4_SensorPlane =
                                            new G4LogicalVolume(solid_pix4_Sensor_pix,  // its solid
                                                                detector_material,      // its material
                                                                "pix1_SensorPlane");    // its name
    
    if( build_pixel_detectors )
    {
        //******************************* FIRST PIXEL MODULE ********************************//
      // ****************** THE ROTATION HERE ONLY APPLY ON Z AXIS, SEE THE OTHER ROTATIONS IS [//] WHICH NOT ACTIVAITED  (AROUND THE BEAM ANGLE AND THAT WHAT WE NEED, NO NEED FOR OTHER ROTATION FOR THE PRIMARY DETECTORS)
        
        // 1st Plane of Si Beam Telescope
        if ( is_pix1_PlaneDUT )
        {
            G4cout << "Building Device Under Test setup: pix1 plane is replaced by DUT" << G4endl;
        
            //rm_pix1->rotateX(pix1_SensorTheta);  // Tilt detector in the beam
            //rm_pix1->rotateY(pix1_SensorTheta);  // Tilt detector in the beam
            rm_pix1->rotateZ(pix1_SensorTheta); // rotate around the beam axis for stereo angle
            
            physi_pix1_Sensor =
            new G4PVPlacement(rm_pix1,
                              pos_pix1_Sensor,
                              logic_pix1_SensorPlane,
                              "DeviceUnderTest_pix1",
                              logicPhantom, // If detector is inside phantom this must be the mother volume or hits disappear HERE YOU NEED CONSIDER THIS NOTES. HOWEVER, IF YOU WANT BUT THE SENSOR OUT THE PHANTOM E.G. BY 30 mm FROM THE EDGE. YOU NEED TO RELEASE IT FROM THE PHANTOM. THEREFORE THE CHANGE WILL BE FROM [ /*logicWorld*/logicPhantom, ] TO [ logicWorld, ]. THIS APPLY BELOW AS WELL
                              false,
                              0, true);   // Must increment for each new pixel detector
                
            
            // Build Row (like strips)
            G4Box * solid_pix1_SensorRow = new G4Box("pix1_SensorRow",
                                                      halfPixSensorPixelSizeX,halfPixSensorSizeY,halfPixSensorPixelSizeZ);
            
            G4LogicalVolume * logic_pix1_SensorRow = new G4LogicalVolume(solid_pix1_SensorRow,detector_material,"pix1_SensorRow");
            
            //Build Pixel
            G4Box * solid_pix1_SensorPixel =
                                            new G4Box("pix1_SensorPixel",
                                                      halfPixSensorPixelSizeX,halfPixSensorPixelSizeY,halfPixSensorPixelSizeZ);
            
            G4LogicalVolume * logic_pix1_SensorPixel =
                                                    new G4LogicalVolume(solid_pix1_SensorPixel,detector_material,"pix1_SensorPixel");

            // Replicate rows
            physi_pix1_SensorRow =
            new G4PVReplica("pix1_SensorRow",                       // its name
                            logic_pix1_SensorRow,                   // its logical volume
                            logic_pix1_SensorPlane,                 // its mother
                            kXAxis,                                 // axis of replication
                            (PixelsensorLength / telePixelPitch),   // number of replica
                            telePixelPitch);                        // width of replica
            
//            // Replicate pixels within rows
//            physi_pix1_SensorPixel =
//            new G4PVReplica("pix1_SensorPixel",                     // its name
//                            logic_pix1_SensorPixel,                 // its logical volume
//                            logic_pix1_SensorRow,                   // its mother
//                            kYAxis,                                 // axis of replication
//                            (PixelsensorLength / telePixelPitch),   // number of replica
//                            telePixelPitch);                        // width of replica
            
            logic_pix1_SensorPlane -> SetVisAttributes(new G4VisAttributes(yellow));
            logic_pix1_SensorRow -> SetVisAttributes(new G4VisAttributes(yellow));
            logic_pix1_SensorPixel -> SetVisAttributes(new G4VisAttributes(yellow));

        }
        else //Construct as plane of silicon without rows/pixels
        {
            //rm_pix1->rotateX(pix1_SensorTheta);  // Tilt detector in the beam
            //rm_pix1->rotateY(pix1_SensorTheta);  // Tilt detector in the beam
            rm_pix1->rotateZ(pix1_SensorTheta); // rotate around the beam axis for stereo angle
            
            physi_pix1_Sensor = new G4PVPlacement(rm_pix1,
                                                  pos_pix1_Sensor,
                                                  logicSensorPlane_pix,
                                                  "DeviceUnderTest_pix1",
                                                 logicPhantom,  // If detector is inside phantom this must be the mother volume // HERE ALSO YOU NEED TO FOLLOW THE ABOVE NOTES, IF YOU WANT CHANGE THE SENSOR POSITION
                                                  false,
                                                  0);   // Must increment for each new pixel detector
        }
    
        //******************************* SECOND PIXEL MODULE ********************************//
        
        // 2nd Plane of Si Beam Telescope
        if ( is_pix2_PlaneDUT )
        {
            G4cout << "Building Device Under Test setup: pix2 plane is replaced by DUT" << G4endl;
            
	    // rm_pix2->rotateX(pix2_SensorTheta);  // Tilt detector in the beam
            //rm_pix2->rotateY(pix2_SensorTheta);  // Tilt detector in the beam
            rm_pix2->rotateZ(pix2_SensorTheta); // rotate around the beam axis for stereo angle
            
	     rm_pix2->rotateZ(90*deg); // rotate 90 deg to make strips run other way // THE ROTATION HERE WITHIN Z AXIS BUT BY 90 DEGREES AND OLY FOR THE 2nd AND 4th DETECTORS. THIS ROTATION TO PROVIDE THE 2D DIRECTION (X AND Y). HERE THE ROTATION BY 90 DEGREES DUE TO IN ABOVE ALL THE SENSORS WERE SET IN IN 0 DEGREES ON THE Z AXIS.
            
            physi_pix2_Sensor =
            new G4PVPlacement(rm_pix2,
                              pos_pix2_Sensor,
                              logic_pix2_SensorPlane,
                              "DeviceUnderTest_pix2",
                              logicPhantom, // If detector is inside phantom this must be the mother volume or hits disappear
                              false,
                              1, true);   // Must increment for each new pixel detector
            
            
            // Build Row (like strips)
            G4Box * solid_pix2_SensorRow = new G4Box("pix2_SensorRow",
                                                     halfPixSensorPixelSizeX,halfPixSensorSizeY,halfPixSensorPixelSizeZ);
            
            G4LogicalVolume * logic_pix2_SensorRow = new G4LogicalVolume(solid_pix2_SensorRow,detector_material,"pix2_SensorRow");
            
            //Build Pixel
            G4Box * solid_pix2_SensorPixel =
            new G4Box("pix2_SensorPixel",
                      halfPixSensorPixelSizeX,halfPixSensorPixelSizeY,halfPixSensorPixelSizeZ);
            
            G4LogicalVolume * logic_pix2_SensorPixel =
            new G4LogicalVolume(solid_pix2_SensorPixel,detector_material,"pix2_SensorPixel");
            
            // Replicate rows
            physi_pix2_SensorRow =
            new G4PVReplica("pix2_SensorRow",                       // its name
                            logic_pix2_SensorRow,                   // its logical volume
                            logic_pix2_SensorPlane,                 // its mother
                            kXAxis,                                // axis of replication
                            (PixelsensorLength / telePixelPitch),   // number of replica
                            telePixelPitch);                        // width of replica
            
//            // Replicate pixels within rows
//            physi_pix2_SensorPixel =
//            new G4PVReplica("pix2_SensorPixel",                     // its name
//                            logic_pix2_SensorPixel,                 // its logical volume
//                            logic_pix2_SensorRow,                   // its mother
//                            kYAxis,                                 // axis of replication
//                            (PixelsensorLength / telePixelPitch),   // number of replica
//                            telePixelPitch);                        // width of replica
            
            logic_pix2_SensorPlane -> SetVisAttributes(new G4VisAttributes(green));
            logic_pix2_SensorRow -> SetVisAttributes(new G4VisAttributes(green));
            logic_pix2_SensorPixel -> SetVisAttributes(new G4VisAttributes(green));
            
        }
        else //Construct as plane of silicon without rows/pixels
        {
            //rm_pix2->rotateX(pix2_SensorTheta);  // Tilt detector in the beam
            //rm_pix2->rotateY(pix2_SensorTheta);  // Tilt detector in the beam
            rm_pix2->rotateZ(pix2_SensorTheta); // rotate around the beam axis for stereo angle
            
            physi_pix2_Sensor = new G4PVPlacement(rm_pix2,
                                                  pos_pix2_Sensor,
                                                  logicSensorPlane_pix,
                                                  "DeviceUnderTest_pix2",
						  logicPhantom, // If detector is inside phantom this must be the mother volume
                                                  false,
                                                  1);   // Must increment for each new pixel detector
        }

        
        
        //******************************* THIRD PIXEL MODULE ********************************//
        
        // 3rd Plane of Si Beam Telescope
        if ( is_pix3_PlaneDUT )
        {
            G4cout << "Building Device Under Test setup: pix3 plane is replaced by DUT" << G4endl;
            
	    // rm_pix3->rotateX(pix3_SensorTheta);  // Tilt detector in the beam
            //rm_pix3->rotateY(pix3_SensorTheta);  // Tilt detector in the beam
           rm_pix3->rotateZ(pix3_SensorTheta); // rotate around the beam axis for stereo angle
            
            physi_pix3_Sensor =
            new G4PVPlacement(rm_pix3,
                              pos_pix3_Sensor,
                              logic_pix3_SensorPlane,
                              "DeviceUnderTest_pix3",
                              logicPhantom, // If detector is inside phantom this must be the mother volume or hits disappear
                              false,
                              2, true);   // Must increment for each new pixel detector
            
            
            // Build Row (like strips)
            G4Box * solid_pix3_SensorRow = new G4Box("pix3_SensorRow",
                                                     halfPixSensorPixelSizeX,halfPixSensorSizeY,halfPixSensorPixelSizeZ);
            
            G4LogicalVolume * logic_pix3_SensorRow = new G4LogicalVolume(solid_pix3_SensorRow,detector_material,"pix3_SensorRow");
            
            //Build Pixel
            G4Box * solid_pix3_SensorPixel =
            new G4Box("pix3_SensorPixel",
                      halfPixSensorPixelSizeX,halfPixSensorPixelSizeY,halfPixSensorPixelSizeZ);
            
            G4LogicalVolume * logic_pix3_SensorPixel =
            new G4LogicalVolume(solid_pix3_SensorPixel,detector_material,"pix3_SensorPixel");
            
            // Replicate rows
            physi_pix3_SensorRow =
            new G4PVReplica("pix3_SensorRow",                       // its name
                            logic_pix3_SensorRow,                   // its logical volume
                            logic_pix3_SensorPlane,                 // its mother
                            kXAxis,                                 // axis of replication
                            (PixelsensorLength / telePixelPitch),   // number of replica
                            telePixelPitch);                        // width of replica
            
            //            // Replicate pixels within rows
            //            physi_pix3_SensorPixel =
            //            new G4PVReplica("pix3_SensorPixel",                     // its name
            //                            logic_pix3_SensorPixel,                 // its logical volume
            //                            logic_pix3_SensorRow,                   // its mother
            //                            kYAxis,                                 // axis of replication
            //                            (PixelsensorLength / telePixelPitch),   // number of replica
            //                            telePixelPitch);                        // width of replica
            
            logic_pix3_SensorPlane -> SetVisAttributes(new G4VisAttributes(blue));
            logic_pix3_SensorRow -> SetVisAttributes(new G4VisAttributes(blue));
            logic_pix3_SensorPixel -> SetVisAttributes(new G4VisAttributes(blue));
            
        }
        else //Construct as plane of silicon without rows/pixels
        {
            //rm_pix3->rotateX(pix3_SensorTheta);  // Tilt detector in the beam
            //rm_pix3->rotateY(pix3_SensorTheta);  // Tilt detector in the beam
            rm_pix3->rotateZ(pix3_SensorTheta); // rotate around the beam axis for stereo angle
            
            physi_pix3_Sensor = new G4PVPlacement(rm_pix3,
                                                  pos_pix3_Sensor,
                                                  logicSensorPlane_pix,
                                                  "DeviceUnderTest_pix3",
                                                  logicPhantom, // If detector is inside phantom this must be the mother volume
                                                  false,
                                                  2);   // Must increment for each new pixel detector
        }


        
        //******************************* FOURTH PIXEL MODULE ********************************//
        
        // 4th Plane of Si Beam Telescope
        if ( is_pix4_PlaneDUT )
        {
            G4cout << "Building Device Under Test setup: pix4 plane is replaced by DUT" << G4endl;
            
            //rm_pix4->rotateX(pix4_SensorTheta);  // Tilt detector in the beam
            //rm_pix4->rotateY(pix4_SensorTheta);  // Tilt detector in the beam
            rm_pix4->rotateZ(pix4_SensorTheta); // rotate around the beam axis for stereo angle
            
	     rm_pix4->rotateZ(90*deg); // rotate 90 deg to make strips run other way
            
            physi_pix4_Sensor =
            new G4PVPlacement(rm_pix4,
                              pos_pix4_Sensor,
                              logic_pix4_SensorPlane,
                              "DeviceUnderTest_pix4",
                              logicPhantom, // If detector is inside phantom this must be the mother volume or hits disappear
                              false,
                              3, true);   // Must increment for each new pixel detector
            
            
            // Build Row (like strips)
            G4Box * solid_pix4_SensorRow = new G4Box("pix4_SensorRow",
                                                     halfPixSensorPixelSizeX,halfPixSensorSizeY,halfPixSensorPixelSizeZ);
            
            G4LogicalVolume * logic_pix4_SensorRow = new G4LogicalVolume(solid_pix4_SensorRow,detector_material,"pix4_SensorRow");
            
            //Build Pixel
            G4Box * solid_pix4_SensorPixel =
            new G4Box("pix4_SensorPixel",
                      halfPixSensorPixelSizeX,halfPixSensorPixelSizeY,halfPixSensorPixelSizeZ);
            
            G4LogicalVolume * logic_pix4_SensorPixel =
            new G4LogicalVolume(solid_pix4_SensorPixel,detector_material,"pix4_SensorPixel");
            
            // Replicate rows
            physi_pix4_SensorRow =
            new G4PVReplica("pix4_SensorRow",                       // its name
                            logic_pix4_SensorRow,                   // its logical volume
                            logic_pix4_SensorPlane,                 // its mother
                            kXAxis,                                 // axis of replication
                            (PixelsensorLength / telePixelPitch),   // number of replica
                            telePixelPitch);                        // width of replica
            
            //            // Replicate pixels within rows
            //            physi_pix4_SensorPixel =
            //            new G4PVReplica("pix4_SensorPixel",                     // its name
            //                            logic_pix4_SensorPixel,                 // its logical volume
            //                            logic_pix4_SensorRow,                   // its mother
            //                            kYAxis,                                 // axis of replication
            //                            (PixelsensorLength / telePixelPitch),   // number of replica
            //                            telePixelPitch);                        // width of replica
            
            logic_pix4_SensorPlane -> SetVisAttributes(new G4VisAttributes(white));
            logic_pix4_SensorRow -> SetVisAttributes(new G4VisAttributes(white));
            logic_pix4_SensorPixel -> SetVisAttributes(new G4VisAttributes(white));
            
        }
        else //Construct as plane of silicon without rows/pixels
        {
            //rm_pix4->rotateX(pix4_SensorTheta);  // Tilt detector in the beam
            //rm_pix4->rotateY(pix4_SensorTheta);  // Tilt detector in the beam
            rm_pix4->rotateZ(pix4_SensorTheta); // rotate around the beam axis for stereo angle
            
            physi_pix4_Sensor = new G4PVPlacement(rm_pix4,
                                                  pos_pix4_Sensor,
                                                  logicSensorPlane_pix,
                                                  "DeviceUnderTest_pix4",
                                                  logicPhantom, // If detector is inside phantom this must be the mother volume
                                                  false,
                                                  3);   // Must increment for each new pixel detector
        }

        
        G4cout << "\nPixel tracker built\n" << G4endl;
        
    } // end of if( build_pixel_detectors )
    
    if(!build_pixel_detectors) {G4cout << "\nPixel tracker NOT built\n" << G4endl;}
    
    //------------------------------
	// Tracker (Strip detector(s))
	//------------------------------

    G4Box * solidSensor = new G4Box("Sensor",
                   halfSensorSizeX,halfSensorSizeY,halfSensorSizeZ);

    G4LogicalVolume * logicSensorPlane =
                    new G4LogicalVolume(solidSensor,        //its solid
                                        detector_material,	    //its material
                                        "SensorPlane");     //its name

    //Define rotations for each sensor x=0deg,u=60deg,v=-60deg
    //also set in macro files
    G4RotationMatrix * rm1 = new G4RotationMatrix;
    G4RotationMatrix * rm2 = new G4RotationMatrix;
    G4RotationMatrix * rm3 = new G4RotationMatrix;
    
    G4double halfSensorStripSizeX = teleStripPitch/2.;
    G4double halfSensorStripSizeY = sensorStripLength/2.;
    G4double halfSensorStripSizeZ = sensorThickness/2.;
    
    //Device under Test - replaces Plane of Si Beam Telescope
    G4Box * solid_x1_Sensor = new G4Box("x1_Sensor",halfSensorSizeX,halfSensorSizeY,halfSensorSizeZ);
    G4Box * solid_u1_Sensor = new G4Box("u1_Sensor",halfSensorSizeX,halfSensorSizeY,halfSensorSizeZ);
    G4Box * solid_v1_Sensor = new G4Box("v1_Sensor",halfSensorSizeX,halfSensorSizeY,halfSensorSizeZ);
    
    G4LogicalVolume * logic_x1_SensorPlane =
                    new G4LogicalVolume(solid_x1_Sensor,        //its solid
                                        detector_material,           //its material
                                        "x1_SensorPlane");      //its name
    
    G4LogicalVolume * logic_u1_SensorPlane =
                    new G4LogicalVolume(solid_u1_Sensor,        //its solid
                                        detector_material,           //its material
                                        "u1_SensorPlane");      //its name
    
    G4LogicalVolume * logic_v1_SensorPlane =
                    new G4LogicalVolume(solid_v1_Sensor,        //its solid
                                        detector_material,           //its material
                                        "v1_SensorPlane");      //its name
    
    if( build_strip_detectors )
    {
        //******************************* FIRST TRACKER MODULE ********************************//
        
        //1st Plane of Si Beam Telescope
        if ( is_x1_PlaneDUT )
        {
            G4cout << "Building Device Under Test setup: x1 plane is replaced by DUT" << G4endl;

            if ( is_x1_PlaneDUT )
            {
                //rm1->rotateY(x1_SensorTheta);
                //rm1->rotateX(x1_SensorTheta);
                rm1->rotateZ(x1_SensorTheta);   // rotate around the beam axis for stereo angle
                
                physi_x1_Sensor =
                new G4PVPlacement(rm1,
                                  pos_x1_Sensor,
                                  logic_x1_SensorPlane,
                                  "DeviceUnderTest_x1",
                                  /*logicWorld*/logicPhantom, // If detector is inside phantom this must be the mother volume or hits disappear
                                  false,
                                  0);
                
                //Build Strips
                G4Box * solid_x1_SensorStrip =
                new G4Box("x1_SensorStrip",
                          halfSensorStripSizeX,halfSensorStripSizeY,halfSensorStripSizeZ);
                
                G4LogicalVolume * logic_x1_SensorStrip =
                new G4LogicalVolume(solid_x1_SensorStrip,detector_material,"x1_SensorStrip");
                
                physi_x1_SensorStrip =
                new G4PVReplica("x1_SensorStrip",           // its name
                                logic_x1_SensorStrip,		// its logical volume
                                logic_x1_SensorPlane,		// its mother
                                kXAxis,                     // axis of replication
                                noOfSensorStrips,           // number of replica
                                teleStripPitch);            // width of replica
                
                logic_x1_SensorPlane -> SetVisAttributes(new G4VisAttributes(yellow));
                logic_x1_SensorStrip -> SetVisAttributes(new G4VisAttributes(red));
                
            }

        }
        else //Construct as plane of silicon without strips
        {
            //rm1->rotateY(x1_SensorTheta);
            //rm1->rotateX(x1_SensorTheta);
            rm1->rotateZ(x1_SensorTheta);   // rotate around the beam axis for stereo angle
            
            physi_x1_Sensor = new G4PVPlacement(rm1,	//rotation
                      pos_x1_Sensor,
                      logicSensorPlane,                 //its logical volume
                      "x1_Sensor",                      //its name
                      /*logicWorld*/logicPhantom,       //its mother volume
                      false,                            //no boolean operation
                      0);                               //copy number
        }

        
        //2nd Plane of Si Beam Telescope
        if ( is_u1_PlaneDUT )
        {
          G4cout << "Building Device Under Test setup: u1 plane is replaced by DUT" << G4endl;
            
            if ( is_u1_PlaneDUT )
            {
                //rm2->rotateY(u1_SensorTheta);
                //rm2->rotateX(u1_SensorTheta);
                rm2->rotateZ(u1_SensorTheta);   // rotate around the beam axis for stereo angle
                
                physi_u1_Sensor =
                new G4PVPlacement(rm2,
                                  pos_u1_Sensor,
                                  logic_u1_SensorPlane,
                                  "DeviceUnderTest_u1",
                                  /*logicWorld*/logicPhantom, // If detector is inside phantom this must be the mother volume or hits disappear
                                  false,
                                  1);
                
                //Build Strips
                G4Box * solid_u1_SensorStrip =
                new G4Box("u1_SensorStrip",
                          halfSensorStripSizeX,halfSensorStripSizeY,halfSensorStripSizeZ);
                
                G4LogicalVolume * logic_u1_SensorStrip =
                new G4LogicalVolume(solid_u1_SensorStrip,detector_material,"u1_SensorStrip");
                
                physi_u1_SensorStrip =
                new G4PVReplica("u1_SensorStrip",           // its name
                                logic_u1_SensorStrip,		// its logical volume
                                logic_u1_SensorPlane,		// its mother
                                kXAxis,                     // axis of replication
                                noOfSensorStrips,           // number of replica
                                teleStripPitch);            // width of replica
                
                logic_u1_SensorPlane -> SetVisAttributes(new G4VisAttributes(yellow));
                logic_u1_SensorStrip -> SetVisAttributes(new G4VisAttributes(red));
                
            }

        
        }
        else //Construct as plane of silicon without strips
        {
            //rm2->rotateY(u1_SensorTheta);
            //rm2->rotateX(u1_SensorTheta);
            rm2->rotateZ(u1_SensorTheta);   // rotate around the beam axis for stereo angle

            physi_u1_Sensor = new G4PVPlacement(rm2,
                              pos_u1_Sensor,
                              logicSensorPlane,
                              "u1_Sensor",
                              /*logicWorld*/logicPhantom, // If detector is inside phantom this must be the mother volume or hits disappear
                              false,
                              1);			//copy number
        }

        //3rd Plane of Si Beam Telescope
        if ( is_v1_PlaneDUT )
        {
            G4cout << "Building Device Under Test setup: v1 plane is replaced by DUT" << G4endl;
            
            if ( is_v1_PlaneDUT )
            {
                //rm3->rotateY(v1_SensorTheta);
                //rm3->rotateX(v1_SensorTheta);
                rm3->rotateZ(v1_SensorTheta);   // rotate around the beam axis for stereo angle
                
                physi_v1_Sensor =
                new G4PVPlacement(rm3,
                                  pos_v1_Sensor,
                                  logic_v1_SensorPlane,
                                  "DeviceUnderTest_v1",
                                  /*logicWorld*/logicPhantom, // If detector is inside phantom this must be the mother volume or hits disappear
                                  false,
                                  2);
                
                //Build Strips
                G4Box * solid_v1_SensorStrip =
                new G4Box("v1_SensorStrip",
                          halfSensorStripSizeX,halfSensorStripSizeY,halfSensorStripSizeZ);
                
                G4LogicalVolume * logic_v1_SensorStrip =
                new G4LogicalVolume(solid_v1_SensorStrip,detector_material,"v1_SensorStrip");
                
                physi_v1_SensorStrip =
                new G4PVReplica("v1_SensorStrip",           // its name
                                logic_v1_SensorStrip,		// its logical volume
                                logic_v1_SensorPlane,		// its mother
                                kXAxis,                     // axis of replication
                                noOfSensorStrips,           // number of replica
                                teleStripPitch);            // width of replica
                
                logic_v1_SensorPlane -> SetVisAttributes(new G4VisAttributes(yellow));
                logic_v1_SensorStrip -> SetVisAttributes(new G4VisAttributes(red));
                
            }
            
        }
        else //Construct as plane of silicon without strips
        {
            //rm3->rotateY(v1_SensorTheta);
            //rm3->rotateX(v1_SensorTheta);
            rm3->rotateZ(v1_SensorTheta);   // rotate around the beam axis for stereo angle
            
            physi_v1_Sensor = new G4PVPlacement(rm3,
                      pos_v1_Sensor,
                      logicSensorPlane,
                      "v1_Sensor",
                      /*logicWorld*/logicPhantom,  // If detector is inside phantom this must be the mother volume or hits disappear
                      false,
                      2);			//copy number

        }
        
        if(build_strip_detectors) {G4cout << "\nStrip tracker built\n" << G4endl;}
        
    } // end of if( build_strip_detectors )
    
    if(!build_strip_detectors) {G4cout << "\nStrip tracker NOT built\n" << G4endl;}
    
    //***********************************************************************************//

	// ----------------------------------------------------------
	// -- Binding SensitiveDetector code to sensor strip volume:
	// -- Note that it is possible to set the sensitive detector
	// -- at construction of the G4LogicalVolume (fith argument
	// -- is a G4VSensitiveDetector*).
	// ----------------------------------------------------------
	// Every time the /det/update command is executed this
	// method is called since geometry is recomputed.
	// However we do not need to create a new SD, but reuse the
	// already existing one
    
    static SensitiveDetector_pix * sensitive_det_pix1 = 0;
    static SensitiveDetector_pix * sensitive_det_pix2 = 0;
    static SensitiveDetector_pix * sensitive_det_pix3 = 0;
    static SensitiveDetector_pix * sensitive_det_pix4 = 0;
    
	static SensitiveDetector* sensitive_det_x1 = 0;
	static SensitiveDetector* sensitive_det_u1 = 0;
	static SensitiveDetector* sensitive_det_v1 = 0;
    
    G4SDManager* SDman = G4SDManager::GetSDMpointer();

    //if( build_pixel_detectors)
    //{
        if ( !sensitive_det_pix1 )
        {
            sensitive_det_pix1 = new SensitiveDetector_pix("SiPixel_pix1");
            //We register now the SD with the manager
            SDman->AddNewDetector(sensitive_det_pix1);
        }
        
        if ( !sensitive_det_pix2 )
        {
            sensitive_det_pix2 = new SensitiveDetector_pix("SiPixel_pix2");
            //We register now the SD with the manager
            SDman->AddNewDetector(sensitive_det_pix2);
        }
    
    if ( !sensitive_det_pix3 )
    {
        sensitive_det_pix3 = new SensitiveDetector_pix("SiPixel_pix3");
        //We register now the SD with the manager
        SDman->AddNewDetector(sensitive_det_pix3);
    }
    
    if ( !sensitive_det_pix4 )
    {
        sensitive_det_pix4 = new SensitiveDetector_pix("SiPixel_pix4");
        //We register now the SD with the manager
        SDman->AddNewDetector(sensitive_det_pix4);
    }
    //}
    
    //if( build_strip_detectors)
    //{
        if ( !sensitive_det_x1 )
        {
            sensitive_det_x1 = new SensitiveDetector("SiStrip_x1");
            //We register now the SD with the manager
            SDman->AddNewDetector(sensitive_det_x1);
        }
        
       
        if ( !sensitive_det_u1 )
        {
            sensitive_det_u1 = new SensitiveDetector("SiStrip_u1");
            //We register now the SD with the manager
            SDman->AddNewDetector(sensitive_det_u1);
        }
        
        
        if ( !sensitive_det_v1 )
        {
            sensitive_det_v1 = new SensitiveDetector("SiStrip_v1");
            //We register now the SD with the manager
            SDman->AddNewDetector(sensitive_det_v1);
        }
    //}
    
	// This part here is used to associated the sensitive detector to the
	// logical volume of a plane in case it is a DUT.
	
    // With DUT we need to attach to the logical volume of the strips the
    // sensitive detector, since we only have a pointer to the physical volume
    // of the plane we need to...
    
    // 1- First get the logical volume associated to a plane
    
    // 2- Get the first daughter associated to it: the strip
    // since strip is a replicated volume (G4PVReplica) there is
    // a single G4PhysicalVolume representing all replicas.
    // We actually need the LogicalVolume.
    // Thus we ask the first daughter ::GetDaughter(0) and ask the
    // associated logical volume
    
    // if DUT is turned off ie for truth plane, then sensitive detector is
    // just defined as a plane with no strips (see else statement) position
    // and energy of events is still read out but stip variables will be empty or 0.
    
    G4cout << "\nAttempting to find sensitive detectors for pixels...\n" << G4endl;
    
    if( build_pixel_detectors )
    {
        if ( is_pix1_PlaneDUT )
        {
            const G4LogicalVolume* log = physi_pix1_Sensor->GetLogicalVolume();
            log->GetDaughter(0)->GetLogicalVolume()->SetSensitiveDetector(sensitive_det_pix1);
        }
        else
        {
            G4LogicalVolume* log = physi_pix1_Sensor->GetLogicalVolume();
            log->SetSensitiveDetector(sensitive_det_pix1);
        }
        
        if ( is_pix2_PlaneDUT )
        {
            const G4LogicalVolume* log = physi_pix2_Sensor->GetLogicalVolume();
            log->GetDaughter(0)->GetLogicalVolume()->SetSensitiveDetector(sensitive_det_pix2);
        }
        else
        {
            G4LogicalVolume* log = physi_pix2_Sensor->GetLogicalVolume();
            log->SetSensitiveDetector(sensitive_det_pix2);
        }
        
        if ( is_pix3_PlaneDUT )
        {
            const G4LogicalVolume* log = physi_pix3_Sensor->GetLogicalVolume();
            log->GetDaughter(0)->GetLogicalVolume()->SetSensitiveDetector(sensitive_det_pix3);
        }
        else
        {
            G4LogicalVolume* log = physi_pix3_Sensor->GetLogicalVolume();
            log->SetSensitiveDetector(sensitive_det_pix3);
        }
        
        if ( is_pix4_PlaneDUT )
        {
            const G4LogicalVolume* log = physi_pix4_Sensor->GetLogicalVolume();
            log->GetDaughter(0)->GetLogicalVolume()->SetSensitiveDetector(sensitive_det_pix4);
        }
        else
        {
            G4LogicalVolume* log = physi_pix4_Sensor->GetLogicalVolume();
            log->SetSensitiveDetector(sensitive_det_pix4);
        }
    }
    
    G4cout << "\nFinished Attempting to find sensitive detectors for pixels...\n" << G4endl;
    
    G4cout << "\nAttempting to find sensitive detectors for strips...\n" << G4endl;
    if( build_strip_detectors )
    {
        if ( is_x1_PlaneDUT )
        {
            const G4LogicalVolume* log = physi_x1_Sensor->GetLogicalVolume();
            log->GetDaughter(0)->GetLogicalVolume()->SetSensitiveDetector(sensitive_det_x1);
        }
        else
        {
            G4LogicalVolume* log = physi_x1_Sensor->GetLogicalVolume();
            log->SetSensitiveDetector(sensitive_det_x1);
        }
        
        if ( is_u1_PlaneDUT )
        {
            const G4LogicalVolume* log = physi_u1_Sensor->GetLogicalVolume();
            log->GetDaughter(0)->GetLogicalVolume()->SetSensitiveDetector(sensitive_det_u1);
        }
        else
        {
            G4LogicalVolume* log = physi_u1_Sensor->GetLogicalVolume();
            log->SetSensitiveDetector(sensitive_det_u1);
        }
        
        if ( is_v1_PlaneDUT )
        {
            const G4LogicalVolume* log = physi_v1_Sensor->GetLogicalVolume();
            log->GetDaughter(0)->GetLogicalVolume()->SetSensitiveDetector(sensitive_det_v1);
        }
        else
        {
            G4LogicalVolume* log = physi_v1_Sensor->GetLogicalVolume();
            log->SetSensitiveDetector(sensitive_det_v1);
        }
    }
    
    G4cout << "\nFinished Attempting to find sensitive detectors for strips...\n" << G4endl;
    
    //--------- Visualization attributes -------------------------------
    
	logicWorld->SetVisAttributes(new G4VisAttributes(white));
	//logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
    G4VisAttributes * PhantomVisAtt = new G4VisAttributes(white);
    PhantomVisAtt->SetForceWireframe(true); // Set the forced wireframe style
    logicPhantom->SetVisAttributes(PhantomVisAtt);

    //******************Shielding changes****************
    G4VisAttributes * ShieldVisAtt = new G4VisAttributes(blue);
    ShieldVisAtt->SetForceWireframe(true); // Set the forced wireframe style
    logicShield->SetVisAttributes(ShieldVisAtt);
    
    //****************************End*******************
    //******************************************************************************************************************************************
   G4VisAttributes * FilmVisAtt = new G4VisAttributes(white);
    FilmVisAtt->SetForceSolid(true); // Set the forced wireframe style
    logicFilm->SetVisAttributes(FilmVisAtt);

  G4VisAttributes * Contact1VisAtt = new G4VisAttributes(red);
    Contact1VisAtt->SetForceSolid(true); // Set the forced wireframe style
    logicContact1->SetVisAttributes(Contact1VisAtt);

  G4VisAttributes * Contact2VisAtt = new G4VisAttributes(red);
    Contact2VisAtt->SetForceSolid(true); // Set the forced wireframe style
    logicContact2->SetVisAttributes(Contact2VisAtt);
    //*****************************************************************************************************************************************
    
    //G4double minStep = 0.2*um; //Smaller steps results in better energy loss values (lower fluctuation)
    //G4double minTrack = 0.2*um;
    //G4UserLimits* stepLimitMin = new G4UserLimits(minStep);
    
    //Available arguements for G4UserLimits()
    /*G4UserLimits(G4double uStepMax = DBL_MAX, G4double uTrakMax = DBL_MAX, G4double uTimeMax = DBL_MAX, G4double uEkinMin = 0.,G4double uRangMin = 0. );*/
    
    
    //Doesn't seem to work at the moment, same cut value of 10um everywhere which is set in PhysicsList.cc
    //need to alter physics list and put into messenger, although assigning specific range cuts to a region
    //is not recommended unless geometry is complex see here:
    //http://geant4.cern.ch/G4UsersDocuments/UsersGuides/ForApplicationDeveloper/html/TrackingAndPhysics/cutsPerRegion.html
    
    G4double uStepMin = 0.2*um;
    G4double uTrakMin = 0.2*um;
    G4UserLimits* stepLimitMin = new G4UserLimits(uStepMin,uTrakMin);
    
    logic_pix1_SensorPlane->SetUserLimits(stepLimitMin);
    logic_pix2_SensorPlane->SetUserLimits(stepLimitMin);
    logic_pix3_SensorPlane->SetUserLimits(stepLimitMin);
    logic_pix4_SensorPlane->SetUserLimits(stepLimitMin);
    
    logic_x1_SensorPlane->SetUserLimits(stepLimitMin);
    logic_u1_SensorPlane->SetUserLimits(stepLimitMin);
    logic_v1_SensorPlane->SetUserLimits(stepLimitMin);
    
    //G4double maxStep = 200*um; //Smaller steps results in better energy loss values (lower fluctuation)
    //G4UserLimits* stepLimitMax = new G4UserLimits(maxStep);

    G4double uStepMax = 200*um;
    G4double uTrakMax = 200*um;
    G4UserLimits* stepLimitMax = new G4UserLimits(uStepMax,uTrakMax);
    
    logicWorld->SetUserLimits(stepLimitMax);
    logicPhantom->SetUserLimits(stepLimitMax);
    logicShield->SetUserLimits(stepLimitMax);//********************
    //**********************************************************************
    logicFilm->SetUserLimits(stepLimitMax);

    logicContact1->SetUserLimits(stepLimitMax);

    logicContact2->SetUserLimits(stepLimitMax);



    //**********************************************************************
    
	//always return the physical World
	return physiWorld;
}


#include "G4RunManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

// This function is called from the messenger to compute
// and build all geometry changes
void DetectorConstruction::UpdateGeometry()
{
    //G4cout << "\nAbout to rebuild geometry\n" << G4endl;
    
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}
