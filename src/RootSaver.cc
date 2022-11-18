
#include "RootSaver.hh"
#include "SiDigi.hh"
#include "SiHit.hh"
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include <sstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <TVector3.h>

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


RootSaver::RootSaver() :

    // Initialise non stl general variables
    rootTree_strip(0),
    rootTree_pixel(0),
    runCounter(0),
    nStrips(0),
    nPixels(0),
    Event_no(0),

    // Initialise non stl truth variables
    Truth_x_pos(0),
    Truth_y_pos(0),
    Truth_z_pos(0),
    TruthTheta_x(0),
    TruthTheta_y(0),
    KE_in(0),
    //KE_out(0)

    // Initialise non stl strip variables
    Hit_mult_x1(0),
    Hit_mult_u1(0),
    Hit_mult_v1(0),
    Signal_x1(0),
    Signal_u1(0),
    Signal_v1(0),
    ClusterSize_x1(0),
    ClusterSize_u1(0),
    ClusterSize_v1(0),
    Det_mult(0),

    // Initialise non stl pixel variables
    Hit_mult_pix1(0),
    Hit_mult_pix2(0),
    Hit_mult_pix3(0),
    Hit_mult_pix4(0),
    Signal_pix1(0),
    Signal_pix2(0),
    Signal_pix3(0),
    Signal_pix4(0)
{
}

RootSaver::~RootSaver()
{
	//Close current file if needed
	if ( rootTree_strip || rootTree_pixel ) {CloseTrees();}
}

void RootSaver::CreateTree_strip_det( const std::string& fileName , const std::string& treeName, const int n_strips)
{
	if ( rootTree_strip )
	{
		std::cerr << "TTree already created, first call CloseTree" << std::endl;
		return;
	}
	std::ostringstream fn;
	fn << fileName << "_run" << runCounter++ << ".root";
	// Create a new file and open it for writing,
    // if the file already exists the file is overwritten
	// TFile* rootFile = TFile::Open( fn.str().data() , "recreate" );
    rootFile = TFile::Open( fn.str().data() , "recreate" );             // TFile variable now included in .hh file
	if ( rootFile == 0 || rootFile->IsZombie() )
	{
		G4cerr << "Error opening the file: " << fn.str() << " TTree will not be saved." << G4endl;
		return;
	}
	rootTree_strip = new TTree( treeName.data() , treeName.data() );
	nStrips = n_strips;  // used to set size of strip signal arrays
    
	Signal_x1 = new Float_t[nStrips];
	Signal_u1 = new Float_t[nStrips];
	Signal_v1 = new Float_t[nStrips];
    
	for ( Int_t strip = 0 ; strip < nStrips ; ++strip )
	{
		Signal_x1[strip] = 0;
		Signal_u1[strip] = 0;
		Signal_v1[strip] = 0;
	}
    
    // Variables that are part of the raw data e.g. energies and positions
    // are recorded to the tree. Other data that are used to identify particles
    // in the hit ie isPrimary and ParticleName are also recorded and are used for
    // example in the print_event_data_for_plane() function in the tracking routine.
    // all other variables are commented out for now in order to reduce the output
    // root file size for the large no. of events needed for all 180 angles. Additionally,
    // many of the commented out variables should only be produced when a threshold in
    // energy has been applied e.g. cluster_width and strip_number so are reconstructed in the tracking.
    // Energy info (signal and edep) for truth planes is not read out as they are now used
    // as single planes with just their position info used for calculating uncertainties
    // in tracking (whether the DUT flag is set in the .mac file or not).
    
    // Variables commented out here are still calculated in the AddEvent() function below.
    // This allows them to be added back into the output tree quickly if needed. If performance
    // is an issue the calculation of the variables could be commented out too.
    
    char branch[50];
    
    // Event variables
    rootTree_strip->Branch( "event_no" , &Event_no );
    rootTree_strip->Branch( "ke_in" , &KE_in );
    
    rootTree_strip->Branch( "truth_x_pos" , &Truth_x_pos );
	rootTree_strip->Branch( "truth_y_pos" , &Truth_y_pos );
	rootTree_strip->Branch( "truth_z_pos" , &Truth_z_pos );
	rootTree_strip->Branch( "truthTheta_x" , &TruthTheta_x );
	rootTree_strip->Branch( "truthTheta_y" , &TruthTheta_y );

//    rootTree_strip->Branch( "det_mult" , &Det_mult );
    
    rootTree_strip->Branch( "hit_mult_x1" , &Hit_mult_x1 );
    rootTree_strip->Branch( "hit_mult_u1" , &Hit_mult_u1 );
    rootTree_strip->Branch( "hit_mult_v1" , &Hit_mult_v1 );
    
    rootTree_strip->Branch( "clusterSize_x1" , &ClusterSize_x1 );
    rootTree_strip->Branch( "clusterSize_u1" , &ClusterSize_u1 );
    rootTree_strip->Branch( "clusterSize_v1" , &ClusterSize_v1 );
    
	// Digit variables
    sprintf(branch, "signal_x1[%i]/F", nStrips);
    rootTree_strip->Branch( "signal_x1", Signal_x1 , branch );
    sprintf(branch, "signal_u1[%i]/F", nStrips);
	rootTree_strip->Branch( "signal_u1", Signal_u1 , branch );
    sprintf(branch, "signal_v1[%i]/F", nStrips);
	rootTree_strip->Branch( "signal_v1", Signal_v1 , branch );
    
	// Hit variables
    rootTree_strip->Branch( "ni_edep_x1" , &NI_Edep_x1 ); // write non-ionising energy loss for x1 plane only (use for dose calculations)
    
    rootTree_strip->Branch( "edep_x1" , &Edep_x1 );
	rootTree_strip->Branch( "edep_u1" , &Edep_u1 );
	rootTree_strip->Branch( "edep_v1" , &Edep_v1 );
    
    rootTree_strip->Branch( "isPrimaryParticle_x1" , &IsPrimaryParticle_x1 );
    rootTree_strip->Branch( "isPrimaryParticle_u1" , &IsPrimaryParticle_u1 );
    rootTree_strip->Branch( "isPrimaryParticle_v1" , &IsPrimaryParticle_v1 );
    
    rootTree_strip->Branch( "trackNumber_x1" , &Track_no_x1 );
    rootTree_strip->Branch( "trackNumber_u1" , &Track_no_u1 );
    rootTree_strip->Branch( "trackNumber_v1" , &Track_no_v1 );
    
    rootTree_strip->Branch( "particleName_x1" , &ParticleName_x1 );
    rootTree_strip->Branch( "particleName_u1" , &ParticleName_u1 );
    rootTree_strip->Branch( "particleName_v1" , &ParticleName_v1 );
    
    rootTree_strip->Branch( "x_pos_x1" , &X_pos_x1 );
	rootTree_strip->Branch( "x_pos_u1" , &X_pos_u1 );
	rootTree_strip->Branch( "x_pos_v1" , &X_pos_v1 );
    
    rootTree_strip->Branch( "y_pos_x1" , &Y_pos_x1 );
	rootTree_strip->Branch( "y_pos_u1" , &Y_pos_u1 );
	rootTree_strip->Branch( "y_pos_v1" , &Y_pos_v1 );
    
    rootTree_strip->Branch( "z_pos_x1" , &Z_pos_x1 );
	rootTree_strip->Branch( "z_pos_u1" , &Z_pos_u1 );
	rootTree_strip->Branch( "z_pos_v1" , &Z_pos_v1 );
}

