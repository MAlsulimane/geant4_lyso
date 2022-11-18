
#include <assert.h>
#include <list>
#include <map>
#include <iostream>
#include "TrackerGeometry.hh"
#include "DiffusionGenerator.hh"
#include "SiDigi_pix.hh"
#include "SiDigitizer_pix.hh"
#include "SiHit_pix.hh"
#include "NoiseGenerator.hh"
#include "MeV2ChargeConverter.hh"
#include "CrosstalkGenerator.hh"

#include "G4DigiManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


//Configuration of digitization, digitization requires several components:
SiDigitizer_pix::SiDigitizer_pix(G4String aName, G4int pixel, G4int pl) :
  G4VDigitizerModule(aName) ,
  //These two are names for digits and hits collections
  digiCollectionName("SiDigitCollection_pix") ,

  hitsCollName_pix1("SiPixel_pix1") ,
  hitsCollName_pix2("SiPixel_pix2") ,
  hitsCollName_pix3("SiPixel_pix3") ,
  hitsCollName_pix4("SiPixel_pix4") ,

  digiCollectionPixels(pixel),
  digiCollectionPlanes(pl),

  // 1 - A pedestal level
  //pedestal(5000.) ,
  pedestal(0.0) ,

  // 2 - A noise generator: a simple gaussian noise
  // Noise standard deviation is 1000 e
  // To turn it off put a value <0
  //noise( 1000. ) ,
  noise( 0.0 ) ,

  // 3 - MeV2Charge converter: converts energy deposits from MeV to Q
  // It needs a parameter: the MeV2Q conversion factor: 3.6 eV/e.
  convert( 1./(3.6*eV) ) //,

  // 4 - Crosstalk Generator:
  // Cross talk needs fraction of charge that leaks.
  // To turn it off set it to 0
  //crosstalk( 0.05 ),
  //crosstalk( 0.0 ),

  // 6 - Charge Diffusion Generator
  // MakeDiffusion function uses these object to implement charge diffusion.
  // needs bias voltage and detector thickness
  //diffusion( 50.0 ),
  //diffusion( 30.0 ),

  // 5 - tracker geometry object used to find detector and geometry parameters for charge diffusion
  //tracker( std::string("/Users/jontaylor/g4_work/silicon_telescope-build/tracker_geom.mac") ),

  //UI cmds
  //messenger(this)
{
	collectionName.push_back( digiCollectionName );
}

