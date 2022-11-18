
#include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4Run.hh"
#include "DetectorConstruction.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4GeneralParticleSource.hh"

#include <string>

RunAction::RunAction(  /*G4VUser*/PrimaryGeneratorAction * thePGAction, EventAction* theEventAction, DetectorConstruction* myDC ) :
	primGenAction(thePGAction), eventAction(theEventAction), myDetector(myDC)
{
	eventAction->SetRootSaver( &saver );
}

void RunAction::BeginOfRunAction(const G4Run* aRun )
{
    
	G4cout << "Starting Run: " << aRun->GetRunID() << G4endl;
	// For each run a new TTree is created, with default names
    
    // Info on tracker geom now written once per run in the EndOfRunAction() function below
    if( myDetector->Get_build_strip_detectors() )
    {
        std::ostringstream fn;
        float z_pos = myDetector->Get_zShift_pixel_tracker();
        fn << "strip_tree" << "_" << z_pos << "mm_depth" ;
        saver.CreateTree_strip_det(fn.str(),"trackerData_strip", myDetector->Get_nb_of_strips());
    }
    
    // unique ID for filename based on system clock, combined with depth info and run no. also
    // written to filename should prevents any overwrites when running in batch mode
    time_t systime = time(NULL);
    
    // Info on tracker geom now written once per run in the EndOfRunAction() function below
    if( myDetector->Get_build_pixel_detectors() )
    {
        std::ostringstream fn;
        float z_pos = myDetector->Get_zShift_pixel_tracker();
        fn << "pixel_tree_" << z_pos << "mm_depth_uid_" << systime;
        saver.CreateTree_pixel_det(fn.str(),"trackerData_pixel", myDetector->Get_nb_of_pixels());
    }
    
    //Print detector rotations to terminal
    /*G4cout << "\nCreating tree with: " << myDetector->Get_nb_of_planes() << " detectors" << G4endl;
    G4cout << "Each detector contains: " << myDetector->Get_nb_of_strips() << " strips\n" << G4endl;
    
    G4cout << "x1 detector at an angle of: " << myDetector->Get_x1_SensorAngle()/deg << " deg" << G4endl;
    G4cout << "u1 detector at an angle of: " << myDetector->Get_u1_SensorAngle()/deg << " deg" << G4endl;
    G4cout << "v1 detector at an angle of: " << myDetector->Get_v1_SensorAngle()/deg << " deg" << G4endl;
     */
    
}