void RootSaver::CreateTree_pixel_det( const std::string& fileName , const std::string& treeName, const int n_pixels)
{
    if ( rootTree_pixel )
    {
        std::cerr << "TTree already created, first call CloseTree" << std::endl;
        return;
    }
    std::ostringstream fn;
    fn << fileName << "_run" << runCounter++ << ".root";
    // If file doesn't exist, create a new file and open it for writing,
    // if the file already exists because rootTree_strip has opened it then simply
    // add the pixel tree in to the same file. If the file exists from a previous run
    // then it will be overwritten.
    if( rootTree_strip ) { rootFile = TFile::Open( fn.str().data() , "UPDATE" ); }
    if( !rootTree_strip ){ rootFile = TFile::Open( fn.str().data() , "RECREATE" ); }
    
    if ( rootFile == 0 || rootFile->IsZombie() )
    {
        G4cerr << "Error opening the file: " << fn.str() << " TTree will not be saved." << G4endl;
        return;
    }
    rootTree_pixel = new TTree( treeName.data() , treeName.data() );
    nPixels = n_pixels;  // used to set size of strip signal arrays

    Signal_pix1 = new Float_t[nPixels];
    Signal_pix2 = new Float_t[nPixels];
    Signal_pix3 = new Float_t[nPixels];
    Signal_pix4 = new Float_t[nPixels];

    
    for ( Int_t pixel = 0 ; pixel < nPixels ; ++pixel )
    {
        Signal_pix1[pixel] = 0;
        Signal_pix2[pixel] = 0;
        Signal_pix3[pixel] = 0;
        Signal_pix4[pixel] = 0;
    }
    
    
    // Variables that are part of the raw data e.g. energies and positions
    // are recorded to the tree. Other data that are used to identify particles
    // in the hit ie isPrimary and ParticleName are also recorded and are used for
    // example in the print_event_data_for_plane() function in the tracking routine.
    // all other variables are commented out for now in order to reduce the output
    // root file size for the large no. of events needed for all 180 angles. Additionally,
    // many of the commented out variables should only be produced when a threshold in
    // energy has been applied e.g. cluster_width and strip_number so are reconstructed in the tracking.
    // Energy info (signal and edep) for truth planes is not read out as they are now used
    // as single planes with just their position info used for calculating uncertainties
    // in tracking (whether the DUT flag is set in the .mac file or not).

    // Variables commented out here are still calculated in the AddEvent() function below.
    // This allows them to be added back into the output tree quickly if needed. If performance
    // is an issue the calculation of the variables could be commented out too.

    char branch[50];
    
    // Event variables
    
    //if( !rootTree_strip )
    //{
        rootTree_pixel->Branch( "event_no" , &Event_no );
        rootTree_pixel->Branch( "ke_in" , &KE_in );
        //rootTree_pixel->Branch( "ke_out" , &KE_out );
        
        rootTree_pixel->Branch( "truth_x_pos" , &Truth_x_pos );
        rootTree_pixel->Branch( "truth_y_pos" , &Truth_y_pos );
        rootTree_pixel->Branch( "truth_z_pos" , &Truth_z_pos );
        rootTree_pixel->Branch( "truthTheta_x" , &TruthTheta_x );
        rootTree_pixel->Branch( "truthTheta_y" , &TruthTheta_y );
    //}

    rootTree_pixel->Branch( "hit_mult_pix1" , &Hit_mult_pix1 );
    rootTree_pixel->Branch( "hit_mult_pix2" , &Hit_mult_pix2 );
    rootTree_pixel->Branch( "hit_mult_pix3" , &Hit_mult_pix3 );
    rootTree_pixel->Branch( "hit_mult_pix4" , &Hit_mult_pix4 );

    // Digit variables
    
    sprintf(branch, "signal_pix1[%i]/F", nPixels);
    rootTree_pixel->Branch( "signal_pix1", Signal_pix1 , branch );
    sprintf(branch, "signal_pix2[%i]/F", nPixels);
    rootTree_pixel->Branch( "signal_pix2", Signal_pix2 , branch );
    sprintf(branch, "signal_pix3[%i]/F", nPixels);
    rootTree_pixel->Branch( "signal_pix3", Signal_pix3 , branch );
    sprintf(branch, "signal_pix4[%i]/F", nPixels);
    rootTree_pixel->Branch( "signal_pix4", Signal_pix4 , branch );
    
    // Energy variables
    
    rootTree_pixel->Branch( "ni_edep_pix1" , &NI_Edep_pix1 );   // non-ionising energy loss (use for dose calculations)
    rootTree_pixel->Branch( "ni_edep_pix2" , &NI_Edep_pix2 );   // non-ionising energy loss (use for dose calculations)
    rootTree_pixel->Branch( "ni_edep_pix3" , &NI_Edep_pix3 );   // non-ionising energy loss (use for dose calculations)
    rootTree_pixel->Branch( "ni_edep_pix4" , &NI_Edep_pix4 );   // non-ionising energy loss (use for dose calculations)
    rootTree_pixel->Branch( "edep_pix1" , &Edep_pix1 );
    rootTree_pixel->Branch( "edep_pix2" , &Edep_pix2 );
    rootTree_pixel->Branch( "edep_pix3" , &Edep_pix3 );
    rootTree_pixel->Branch( "edep_pix4" , &Edep_pix4 );
    //***************************************************************************************************************
    // Here we imblement the K.E for the particles at each pixels, I will created as a leave not a string, So here we go ;)

    rootTree_pixel->Branch( "truth_KE_pix1" , &Truth_KE_pix1 );
    rootTree_pixel->Branch( "truth_KE_pix2" , &Truth_KE_pix2 );
    rootTree_pixel->Branch( "truth_KE_pix3" , &Truth_KE_pix3 );
    rootTree_pixel->Branch( "truth_KE_pix4" , &Truth_KE_pix4 );

    //*****************************************************************************************************************

    
    // Position and particle type
    
    rootTree_pixel->Branch( "isPrimaryParticle_pix1" , &IsPrimaryParticle_pix1 );
    rootTree_pixel->Branch( "isPrimaryParticle_pix2" , &IsPrimaryParticle_pix2 );
    rootTree_pixel->Branch( "isPrimaryParticle_pix3" , &IsPrimaryParticle_pix3 );
    rootTree_pixel->Branch( "isPrimaryParticle_pix4" , &IsPrimaryParticle_pix4 );
    
    rootTree_pixel->Branch( "trackNumber_pix1" , &Track_no_pix1 );
    rootTree_pixel->Branch( "trackNumber_pix2" , &Track_no_pix2 );
    rootTree_pixel->Branch( "trackNumber_pix3" , &Track_no_pix3 );
    rootTree_pixel->Branch( "trackNumber_pix4" , &Track_no_pix4 );
    
    rootTree_pixel->Branch( "particleName_pix1" , &ParticleName_pix1 );
    rootTree_pixel->Branch( "particleName_pix2" , &ParticleName_pix2 );
    rootTree_pixel->Branch( "particleName_pix3" , &ParticleName_pix3 );
    rootTree_pixel->Branch( "particleName_pix4" , &ParticleName_pix4 );

    rootTree_pixel->Branch( "Process_Name_pix1" , &ProcessName_pix1 );
     rootTree_pixel->Branch( "process_Name_pix2" , &ProcessName_pix2 );
     rootTree_pixel->Branch( "process_Name_pix3" , &ProcessName_pix3 );
     rootTree_pixel->Branch( "process_Name_pix4" , &ProcessName_pix4 );

    rootTree_pixel->Branch( "x_pos_pix1" , &X_pos_pix1 );
    rootTree_pixel->Branch( "x_pos_pix2" , &X_pos_pix2 );
    rootTree_pixel->Branch( "x_pos_pix3" , &X_pos_pix3 );
    rootTree_pixel->Branch( "x_pos_pix4" , &X_pos_pix4 );
    
    rootTree_pixel->Branch( "y_pos_pix1" , &Y_pos_pix1 );
    rootTree_pixel->Branch( "y_pos_pix2" , &Y_pos_pix2 );
    rootTree_pixel->Branch( "y_pos_pix3" , &Y_pos_pix3 );
    rootTree_pixel->Branch( "y_pos_pix4" , &Y_pos_pix4 );
    
    rootTree_pixel->Branch( "z_pos_pix1" , &Z_pos_pix1 );
    rootTree_pixel->Branch( "z_pos_pix2" , &Z_pos_pix2 );
    rootTree_pixel->Branch( "z_pos_pix3" , &Z_pos_pix3 );
    rootTree_pixel->Branch( "z_pos_pix4" , &Z_pos_pix4 );

    rootTree_pixel->Branch( "truth_x_pos_pix1" , &Truth_X_pos_pix1 );
    rootTree_pixel->Branch( "truth_x_pos_pix2" , &Truth_X_pos_pix2 );
    rootTree_pixel->Branch( "truth_x_pos_pix3" , &Truth_X_pos_pix3 );
    rootTree_pixel->Branch( "truth_x_pos_pix4" , &Truth_X_pos_pix4 );
    
    rootTree_pixel->Branch( "truth_y_pos_pix1" , &Truth_Y_pos_pix1 );
    rootTree_pixel->Branch( "truth_y_pos_pix2" , &Truth_Y_pos_pix2 );
    rootTree_pixel->Branch( "truth_y_pos_pix3" , &Truth_Y_pos_pix3 );
    rootTree_pixel->Branch( "truth_y_pos_pix4" , &Truth_Y_pos_pix4 );
    
    rootTree_pixel->Branch( "truth_z_pos_pix1" , &Truth_Z_pos_pix1 );
    rootTree_pixel->Branch( "truth_z_pos_pix2" , &Truth_Z_pos_pix2 );
    rootTree_pixel->Branch( "truth_z_pos_pix3" , &Truth_Z_pos_pix3 );
    rootTree_pixel->Branch( "truth_z_pos_pix4" , &Truth_Z_pos_pix4 );
    
}

