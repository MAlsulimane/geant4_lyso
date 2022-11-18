#include "SiDigi_pix.hh"
#include "SiHit_pix.hh"
#include "SiDigitizer_pix.hh"
#include "EventAction.hh"
#include "RootSaver.hh"
#include "SiDigi.hh"
#include "SiHit.hh"
#include "SiDigitizer.hh"
#include "DetectorConstruction.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4Event.hh"

#include "G4TrackingManager.hh"
#include "G4EventManager.hh"
#include "G4VParticleChange.hh"
#include "G4DynamicParticle.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"



EventAction::EventAction(DetectorConstruction* myDC) :
	rootSaver(0),
    // N.B. these names must match those given for SD's
    // in DetectorConstruction.cc
    hitsCollName_x1("SiStrip_x1") ,
    hitsCollName_u1("SiStrip_u1") ,
    hitsCollName_v1("SiStrip_v1") ,
    hitsCollName_pix1("SiPixel_pix1") ,
    hitsCollName_pix2("SiPixel_pix2") ,
    hitsCollName_pix3("SiPixel_pix3") ,
    hitsCollName_pix4("SiPixel_pix4") ,

	digitsCollName("SiDigitCollection"),
    digitsCollName_pix("SiDigitCollection_pix"),
    myDetector(myDC)
{
    //G4cout << "\nInitialising Event.. " << G4endl;
    
    
	//We build the digitization module
	SiDigitizer* digitizer = new SiDigitizer("SiDigitizer",myDetector->Get_nb_of_strips(),myDetector->Get_nb_of_planes());
	G4DigiManager * digiManager = G4DigiManager::GetDMpointer();
	digiManager->AddNewModule( digitizer );
    
    //We build the digitization module
    SiDigitizer_pix* digitizer_pix = new SiDigitizer_pix("SiDigitizer_pix",myDetector->Get_nb_of_pixels(),myDetector->Get_nb_of_pix_planes());
//    G4DigiManager * digiManager_pix = G4DigiManager::GetDMpointer();
//    digiManager_pix->AddNewModule( digitizer_pix );
    digiManager->AddNewModule( digitizer_pix );
    
//    G4cout << "\ndigiManager->List(): " << G4endl;
//    digiManager->List();
//    G4cout << "\ndigiManager->GetCollectionCapacity(): " << G4endl;
//    digiManager->GetCollectionCapacity();
    
}