void SiDigitizer_pix::Digitize()
{
  // First we create a digits collection...
  SiDigi_pixCollection * digiCollection = new SiDigi_pixCollection("SiDigitizer_pix",digiCollectionName);
  // Create a empty collection with one digit for each pixel
  const G4int numPlanes = digiCollectionPlanes;  // Number of Si detectors
  const G4int numPixels = digiCollectionPixels;  // Number of pixel per plane

  // The following matrix is used to map: (plane,pixel) to
  // its corresponding digit.
  // Example plane = 1 , pixel = 10
  // Digi* theDigi = digitsMap[plane][pixel]
  // Initialize with NULL pointers
  std::vector< std::vector<SiDigi_pix*> > digitsMap(numPlanes);
  for ( G4int i = 0 ; i < numPlanes ; ++i )
  {
	  digitsMap[i].resize(numPixels,static_cast<SiDigi_pix*>(0));
  }

 
  // Create empty digits
  //G4cout << "\nnumPlanes = " << numPlanes << ", numPixels = " << numPixels << "\n" << G4endl;
  for ( G4int plane = 0 ; plane < numPlanes ; ++plane ) {
    for ( G4int pixel = 0 ; pixel < numPixels ; ++pixel )
      {
        SiDigi_pix* newDigi = new SiDigi_pix(plane,pixel);
        // Remember the hit so we can find it by plane,pixel
        digitsMap[plane][pixel] = newDigi;
        // Now insert the digit in the digit collection
        digiCollection->insert(newDigi);
      }
  }
  // We can now simulate the electronic circuit.

  //We search and retrieve the hits collection
  G4DigiManager* digMan = G4DigiManager::GetDMpointer();
    
  G4int SiHitCollID_pix1 = digMan->GetHitsCollectionID( hitsCollName_pix1 ); // Number associated to hits collection names hitsCollName
  G4int SiHitCollID_pix2 = digMan->GetHitsCollectionID( hitsCollName_pix2 ); // Number associated to hits collection names hitsCollName
  G4int SiHitCollID_pix3 = digMan->GetHitsCollectionID( hitsCollName_pix3 ); // Number associated to hits collection names hitsCollName
  G4int SiHitCollID_pix4 = digMan->GetHitsCollectionID( hitsCollName_pix4 ); // Number associated to hits collection names hitsCollName
    
  const SiHit_pixCollection* hitCollection_pix1 = static_cast<const SiHit_pixCollection*>(digMan->GetHitsCollection(SiHitCollID_pix1));
  const SiHit_pixCollection* hitCollection_pix2 = static_cast<const SiHit_pixCollection*>(digMan->GetHitsCollection(SiHitCollID_pix2));
  const SiHit_pixCollection* hitCollection_pix3 = static_cast<const SiHit_pixCollection*>(digMan->GetHitsCollection(SiHitCollID_pix3));
  const SiHit_pixCollection* hitCollection_pix4 = static_cast<const SiHit_pixCollection*>(digMan->GetHitsCollection(SiHitCollID_pix4));

  //********************************** FIRST TRACKER MODULE **********************************//
    
    if(hitCollection_pix1)
    {
      //G4cout << "Number of hits in pix1: " << hitCollection_pix1->entries() << G4endl;
      //G4cout << "\n" << G4endl;
	  for ( G4int i = 0 ; i < hitCollection_pix1->entries() ; ++i )
        {
            //For each Hit get which pixel it belongs to and convert its edep into charge units
            SiHit_pix* aHit = (*hitCollection_pix1)[i];
            //if ( hit->GetIsPrimary() == false ) continue;   //Un-comment primary energy depositions only
            G4int hitPlane = aHit->GetPlaneNumber();
            G4int hitPixel = aHit->GetPixelNumber();
            G4double edep = aHit->GetEdep();
            G4double charge = convert( edep/MeV );
            /*G4cout << "pix1 The plane from aHit  = " << hitPlane+1;
            G4cout << ", pix1 The pixel from aHit  = " << hitPixel;
            G4cout << ", pix1 The charge from aHit  = " << charge << G4endl;*/
            digitsMap.at(hitPlane).at(hitPixel)->Add(charge);
            
            //This will effectivly set the SiDigi_pix hit_pos as the position of the last sensitive detector hit in the collection
            //This should be accurate enough for charge diffusion as all hits will be in close proximity provided they occur on
            //the same pixel. G4ThreeVector converted to TVector3 to allow processing by MakeDiffusion function.
            digitsMap.at(hitPlane).at(hitPixel)->SetPos(TVector3(aHit->GetPosition().x(),aHit->GetPosition().y(),aHit->GetPosition().z()));
        }
    }
    
    
    //********************************** SECOND TRACKER MODULE **********************************//
    
    if(hitCollection_pix2)
    {
        //G4cout << "Number of hits in pix2: " << hitCollection_pix2->entries() << G4endl;
        //G4cout << "\n" << G4endl;
        for ( G4int i = 0 ; i < hitCollection_pix2->entries() ; ++i )
        {
            //G4cout << "Digitise stage 0"<< G4endl;
            //For each Hit get which pixel it belongs to and convert its edep into charge units
            SiHit_pix* aHit = (*hitCollection_pix2)[i];  //G4cout << "Digitise stage 1"<< G4endl;
            //if ( hit->GetIsPrimary() == false ) continue;   //Un-comment primary energy depositions only
            G4int hitPlane = aHit->GetPlaneNumber(); //G4cout << "Digitise stage 2"<< G4endl;
            //G4cout << "u1 The plane from aHit  = " << hitPlane << G4endl;
            G4int hitPixel = aHit->GetPixelNumber();  //G4cout << "Digitise stage 3"<< G4endl;
            G4double edep = aHit->GetEdep(); //G4cout << "Digitise stage 4"<< G4endl;
            G4double charge = convert( edep/MeV ); //G4cout << "Digitise stage 5"<< G4endl;
            /*G4cout << "pix2 The plane from aHit  = " << hitPlane+1;
            G4cout << ", pix2 The pixel from aHit  = " << hitPixel;
            G4cout << ", pix2 The charge from aHit  = " << charge << G4endl;*/
            digitsMap.at(hitPlane).at(hitPixel)->Add(charge); //G4cout << "Digitise stage 6"<< G4endl;
            
            //This will effectivly set the SiDigi_pix hit_pos as the position of the last sensitive detector hit in the collection
            //This should be accurate enough for charge diffusion as all hits will be in close proximity provided they occur on
            //the same pixel. G4ThreeVector converted to TVector3 to allow processing by MakeDiffusion function.
            digitsMap.at(hitPlane).at(hitPixel)->SetPos(TVector3(aHit->GetPosition().x(),aHit->GetPosition().y(),aHit->GetPosition().z()));
        }
    }
    
    
    //********************************** THIRD TRACKER MODULE **********************************//
    
    if(hitCollection_pix3)
    {
        //G4cout << "Number of hits in pix3: " << hitCollection_pix3->entries() << G4endl;
        //G4cout << "\n" << G4endl;
        for ( G4int i = 0 ; i < hitCollection_pix3->entries() ; ++i )
        {
            //G4cout << "Digitise stage 0"<< G4endl;
            //For each Hit get which pixel it belongs to and convert its edep into charge units
            SiHit_pix* aHit = (*hitCollection_pix3)[i];
            //if ( hit->GetIsPrimary() == false ) continue;   //Un-comment primary energy depositions only
            G4int hitPlane = aHit->GetPlaneNumber();
            G4int hitPixel = aHit->GetPixelNumber();
            G4double edep = aHit->GetEdep();
            G4double charge = convert( edep/MeV );
            digitsMap.at(hitPlane).at(hitPixel)->Add(charge);
            
            //This will effectivly set the SiDigi_pix hit_pos as the position of the last sensitive detector hit in the collection
            //This should be accurate enough for charge diffusion as all hits will be in close proximity provided they occur on
            //the same pixel. G4ThreeVector converted to TVector3 to allow processing by MakeDiffusion function.
            digitsMap.at(hitPlane).at(hitPixel)->SetPos(TVector3(aHit->GetPosition().x(),aHit->GetPosition().y(),aHit->GetPosition().z()));
        }
    }
    
    //********************************** FOURTH TRACKER MODULE **********************************//
    
    if(hitCollection_pix4)
    {
        //G4cout << "Number of hits in pix4: " << hitCollection_pix4->entries() << G4endl;
        //G4cout << "\n" << G4endl;
        for ( G4int i = 0 ; i < hitCollection_pix4->entries() ; ++i )
        {
            //G4cout << "Digitise stage 0"<< G4endl;
            //For each Hit get which pixel it belongs to and convert its edep into charge units
            SiHit_pix* aHit = (*hitCollection_pix4)[i];
            //if ( hit->GetIsPrimary() == false ) continue;   //Un-comment primary energy depositions only
            G4int hitPlane = aHit->GetPlaneNumber();
            G4int hitPixel = aHit->GetPixelNumber();
            G4double edep = aHit->GetEdep();
            G4double charge = convert( edep/MeV );
            digitsMap.at(hitPlane).at(hitPixel)->Add(charge);
            
            //This will effectivly set the SiDigi_pix hit_pos as the position of the last sensitive detector hit in the collection
            //This should be accurate enough for charge diffusion as all hits will be in close proximity provided they occur on
            //the same pixel. G4ThreeVector converted to TVector3 to allow processing by MakeDiffusion function.
            digitsMap.at(hitPlane).at(hitPixel)->SetPos(TVector3(aHit->GetPosition().x(),aHit->GetPosition().y(),aHit->GetPosition().z()));
        }
    }
    
  //*******************************************************************************************//
    
  if(!hitCollection_pix1 && !hitCollection_pix2 && !hitCollection_pix3 && !hitCollection_pix4)
    {
      G4cerr << "Could not find SiHit_pix collection" << G4endl;         //Something really bad happened...
    }

  //Important: crosstalk and charge diffusion should be simulated
  //before noise and pedestal is added

  //MakeCrosstalk( digitsMap );   //Simulate the crosstalk
  
  //MakeDiffusion( digitsMap );   //Simulate the charge diffusion
    
  //We can now add, for each pixel the noise and pedestal values
  //G4cout << "The no. of digits in the collection is: " << digiCollection->GetSize() << G4endl;
  for ( size_t d = 0 ; d < digiCollection->GetSize(); ++d )
  {
	  SiDigi_pix * digi = static_cast< SiDigi_pix* >( digiCollection->GetDigi(d) );
	  //First we add a pedestal
	  digi->Add( pedestal );

	  //Then we smear for the noise
	  digi->Add( noise() );

	  //Debug Output
	  //G4cout << "Digit :" << d <<" ";
	  //digi->Print();

  }

  //This line is very important,
  //differently from hits we need to store the digits
  //for each event explicitly.
  //This actually gives us quite a lot of flexibility
  //For example it is possible to simulate a malfunctioning
  //detector: you can comment this line and the digits
  //will not be available. Actually this example can be useful
  //for example if you want to study the effect of "dead" channels
  //in the physics measurement you perform.
  StoreDigiCollection(digiCollection);
}

