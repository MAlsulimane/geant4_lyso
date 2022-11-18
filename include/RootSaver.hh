
#ifndef ROOTSAVER_HH_
#define ROOTSAVER_HH_

#include <string>
#include <vector>
#include <TTree.h>
#include "SiDigi.hh"
#include "SiHit.hh"

#include "SiDigi_pix.hh"
#include "SiHit_pix.hh"

class TFile;


// Save hits and digits to a ROOT TTree.
// This class can be used to save in a TTree hits
// and digits. The TTree structure is described below.

class RootSaver
{
public:
    
	// Default constructor
	RootSaver();
    
	// Default destructor
	virtual ~RootSaver();
    
	 /* Open a TFile and create ROOT TTree.
	 *
	 * Each time that a /run/beamOn command is issued
	 * this method should be called
	 * Every time this method is called the run counter
	 * is incremented and the file name is modified accordingly:
	 * tree_run<n>.root
	 * CloseTree()
	 * fileName : The ROOT file name prefix
	 * treeName : The name of the TTree
	 */
    
	virtual void CreateTree_strip_det( const std::string& fileName = "s_tree",
                                       const std::string& treeName = "strip_tracker",
                                       const int strips = 0);
    
    virtual void CreateTree_pixel_det( const std::string& fileName = "p_tree",
                                       const std::string& treeName = "pixel_tracker",
                                       const int pixels = 0);
	
    // Close the file and save ROOT TTree
    // The ROOT file should be closed at the end of each /run/beamOn
	virtual void CloseTrees();
    
	// Add hits and digi container for this event
	virtual void AddEvent_strip_det(const G4int event,
                                    const SiHitCollection * const hits_x1,
                                    const SiHitCollection * const hits_u1,
                                    const SiHitCollection * const hits_v1,
                                    const SiDigiCollection * const digits,
                                    const G4ThreeVector& primaryPos,
                                    const G4ThreeVector& primaryMom,

                                    const G4float KE_in);
    
    // Add hits and digi container for this event
    virtual void AddEvent_pixel_det(const G4int event,
                                    const SiHit_pixCollection * const hits_pix1,
                                    const SiHit_pixCollection * const hits_pix2,
                                    const SiHit_pixCollection * const hits_pix3,
                                    const SiHit_pixCollection * const hits_pix4,
                                    const SiDigi_pixCollection * const digits,
                                    const G4ThreeVector& primaryPos,
                                    const G4ThreeVector& primaryMom,

                                    const G4float KE_in);
    
    TTree * Get_root_tree_strip(){return rootTree_strip;}
    TTree * Get_root_tree_pixel(){return rootTree_pixel;}
    TFile * Get_root_file(){return rootFile;}
    
private:
    
	TTree * rootTree_strip;            // Pointer to the ROOT TTree for strip data
	TTree * rootTree_pixel;            // Pointer to the ROOT TTree for pixel data
    TFile * rootFile;                   // Pointer to the ROOT TFile
	unsigned int runCounter;            // Run counter to uniquely identify ROOT file
	Int_t nStrips;                      // Number of strips in each det. plane
    Int_t nPixels;                      // Number of pixels in each det. plane
    
	//*** TTree variables ***//
    
    Int_t Event_no;
    
    // Position and angle of primary particle at origin
    Float_t Truth_x_pos;
    Float_t Truth_y_pos;
    Float_t Truth_z_pos;
    Float_t TruthTheta_x;
    Float_t TruthTheta_y;
    
    // Kinetic Energy of primary at origin
    Float_t KE_in;
    
    
    // STRIP DETECTOR VARIABLES
    
    // Hit multiplicity for det. (no. of tracks (particles) / det. / event )
    Int_t Hit_mult_x1;
    Int_t Hit_mult_u1;
    Int_t Hit_mult_v1;

    // Signal in electrons for each strip on each det.
	Float_t* Signal_x1;
    Float_t* Signal_u1;
    Float_t* Signal_v1;
    
    // strips fired / det. / event
	std::vector<Int_t> Strip_no_x1;
    std::vector<Int_t> Strip_no_u1;
    std::vector<Int_t> Strip_no_v1;
    
    // total no. of strips fired / det. / event
    Int_t ClusterSize_x1;
    Int_t ClusterSize_u1;
    Int_t ClusterSize_v1;
    
    // total no. of det. fired / event
    Int_t Det_mult;
    
    // non ionising edep stored for x1 only for use in dose calcs
    std::vector<Double_t> NI_Edep_x1;
    
    // Sum of edep (MeV) in det.
	std::vector<Double_t> Edep_x1;
	std::vector<Double_t> Edep_u1;
	std::vector<Double_t> Edep_v1;
    
    // Polar angle in the xz plane (measured from z-axis)
    std::vector<Double_t> Theta_x1;
    std::vector<Double_t> Theta_u1;
    std::vector<Double_t> Theta_v1;
    
    // x position of particle in det. (must be double as float/Float_t dont work! (bug))
	std::vector<Double_t> X_pos_x1;
	std::vector<Double_t> X_pos_u1;
	std::vector<Double_t> X_pos_v1;
    
    // y position of particle in det. (must be double as float/Float_t dont work! (bug))
	std::vector<Double_t> Y_pos_x1;
	std::vector<Double_t> Y_pos_u1;
	std::vector<Double_t> Y_pos_v1;
    
    // z position of particle in det. (must be double as float/Float_t dont work! (bug))
	std::vector<Double_t> Z_pos_x1;
	std::vector<Double_t> Z_pos_u1;
	std::vector<Double_t> Z_pos_v1;
    