void RunAction::EndOfRunAction( const G4Run* aRun )
{
    if( myDetector->Get_build_strip_detectors() )
    {
        // Geometry info saved to file for tracking could be done by creating a TrackerGeometry/DetectorConstruction
        // object and saving to root tree, however this requires extra libraries dict.h/dict.cxx and linkdef.h to be
        // generated and the makefile altered (see hadron_phantom program).
        // Instead we re-save each of the necessary DetectorConstruction parameters to to a separate TTree: trackerGeom
        // this is saved in the same root file as the TTree for the data from each event (trackerData)
        
        TTree * geomTree = new TTree( "trackerGeom" , "trackerGeom" );
        
        // pass info on parameters used to position all geometry to root file for writing
        // these parameters are used for tracking in external scripts.
        // variables must be declared as branches don't seem to like being given myDetector->Get...
        
        G4int noOfSensorStrips = myDetector->Get_nb_of_strips();
        G4double teleStripPitch = myDetector->Get_strip_pitch();
        G4double sensorStripLength = myDetector->Get_strip_length();
        G4double sensorThickness = myDetector->Get_sensor_thickness();
        
        G4double inter_plane_dist = myDetector->Get_inter_plane_dist();
        G4double inter_module_dist = myDetector->Get_inter_module_dist();
        G4double phantom_gap = myDetector->Get_phantom_gap();
        G4double phantom_zShift = myDetector->Get_PhantomPosition().getZ();
	G4double shield_zShift = myDetector->Get_ShieldPosition().getZ();//**************************
        G4double halfPhantomSizeZ = myDetector->Get_halfPhantomSizeZ(); 

	G4double halfShieldSizeZ = myDetector->Get_halfShieldSizeZ();//****************
	//*********************************************************************************************************************
  G4double film_gap = myDetector->Get_film_gap();
        G4double film_zShift = myDetector->Get_FilmPosition().getZ();
        
        G4double halfFilmSizeZ = myDetector->Get_halfFilmSizeZ(); 
	//**************************************************

	  G4double contact1_gap = myDetector->Get_contact1_gap();
        G4double contact1_zShift = myDetector->Get_Contact1Position().getZ();
        
        G4double halfContact1SizeZ = myDetector->Get_halfContact1SizeZ();

	//********************************************************
 G4double contact2_gap = myDetector->Get_contact2_gap();
        G4double contact2_zShift = myDetector->Get_Contact2Position().getZ();
        
        G4double halfContact2SizeZ = myDetector->Get_halfContact2SizeZ();
	//*********************************************************************************************************************   
        
        // Here we get the x/y positions and stereo angles of each detector plane. GEANT4 adds any offsets that are
        // present in .mac files to the original positions and stereo angles. We subtract those offsets in order to store
        // separately the values and their offsets so that the tracking routines can track with or without offsets present
        G4double x1_SensorPos_x = myDetector->Get_x1_SensorPosition().getX() - myDetector->Get_x1_SensorPositionOffset().getX();
        G4double u1_SensorPos_x = myDetector->Get_u1_SensorPosition().getX() - myDetector->Get_u1_SensorPositionOffset().getX();
        G4double v1_SensorPos_x = myDetector->Get_v1_SensorPosition().getX() - myDetector->Get_v1_SensorPositionOffset().getX();

        G4double x1_SensorPos_y = myDetector->Get_x1_SensorPosition().getY() - myDetector->Get_x1_SensorPositionOffset().getY();
        G4double u1_SensorPos_y = myDetector->Get_u1_SensorPosition().getY() - myDetector->Get_u1_SensorPositionOffset().getY();
        G4double v1_SensorPos_y = myDetector->Get_v1_SensorPosition().getY() - myDetector->Get_v1_SensorPositionOffset().getY();
       
        G4double x1_SensorAngle = myDetector->Get_x1_SensorAngle() - myDetector->Get_x1_SensorAngleOffset();
        G4double u1_SensorAngle = myDetector->Get_u1_SensorAngle() - myDetector->Get_u1_SensorAngleOffset();
        G4double v1_SensorAngle = myDetector->Get_v1_SensorAngle() - myDetector->Get_v1_SensorAngleOffset();
        
        // Get stereo angle offsets (set by user in macro), kept separate from stereo angle so tracking can
        // be carried out with/without knowing the offset to see effect. stereo angle of sensor calculated
        // by GEANT4 by adding stereo angle to stereo angle offset.
        G4double x1_SensorAngleOffset = myDetector->Get_x1_SensorAngleOffset();
        G4double u1_SensorAngleOffset = myDetector->Get_u1_SensorAngleOffset();
        G4double v1_SensorAngleOffset = myDetector->Get_v1_SensorAngleOffset();
        
        // Get position offsets (set by user in macro), kept separate from position vectors so tracking can
        // be carried out with/without knowing the offset to see effect. position of sensor centre calculated
        // by GEANT4 by adding position vector to offset vector.
        G4double x1_SensorPosOffset_x = myDetector->Get_x1_SensorPositionOffset().getX();
        G4double u1_SensorPosOffset_x = myDetector->Get_u1_SensorPositionOffset().getX();
        G4double v1_SensorPosOffset_x = myDetector->Get_v1_SensorPositionOffset().getX();
        
        G4double x1_SensorPosOffset_y = myDetector->Get_x1_SensorPositionOffset().getY();
        G4double u1_SensorPosOffset_y = myDetector->Get_u1_SensorPositionOffset().getY();
        G4double v1_SensorPosOffset_y = myDetector->Get_v1_SensorPositionOffset().getY();
        
        // Material variables must be TString otherwise won't work
        TString worldMaterial = myDetector->GetWorldMaterial()->GetName();
        TString phantomMaterial = myDetector->GetPhantomMaterial()->GetName();
	//*******************************************************************************
        TString filmMaterial = myDetector->GetFilmMaterial()->GetName();

	 TString contact1Material = myDetector->GetContact1Material()->GetName();
	 TString contact2Material = myDetector->GetContact2Material()->GetName();
	 TString shieldMaterial = myDetector->GetShieldMaterial()->GetName(); //*****************


	//******************************************************************************
        TString detectorMaterial = myDetector->GetDetectorMaterial()->GetName();
        
        // Set branches in trackerGeom tree
        geomTree->Branch( "noOfSensorStrips" , &noOfSensorStrips );
        geomTree->Branch( "teleStripPitch" , &teleStripPitch );
        geomTree->Branch( "sensorStripLength" , &sensorStripLength );
        geomTree->Branch( "sensorThickness" , &sensorThickness );
        
        geomTree->Branch( "inter_plane_dist" , &inter_plane_dist );
        geomTree->Branch( "inter_module_dist" , &inter_module_dist );
        geomTree->Branch( "phantom_gap" , &phantom_gap );
        geomTree->Branch( "phantom_zShift" , &phantom_zShift );
	geomTree->Branch( "shield_zShift" , &shield_zShift );//*****************************
	geomTree->Branch( "halfShieldSizeZ" , &halfShieldSizeZ );//***************************        
        geomTree->Branch( "halfPhantomSizeZ" , &halfPhantomSizeZ );
	//********************************************************************************************
 geomTree->Branch( "film_gap" , &film_gap );
        geomTree->Branch( "film_zShift" , &film_zShift );
        
        geomTree->Branch( "halfFilmSizeZ" , &halfFilmSizeZ );
	//************************************
	geomTree->Branch( "contact1_gap" , &contact1_gap );
        geomTree->Branch( "contact1_zShift" , &contact1_zShift );
        
        geomTree->Branch( "halfContact1SizeZ" , &halfContact1SizeZ );
	//************************************
	geomTree->Branch( "contact2_gap" , &contact2_gap );
        geomTree->Branch( "contact2_zShift" , &contact2_zShift );
        
        geomTree->Branch( "halfContact2SizeZ" , &halfContact2SizeZ );
	//********************************************************************************************

        geomTree->Branch( "x1_SensorPos_x" , &x1_SensorPos_x );
        geomTree->Branch( "u1_SensorPos_x" , &u1_SensorPos_x );
        geomTree->Branch( "v1_SensorPos_x" , &v1_SensorPos_x );
        
        geomTree->Branch( "x1_SensorPos_y" , &x1_SensorPos_y );
        geomTree->Branch( "u1_SensorPos_y" , &u1_SensorPos_y );
        geomTree->Branch( "v1_SensorPos_y" , &v1_SensorPos_y );
       
        geomTree->Branch( "x1_SensorAngle" , &x1_SensorAngle );
        geomTree->Branch( "u1_SensorAngle" , &u1_SensorAngle );
        geomTree->Branch( "v1_SensorAngle" , &v1_SensorAngle );
        
        geomTree->Branch( "x1_SensorAngleOffset" , &x1_SensorAngleOffset );
        geomTree->Branch( "u1_SensorAngleOffset" , &u1_SensorAngleOffset );
        geomTree->Branch( "v1_SensorAngleOffset" , &v1_SensorAngleOffset );

        geomTree->Branch( "x1_SensorPosOffset_x" , &x1_SensorPosOffset_x );
        geomTree->Branch( "u1_SensorPosOffset_x" , &u1_SensorPosOffset_x );
        geomTree->Branch( "v1_SensorPosOffset_x" , &v1_SensorPosOffset_x );
        
        geomTree->Branch( "x1_SensorPosOffset_y" , &x1_SensorPosOffset_y );
        geomTree->Branch( "u1_SensorPosOffset_y" , &u1_SensorPosOffset_y );
        geomTree->Branch( "v1_SensorPosOffset_y" , &v1_SensorPosOffset_y );
        
        geomTree->Branch( "worldMaterial" , &worldMaterial );
        geomTree->Branch( "phantomMaterial" , &phantomMaterial );
	//************************************************************************************
        geomTree->Branch( "filmMaterial" , &filmMaterial );

	 geomTree->Branch( "contact1Material" , &contact1Material );

	 geomTree->Branch( "contact2Material" , &contact2Material );
	 geomTree->Branch( "shieldMaterial" , &shieldMaterial );//*********************
	//***********************************************************************************
        geomTree->Branch( "detectorMaterial" , &detectorMaterial );
        
        geomTree->Fill();
        //geomTree->Print();
        geomTree->Write();
        
        G4cout << "\nWriting ROOT TTree: " << geomTree->GetName() << G4endl;
        
        geomTree = 0;
    
    } // end of if( myDetector->Get_build_strip_detectors() )
    
    
    // Closes tree containing data from each event in the run
    // and geometry variables defined above
    
    saver.CloseTrees();
    G4cout << "Ending Run: " << aRun->GetRunID() << G4endl;
    // TTree are closed, with default names      
}