void RootSaver::CloseTrees()
{
	// Check if ROOT TTree exists,
	// in case get the associated file and close it.
	// Note that if a TFile goes above 2GB a new file
	// will be automatically opened. We have thus to get,
	// from the TTree the current opened file

    if ( rootTree_strip )
	{
		G4cout << "\nWriting ROOT TTree: " << rootTree_strip->GetName() << "\n" << G4endl;
		//rootTree_strip->Print();
		rootTree_strip->Write();
		TFile* currentFile = rootTree_strip->GetCurrentFile();
		if ( currentFile == 0 || currentFile->IsZombie() )
		{
			G4cerr << "Error closing TFile " << G4endl;
			return;
		}
		currentFile->Close();
		//The root is automatically deleted.
		rootTree_strip = 0;

		delete[] Signal_x1;
		delete[] Signal_u1;
		delete[] Signal_v1;
	}
    
    if ( rootTree_pixel )
    {
        G4cout << "\nWriting ROOT TTree: " << rootTree_pixel->GetName() << "\n" << G4endl;
        //rootTree_pixel->Print();
        rootTree_pixel->Write();
        TFile* currentFile = rootTree_pixel->GetCurrentFile();
        if ( currentFile == 0 || currentFile->IsZombie() )
        {
            G4cerr << "Error closing TFile " << G4endl;
            return;
        }
        currentFile->Close();
        //The root is automatically deleted.
        rootTree_pixel = 0;
        
        delete[] Signal_pix1;
        delete[] Signal_pix2;
        delete[] Signal_pix3;
        delete[] Signal_pix4;
    }
}