    // Does hit in det. contain primary event?
    std::vector<Bool_t> IsPrimaryParticle_x1;
    std::vector<Bool_t> IsPrimaryParticle_u1;
    std::vector<Bool_t> IsPrimaryParticle_v1;
    
    // Particle track no. (for counting number of particles / det. / event)
    std::vector<Int_t> Track_no_x1;
    std::vector<Int_t> Track_no_u1;
    std::vector<Int_t> Track_no_v1;
    
    // Particle name / det. / event (must be TString otherwise won't work),
    // At command line use e.g. tv__tree->Draw("hit_mult_x1","ParticleName_x1==\"e-\""); to select as condition
    std::vector<TString> ParticleName_x1;
    std::vector<TString> ParticleName_u1;
    std::vector<TString> ParticleName_v1;
    
    
    // PIXEL DETECTOR VARIABLES
    
    // Hit multiplicity for det. (no. of tracks (particles) / det. / event )
    Int_t Hit_mult_pix1;
    Int_t Hit_mult_pix2;
    Int_t Hit_mult_pix3;
    Int_t Hit_mult_pix4;
    
    // Signal in electrons for each strip on each det.
    Float_t * Signal_pix1;
    Float_t * Signal_pix2;
    Float_t * Signal_pix3;
    Float_t * Signal_pix4;
    
    // non ionising edep stored for x1 only for use in dose calcs
    std::vector<Double_t> NI_Edep_pix1;
    std::vector<Double_t> NI_Edep_pix2;
    std::vector<Double_t> NI_Edep_pix3;
    std::vector<Double_t> NI_Edep_pix4;
    
    // Sum of edep (MeV) in det.
    std::vector<Double_t> Edep_pix1;
    std::vector<Double_t> Edep_pix2;
    std::vector<Double_t> Edep_pix3;
    std::vector<Double_t> Edep_pix4;

  //***************************Welcome new histogram the Kinetic Energy ;)*******************************************************

  // Sum of Kinetic Energy (MeV) in det.
    std::vector<Double_t> Truth_KE_pix1;
    std::vector<Double_t> Truth_KE_pix2;
    std::vector<Double_t> Truth_KE_pix3;
    std::vector<Double_t> Truth_KE_pix4;
  //********************************************************************************

    // x position of particle in det. (must be double as float/Float_t dont work! (bug))
    std::vector<Double_t> X_pos_pix1;
    std::vector<Double_t> X_pos_pix2;
    std::vector<Double_t> X_pos_pix3;
    std::vector<Double_t> X_pos_pix4;
    
    // y position of particle in det. (must be double as float/Float_t dont work! (bug))
    std::vector<Double_t> Y_pos_pix1;
    std::vector<Double_t> Y_pos_pix2;
    std::vector<Double_t> Y_pos_pix3;
    std::vector<Double_t> Y_pos_pix4;
    
    // z position of particle in det. (must be double as float/Float_t dont work! (bug))
    std::vector<Double_t> Z_pos_pix1;
    std::vector<Double_t> Z_pos_pix2;
    std::vector<Double_t> Z_pos_pix3;
    std::vector<Double_t> Z_pos_pix4;
  //*******************************************************************************************************************************
    // x Truth position of particle in det. (must be double as float/Float_t dont work! (bug))
    std::vector<Double_t> Truth_X_pos_pix1;
    std::vector<Double_t> Truth_X_pos_pix2;
    std::vector<Double_t> Truth_X_pos_pix3;
    std::vector<Double_t> Truth_X_pos_pix4;
    
    // y Truth position of particle in det. (must be double as float/Float_t dont work! (bug))
    std::vector<Double_t> Truth_Y_pos_pix1;
    std::vector<Double_t> Truth_Y_pos_pix2;
    std::vector<Double_t> Truth_Y_pos_pix3;
    std::vector<Double_t> Truth_Y_pos_pix4;
    
    // z Truth position of particle in det. (must be double as float/Float_t dont work! (bug))
    std::vector<Double_t> Truth_Z_pos_pix1;
    std::vector<Double_t> Truth_Z_pos_pix2;
    std::vector<Double_t> Truth_Z_pos_pix3;
    std::vector<Double_t> Truth_Z_pos_pix4;
 

  //*******************************************************************************************************************************
    // Does hit in det. contain primary event?
    std::vector<Bool_t> IsPrimaryParticle_pix1;
    std::vector<Bool_t> IsPrimaryParticle_pix2;
    std::vector<Bool_t> IsPrimaryParticle_pix3;
    std::vector<Bool_t> IsPrimaryParticle_pix4;
    
    // Particle track no. (for counting number of particles / det. / event)
    std::vector<Int_t> Track_no_pix1;
    std::vector<Int_t> Track_no_pix2;
    std::vector<Int_t> Track_no_pix3;
    std::vector<Int_t> Track_no_pix4;
    
    // Particle name / det. / event (must be TString otherwise won't work),
    // At command line use e.g. tv__tree->Draw("hit_mult_x1","ParticleName_x1==\"e-\""); to select as condition
    std::vector<TString> ParticleName_pix1;
    std::vector<TString> ParticleName_pix2;
    std::vector<TString> ParticleName_pix3;
    std::vector<TString> ParticleName_pix4;


  //********************************************************************
  //  std::vector<TString> ParticleParent_pix1;
  // std::vector<TString> ParticleParent_pix2;
  //  std::vector<TString> ParticleParent_pix3;
  //  std::vector<TString> ParticleParent_pix4;

   std::vector<TString> ProcessName_pix1;
   std::vector<TString> ProcessName_pix2;
    std::vector<TString> ProcessName_pix3;
    std::vector<TString> ProcessName_pix4;
    
};

#endif /* ROOTSAVER_HH_ */