void EventAction::BeginOfEventAction(const G4Event* anEvent )
{
	//if ( anEvent->GetEventID() % 1000 == 0 )
	//{
		G4cout << "\nStarting Event: " << anEvent->GetEventID() << G4endl;
	//}
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
    //G4cout << "\nEnding Event: " << anEvent->GetEventID() << G4endl;
	
    // Digitize!!
	G4DigiManager * digiManager = G4DigiManager::GetDMpointer();
    
	SiDigitizer* digiModule = static_cast<SiDigitizer*>( digiManager->FindDigitizerModule("SiDigitizer") );
    SiDigitizer_pix* digiModule_pix = static_cast<SiDigitizer_pix*>( digiManager->FindDigitizerModule("SiDigitizer_pix") );

    if(anEvent->GetEventID() == 0) // only necessary to reset the value once at first event
    {
        // Used for re-setting the no. of planes and strips in case they have been changed from the default
        // vaules set in DetectorConstruction.cc by the .mac file. Necessary because DetectorConsturcion object with
        // default parameters is passed to EventAction before the .mac file is processed. EventAction then uses the
        // DetectorConstruction object to construct root file with array elements = to the defined no. of strips.
        // This then creates an error if the .mac file alters these parameters and recompiles the DetectorConstruction object.
        digiModule->ReSetDigiCollectionStrips( myDetector->Get_nb_of_strips() );
        digiModule->ReSetDigiCollectionPlanes( myDetector->Get_nb_of_planes() );
        
        digiModule_pix->ReSetDigiCollectionPixels( myDetector->Get_nb_of_pixels() );
        digiModule_pix->ReSetDigiCollectionPlanes( myDetector->Get_nb_of_pix_planes() );
    }
    
	if ( digiModule )       {digiModule->Digitize();}
    if ( digiModule_pix )   {digiModule_pix->Digitize();}

	//Store information from strip detectors
	if ( rootSaver && myDetector->Get_build_strip_detectors() )
	{
		//Retrieve digits collection
		static G4int digiCollID = -1;
		if ( digiCollID < 0 ) {digiCollID = digiManager->GetDigiCollectionID( digitsCollName );}
		
        G4DCofThisEvent* digitsCollections = anEvent->GetDCofThisEvent();
		SiDigiCollection* digits = 0;
		if ( digitsCollections ) {digits = static_cast<SiDigiCollection*>( digitsCollections->GetDC(digiCollID) );}
        
		//Retrieve hits collections
		static G4int hitsCollID_x1 = -1;
		static G4int hitsCollID_u1 = -1;
		static G4int hitsCollID_v1 = -1;
        
		if ( hitsCollID_x1 < 0 )
		{
			// Also the following line works:
			// hitsCollID_1 = digiManager->GetHitsCollectionID(hitscollname_SD1);
			// But we prefer to show here a (longer) version, in case no digiManager is used
			G4SDManager * SDman = G4SDManager::GetSDMpointer();
            
			hitsCollID_x1 = SDman->GetCollectionID(hitsCollName_x1);
			hitsCollID_u1 = SDman->GetCollectionID(hitsCollName_u1);
			hitsCollID_v1 = SDman->GetCollectionID(hitsCollName_v1);
		}
        
		G4HCofThisEvent* HCE = anEvent->GetHCofThisEvent();
        
        //int n_hit = HCE->GetCapacity();
        //G4cout << "x1 has " << n_hit << " hits." << G4endl;
        
		SiHitCollection* hits_x1 = 0;
		SiHitCollection* hits_u1 = 0;
		SiHitCollection* hits_v1 = 0;
        
		if ( HCE )
		{
            //G4cout << "about to static cast for hitsCollID_x1 = " << hitsCollID_x1 << ", for hitsCollName_x1 = " << hitsCollName_x1 << G4endl;
			hits_x1 = static_cast<SiHitCollection*>( HCE->GetHC(hitsCollID_x1) );
			hits_u1 = static_cast<SiHitCollection*>( HCE->GetHC(hitsCollID_u1) );
			hits_v1 = static_cast<SiHitCollection*>( HCE->GetHC(hitsCollID_v1) );
		}
        
        //Enable for printouts
        
        
//        if(hits_x1)
//        {
//            int n_hit = hits_x1->entries();
//            G4cout << "x1 has " << n_hit << " hits." << G4endl;
//            for(int i1=0;i1<n_hit;i1++)
//            {
//                SiHit* aHit = (*hits_x1)[i1];
//                if(aHit->GetNonIonisingEdep()>0) aHit->Print();
//            }
//        }
//        
//        
//        if(hits_u1)
//        {
//            int n_hit = hits_u1->entries();
//            G4cout << "u1 has " << n_hit << " hits." << G4endl;
//            for(int i1=0;i1<n_hit;i1++)
//            {
//                SiHit* aHit = (*hits_u1)[i1];
//                aHit->Print();
//            }
//        }
//        
//        if(hits_v1)
//        {
//            int n_hit = hits_v1->entries();
//            G4cout << "v1 has " << n_hit << " hits." << G4endl;
//            for(int i1=0;i1<n_hit;i1++)
//            {
//                SiHit* aHit = (*hits_v1)[i1];
//                aHit->Print();
//            }
//        }
        
        
        
		//Get Postion and Momentum of primary
		//This is needed to write to root file info @ z=0
		const G4ThreeVector& pos = anEvent->GetPrimaryVertex()->GetPosition();
		const G4ThreeVector& mom = anEvent->GetPrimaryVertex()->GetPrimary()->GetMomentum();
        const G4int event = anEvent->GetEventID();
        
        //Getting KE at entrance and exit..
        //G4float KE_out = 0; //now set in Rootsaver.cc using KE of primary hit in last tracking detector. 
        const G4float KE_in = anEvent->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();
        
        
        // Save event to root file
		if( myDetector->Get_build_strip_detectors() )
        {rootSaver->AddEvent_strip_det(event, hits_x1, hits_u1, hits_v1, digits, pos, mom, KE_in/*,KE_out*/);} // initial/final particle info
        
		// Print information about hits. Hits container has a method:
        // G4VHitsCollection::PrintAllHits() that
		// recursively calls G4VHit::Print() method
		
		//hits_x1->PrintAllHits();
		//hits_u1->PrintAllHits();
		//hits_v1->PrintAllHits();
    
        // Printout disables for SiDigi now as noise on each strip for each event results in huge printout
        //digits->PrintAllDigi();

	}
    
    //Store information from strip detectors
    if ( rootSaver && myDetector->Get_build_pixel_detectors() )
    {
        //Retrieve digits collection
        static G4int digiCollID = -1;
        if ( digiCollID < 0 ) {digiCollID = digiManager->GetDigiCollectionID( digitsCollName_pix );}
        
        G4DCofThisEvent * digitsCollections = anEvent->GetDCofThisEvent();
        SiDigi_pixCollection * digits = 0;
        if ( digitsCollections ) {digits = static_cast<SiDigi_pixCollection*>( digitsCollections->GetDC(digiCollID) );}
        
        //Retrieve hits collections
        static G4int hitsCollID_pix1 = -1;
        static G4int hitsCollID_pix2 = -1;
        static G4int hitsCollID_pix3 = -1;
        static G4int hitsCollID_pix4 = -1;
        
        if ( hitsCollID_pix1 < 0 )
        {
            // Also the following line works:
            // hitsCollID_1 = digiManager->GetHitsCollectionID(hitscollname_SD1);
            // But we prefer to show here a (longer) version, in case no digiManager is used
            G4SDManager * SDman = G4SDManager::GetSDMpointer();
            //G4cout << "\nHere's a list of the names of SD's registered: " << G4endl; SDman->ListTree();
            hitsCollID_pix1 = SDman->GetCollectionID(hitsCollName_pix1);
            hitsCollID_pix2 = SDman->GetCollectionID(hitsCollName_pix2);
            hitsCollID_pix3 = SDman->GetCollectionID(hitsCollName_pix3);
            hitsCollID_pix4 = SDman->GetCollectionID(hitsCollName_pix4);
            
            //G4cout << "\nThe name and ID feched were hitsCollName_pix1: " << hitsCollName_pix1;
            //G4cout << " and hitsCollID_pix1: " << hitsCollID_pix1 << G4endl;
        }
        
        G4HCofThisEvent* HCE = anEvent->GetHCofThisEvent();
        //G4cout << "\nHCE->GetNumberOfCollections() =  " << HCE->GetNumberOfCollections() << "\n" << G4endl;
        //G4cout << "\npix1 (HCE->GetCapacity()) has " << HCE->GetCapacity() << " hits.\n" << G4endl;
        
        SiHit_pixCollection * hits_pix1 = 0;
        SiHit_pixCollection * hits_pix2 = 0;
        SiHit_pixCollection * hits_pix3 = 0;
        SiHit_pixCollection * hits_pix4 = 0;

        if ( HCE )
        {
            //G4cout << "about to static cast for hitsCollID_pix1 = " << hitsCollID_pix1;
            //G4cout << ", for hitsCollName_pix1 = " << hitsCollName_pix1 << G4endl;
            hits_pix1 = static_cast<SiHit_pixCollection*>( HCE->GetHC(hitsCollID_pix1) );
            hits_pix2 = static_cast<SiHit_pixCollection*>( HCE->GetHC(hitsCollID_pix2) );
            hits_pix3 = static_cast<SiHit_pixCollection*>( HCE->GetHC(hitsCollID_pix3) );
            hits_pix4 = static_cast<SiHit_pixCollection*>( HCE->GetHC(hitsCollID_pix4) );
        }
        
        //Enable for printouts
        
//        if(hits_pix1)
//        {
//            int n_hit = hits_pix1->entries();
//            G4cout << "pix1 has " << n_hit << " hits." << G4endl;
//            //if(n_hit > 0) {exit(-1);}
//            for(int i1=0;i1<n_hit;i1++)
//            {
//                SiHit_pix * aHit = (*hits_pix1)[i1];
//                /*if(aHit->GetNonIonisingEdep()>0)*/ aHit->Print();
//            }
//        }
//
//
//        if(hits_pix2)
//        {
//            int n_hit = hits_pix2->entries();
//            G4cout << "pix2 has " << n_hit << " hits." << G4endl;
//            for(int i1=0;i1<n_hit;i1++)
//            {
//                SiHit_pix * aHit = (*hits_pix2)[i1];
//                aHit->Print();
//            }
//        }

        
        // Get Postion and Momentum of primary
        // This is needed to write to root file info @ z=0
        const G4ThreeVector& pos = anEvent->GetPrimaryVertex()->GetPosition();
        const G4ThreeVector& mom = anEvent->GetPrimaryVertex()->GetPrimary()->GetMomentum();
        const G4int event = anEvent->GetEventID();

        // Getting KE at entrance and exit..
        //G4float KE_out = 0; //now set in Rootsaver.cc using KE of primary hit in last tracking detector.
        const G4float KE_in = anEvent->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();

        // Save event to root file
        if( myDetector->Get_build_pixel_detectors() )
        {rootSaver->AddEvent_pixel_det(event, hits_pix1, hits_pix2, hits_pix3, hits_pix4, digits, pos, mom, KE_in/*,KE_out*/);} // initial/final particle info
        
        // Print information about hits. Hits container has a method:
        // G4VHitsCollection::PrintAllHits() that
        // recursively calls G4VHit::Print() method
        
        //hits_pix1->PrintAllHits();
        //hits_pix2->PrintAllHits();
        //hits_pix3->PrintAllHits();
        //hits_pix4->PrintAllHits();
        
        // Printout disables for SiDigi now as noise on each strip for each event results in huge printout
        //digits->PrintAllDigi();
        
    }

}