void RootSaver::AddEvent_strip_det(const G4int event,
                                   const SiHitCollection* const hits_x1,
                                   const SiHitCollection* const hits_u1,
                                   const SiHitCollection* const hits_v1,
                                   const SiDigiCollection* const digits,
                                   const G4ThreeVector& primPos,
                                   const G4ThreeVector& primMom,

                                   const G4float K_E_in/*, const G4float K_E_out*/)
{
	//If root TTree is not created ends
	if ( rootTree_strip == 0 ) {return;}
    
    //Initialise variables
    Event_no = event;
    //Det_mult = 0;
    KE_in = K_E_in;
    //KE_out = K_E_out;   //set at the end with truth variables
    
    ClusterSize_x1 = 0;
    ClusterSize_u1 = 0;
    ClusterSize_v1 = 0;
    
	//Store Digits information
	if ( digits )
	{
		G4int nDigits = digits->entries();
        G4int is_det_x1, is_det_u1, is_det_v1;
        
        is_det_x1 = 0;
        is_det_u1 = 0;
        is_det_v1 = 0;

		for ( G4int d = 0 ; d<nDigits ; ++d )
		{
			const SiDigi* digi = static_cast<const SiDigi*>( digits->GetDigi( d ) );
			G4int stripNum = digi->GetStripNumber();
            
			//Safety check
			if ( stripNum >= nStrips )
			{
				G4cerr << "Digi Error: Strip number "<< stripNum << " expected max value:" << nStrips << G4endl;
				continue;//Go to next digit
			}
			G4int planeNum = digi->GetPlaneNumber();
            
			if ( planeNum == 0 )
			{
				Signal_x1[ stripNum ] = static_cast<Float_t>(digi->GetCharge());
                if(Signal_x1[ stripNum ] > 0) {is_det_x1 = 1; ClusterSize_x1++; Strip_no_x1.push_back(stripNum);}
			}
            
			else if ( planeNum == 1 )
			{
				Signal_u1[ stripNum ] = static_cast<Float_t>(digi->GetCharge());
                if(Signal_u1[ stripNum ] > 0) {is_det_u1 = 1; ClusterSize_u1++; Strip_no_u1.push_back(stripNum);}
			}
            
			else if ( planeNum == 2 )
			{
				Signal_v1[ stripNum ] = static_cast<Float_t>(digi->GetCharge());
                if(Signal_v1[ stripNum ] > 0) {is_det_v1 = 1; ClusterSize_v1++; Strip_no_v1.push_back(stripNum);}
			}
            
			else
			{
				G4cerr << "Digi Error: Plane number not set correctly in DetectorConstruction.cc" << G4endl;
			}
            
		}
    
        //Generate detector multiplicity, truth planes not included.
        Det_mult = is_det_x1 + is_det_u1 + is_det_v1;
	}
	else
	{
		G4cerr << "Error: No digi collection passed to RootSaver" << G4endl;
	}

	//Store Hits information
	if ( hits_x1 || hits_u1 || hits_v1 )
	{
		//Set defaults
        Hit_mult_x1 = 0;
        Hit_mult_u1 = 0;
        Hit_mult_v1 = 0;
        
        NI_Edep_x1.clear();
        
        Edep_x1.clear();
        Edep_u1.clear();
		Edep_v1.clear();
        
        G4float th_x1 = 0;
		G4float th_u1 = 0;
		G4float th_v1 = 0;
        
        G4float x0 = 0;
        G4float y0 = 0;
        G4float z0 = 0;
        
        G4float x1 = 0;
        G4float y1 = 0;
        G4float z1 = 0;
        
        IsPrimaryParticle_x1.clear();
        IsPrimaryParticle_u1.clear();
        IsPrimaryParticle_v1.clear();
        
        Track_no_x1.clear();
        Track_no_u1.clear();
        Track_no_v1.clear();
        
        ParticleName_x1.clear();
        ParticleName_u1.clear();
        ParticleName_v1.clear();
        
        X_pos_x1.clear();
        X_pos_u1.clear();
        X_pos_v1.clear();
        
        Y_pos_x1.clear();
        Y_pos_u1.clear();
        Y_pos_v1.clear();
        
        Z_pos_x1.clear();
        Z_pos_u1.clear();
        Z_pos_v1.clear();
        
		// Loop on all hits, to obtain energy, pos and angle
		// Position is weighted average of hit x(), see SensitiveDetector.cc
        
        
        //********************************** FIRST TRACKER MODULE **********************************//
        
        G4int nHits = hits_x1->entries();
        
		for ( G4int h = 0 ; (h<nHits) ; ++h )
		{
            const SiHit* hit = static_cast<const SiHit*>( hits_x1->GetHit( h ) );
            
			//Uncomment this line if you want to record only
			//primary energy depositions
			//if ( hit->GetIsPrimary() == false ) continue;

            G4ThreeVector particle_pos = hit->GetPosition();
	    //    G4ThreeVector truth_particle_pos = hit->GetPosition();//**********************

			//We save energy in MeV
			Float_t edep = static_cast<Float_t>(hit->GetEdep());
			edep /= MeV;
            
            //We save energy in MeV (non-ionising energy loss for x1 plane only (use for dose calculations))
            Float_t ni_edep = static_cast<Float_t>(hit->GetNonIonisingEdep());
            ni_edep /= MeV;
            
            x0 = static_cast<Float_t>(primPos.x());
            y0 = static_cast<Float_t>(primPos.y());
            z0 = static_cast<Float_t>(primPos.z());
            
            x1 = static_cast<Float_t>(particle_pos.x());
            y1 = static_cast<Float_t>(particle_pos.y());
            z1 = static_cast<Float_t>(particle_pos.z());
            
            //We save positions in mm (world coordinates)
            x0 /= mm;
            y0 /= mm;
            z0 /= mm;
            
            x1 /= mm;
            y1 /= mm;
            z1 /= mm;
            
            th_x1 = std::atan( (x1-x0) / (z1-z0) );
            th_x1 /= mrad;
            
            // only one element per track (particle) per sensitive detector is recorded with the index of each element
            // in the vector being equal to the track no. this could be changed such that multiple hits per track (particle)
            // per sensitive detector are recorded. hit->GetTrackNumber()) == (Track_no_x1.back()) ensures the same element
            // is overwritten each time if the hit belongs to the same track (particle), except for energy which is not
            // overwritten but added up.
            if( Track_no_x1.size()>0 && (hit->GetTrackNumber()) == (Track_no_x1.back()) )  // ensures each element represents a track, element [0] = track 1
            {
                // Track_no_x1.size()-1 means elements from different vectors correspond to the
                // same particle when the same element no. is chosen. For each events all vectors are same
                // size, this can be verified with print out statement above.
                ParticleName_x1[Track_no_x1.size()-1] = hit->GetParticleName();
                Track_no_x1[Track_no_x1.size()-1] = hit->GetTrackNumber();
                IsPrimaryParticle_x1[Track_no_x1.size()-1] = hit->GetIsPrimary();
                X_pos_x1[Track_no_x1.size()-1] = x1;
                Y_pos_x1[Track_no_x1.size()-1] = y1;
                Z_pos_x1[Track_no_x1.size()-1] = z1;
                Theta_x1[Track_no_x1.size()-1] = th_x1;
                Edep_x1[Track_no_x1.size()-1] += edep;
                
                NI_Edep_x1[Track_no_x1.size()-1] += ni_edep;      //write non-ionising energy loss for x1 plane only (use for dose calculations)
                
            }
            else
            {
                //G4cout << "Track_no_x1 = " << hit->GetTrackNumber() << G4endl;
                
                ParticleName_x1.push_back(hit->GetParticleName());
                Track_no_x1.push_back(hit->GetTrackNumber());
                IsPrimaryParticle_x1.push_back(hit->GetIsPrimary());
                X_pos_x1.push_back(x1);
                Y_pos_x1.push_back(y1);
                Z_pos_x1.push_back(z1);
                Theta_x1.push_back(th_x1);
                Edep_x1.push_back(edep);
                
                NI_Edep_x1.push_back(ni_edep);      //write non-ionising energy loss for x1 plane only (use for dose calculations)
            }
            
            Hit_mult_x1 = X_pos_x1.size();
			//G4int planeNum = hit->GetPlaneNumber();
            
            /*
            if((hit->GetIsPrimary() == true))
            {G4cout << "The position of the primary " << hit->GetParticleName() << " hit in det x1 = (" << x1 << "," << y1 << "," << z1 << ") mm, it's energy is: " <<  edep << " MeV, energy sum is: " << Edep_x1.back() << " MeV, it's track no. is: " << hit->GetTrackNumber() << G4endl;}
            else
            {G4cout << "The position of the secondary " << hit->GetParticleName() << " hit in det x1 = (" << x1 << "," << y1 << "," << z1 << ") mm, it's energy is: " <<  edep << " MeV, energy sum is: " << Edep_x1.back() << " MeV, it's track no. is: " << hit->GetTrackNumber() << G4endl;}*/
        }
        
        nHits = hits_u1->entries();
        Edep_u1.clear();
        
		for ( G4int h = 0 ; (h<nHits) ; ++h )
		{
			const SiHit* hit = static_cast<const SiHit*>( hits_u1->GetHit( h ) );
            
			//Uncomment this line if you want to record only
			//primary energy depositions
			//if ( hit->GetIsPrimary() == false ) continue;
			
            G4ThreeVector particle_pos = hit->GetPosition();
	    //  G4ThreeVector truth_particle_pos = hit->GetPosition();//***************************

			//We save energy in MeV
			Float_t edep = static_cast<Float_t>(hit->GetEdep());
			edep /= MeV;
            
            x0 = static_cast<Float_t>(primPos.x());
            y0 = static_cast<Float_t>(primPos.y());
            z0 = static_cast<Float_t>(primPos.z());
            
            x1 = static_cast<Float_t>(particle_pos.x());
            y1 = static_cast<Float_t>(particle_pos.y());
            z1 = static_cast<Float_t>(particle_pos.z());
            
            //We save positions in mm (world coordinates)
            x0 /= mm;
            y0 /= mm;
            z0 /= mm;
            
            x1 /= mm;
            y1 /= mm;
            z1 /= mm;
            
			//Edep_u1 += edep;
            th_u1 = std::atan( (x1-x0) / (z1-z0) );
            th_u1 /= mrad;
            
            //if( (hit->GetTrackNumber()) <= (ParticleName_u1.size()) )   //ensures each element represents a track, element [0] = track 1
            if( Track_no_u1.size()>0 && (hit->GetTrackNumber()) == (Track_no_u1.back()) )   //ensures each element represents a track, element [0] = track 1
            {
                
                ParticleName_u1[Track_no_u1.size()-1] = hit->GetParticleName();
                Track_no_u1[Track_no_u1.size()-1] = hit->GetTrackNumber();
                IsPrimaryParticle_u1[Track_no_u1.size()-1] = hit->GetIsPrimary();
                X_pos_u1[Track_no_u1.size()-1] = x1;
                Y_pos_u1[Track_no_u1.size()-1] = y1;
                Z_pos_u1[Track_no_u1.size()-1] = z1;
                Theta_u1[Track_no_u1.size()-1] = th_u1;
                Edep_u1[Track_no_u1.size()-1] += edep;
                
            }
            else
            {
                ParticleName_u1.push_back(hit->GetParticleName());
                Track_no_u1.push_back(hit->GetTrackNumber());
                IsPrimaryParticle_u1.push_back(hit->GetIsPrimary());
                X_pos_u1.push_back(x1);
                Y_pos_u1.push_back(y1);
                Z_pos_u1.push_back(z1);
                Theta_u1.push_back(th_u1);
                Edep_u1.push_back(edep);
            }
            
            Hit_mult_u1 = X_pos_u1.size();
			//G4int planeNum = hit->GetPlaneNumber();
            
            /*if((hit->GetIsPrimary() == true))
            {G4cout << "The position of the primary " << hit->GetParticleName() << " hit in det u1 = (" << x1 << "," << y1 << "," << z1 << ") mm, it's energy is: " <<  edep << " MeV, energy sum is: " << Edep_u1.back() << " MeV, it's track no. is: " << hit->GetTrackNumber() << G4endl;}
            else
            {G4cout << "The position of the secondary " << hit->GetParticleName() << " hit in det u1 = (" << x1 << "," << y1 << "," << z1 << ") mm, it's energy is: " <<  edep << " MeV, energy sum is: " << Edep_u1.back() << " MeV, it's track no. is: " << hit->GetTrackNumber() << G4endl;}*/

        }
        
        nHits = hits_v1->entries();
		Edep_v1.clear();
        
		for ( G4int h = 0 ; (h<nHits) ; ++h )
		{
			const SiHit* hit = static_cast<const SiHit*>( hits_v1->GetHit( h ) );
            
			//Uncomment this line if you want to record only
			//primary energy depositions
			//if ( hit->GetIsPrimary() == false ) continue;
			
            G4ThreeVector particle_pos = hit->GetPosition();
	    //  G4ThreeVector truth_particle_pos = hit->GetPosition();//**********************

			//We save energy in MeV
			Float_t edep = static_cast<Float_t>(hit->GetEdep());
			edep /= MeV;
            
            x0 = static_cast<Float_t>(primPos.x());
            y0 = static_cast<Float_t>(primPos.y());
            z0 = static_cast<Float_t>(primPos.z());
            
            x1 = static_cast<Float_t>(particle_pos.x());
            y1 = static_cast<Float_t>(particle_pos.y());
            z1 = static_cast<Float_t>(particle_pos.z());
            
            //We save positions in mm (world coordinates)
            x0 /= mm;
            y0 /= mm;
            z0 /= mm;
            
            x1 /= mm;
            y1 /= mm;
            z1 /= mm;
            
            ///Edep_v1 += edep;
            th_v1 =  std::atan( (x1-x0) / (z1-z0) );
            th_v1 /= mrad;
            
            if( Track_no_v1.size()>0 && (hit->GetTrackNumber()) == (Track_no_v1.back()) )   //ensures each element represents a track, element [0] = track 1
            {
                ParticleName_v1[Track_no_v1.size()-1] = hit->GetParticleName();
                Track_no_v1[Track_no_v1.size()-1] = hit->GetTrackNumber();
                IsPrimaryParticle_v1[Track_no_v1.size()-1] = hit->GetIsPrimary();
                X_pos_v1[Track_no_v1.size()-1] = x1;
                Y_pos_v1[Track_no_v1.size()-1] = y1;
                Z_pos_v1[Track_no_v1.size()-1] = z1;
                Theta_v1[Track_no_v1.size()-1] = th_v1;
                Edep_v1[Track_no_v1.size()-1] += edep;
                //if(edep==0)G4cout << "ZERO ENERGY DEPOSIT in det. v1 for particle: " << hit->GetParticleName() << ", Edep_v1.back() = " << Edep_v1.back() << G4endl;

            }
            else
            {
                ParticleName_v1.push_back(hit->GetParticleName());
                Track_no_v1.push_back(hit->GetTrackNumber());
                IsPrimaryParticle_v1.push_back(hit->GetIsPrimary());
                X_pos_v1.push_back(x1);
                Y_pos_v1.push_back(y1);
                Z_pos_v1.push_back(z1);
                Theta_v1.push_back(th_v1);
                Edep_v1.push_back(edep);
                //if(edep==0)G4cout << "ZERO ENERGY DEPOSIT in det. v1 for particle: " << hit->GetParticleName() << ", Edep_v1[(hit->GetTrackNumber())-1] = " << Edep_v1.back() << G4endl;
            }
            
            Hit_mult_v1 = X_pos_v1.size();
			//G4int planeNum = hit->GetPlaneNumber();
            
            /*if((hit->GetIsPrimary() == true))
            {G4cout << "The position of the primary " << hit->GetParticleName() << " hit in det v1 = (" << x1 << "," << y1 << "," << z1 << ") mm, it's energy is: " <<  edep << " MeV, energy sum is: " << Edep_v1.back() << " MeV, it's track no. is: " << hit->GetTrackNumber() << G4endl;}
            else
            {G4cout << "The position of the secondary " << hit->GetParticleName() << " hit in det v1 = (" << x1 << "," << y1 << "," << z1 << ") mm, it's energy is: " <<  edep << " MeV, energy sum is: " << Edep_v1.back() << " MeV, it's track no. is: " << hit->GetTrackNumber() << G4endl;}*/
        }
            
        
  } //End of if(hits_x1 || hits_u1 .... ) condition
    
  //******************************************************************************************//

    else {G4cerr << "Error: No hits collection passed to RootSaver for this event" << G4endl;}
    
	Truth_x_pos = static_cast<Float_t>( primPos.x() );
	Truth_y_pos = static_cast<Float_t>( primPos.y() );
	Truth_z_pos = static_cast<Float_t>( primPos.z() );
    
	//Measure angle of the beam in xz plane measured from z+ direction
	// -pi<Theta<=pi (positive when close to x positive direction)
    /*
	Float_t sign_z = ( primMom.z()>= 0 ) ? +1 : -1;
	Float_t sign_x = ( primMom.x()>= 0 ) ? +1 : -1;
	TruthTheta = ( primMom.z() != 0 ) ?
    TMath::PiOver2()*sign_x*(1-sign_z)+std::atan( primMom.x()/primMom.z() ) : sign_x*TMath::PiOver2(); //beam perpendicular to z
     */
    
    TruthTheta_x = std::atan( primMom.x()/primMom.z() );
    TruthTheta_x /= mrad;
    
    TruthTheta_y = std::atan( primMom.y()/primMom.z() );
    TruthTheta_y /= mrad;
    
	rootTree_strip->Fill();
}