/*
void SiDigitizer_pix::MakeCrosstalk(std::vector< std::vector< SiDigi* > >& digitsMap )
{
	//We have to make some conversions:
	//1- Take the digits of plane 2: we make crosstalk only for the second plane
	//2- Make a HepVector of the collected charges, ordered by pixel number
	//3- Apply transformation
	//4- Update digits with the new charge
	int plane = 1;//xtalk only on middle plane (DUT)
	std::vector< SiDigi* >& thisPlane = digitsMap[plane];
	size_t numStrips = thisPlane.size();
	size_t strip = 0;
	CLHEP::HepVector in(numStrips);
	for ( ; strip<numStrips;++strip )
	{
		in[strip] = thisPlane[strip]->GetCharge();
	}
	CLHEP::HepVector out = crosstalk( in );         //calls virtual CLHEP::HepVector operator() which returns xtalk*input
	for ( strip=0; strip < numStrips ; ++strip )
	{
		thisPlane[strip]->SetCharge( out[strip] );
	}
}
*/

//void SiDigitizer_pix::MakeDiffusion(std::vector< std::vector< SiDigi* > >& digitsMap )
//{
//    double *lim;
//    double limits[2], dist_to_hit, charge_frac, seed_charge;
//    TVector3 strip_origin, det_origin, det_opp_origin;
//    
//    for(int hit_plane = 0; hit_plane < digiCollectionPlanes; ++hit_plane)
//    {
//        std::vector< SiDigi* >& thisPlane = digitsMap[hit_plane]; //could use digitsMap directy but this is slightly faster
//        
//        for (int strip=1; strip <= tracker.get_strips(); ++strip )
//        {
//            seed_charge = thisPlane[strip-1]->GetCharge();
//            
//            //All strips have default initialised hit position of (0,0,z), thus if a strip contains charge but x = y = 0 then
//            //we know that it has no hit on it and has recieved charge from diffusion. this prevents us from diffusing charge
//            //that is present from charge diffusion, ie we ensure charge diffusion only occurs away from the hit not towards it.
//            if(seed_charge>0 && thisPlane[strip-1]->GetPos().x() != 0 && thisPlane[strip-1]->GetPos().y() != 0)
//            {
//                //Integrate over ONE strip either side of hit strip
//                //Set integration limits for charge diffusion, one strip either side of
//                //seed should be sufficient since diffusion sigma is only 5-6um. therefore +/- pitch is equal to +/- 13sigma!
//                int s_rng_l = ( strip > 1 ) ? (strip - 1) : 1;
//                int s_rng_h = ( strip < tracker.get_strips() ) ? (strip + 1) : tracker.get_strips();
//                
//                //Integrate over TWO strips either side of hit strip
//                //int s_rng_l = ( strip > 2 ) ? (strip - 2) : 1;
//                //int s_rng_h = ( strip < tracker.get_strips() - 1 ) ? (strip + 2) : tracker.get_strips();
//                
//                //For debug
//                /*G4cout << "*************************************************************************\n" << G4endl;
//                G4cout << "Before charge diffusion.." << " plane = " << hit_plane + 1 << ", seed strip = " << strip << G4endl;
//                G4cout << "charge on seed strip - 1 = " << thisPlane[strip-2]->GetCharge() << " e-" << G4endl;
//                G4cout << "charge on seed strip = " << seed_charge << " e- (SEED strip)" << G4endl;
//                G4cout << "charge on seed strip + 1 = " << thisPlane[strip]->GetCharge() << " e-\n" << G4endl;*/
//                
//                for(int i=s_rng_l; i<=s_rng_h; i++)
//                {
//                    switch (hit_plane)
//                    {
//                        case 0:
//                            //cout << "for x1"<< endl;
//                            strip_origin = tracker.get_strip_origin(tracker.get_A_x1(), i , tracker.get_stereo_x1());
//                            det_origin = tracker.get_A_x1();
//                            det_opp_origin = tracker.get_D_x1();
//                            break;
//                            
//                        case 1:
//                            //cout << "for u1"<< endl;
//                            strip_origin = tracker.get_strip_origin(tracker.get_A_u1(), i , tracker.get_stereo_u1());
//                            det_origin = tracker.get_A_u1();
//                            det_opp_origin = tracker.get_D_u1();
//                            break;
//                            
//                        case 2:
//                            //cout << "for v1"<< endl;
//                            strip_origin = tracker.get_strip_origin(tracker.get_A_v1(), i , tracker.get_stereo_v1());
//                            det_origin = tracker.get_A_v1();
//                            det_opp_origin = tracker.get_D_v1();
//                            break;
//                            
//                        case 3:
//                            //cout << "for x2"<< endl;
//                            strip_origin = tracker.get_strip_origin(tracker.get_A_x2(), i , tracker.get_stereo_x2());
//                            det_origin = tracker.get_A_x2();
//                            det_opp_origin = tracker.get_D_x2();
//                            break;
//                            
//                        case 4:
//                            //cout << "for u2"<< endl;
//                            strip_origin = tracker.get_strip_origin(tracker.get_A_u2(), i , tracker.get_stereo_u2());
//                            det_origin = tracker.get_A_u2();
//                            det_opp_origin = tracker.get_D_u2();
//                            break;
//                            
//                        case 5:
//                            //cout << "for v2"<< endl;
//                            strip_origin = tracker.get_strip_origin(tracker.get_A_v2(), i , tracker.get_stereo_v2());
//                            det_origin = tracker.get_A_v2();
//                            det_opp_origin = tracker.get_D_v2();
//                            break;
//                            
//                        case 6:
//                            //cout << "for TP1" << endl; //No charge sharing for truth planes
//                            break;
//                            
//                        case 7:
//                            //cout << "for TP2" << endl; //No charge sharing for truth planes
//                            break;
//                            
//                        case 8:
//                            //cout << "for TP3" << endl; //No charge sharing for truth planes
//                            break;
//                            
//                        case 9:
//                            //cout << "for TP4" << endl; //No charge sharing for truth planes
//                            break;
//
//                        case 10:
//                            //cout << "for x3"<< endl;
//                            strip_origin = tracker.get_strip_origin(tracker.get_A_x3(), i , tracker.get_stereo_x3());
//                            det_origin = tracker.get_A_x3();
//                            det_opp_origin = tracker.get_D_x3();
//                            break;
//                            
//                        case 11:
//                            //cout << "for u3"<< endl;
//                            strip_origin = tracker.get_strip_origin(tracker.get_A_u3(), i , tracker.get_stereo_u3());
//                            det_origin = tracker.get_A_u3();
//                            det_opp_origin = tracker.get_D_u3();
//                            break;
//                            
//                        case 12:
//                            //cout << "for v3"<< endl;
//                            strip_origin = tracker.get_strip_origin(tracker.get_A_v3(), i , tracker.get_stereo_v3());
//                            det_origin = tracker.get_A_v3();
//                            det_opp_origin = tracker.get_D_v3();
//                            break;
//                            
//                        case 13:
//                            //cout << "for x4"<< endl;
//                            strip_origin = tracker.get_strip_origin(tracker.get_A_x4(), i , tracker.get_stereo_x4());
//                            det_origin = tracker.get_A_x4();
//                            det_opp_origin = tracker.get_D_x4();
//                            break;
//                            
//                        case 14:
//                            //cout << "for u4"<< endl;
//                            strip_origin = tracker.get_strip_origin(tracker.get_A_u4(), i , tracker.get_stereo_u4());
//                            det_origin = tracker.get_A_u4();
//                            det_opp_origin = tracker.get_D_u4();
//                            break;
//                            
//                        case 15:
//                            //cout << "for v4"<< endl;
//                            strip_origin = tracker.get_strip_origin(tracker.get_A_v4(), i , tracker.get_stereo_v4());
//                            det_origin = tracker.get_A_v4();
//                            det_opp_origin = tracker.get_D_v4();
//                            break;
//                            
//                        case 16:
//                            //cout << "for TP5" << endl; //No charge sharing for truth planes
//                            break;
//                            
//                        default:
//                            G4cout << "plane does not exist for charge sharing!" << G4endl;
//                            break;
//                    }
//                    
//                    //Exclude the truth planes or their charge will be reduced to zero by the charge sharing functions below
//                    //this will result in no strip numbers being recorded for the hits by the RootSaver object
//                    if(hit_plane != 6 && hit_plane != 7 && hit_plane != 8 && hit_plane != 9 && hit_plane != 16)
//                    {
//                        //Find distance from detector origin to hit so that integral limits set by strip boundaries can be found in detector co-ordinates
//                        //ie integration limits will be independant of detector orientaion/rotation. Strips that contain charge but no hit ie strips that
//                        //have charge diffused onto them from a hit strip will be processed but have a hit pos = 0.0 giving a dist_to_hit = det_size/2
//                        //for which the erf() will nearly always return zero preventing the charge gained by diffusion from being diffused again.
//                        dist_to_hit = tracker.world_2_det_transform(det_origin, det_opp_origin, digitsMap[hit_plane][strip-1]->GetPos());
//                        lim = diffusion.get_igral_limits(limits,tracker.world_2_det_transform(det_origin, det_opp_origin, strip_origin),
//                                                                 dist_to_hit,tracker.get_s_pitch());
//                        
//                        //Modify the charge distribution on each strip
//                        charge_frac = diffusion.get_charge_frac(lim);
//                        //thisPlane[i-1]->SetCharge( thisPlane[i-1]->GetCharge() + seed_charge * charge_frac );
//                        if(i == strip) thisPlane[i-1]->SetCharge( seed_charge * charge_frac );
//                        if(i != strip) thisPlane[i-1]->SetCharge( thisPlane[i-1]->GetCharge() + seed_charge * charge_frac );
//                        
//                        //For debug
//                        /*G4cout << "hit location = (" << digitsMap[hit_plane][strip-1]->GetPos().x() << ","
//                        << digitsMap[hit_plane][strip-1]->GetPos().y() << ","
//                        << digitsMap[hit_plane][strip-1]->GetPos().z() << ")" << G4endl;
//                        G4cout << "dist_to_hit = " << dist_to_hit << G4endl;
//                        G4cout << "distance from hit to strip boundary: " << limits[0] << " and " << limits[1] << " diff = " << limits[1] - limits[0] << G4endl;
//                        G4cout << "finished charge diffusion for strip: " << i << G4endl;*/
//
//                    }
//                    
//                }
//                
//                //For debug
//                /*G4cout << "\nAfter ALL charge diffusion.." << " plane = " << hit_plane + 1 << ", strip = " << strip << G4endl;
//                G4cout << "charge on seed strip - 1 = " << thisPlane[strip-2]->GetCharge() << " e-" << G4endl;
//                G4cout << "charge on seed strip = " << thisPlane[strip-1]->GetCharge() << " e- (SEED strip)" << G4endl;
//                G4cout << "charge on seed strip + 1 = " << thisPlane[strip]->GetCharge() << " e-\n" << G4endl;*/
//            }
//        }
//        
//    }
//    
//}