void RootSaver::AddEvent_pixel_det(const G4int event,
                                   const SiHit_pixCollection* const hits_pix1,
                                   const SiHit_pixCollection* const hits_pix2,
                                   const SiHit_pixCollection* const hits_pix3,
                                   const SiHit_pixCollection* const hits_pix4,
                                   const SiDigi_pixCollection* const digits,
                                   const G4ThreeVector& primPos,
				   // const G4ThreeVector& truth_Pos, //**********************************************

                                   const G4ThreeVector& primMom,

                                   const G4float K_E_in/*, const G4float K_E_out*/)
{
    //If root TTree is not created ends
    if ( rootTree_pixel == 0 ) {return;}
    
    //Initialise variables
    Event_no = event;
    KE_in = K_E_in;
    //KE_out = K_E_out;   //set at the end with truth variables
    
    //Store Digits information
    if ( digits )
    {
        G4int nDigits = digits->entries();
        for ( G4int d = 0 ; d<nDigits ; ++d )
        {
            const SiDigi_pix * digi = static_cast<const SiDigi_pix*>( digits->GetDigi( d ) );
            G4int pixelNum = digi->GetPixelNumber();
            
            //Safety check
            if ( pixelNum >= nPixels )
            {
                G4cerr << "Digi Error: Pixel number "<< pixelNum << " expected max value:" << nPixels << G4endl;
                continue;//Go to next digit
            }
            G4int planeNum = digi->GetPlaneNumber();
            if ( planeNum == 0 )        {Signal_pix1[ pixelNum ] = static_cast<Float_t>(digi->GetCharge());}
            else if ( planeNum == 1 )   {Signal_pix2[ pixelNum ] = static_cast<Float_t>(digi->GetCharge());}
            else if ( planeNum == 2 )   {Signal_pix3[ pixelNum ] = static_cast<Float_t>(digi->GetCharge());}
            else if ( planeNum == 3 )   {Signal_pix4[ pixelNum ] = static_cast<Float_t>(digi->GetCharge());}
            else{G4cerr << "Digi Error: Plane number not set correctly in DetectorConstruction.cc, it is: " << planeNum << G4endl;}
            
        }
    }
    else
    {
        G4cerr << "Error: No digi collection for pixel detector(s)s passed to RootSaver" << G4endl;
    }
    
    //Store Hits information
    if ( hits_pix1 || hits_pix2 || hits_pix3 || hits_pix4)
    {
        // Set defaults
        Hit_mult_pix1 = 0;
        Hit_mult_pix2 = 0;
        Hit_mult_pix3 = 0;
        Hit_mult_pix4 = 0;
        
        NI_Edep_pix1.clear();
        NI_Edep_pix2.clear();
        NI_Edep_pix3.clear();
        NI_Edep_pix4.clear();
        
        Edep_pix1.clear();
        Edep_pix2.clear();
        Edep_pix3.clear();
        Edep_pix4.clear();

        Truth_KE_pix1.clear();
        Truth_KE_pix2.clear();
        Truth_KE_pix3.clear();
        Truth_KE_pix4.clear();
        
        IsPrimaryParticle_pix1.clear();
        IsPrimaryParticle_pix2.clear();
        IsPrimaryParticle_pix3.clear();
        IsPrimaryParticle_pix4.clear();
        
        Track_no_pix1.clear();
        Track_no_pix2.clear();
        Track_no_pix3.clear();
        Track_no_pix4.clear();
        
        ParticleName_pix1.clear();
        ParticleName_pix2.clear();
        ParticleName_pix3.clear();
        ParticleName_pix4.clear();

	 ProcessName_pix1.clear();
	 ProcessName_pix2.clear();
	 ProcessName_pix3.clear();
	 ProcessName_pix4.clear();

        X_pos_pix1.clear();
        Y_pos_pix1.clear();
        Z_pos_pix1.clear();
        
        X_pos_pix2.clear();
        Y_pos_pix2.clear();
        Z_pos_pix2.clear();
        
        X_pos_pix3.clear();
        Y_pos_pix3.clear();
        Z_pos_pix3.clear();
        
        X_pos_pix4.clear();
        Y_pos_pix4.clear();
        Z_pos_pix4.clear();

      Truth_X_pos_pix1.clear();
        Truth_Y_pos_pix1.clear();
        Truth_Z_pos_pix1.clear();
        
       Truth_X_pos_pix2.clear();
        Truth_Y_pos_pix2.clear();
        Truth_Z_pos_pix2.clear();
        
        Truth_X_pos_pix3.clear();
        Truth_Y_pos_pix3.clear();
        Truth_Z_pos_pix3.clear();
        
        Truth_X_pos_pix4.clear();
       Truth_Y_pos_pix4.clear();
        Truth_Z_pos_pix4.clear();

        G4float x0 = 0;
        G4float y0 = 0;
        G4float z0 = 0;
        
        G4float x1 = 0;
        G4float y1 = 0;
        G4float z1 = 0;
	//These cordinates will be for the truth position ,,,, x1, y1, z1 its for the scoring position and are random ,,,, x0, y0, z0 its for the truth_pos for the proton gun and you dont need to change it
	G4float x3 = 0; //********************** This have been done for the original positions
        G4float y3 = 0;//*********************
        G4float z3 = 0;//*********************
        // Loop on all hits, to obtain energy, pos and angle
        // Position is weighted average of hit x(), see SensitiveDetector.cc

        
        //********************************** FIRST PIXEL MODULE **********************************//

        G4int nHits = hits_pix1->entries();
        
        for ( G4int h = 0 ; h < nHits; ++h )
        {
            const SiHit_pix * hit = static_cast<const SiHit_pix*>( hits_pix1->GetHit( h ) );
            
            // Uncomment this line if you want to record only primary energy depositions
            //if ( hit->GetIsPrimary() == false ) continue;
            
            G4ThreeVector particle_pos = hit->GetPosition(); // If you look here to the sensitive pix detector class. the code getting the Position from pointE which is the diff between point2 - point1 which shosen randomly

	    G4ThreeVector truth_pos = hit->GetTruth_Position();//********************************************** Here the code should draw ( point) which should the truth position

            // We save energy in MeV
            Float_t edep = static_cast<Float_t>(hit->GetEdep());
            edep /= MeV;

	    Float_t truth_KE = static_cast<Float_t>(hit->GetTruth_KE());
            truth_KE /= MeV;
 

            // We save energy in MeV (non-ionising energy loss (use for dose calculations))
            Float_t ni_edep = static_cast<Float_t>(hit->GetNonIonisingEdep());
            ni_edep /= MeV;
            
            x0 = static_cast<Float_t>(primPos.x());
            y0 = static_cast<Float_t>(primPos.y());
            z0 = static_cast<Float_t>(primPos.z());
            
            x1 = static_cast<Float_t>(particle_pos.x()); 
            y1 = static_cast<Float_t>(particle_pos.y()); 
            z1 = static_cast<Float_t>(particle_pos.z());
       
	     x3 = static_cast<Float_t>(truth_pos.x());
	     y3 = static_cast<Float_t>(truth_pos.y());
	     z3 = static_cast<Float_t>(truth_pos.z());
            //We save positions in mm (world coordinates)
            x0 /= mm;
            y0 /= mm;
            z0 /= mm;
            
            x1 /= mm;
            y1 /= mm;
            z1 /= mm;

             x3 /= mm;
            y3 /= mm;
            z3 /= mm;
            //th_pix1 = std::atan( (x1-x0) / (z1-z0) );
            //th_pix1 /= mrad;

            // only one element per track (particle) per sensitive detector is recorded with the index of each element
            // in the vector being equal to the track no. this could be changed such that multiple hits per track (particle)
            // per sensitive detector are recorded. hit->GetTrackNumber()) == (Track_no_pix1.back()) ensures the same element
            // is overwritten each time if the hit belongs to the same track (particle), except for energy which is not
            // overwritten but added up.
            
            // ensures each element represents a track, element [0] = track 1
            if( Track_no_pix1.size() > 0 && (hit->GetTrackNumber()) == (Track_no_pix1.back()) )
            {
                // Track_no_pix1.size()-1 means elements from different vectors correspond to the
                // same particle when the same element no. is chosen. For each events all vectors are same
                // size, this can be verified with print out statement above.
                ParticleName_pix1[Track_no_pix1.size()-1] = hit->GetParticleName();
		// ParticleParent_pix1[Track_no_pix1.size()-1] = hit->GetParticleParent(); 
		 ProcessName_pix1[Track_no_pix1.size()-1] = hit->GetProcessName();

                Track_no_pix1[Track_no_pix1.size()-1] = hit->GetTrackNumber();
                IsPrimaryParticle_pix1[Track_no_pix1.size()-1] = hit->GetIsPrimary();
                X_pos_pix1[Track_no_pix1.size()-1] = x1;
                Y_pos_pix1[Track_no_pix1.size()-1] = y1;
                Z_pos_pix1[Track_no_pix1.size()-1] = z1;

		Truth_X_pos_pix1[Track_no_pix1.size()-1] = x3; 
                Truth_Y_pos_pix1[Track_no_pix1.size()-1] = y3;
                Truth_Z_pos_pix1[Track_no_pix1.size()-1] = z3;

                //Theta_pix1[Track_no_pix1.size()-1] = th_pix1;
                Edep_pix1[Track_no_pix1.size()-1] += edep;
                NI_Edep_pix1[Track_no_pix1.size()-1] += ni_edep;

                Truth_KE_pix1[Track_no_pix1.size()-1] = truth_KE; 


            }
            else
            {
                //G4cout << "Track_no_pix1 = " << hit->GetTrackNumber() << G4endl;
                
                ParticleName_pix1.push_back(hit->GetParticleName());
		  ProcessName_pix1.push_back(hit->GetProcessName()); 


                Track_no_pix1.push_back(hit->GetTrackNumber());
                IsPrimaryParticle_pix1.push_back(hit->GetIsPrimary());
                X_pos_pix1.push_back(x1);
                Y_pos_pix1.push_back(y1);
                Z_pos_pix1.push_back(z1);
		Truth_X_pos_pix1.push_back(x3); 
                Truth_Y_pos_pix1.push_back(y3);
                Truth_Z_pos_pix1.push_back(z3);
                //Theta_pix1.push_back(th_pix1);
                Edep_pix1.push_back(edep);
                NI_Edep_pix1.push_back(ni_edep);

                Truth_KE_pix1.push_back(truth_KE); 

            }
            
            Hit_mult_pix1 = X_pos_pix1.size();
            //G4int planeNum = hit->GetPlaneNumber();
            
            /*
             if((hit->GetIsPrimary() == true))
             {G4cout << "The position of the primary " << hit->GetParticleName() << " hit in det x1 = (" << x1 << "," << y1 << "," << z1 << ") mm, it's energy is: " <<  edep << " MeV, energy sum is: " << Edep_pix1.back() << " MeV, it's track no. is: " << hit->GetTrackNumber() << G4endl;}
             else
             {G4cout << "The position of the secondary " << hit->GetParticleName() << " hit in det x1 = (" << x1 << "," << y1 << "," << z1 << ") mm, it's energy is: " <<  edep << " MeV, energy sum is: " << Edep_pix1.back() << " MeV, it's track no. is: " << hit->GetTrackNumber() << G4endl;}*/
        }
        
        //********************************** SECOND PIXEL MODULE **********************************//
        
        nHits = hits_pix2->entries();
        
        for ( G4int h = 0 ; h < nHits; ++h )
        {
            const SiHit_pix * hit = static_cast<const SiHit_pix*>( hits_pix2->GetHit( h ) );
            
            
            G4ThreeVector particle_pos = hit->GetPosition();
            G4ThreeVector truth_pos = hit->GetTruth_Position();

            // We save energy in MeV
            Float_t edep = static_cast<Float_t>(hit->GetEdep());
            edep /= MeV;
           
             Float_t truth_KE = static_cast<Float_t>(hit->GetTruth_KE());
	     truth_KE /= MeV;         

            // We save energy in MeV (non-ionising energy loss (use for dose calculations))
            Float_t ni_edep = static_cast<Float_t>(hit->GetNonIonisingEdep());
            ni_edep /= MeV;
            
            x0 = static_cast<Float_t>(primPos.x());
            y0 = static_cast<Float_t>(primPos.y());
            z0 = static_cast<Float_t>(primPos.z());
            
            x1 = static_cast<Float_t>(particle_pos.x());
            y1 = static_cast<Float_t>(particle_pos.y());
            z1 = static_cast<Float_t>(particle_pos.z());
            
            x3 = static_cast<Float_t>(truth_pos.x()); 
            y3 = static_cast<Float_t>(truth_pos.y());
            z3 = static_cast<Float_t>(truth_pos.z());
            
            //We save positions in mm (world coordinates)
            x0 /= mm;
            y0 /= mm;
            z0 /= mm;
            
            x1 /= mm;
            y1 /= mm;
            z1 /= mm;

	    x3 /= mm; //************************
            y3 /= mm;//**********************
            z3 /= mm;//********************

            //th_pix2 = std::atan( (x1-x0) / (z1-z0) );
            //th_pix2 /= mrad;
            
            // only one element per track (particle) per sensitive detector is recorded with the index of each element
            // in the vector being equal to the track no. this could be changed such that multiple hits per track (particle)
            // per sensitive detector are recorded. hit->GetTrackNumber()) == (Track_no_pix2.back()) ensures the same element
            // is overwritten each time if the hit belongs to the same track (particle), except for energy which is not
            // overwritten but added up.
            
            // ensures each element represents a track, element [0] = track 1
            if( Track_no_pix2.size() > 0 && (hit->GetTrackNumber()) == (Track_no_pix2.back()) )
            {
                // Track_no_pix2.size()-1 means elements from different vectors correspond to the
                // same particle when the same element no. is chosen. For each events all vectors are same
                // size, this can be verified with print out statement above.
                ParticleName_pix2[Track_no_pix2.size()-1] = hit->GetParticleName();
		 ProcessName_pix2[Track_no_pix2.size()-1] = hit->GetProcessName(); 


                Track_no_pix2[Track_no_pix2.size()-1] = hit->GetTrackNumber();
                IsPrimaryParticle_pix2[Track_no_pix2.size()-1] = hit->GetIsPrimary();
                X_pos_pix2[Track_no_pix2.size()-1] = x1;
                Y_pos_pix2[Track_no_pix2.size()-1] = y1;
                Z_pos_pix2[Track_no_pix2.size()-1] = z1;

                Truth_X_pos_pix2[Track_no_pix2.size()-1] = x3;
                Truth_Y_pos_pix2[Track_no_pix2.size()-1] = y3;
                Truth_Z_pos_pix2[Track_no_pix2.size()-1] = z3;
                Edep_pix2[Track_no_pix2.size()-1] += edep;
                NI_Edep_pix2[Track_no_pix2.size()-1] += ni_edep;
                
                Truth_KE_pix2[Track_no_pix2.size()-1] = truth_KE;

            }
            else
            {
                //G4cout << "Track_no_pix2 = " << hit->GetTrackNumber() << G4endl;
                
                ParticleName_pix2.push_back(hit->GetParticleName());
		  ProcessName_pix2.push_back(hit->GetProcessName());

                Track_no_pix2.push_back(hit->GetTrackNumber());
                IsPrimaryParticle_pix2.push_back(hit->GetIsPrimary());
                X_pos_pix2.push_back(x1);
                Y_pos_pix2.push_back(y1);
                Z_pos_pix2.push_back(z1);

                Truth_X_pos_pix2.push_back(x3);
                Truth_Y_pos_pix2.push_back(y3);
                Truth_Z_pos_pix2.push_back(z3);
                Edep_pix2.push_back(edep);
                NI_Edep_pix2.push_back(ni_edep);
                Truth_KE_pix2.push_back(truth_KE);

            }
            
            Hit_mult_pix2 = X_pos_pix2.size();
            //G4int planeNum = hit->GetPlaneNumber();
        }
        
        
        //********************************** THIRD PIXEL MODULE **********************************//
        
        nHits = hits_pix3->entries();
        
        for ( G4int h = 0 ; h < nHits; ++h )
        {
            const SiHit_pix * hit = static_cast<const SiHit_pix*>( hits_pix3->GetHit( h ) );
            
            // Uncomment this line if you want to record only primary energy depositions
            //if ( hit->GetIsPrimary() == false ) continue;
            
            G4ThreeVector particle_pos = hit->GetPosition();
	    G4ThreeVector truth_pos = hit->GetTruth_Position();

            // We save energy in MeV
            Float_t edep = static_cast<Float_t>(hit->GetEdep());
            edep /= MeV;

            Float_t truth_KE = static_cast<Float_t>(hit->GetTruth_KE()); 
            truth_KE /= MeV;
            
            // We save energy in MeV (non-ionising energy loss (use for dose calculations))
            Float_t ni_edep = static_cast<Float_t>(hit->GetNonIonisingEdep());
            ni_edep /= MeV;
            
            x0 = static_cast<Float_t>(primPos.x());
            y0 = static_cast<Float_t>(primPos.y());
            z0 = static_cast<Float_t>(primPos.z());
            
            x1 = static_cast<Float_t>(particle_pos.x());
            y1 = static_cast<Float_t>(particle_pos.y());
            z1 = static_cast<Float_t>(particle_pos.z());
            
	     x3 = static_cast<Float_t>(truth_pos.x()); 
	     y3 = static_cast<Float_t>(truth_pos.y());
	     z3 = static_cast<Float_t>(truth_pos.z());
            //We save positions in mm (world coordinates)
            x0 /= mm;
            y0 /= mm;
            z0 /= mm;
            
            x1 /= mm;
            y1 /= mm;
            z1 /= mm;

	    x3 /= mm;
            y3 /= mm;
            z3 /= mm;
            //th_pix3 = std::atan( (x1-x0) / (z1-z0) );
            //th_pix3 /= mrad;
            
            // only one element per track (particle) per sensitive detector is recorded with the index of each element
            // in the vector being equal to the track no. this could be changed such that multiple hits per track (particle)
            // per sensitive detector are recorded. hit->GetTrackNumber()) == (Track_no_pix2.back()) ensures the same element
            // is overwritten each time if the hit belongs to the same track (particle), except for energy which is not
            // overwritten but added up.
            
            // ensures each element represents a track, element [0] = track 1
            if( Track_no_pix3.size() > 0 && (hit->GetTrackNumber()) == (Track_no_pix3.back()) )
            {
                // Track_no_pix3.size()-1 means elements from different vectors correspond to the
                // same particle when the same element no. is chosen. For each events all vectors are same
                // size, this can be verified with print out statement above.
                ParticleName_pix3[Track_no_pix3.size()-1] = hit->GetParticleName();
		 ProcessName_pix3[Track_no_pix3.size()-1] = hit->GetProcessName(); 


                Track_no_pix3[Track_no_pix3.size()-1] = hit->GetTrackNumber();
                IsPrimaryParticle_pix3[Track_no_pix3.size()-1] = hit->GetIsPrimary();
                X_pos_pix3[Track_no_pix3.size()-1] = x1;
                Y_pos_pix3[Track_no_pix3.size()-1] = y1;
                Z_pos_pix3[Track_no_pix3.size()-1] = z1;

		Truth_X_pos_pix3[Track_no_pix3.size()-1] = x3;
                Truth_Y_pos_pix3[Track_no_pix3.size()-1] = y3;
                Truth_Z_pos_pix3[Track_no_pix3.size()-1] = z3;
                Edep_pix3[Track_no_pix3.size()-1] += edep;
               
                Truth_KE_pix3[Track_no_pix3.size()-1] = truth_KE; 

                NI_Edep_pix3[Track_no_pix3.size()-1] += ni_edep;
                
            }
            else
            {
                //G4cout << "Track_no_pix3 = " << hit->GetTrackNumber() << G4endl;
                
                ParticleName_pix3.push_back(hit->GetParticleName());
		   ProcessName_pix3.push_back(hit->GetProcessName());

                Track_no_pix3.push_back(hit->GetTrackNumber());
                IsPrimaryParticle_pix3.push_back(hit->GetIsPrimary());
                X_pos_pix3.push_back(x1);
                Y_pos_pix3.push_back(y1);
                Z_pos_pix3.push_back(z1);

		Truth_X_pos_pix3.push_back(x3);
                Truth_Y_pos_pix3.push_back(y3);
                Truth_Z_pos_pix3.push_back(z3);

                Edep_pix3.push_back(edep);

                Truth_KE_pix3.push_back(truth_KE); 

                NI_Edep_pix3.push_back(ni_edep);
            }
            
            Hit_mult_pix3 = X_pos_pix3.size();
        }
        
        //********************************** FOURTH PIXEL MODULE **********************************//
        
        nHits = hits_pix4->entries();
        
        for ( G4int h = 0 ; h < nHits; ++h )
        {
            const SiHit_pix * hit = static_cast<const SiHit_pix*>( hits_pix4->GetHit( h ) );
            
            // Uncomment this line if you want to record only primary energy depositions
            //if ( hit->GetIsPrimary() == false ) continue;
            
            G4ThreeVector particle_pos = hit->GetPosition();
	    G4ThreeVector truth_pos = hit->GetTruth_Position();

            // We save energy in MeV
            Float_t edep = static_cast<Float_t>(hit->GetEdep());
            edep /= MeV;

            Float_t truth_KE = static_cast<Float_t>(hit->GetTruth_KE()); 
            truth_KE /= MeV;
            
            // We save energy in MeV (non-ionising energy loss (use for dose calculations))
            Float_t ni_edep = static_cast<Float_t>(hit->GetNonIonisingEdep());
            ni_edep /= MeV;
            
            x0 = static_cast<Float_t>(primPos.x());
            y0 = static_cast<Float_t>(primPos.y());
            z0 = static_cast<Float_t>(primPos.z());
            
            x1 = static_cast<Float_t>(particle_pos.x());
            y1 = static_cast<Float_t>(particle_pos.y());
            z1 = static_cast<Float_t>(particle_pos.z());

	     x3 = static_cast<Float_t>(truth_pos.x());
	    y3 = static_cast<Float_t>(truth_pos.y());
	    z3 = static_cast<Float_t>(truth_pos.z());

            //We save positions in mm (world coordinates)
            x0 /= mm;
            y0 /= mm;
            z0 /= mm;
            
            x1 /= mm;
            y1 /= mm;
            z1 /= mm;
            
            x3 /= mm;
            y3 /= mm;
            z3 /= mm;
            //th_pix4 = std::atan( (x1-x0) / (z1-z0) );
            //th_pix4 /= mrad;
            
            // only one element per track (particle) per sensitive detector is recorded with the index of each element
            // in the vector being equal to the track no. this could be changed such that multiple hits per track (particle)
            // per sensitive detector are recorded. hit->GetTrackNumber()) == (Track_no_pix2.back()) ensures the same element
            // is overwritten each time if the hit belongs to the same track (particle), except for energy which is not
            // overwritten but added up.
            
            // ensures each element represents a track, element [0] = track 1
            if( Track_no_pix4.size() > 0 && (hit->GetTrackNumber()) == (Track_no_pix4.back()) )
            {
                // Track_no_pix3.size()-1 means elements from different vectors correspond to the
                // same particle when the same element no. is chosen. For each events all vectors are same
                // size, this can be verified with print out statement above.
                ParticleName_pix4[Track_no_pix4.size()-1] = hit->GetParticleName();
		  ProcessName_pix4[Track_no_pix4.size()-1] = hit->GetProcessName(); 

                Track_no_pix4[Track_no_pix4.size()-1] = hit->GetTrackNumber();
                IsPrimaryParticle_pix4[Track_no_pix4.size()-1] = hit->GetIsPrimary();
                X_pos_pix4[Track_no_pix4.size()-1] = x1;
                Y_pos_pix4[Track_no_pix4.size()-1] = y1;
                Z_pos_pix4[Track_no_pix4.size()-1] = z1;

		Truth_X_pos_pix4[Track_no_pix4.size()-1] = x3; 
                Truth_Y_pos_pix4[Track_no_pix4.size()-1] = y3;
                Truth_Z_pos_pix4[Track_no_pix4.size()-1] = z3;
                Edep_pix4[Track_no_pix4.size()-1] += edep;

                Truth_KE_pix4[Track_no_pix4.size()-1] = truth_KE;

                NI_Edep_pix4[Track_no_pix4.size()-1] += ni_edep;
                
            }
            else
            {
                //G4cout << "Track_no_pix4 = " << hit->GetTrackNumber() << G4endl;
                
                ParticleName_pix4.push_back(hit->GetParticleName());
		    ProcessName_pix4.push_back(hit->GetProcessName()); 

                Track_no_pix4.push_back(hit->GetTrackNumber());
                IsPrimaryParticle_pix4.push_back(hit->GetIsPrimary());
                X_pos_pix4.push_back(x1);
                Y_pos_pix4.push_back(y1);
                Z_pos_pix4.push_back(z1);

		Truth_X_pos_pix4.push_back(x3); 
		Truth_Y_pos_pix4.push_back(y3);
		Truth_Z_pos_pix4.push_back(z3);
                Edep_pix4.push_back(edep);
                NI_Edep_pix4.push_back(ni_edep);

                Truth_KE_pix4.push_back(truth_KE); 


            }
            
            Hit_mult_pix4 = X_pos_pix4.size();
        }
        
        
    } //End of if(hits_pix1 || hits_pix2 .... ) condition
    

    else {G4cerr << "Error: No hits collection passed to RootSaver for this event" << G4endl;}

    Truth_x_pos = static_cast<Float_t>( primPos.x() );
    Truth_y_pos = static_cast<Float_t>( primPos.y() );
    Truth_z_pos = static_cast<Float_t>( primPos.z() );
    
    //Measure angle of the beam in xz plane measured from z+ direction
    // -pi<Theta<=pi (positive when close to x positive direction)
    /*
     Float_t sign_z = ( primMom.z()>= 0 ) ? +1 : -1;
     Float_t sign_x = ( primMom.x()>= 0 ) ? +1 : -1;
     TruthTheta = ( primMom.z() != 0 ) ?
     TMath::PiOver2()*sign_x*(1-sign_z)+std::atan( primMom.x()/primMom.z() ) : sign_x*TMath::PiOver2(); //beam perpendicular to z
     */
    
    TruthTheta_x = std::atan( primMom.x()/primMom.z() );
    TruthTheta_x /= mrad;
    
    TruthTheta_y = std::atan( primMom.y()/primMom.z() );
    TruthTheta_y /= mrad;
    
    rootTree_pixel->Fill();
}

