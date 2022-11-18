
// This class determines the overall geometry of the tracker taking
// into account all rotations and alignment of sensors wrt the centre
// of the phantom. it can be used to obtain the position of a detector
// centre, corner or strip in either world or detector co-ordinates

// The constructor for this class is overloaded such that the class
// can be constructed from the default detector positions and rotations
// that are hard coded here, or taken from the tracker_geom.mac file or
// from the tracker_geom TTree contained in an output root file from GEANT4.

// Positions of all detectors calculated in the same way as DetectorConstruction
// we cannot include the DetectorConstruction class here since this class is accessed
// outside GEANT4 in the tracking routines

// necessary to include full path for when this class in included outside geant4 i.e.
// in the tracking routines
#include "TrackerGeometry.hh"
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <TGraph.h>
#include <TBranch.h>
#include <TBranchElement.h>

// Default constructor
TrackerGeometry::TrackerGeometry()
{
    // Take existing setup/detector parameters from input file or class
	InitParameters();
    // Calculate new parameters relevant to tracker
	ComputeParameters();
}

// Constructor using input .mac file
TrackerGeometry::TrackerGeometry(std::string input_macro_file)
{
    // Take existing setup/detector parameters from input file or class
	InitParameters(input_macro_file);
    // Calculate new parameters relevant to tracker
	ComputeParameters();
}

// Constructor using input .root file (used by tracking, not used by geant4)
TrackerGeometry::TrackerGeometry(TTree * geom_tree)
{
    // Take existing setup/detector parameters from input file or class
	InitParameters(geom_tree);
    // Calculate new parameters relevant to tracker
	ComputeParameters();
}

TrackerGeometry::~TrackerGeometry()
{
    
}

// Initialise parameters to default values
void TrackerGeometry::InitParameters()
{
    // This function obtains the detector and tracker geometry
    
    // Det. geometry parameters
    s_pitch = 0.0908;           // Strip pitch in mm (PRaVDA det.)
    s_length = 96.0;            // Length of strips in mm, slightly longer than width (pitch*strips)
    strips = 1024;              // No. of strips on each detector
    thickness = 0.150;
    
    // tracker geometry parameters
    inter_plane_dist = 10.0;          // default
    inter_module_dist = 100.0;        // default
    phantom_gap = 80.0;               // default
    phantom_pos = TVector3(0.0, 0.0, 0.0);
    phantom_zShift = 0.0;
    
    halfPhantomSizeZ = 37.5;          // default
    halfCalorimeterSizeZ = 125.0;     // default
    calorimeter_gap = 41.0;           // default
    
    //*****************************************
    shield_pos = TVector3(0.0, 0.0, 0.0);
    shield_zShift = 0.0;
    halfShieldSizeZ = 37.5;          // default
    //*****************************************
    
    // Initialise position values to zero (z values set in ComputeParameters() along with any offset in x/y )
    x1_pos = TVector3(0,0,0), u1_pos = TVector3(0,0,0), v1_pos = TVector3(0,0,0);
    x2_pos = TVector3(0,0,0), u2_pos = TVector3(0,0,0), v2_pos = TVector3(0,0,0);
    x3_pos = TVector3(0,0,0), u3_pos = TVector3(0,0,0), v3_pos = TVector3(0,0,0);
    x4_pos = TVector3(0,0,0), u4_pos = TVector3(0,0,0), v4_pos = TVector3(0,0,0);
    
    
    // Use default stereo angles
    stereo_x1 = 0.0 * TMath::DegToRad();
    stereo_u1 = 60.0 * TMath::DegToRad();
    stereo_v1 = -60. * TMath::DegToRad();
    
    stereo_x2 = 0.0 * TMath::DegToRad();
    stereo_u2 = 60.0 * TMath::DegToRad();
    stereo_v2 = -60.0 * TMath::DegToRad();
    
    stereo_x3 = 0.0 * TMath::DegToRad();
    stereo_u3 = 60.0 * TMath::DegToRad();
    stereo_v3 = -60.0 * TMath::DegToRad();
    
    stereo_x4 = 0.0 * TMath::DegToRad();
    stereo_u4 = 60.0 * TMath::DegToRad();
    stereo_v4 = -60.0 * TMath::DegToRad();
    
    
    // Use default position offset values (added to position vectors in ComputeParameters() )
    x1_pos_offset = TVector3(0,0,0), u1_pos_offset = TVector3(0,0,0), v1_pos_offset = TVector3(0,0,0);
    x2_pos_offset = TVector3(0,0,0), u2_pos_offset = TVector3(0,0,0), v2_pos_offset = TVector3(0,0,0);
    x3_pos_offset = TVector3(0,0,0), u3_pos_offset = TVector3(0,0,0), v3_pos_offset = TVector3(0,0,0);
    x4_pos_offset = TVector3(0,0,0), u4_pos_offset = TVector3(0,0,0), v4_pos_offset = TVector3(0,0,0);
    
    // Use default angular offset values (added to position vectors in ComputeParameters() )
    stereo_x1_offset = 0, stereo_u1_offset = 0, stereo_v1_offset = 0;
    stereo_x2_offset = 0, stereo_u2_offset = 0, stereo_v2_offset = 0;
    stereo_x3_offset = 0, stereo_u3_offset = 0, stereo_v3_offset = 0;
    stereo_x4_offset = 0, stereo_u4_offset = 0, stereo_v4_offset = 0;
    
    
    // Printout all variables for debug
//    std::cout << "\nReading default geom params.." << std::endl;
//    std::cout << "s_pitch = " << s_pitch << std::endl;
//    std::cout << "s_length = " << s_length << std::endl;
//    std::cout << "strips = " << strips << std::endl;
//    
//    std::cout << "halfPhantomSizeZ = " << halfPhantomSizeZ << std::endl;
//    std::cout << "halfCalorimeterSizeZ = " << halfCalorimeterSizeZ << std::endl;
//    std::cout << "calorimeter_gap = " << calorimeter_gap << std::endl;
//    
//    std::cout << "plane_dist = " << inter_plane_dist << std::endl;
//    std::cout << "module_dist = " << inter_module_dist << std::endl;
//    std::cout << "phantom_gap = " << phantom_gap << std::endl;
//    std::cout << "phantom_zShift = " << phantom_zShift << std::endl;
//
//    std::cout << "x1_pos = (" << x1_pos.X() << "," << x1_pos.Y() << "," << x1_pos.Z() << ")" << std::endl;
//    std::cout << "u1_pos = (" << u1_pos.X() << "," << u1_pos.Y() << "," << u1_pos.Z() << ")" << std::endl;
//    std::cout << "v1_pos = (" << v1_pos.X() << "," << v1_pos.Y() << "," << v1_pos.Z() << ")" << std::endl;
//     
//    std::cout << "x2_pos = (" << x2_pos.X() << "," << x2_pos.Y() << "," << x2_pos.Z() << ")" << std::endl;
//    std::cout << "u2_pos = (" << u2_pos.X() << "," << u2_pos.Y() << "," << u2_pos.Z() << ")" << std::endl;
//    std::cout << "v2_pos = (" << v2_pos.X() << "," << v2_pos.Y() << "," << v2_pos.Z() << ")" << std::endl;
//     
//    std::cout << "x3_pos = (" << x3_pos.X() << "," << x3_pos.Y() << "," << x3_pos.Z() << ")" << std::endl;
//    std::cout << "u3_pos = (" << u3_pos.X() << "," << u3_pos.Y() << "," << u3_pos.Z() << ")" << std::endl;
//    std::cout << "v3_pos = (" << v3_pos.X() << "," << v3_pos.Y() << "," << v3_pos.Z() << ")" << std::endl;
//     
//    std::cout << "x4_pos = (" << x4_pos.X() << "," << x4_pos.Y() << "," << x4_pos.Z() << ")" << std::endl;
//    std::cout << "u4_pos = (" << u4_pos.X() << "," << u4_pos.Y() << "," << u4_pos.Z() << ")" << std::endl;
//    std::cout << "v4_pos = (" << v4_pos.X() << "," << v4_pos.Y() << "," << v4_pos.Z() << ")" << std::endl;
//     
//    std::cout << "x1_pos_offset = (" << x1_pos_offset.X() << "," << x1_pos_offset.Y() << "," << x1_pos_offset.Z() << ")" << std::endl;
//    std::cout << "u1_pos_offset = (" << u1_pos_offset.X() << "," << u1_pos_offset.Y() << "," << u1_pos_offset.Z() << ")" << std::endl;
//    std::cout << "v1_pos_offset = (" << v1_pos_offset.X() << "," << v1_pos_offset.Y() << "," << v1_pos_offset.Z() << ")" << std::endl;
//
//    std::cout << "x2_pos_offset = (" << x2_pos_offset.X() << "," << x2_pos_offset.Y() << "," << x2_pos_offset.Z() << ")" << std::endl;
//    std::cout << "u2_pos_offset = (" << u2_pos_offset.X() << "," << u2_pos_offset.Y() << "," << u2_pos_offset.Z() << ")" << std::endl;
//    std::cout << "v2_pos_offset = (" << v2_pos_offset.X() << "," << v2_pos_offset.Y() << "," << v2_pos_offset.Z() << ")" << std::endl;
//     
//    std::cout << "x3_pos_offset = (" << x3_pos_offset.X() << "," << x3_pos_offset.Y() << "," << x3_pos_offset.Z() << ")" << std::endl;
//    std::cout << "u3_pos_offset = (" << u3_pos_offset.X() << "," << u3_pos_offset.Y() << "," << u3_pos_offset.Z() << ")" << std::endl;
//    std::cout << "v3_pos_offset = (" << v3_pos_offset.X() << "," << v3_pos_offset.Y() << "," << v3_pos_offset.Z() << ")" << std::endl;
//     
//    std::cout << "x4_pos_offset = (" << x4_pos_offset.X() << "," << x4_pos_offset.Y() << "," << x4_pos_offset.Z() << ")" << std::endl;
//    std::cout << "u4_pos_offset = (" << u4_pos_offset.X() << "," << u4_pos_offset.Y() << "," << u4_pos_offset.Z() << ")" << std::endl;
//    std::cout << "v4_pos_offset = (" << v4_pos_offset.X() << "," << v4_pos_offset.Y() << "," << v4_pos_offset.Z() << ")" << std::endl;
//     
//    std::cout << "x1_stereo angle = " << stereo_x1 << ", the offset = " << stereo_x1_offset << std::endl;
//    std::cout << "u1_stereo angle = " << stereo_u1 << ", the offset = " << stereo_u1_offset << std::endl;
//    std::cout << "v1_stereo angle = " << stereo_v1 << ", the offset = " << stereo_v1_offset << std::endl;
//    
//    std::cout << "x2_stereo angle = " << stereo_x2 << ", the offset = " << stereo_x2_offset << std::endl;
//    std::cout << "u2_stereo angle = " << stereo_u2 << ", the offset = " << stereo_u2_offset << std::endl;
//    std::cout << "v2_stereo angle = " << stereo_v2 << ", the offset = " << stereo_v2_offset << std::endl;
//    
//    std::cout << "x3_stereo angle = " << stereo_x3 << ", the offset = " << stereo_x3_offset << std::endl;
//    std::cout << "u3_stereo angle = " << stereo_u3 << ", the offset = " << stereo_u3_offset << std::endl;
//    std::cout << "v3_stereo angle = " << stereo_v3 << ", the offset = " << stereo_v3_offset << std::endl;
//    
//    std::cout << "x4_stereo angle = " << stereo_x4 << ", the offset = " << stereo_x4_offset << std::endl;
//    std::cout << "u4_stereo angle = " << stereo_u4 << ", the offset = " << stereo_u4_offset << std::endl;
//    std::cout << "v4_stereo angle = " << stereo_v4 << ", the offset = " << stereo_v4_offset << std::endl;
}

// This function overloads the previous one to allow it to obtain
// the parameter values from a .mac file input for geant4.
void TrackerGeometry::InitParameters(std::string input_macro_file)
{
    // This function obtains the detector and tracker geometry
    
    //*** Read in the value of the tracking/detector geometry variables from input .mac file ***//
    
    const int MAX_CHARS_PER_LINE = 1024;
    const int MAX_WORDS_PER_LINE = 50;
    const char* const DELIMITER = " ";
    
    // create a file-reading object
    std::ifstream fin;
    fin.open(input_macro_file); // open proton_no_vis.mac as default
    if (!fin.good()){exit(-1);}
    
    // define variables for all geometry related parameters that will be
    // read in from macro
    std::string n_strips = "/det/n_strips";
    std::string strip_pitch = "/det/strip_pitch";
    std::string strip_length = "/det/strip_length";
    std::string sensor_thickness = "/det/thickness";
    
    std::string plane_d = "/geom/plane_dist";
    std::string module_d = "/geom/module_dist";
    std::string phantom_g = "/geom/phantom_gap";
    std::string phantom_z_Shift = "/phantom/zShift";
    
    std::string hPhantomSizeZ = "/geom/halfPhantomSizeZ";
    std::string hCalorimeterSizeZ = "/geom/halfCalorimeterSizeZ";
    std::string calorimeter_g = "/geom/calorimeter_gap";
    
    std::string shield_z_Shift = "/shield/zShift";//************************
    std::string hShieldSizeZ = "/geom/halfShieldSizeZ";//*********************
    
    std::string x1_pos_x = "/det/x1_Sensor/x_pos";
    std::string u1_pos_x = "/det/u1_Sensor/x_pos";
    std::string v1_pos_x = "/det/v1_Sensor/x_pos";

    std::string x2_pos_x = "/det/x2_Sensor/x_pos";
    std::string u2_pos_x = "/det/u2_Sensor/x_pos";
    std::string v2_pos_x = "/det/v2_Sensor/x_pos";

    std::string x3_pos_x = "/det/x3_Sensor/x_pos";
    std::string u3_pos_x = "/det/u3_Sensor/x_pos";
    std::string v3_pos_x = "/det/v3_Sensor/x_pos";
    
    std::string x4_pos_x = "/det/x4_Sensor/x_pos";
    std::string u4_pos_x = "/det/u4_Sensor/x_pos";
    std::string v4_pos_x = "/det/v4_Sensor/x_pos";
    
    
    std::string x1_pos_y = "/det/x1_Sensor/y_pos";
    std::string u1_pos_y = "/det/u1_Sensor/y_pos";
    std::string v1_pos_y = "/det/v1_Sensor/y_pos";
    
    std::string x2_pos_y = "/det/x2_Sensor/y_pos";
    std::string u2_pos_y = "/det/u2_Sensor/y_pos";
    std::string v2_pos_y = "/det/v2_Sensor/y_pos";
    
    std::string x3_pos_y = "/det/x3_Sensor/y_pos";
    std::string u3_pos_y = "/det/u3_Sensor/y_pos";
    std::string v3_pos_y = "/det/v3_Sensor/y_pos";
    
    std::string x4_pos_y = "/det/x4_Sensor/y_pos";
    std::string u4_pos_y = "/det/u4_Sensor/y_pos";
    std::string v4_pos_y = "/det/v4_Sensor/y_pos";
    
    
    std::string x1_stereo = "/det/x1_Sensor/stereo_angle";
    std::string u1_stereo = "/det/u1_Sensor/stereo_angle";
    std::string v1_stereo = "/det/v1_Sensor/stereo_angle";
    
    std::string x2_stereo = "/det/x2_Sensor/stereo_angle";
    std::string u2_stereo = "/det/u2_Sensor/stereo_angle";
    std::string v2_stereo = "/det/v2_Sensor/stereo_angle";
    
    std::string x3_stereo = "/det/x3_Sensor/stereo_angle";
    std::string u3_stereo = "/det/u3_Sensor/stereo_angle";
    std::string v3_stereo = "/det/v3_Sensor/stereo_angle";
    
    std::string x4_stereo = "/det/x4_Sensor/stereo_angle";
    std::string u4_stereo = "/det/u4_Sensor/stereo_angle";
    std::string v4_stereo = "/det/v4_Sensor/stereo_angle";
    
    
    std::string x1_pos_offset_x = "/det/x1_Sensor/xShift";
    std::string u1_pos_offset_x = "/det/u1_Sensor/xShift";
    std::string v1_pos_offset_x = "/det/v1_Sensor/xShift";
    
    std::string x2_pos_offset_x = "/det/x2_Sensor/xShift";
    std::string u2_pos_offset_x = "/det/u2_Sensor/xShift";
    std::string v2_pos_offset_x = "/det/v2_Sensor/xShift";
    
    std::string x3_pos_offset_x = "/det/x3_Sensor/xShift";
    std::string u3_pos_offset_x = "/det/u3_Sensor/xShift";
    std::string v3_pos_offset_x = "/det/v3_Sensor/xShift";
    
    std::string x4_pos_offset_x = "/det/x4_Sensor/xShift";
    std::string u4_pos_offset_x = "/det/u4_Sensor/xShift";
    std::string v4_pos_offset_x = "/det/v4_Sensor/xShift";
    
    
    std::string x1_pos_offset_y = "/det/x1_Sensor/yShift";
    std::string u1_pos_offset_y = "/det/u1_Sensor/yShift";
    std::string v1_pos_offset_y = "/det/v1_Sensor/yShift";
    
    std::string x2_pos_offset_y = "/det/x2_Sensor/yShift";
    std::string u2_pos_offset_y = "/det/u2_Sensor/yShift";
    std::string v2_pos_offset_y = "/det/v2_Sensor/yShift";
    
    std::string x3_pos_offset_y = "/det/x3_Sensor/yShift";
    std::string u3_pos_offset_y = "/det/u3_Sensor/yShift";
    std::string v3_pos_offset_y = "/det/v3_Sensor/yShift";
    
    std::string x4_pos_offset_y = "/det/x4_Sensor/yShift";
    std::string u4_pos_offset_y = "/det/u4_Sensor/yShift";
    std::string v4_pos_offset_y = "/det/v4_Sensor/yShift";
    
    
    std::string x1_stereo_offset = "/det/x1_Sensor/stereoShift";
    std::string u1_stereo_offset = "/det/u1_Sensor/stereoShift";
    std::string v1_stereo_offset = "/det/v1_Sensor/stereoShift";
    
    std::string x2_stereo_offset = "/det/x2_Sensor/stereoShift";
    std::string u2_stereo_offset = "/det/u2_Sensor/stereoShift";
    std::string v2_stereo_offset = "/det/v2_Sensor/stereoShift";
    
    std::string x3_stereo_offset = "/det/x3_Sensor/stereoShift";
    std::string u3_stereo_offset = "/det/u3_Sensor/stereoShift";
    std::string v3_stereo_offset = "/det/v3_Sensor/stereoShift";
    
    std::string x4_stereo_offset = "/det/x4_Sensor/stereoShift";
    std::string u4_stereo_offset = "/det/u4_Sensor/stereoShift";
    std::string v4_stereo_offset = "/det/v4_Sensor/stereoShift";
    
    
    // read each line of the file
    while (!fin.eof())
    {
        // read an entire line into memory
        char buf[MAX_CHARS_PER_LINE];
        fin.getline(buf, MAX_CHARS_PER_LINE);
        
        // parse the line into blank-delimited words
        int n = 0; // a for-loop index
        
        // array to store memory addresses of the words in buf
        const char* token[MAX_WORDS_PER_LINE] = {}; // initialize to 0
        
        // parse the line
        token[0] = strtok(buf, DELIMITER); // first token
        if (token[0]) // zero if line is blank
        {
            for (n = 1; n < MAX_WORDS_PER_LINE; n++)
            {
                token[n] = strtok(0, DELIMITER); // subsequent words
                if (!token[n]) break; // no more words
            }
        }
        
        
        // process the words on a line and extract the parameters
        for (int i = 0; i < n; i++) // n = #of words
        {
            std::string s1 = token[0]; //convert to string for find fn
            
            // Do not read lines that are commented out, also prevents variables being overwritten
            // if e.g. the same variable is on two consecutive lines with the commented out one second
            if(s1.find("#") != std::string::npos) {break;}
            
            // If string/token at the start of a line matches variable name then variable is equal to the next token
            
            if(s1.find(n_strips) != std::string::npos)  { strips = atof(token[1]); }
            else if(s1.find(strip_pitch) != std::string::npos)  { s_pitch = atof(token[1]); }
            else if(s1.find(strip_length) != std::string::npos)  { s_length = atof(token[1]); }
            else if(s1.find(sensor_thickness) != std::string::npos)  { thickness = atof(token[1]); }
            
            else if(s1.find(plane_d) != std::string::npos)   { inter_plane_dist = atof(token[1]); }
            else if(s1.find(module_d) != std::string::npos)  { inter_module_dist = atof(token[1]); }
            else if(s1.find(phantom_g) != std::string::npos) { phantom_gap = atof(token[1]); }
            else if(s1.find(phantom_z_Shift) != std::string::npos) { phantom_zShift = atof(token[1]); }
            
            else if(s1.find(hPhantomSizeZ) != std::string::npos)  { halfPhantomSizeZ = atof(token[1]); }
            else if(s1.find(hCalorimeterSizeZ) != std::string::npos) { halfCalorimeterSizeZ = atof(token[1]); }
            else if(s1.find(calorimeter_g) != std::string::npos) { calorimeter_gap = atof(token[1]); }
            

	    else if(s1.find(shield_z_Shift) != std::string::npos) { shield_zShift = atof(token[1]); }//********************************
            
            else if(s1.find(hShieldSizeZ) != std::string::npos)  { halfShieldSizeZ = atof(token[1]); }//********************************
            
            else if(s1.find(x1_pos_x) != std::string::npos)  { x1_pos.SetX( atof(token[1]) ); }
            else if(s1.find(u1_pos_x) != std::string::npos)  { u1_pos.SetX( atof(token[1]) ); }
            else if(s1.find(v1_pos_x) != std::string::npos)  { v1_pos.SetX( atof(token[1]) ); }
            
            else if(s1.find(x2_pos_x) != std::string::npos)  { x2_pos.SetX( atof(token[1]) ); }
            else if(s1.find(u2_pos_x) != std::string::npos)  { u2_pos.SetX( atof(token[1]) ); }
            else if(s1.find(v2_pos_x) != std::string::npos)  { v2_pos.SetX( atof(token[1]) ); }
            
            else if(s1.find(x3_pos_x) != std::string::npos)  { x3_pos.SetX( atof(token[1]) ); }
            else if(s1.find(u3_pos_x) != std::string::npos)  { u3_pos.SetX( atof(token[1]) ); }
            else if(s1.find(v3_pos_x) != std::string::npos)  { v3_pos.SetX( atof(token[1]) ); }
            
            else if(s1.find(x4_pos_x) != std::string::npos)  { x4_pos.SetX( atof(token[1]) ); }
            else if(s1.find(u4_pos_x) != std::string::npos)  { u4_pos.SetX( atof(token[1]) ); }
            else if(s1.find(v4_pos_x) != std::string::npos)  { v4_pos.SetX( atof(token[1]) ); }
            
            
            else if(s1.find(x1_pos_y) != std::string::npos)  { x1_pos.SetY( atof(token[1]) ); }
            else if(s1.find(u1_pos_y) != std::string::npos)  { u1_pos.SetY( atof(token[1]) ); }
            else if(s1.find(v1_pos_y) != std::string::npos)  { v1_pos.SetY( atof(token[1]) ); }
            
            else if(s1.find(x2_pos_y) != std::string::npos)  { x2_pos.SetY( atof(token[1]) ); }
            else if(s1.find(u2_pos_y) != std::string::npos)  { u2_pos.SetY( atof(token[1]) ); }
            else if(s1.find(v2_pos_y) != std::string::npos)  { v2_pos.SetY( atof(token[1]) ); }
            
            else if(s1.find(x3_pos_y) != std::string::npos)  { x3_pos.SetY( atof(token[1]) ); }
            else if(s1.find(u3_pos_y) != std::string::npos)  { u3_pos.SetY( atof(token[1]) ); }
            else if(s1.find(v3_pos_y) != std::string::npos)  { v3_pos.SetY( atof(token[1]) ); }
            
            else if(s1.find(x4_pos_y) != std::string::npos)  { x4_pos.SetY( atof(token[1]) ); }
            else if(s1.find(u4_pos_y) != std::string::npos)  { u4_pos.SetY( atof(token[1]) ); }
            else if(s1.find(v4_pos_y) != std::string::npos)  { v4_pos.SetY( atof(token[1]) ); }
            
            
            else if(s1.find(x1_stereo) != std::string::npos) { stereo_x1 = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(u1_stereo) != std::string::npos) { stereo_u1 = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(v1_stereo) != std::string::npos) { stereo_v1 = atof(token[1]) * TMath::DegToRad(); }
            
            else if(s1.find(x2_stereo) != std::string::npos) { stereo_x2 = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(u2_stereo) != std::string::npos) { stereo_u2 = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(v2_stereo) != std::string::npos) { stereo_v2 = atof(token[1]) * TMath::DegToRad(); }
            
            else if(s1.find(x3_stereo) != std::string::npos) { stereo_x3 = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(u3_stereo) != std::string::npos) { stereo_u3 = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(v3_stereo) != std::string::npos) { stereo_v3 = atof(token[1]) * TMath::DegToRad(); }
            
            else if(s1.find(x4_stereo) != std::string::npos) { stereo_x4 = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(u4_stereo) != std::string::npos) { stereo_u4 = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(v4_stereo) != std::string::npos) { stereo_v4 = atof(token[1]) * TMath::DegToRad(); }
            
            
            else if(s1.find(x1_pos_offset_x) != std::string::npos)  { x1_pos_offset.SetX( atof(token[1]) ); }
            else if(s1.find(u1_pos_offset_x) != std::string::npos)  { u1_pos_offset.SetX( atof(token[1]) ); }
            else if(s1.find(v1_pos_offset_x) != std::string::npos)  { v1_pos_offset.SetX( atof(token[1]) ); }
            
            else if(s1.find(x2_pos_offset_x) != std::string::npos)  { x2_pos_offset.SetX( atof(token[1]) ); }
            else if(s1.find(u2_pos_offset_x) != std::string::npos)  { u2_pos_offset.SetX( atof(token[1]) ); }
            else if(s1.find(v2_pos_offset_x) != std::string::npos)  { v2_pos_offset.SetX( atof(token[1]) ); }
            
            else if(s1.find(x3_pos_offset_x) != std::string::npos)  { x3_pos_offset.SetX( atof(token[1]) ); }
            else if(s1.find(u3_pos_offset_x) != std::string::npos)  { u3_pos_offset.SetX( atof(token[1]) ); }
            else if(s1.find(v3_pos_offset_x) != std::string::npos)  { v3_pos_offset.SetX( atof(token[1]) ); }
            
            else if(s1.find(x4_pos_offset_x) != std::string::npos)  { x4_pos_offset.SetX( atof(token[1]) ); }
            else if(s1.find(u4_pos_offset_x) != std::string::npos)  { u4_pos_offset.SetX( atof(token[1]) ); }
            else if(s1.find(v4_pos_offset_x) != std::string::npos)  { v4_pos_offset.SetX( atof(token[1]) ); }
            
            
            else if(s1.find(x1_pos_offset_y) != std::string::npos)  { x1_pos_offset.SetY( atof(token[1]) ); }
            else if(s1.find(u1_pos_offset_y) != std::string::npos)  { u1_pos_offset.SetY( atof(token[1]) ); }
            else if(s1.find(v1_pos_offset_y) != std::string::npos)  { v1_pos_offset.SetY( atof(token[1]) ); }
            
            else if(s1.find(x2_pos_offset_y) != std::string::npos)  { x2_pos_offset.SetY( atof(token[1]) ); }
            else if(s1.find(u2_pos_offset_y) != std::string::npos)  { u2_pos_offset.SetY( atof(token[1]) ); }
            else if(s1.find(v2_pos_offset_y) != std::string::npos)  { v2_pos_offset.SetY( atof(token[1]) ); }
            
            else if(s1.find(x3_pos_offset_y) != std::string::npos)  { x3_pos_offset.SetY( atof(token[1]) ); }
            else if(s1.find(u3_pos_offset_y) != std::string::npos)  { u3_pos_offset.SetY( atof(token[1]) ); }
            else if(s1.find(v3_pos_offset_y) != std::string::npos)  { v3_pos_offset.SetY( atof(token[1]) ); }
            
            else if(s1.find(x4_pos_offset_y) != std::string::npos)  { x4_pos_offset.SetY( atof(token[1]) ); }
            else if(s1.find(u4_pos_offset_y) != std::string::npos)  { u4_pos_offset.SetY( atof(token[1]) ); }
            else if(s1.find(v4_pos_offset_y) != std::string::npos)  { v4_pos_offset.SetY( atof(token[1]) ); }
            
            
            else if(s1.find(x1_stereo_offset) != std::string::npos) { stereo_x1_offset = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(u1_stereo_offset) != std::string::npos) { stereo_u1_offset = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(v1_stereo_offset) != std::string::npos) { stereo_v1_offset = atof(token[1]) * TMath::DegToRad(); }
            
            else if(s1.find(x2_stereo_offset) != std::string::npos) { stereo_x2_offset = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(u2_stereo_offset) != std::string::npos) { stereo_u2_offset = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(v2_stereo_offset) != std::string::npos) { stereo_v2_offset = atof(token[1]) * TMath::DegToRad(); }
            
            else if(s1.find(x3_stereo_offset) != std::string::npos) { stereo_x3_offset = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(u3_stereo_offset) != std::string::npos) { stereo_u3_offset = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(v3_stereo_offset) != std::string::npos) { stereo_v3_offset = atof(token[1]) * TMath::DegToRad(); }
            
            else if(s1.find(x4_stereo_offset) != std::string::npos) { stereo_x4_offset = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(u4_stereo_offset) != std::string::npos) { stereo_u4_offset = atof(token[1]) * TMath::DegToRad(); }
            else if(s1.find(v4_stereo_offset) != std::string::npos) { stereo_v4_offset = atof(token[1]) * TMath::DegToRad(); }
            
            
            break;  // prevents looping through all words on line after value is found
        }
    }
    
    
    // Printout all variables for debug
    /*std::cout << "\nReading default geom params.." << std::endl;
     std::cout << "s_pitch = " << s_pitch << std::endl;
     std::cout << "s_length = " << s_length << std::endl;
     std::cout << "strips = " << strips << std::endl;
     
     std::cout << "halfPhantomSizeZ = " << halfPhantomSizeZ << std::endl;
     std::cout << "halfCalorimeterSizeZ = " << halfCalorimeterSizeZ << std::endl;
     std::cout << "calorimeter_gap = " << calorimeter_gap << std::endl;
     
     std::cout << "plane_dist = " << inter_plane_dist << std::endl;
     std::cout << "module_dist = " << inter_module_dist << std::endl;
     std::cout << "phantom_gap = " << phantom_gap << std::endl;
     std::cout << "phantom_zShift = " << phantom_zShift << std::endl;
     
     std::cout << "x1_pos = (" << x1_pos.X() << "," << x1_pos.Y() << "," << x1_pos.Z() << ")" << std::endl;
     std::cout << "u1_pos = (" << u1_pos.X() << "," << u1_pos.Y() << "," << u1_pos.Z() << ")" << std::endl;
     std::cout << "v1_pos = (" << v1_pos.X() << "," << v1_pos.Y() << "," << v1_pos.Z() << ")" << std::endl;
     
     std::cout << "x2_pos = (" << x2_pos.X() << "," << x2_pos.Y() << "," << x2_pos.Z() << ")" << std::endl;
     std::cout << "u2_pos = (" << u2_pos.X() << "," << u2_pos.Y() << "," << u2_pos.Z() << ")" << std::endl;
     std::cout << "v2_pos = (" << v2_pos.X() << "," << v2_pos.Y() << "," << v2_pos.Z() << ")" << std::endl;
     
     std::cout << "x3_pos = (" << x3_pos.X() << "," << x3_pos.Y() << "," << x3_pos.Z() << ")" << std::endl;
     std::cout << "u3_pos = (" << u3_pos.X() << "," << u3_pos.Y() << "," << u3_pos.Z() << ")" << std::endl;
     std::cout << "v3_pos = (" << v3_pos.X() << "," << v3_pos.Y() << "," << v3_pos.Z() << ")" << std::endl;
     
     std::cout << "x4_pos = (" << x4_pos.X() << "," << x4_pos.Y() << "," << x4_pos.Z() << ")" << std::endl;
     std::cout << "u4_pos = (" << u4_pos.X() << "," << u4_pos.Y() << "," << u4_pos.Z() << ")" << std::endl;
     std::cout << "v4_pos = (" << v4_pos.X() << "," << v4_pos.Y() << "," << v4_pos.Z() << ")" << std::endl;
     
     std::cout << "x1_pos_offset = (" << x1_pos_offset.X() << "," << x1_pos_offset.Y() << "," << x1_pos_offset.Z() << ")" << std::endl;
     std::cout << "u1_pos_offset = (" << u1_pos_offset.X() << "," << u1_pos_offset.Y() << "," << u1_pos_offset.Z() << ")" << std::endl;
     std::cout << "v1_pos_offset = (" << v1_pos_offset.X() << "," << v1_pos_offset.Y() << "," << v1_pos_offset.Z() << ")" << std::endl;
     
     std::cout << "x2_pos_offset = (" << x2_pos_offset.X() << "," << x2_pos_offset.Y() << "," << x2_pos_offset.Z() << ")" << std::endl;
     std::cout << "u2_pos_offset = (" << u2_pos_offset.X() << "," << u2_pos_offset.Y() << "," << u2_pos_offset.Z() << ")" << std::endl;
     std::cout << "v2_pos_offset = (" << v2_pos_offset.X() << "," << v2_pos_offset.Y() << "," << v2_pos_offset.Z() << ")" << std::endl;
     
     std::cout << "x3_pos_offset = (" << x3_pos_offset.X() << "," << x3_pos_offset.Y() << "," << x3_pos_offset.Z() << ")" << std::endl;
     std::cout << "u3_pos_offset = (" << u3_pos_offset.X() << "," << u3_pos_offset.Y() << "," << u3_pos_offset.Z() << ")" << std::endl;
     std::cout << "v3_pos_offset = (" << v3_pos_offset.X() << "," << v3_pos_offset.Y() << "," << v3_pos_offset.Z() << ")" << std::endl;
     
     std::cout << "x4_pos_offset = (" << x4_pos_offset.X() << "," << x4_pos_offset.Y() << "," << x4_pos_offset.Z() << ")" << std::endl;
     std::cout << "u4_pos_offset = (" << u4_pos_offset.X() << "," << u4_pos_offset.Y() << "," << u4_pos_offset.Z() << ")" << std::endl;
     std::cout << "v4_pos_offset = (" << v4_pos_offset.X() << "," << v4_pos_offset.Y() << "," << v4_pos_offset.Z() << ")" << std::endl;
     
     std::cout << "x1_stereo angle = " << stereo_x1 << ", the offset = " << stereo_x1_offset << std::endl;
     std::cout << "u1_stereo angle = " << stereo_u1 << ", the offset = " << stereo_u1_offset << std::endl;
     std::cout << "v1_stereo angle = " << stereo_v1 << ", the offset = " << stereo_v1_offset << std::endl;
     
     std::cout << "x2_stereo angle = " << stereo_x2 << ", the offset = " << stereo_x2_offset << std::endl;
     std::cout << "u2_stereo angle = " << stereo_u2 << ", the offset = " << stereo_u2_offset << std::endl;
     std::cout << "v2_stereo angle = " << stereo_v2 << ", the offset = " << stereo_v2_offset << std::endl;
     
     std::cout << "x3_stereo angle = " << stereo_x3 << ", the offset = " << stereo_x3_offset << std::endl;
     std::cout << "u3_stereo angle = " << stereo_u3 << ", the offset = " << stereo_u3_offset << std::endl;
     std::cout << "v3_stereo angle = " << stereo_v3 << ", the offset = " << stereo_v3_offset << std::endl;
     
     std::cout << "x4_stereo angle = " << stereo_x4 << ", the offset = " << stereo_x4_offset << std::endl;
     std::cout << "u4_stereo angle = " << stereo_u4 << ", the offset = " << stereo_u4_offset << std::endl;
     std::cout << "v4_stereo angle = " << stereo_v4 << ", the offset = " << stereo_v4_offset << std::endl;*/
    
//    std::cout << "\nInitParameters .mac END" << std::endl;
//    std::cout << "x1_pos = (" << get_x1_pos().X() << "," << get_x1_pos().Y() << "," << get_x1_pos().Z() << ")" << std::endl;
//    std::cout << "u1_pos = (" << get_u1_pos().X() << "," << get_u1_pos().Y() << "," << get_u1_pos().Z() << ")" << std::endl;
//    std::cout << "v1_pos = (" << get_v1_pos().X() << "," << get_v1_pos().Y() << "," << get_v1_pos().Z() << ")" << std::endl;
//    
//    std::cout << "x2_pos = (" << get_x2_pos().X() << "," << get_x2_pos().Y() << "," << get_x2_pos().Z() << ")" << std::endl;
//    std::cout << "u2_pos = (" << get_u2_pos().X() << "," << get_u2_pos().Y() << "," << get_u2_pos().Z() << ")" << std::endl;
//    std::cout << "v2_pos = (" << get_v2_pos().X() << "," << get_v2_pos().Y() << "," << get_v2_pos().Z() << ")" << std::endl;
//    
//    std::cout << "x3_pos = (" << get_x3_pos().X() << "," << get_x3_pos().Y() << "," << get_x3_pos().Z() << ")" << std::endl;
//    std::cout << "u3_pos = (" << get_u3_pos().X() << "," << get_u3_pos().Y() << "," << get_u3_pos().Z() << ")" << std::endl;
//    std::cout << "v3_pos = (" << get_v3_pos().X() << "," << get_v3_pos().Y() << "," << get_v3_pos().Z() << ")" << std::endl;
//    
//    std::cout << "x4_pos = (" << get_x4_pos().X() << "," << get_x4_pos().Y() << "," << get_x4_pos().Z() << ")" << std::endl;
//    std::cout << "u4_pos = (" << get_u4_pos().X() << "," << get_u4_pos().Y() << "," << get_u4_pos().Z() << ")" << std::endl;
//    std::cout << "v4_pos = (" << get_v4_pos().X() << "," << get_v4_pos().Y() << "," << get_v4_pos().Z() << ")" << std::endl;
//    
//    std::cout << "x1_pos_offset = (" << get_x1_pos_offset().X() << "," << get_x1_pos_offset().Y() << "," << get_x1_pos_offset().Z() << ")" << std::endl;
//    std::cout << "u1_pos_offset = (" << get_u1_pos_offset().X() << "," << get_u1_pos_offset().Y() << "," << get_u1_pos_offset().Z() << ")" << std::endl;
//    std::cout << "v1_pos_offset = (" << get_v1_pos_offset().X() << "," << get_v1_pos_offset().Y() << "," << get_v1_pos_offset().Z() << ")" << std::endl;
//    
//    std::cout << "x2_pos_offset = (" << get_x2_pos_offset().X() << "," << get_x2_pos_offset().Y() << "," << get_x2_pos_offset().Z() << ")" << std::endl;
//    std::cout << "u2_pos_offset = (" << get_u2_pos_offset().X() << "," << get_u2_pos_offset().Y() << "," << get_u2_pos_offset().Z() << ")" << std::endl;
//    std::cout << "v2_pos_offset = (" << get_v2_pos_offset().X() << "," << get_v2_pos_offset().Y() << "," << get_v2_pos_offset().Z() << ")" << std::endl;
//    
//    std::cout << "x3_pos_offset = (" << get_x3_pos_offset().X() << "," << get_x3_pos_offset().Y() << "," << get_x3_pos_offset().Z() << ")" << std::endl;
//    std::cout << "u3_pos_offset = (" << get_u3_pos_offset().X() << "," << get_u3_pos_offset().Y() << "," << get_u3_pos_offset().Z() << ")" << std::endl;
//    std::cout << "v3_pos_offset = (" << get_v3_pos_offset().X() << "," << get_v3_pos_offset().Y() << "," << get_v3_pos_offset().Z() << ")" << std::endl;
//    
//    std::cout << "x4_pos_offset = (" << get_x4_pos_offset().X() << "," << get_x4_pos_offset().Y() << "," << get_x4_pos_offset().Z() << ")" << std::endl;
//    std::cout << "u4_pos_offset = (" << get_u4_pos_offset().X() << "," << get_u4_pos_offset().Y() << "," << get_u4_pos_offset().Z() << ")" << std::endl;
//    std::cout << "v4_pos_offset = (" << get_v4_pos_offset().X() << "," << get_v4_pos_offset().Y() << "," << get_v4_pos_offset().Z() << ")" << std::endl;
//    
//    
//    
//    std::cout << "The angle of x1 = " << stereo_x1 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of u1 = " << stereo_u1 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of v1 = " << stereo_v1 * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The angle of x2 = " << stereo_x2 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of u2 = " << stereo_u2 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of v2 = " << stereo_v2 * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The angle of x3 = " << stereo_x3 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of u3 = " << stereo_u3 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of v3 = " << stereo_v3 * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The angle of x4 = " << stereo_x4 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of u4 = " << stereo_u4 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of v4 = " << stereo_v4 * TMath::RadToDeg() << " deg." << std::endl;
//    
//    
//    std::cout << "The offset in angle of x1 = " << stereo_x1_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of u1 = " << stereo_u1_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of v1 = " << stereo_v1_offset * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The offset in angle of x2 = " << stereo_x2_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of u2 = " << stereo_u2_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of v2 = " << stereo_v2_offset * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The offset in angle of x3 = " << stereo_x3_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of u3 = " << stereo_u3_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of v3 = " << stereo_v3_offset * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The offset in angle of x4 = " << stereo_x4_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of u4 = " << stereo_u4_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of v4 = " << stereo_v4_offset * TMath::RadToDeg() << " deg." << std::endl;
}

// This function overloads the previous one to allow it to obtain
// the parameter values from the .root file output from geant4.
void TrackerGeometry::InitParameters(TTree * t_geom)
{
    // This function obtains the detector and tracker geometry
    
    // List of branches
    TBranch        *b_noOfSensorStrips;   //!
    TBranch        *b_teleStripPitch;   //!
    TBranch        *b_sensorStripLength;   //!
    TBranch        *b_sensorStripThickness;   //!
    
    TBranch        *b_inter_plane_dist;   //!
    TBranch        *b_inter_module_dist;   //!
    TBranch        *b_phantom_gap;   //!
    TBranch        *b_phantom_zShift;   //!
    
    TBranch        *b_halfPhantomSizeZ;   //!
    TBranch        *b_halfCalorimeterSizeZ;   //!
    TBranch        *b_calorimeter_gap;   //!

    TBranch        *b_shield_zShift;   //! //**************************************
    TBranch        *b_halfShieldSizeZ;   //!//**********************************

    // Get position and stereo angle of detectors (this will have the offsets below applied),
    // they are stored separatly from offsets below so tracking can be carried out with/without
    // 'knowing' the offset to see effect of unknown misalignment in the real data.
    TBranch        *b_x1_SensorPos_x;   //!
    TBranch        *b_u1_SensorPos_x;   //!
    TBranch        *b_v1_SensorPos_x;   //!
    TBranch        *b_x2_SensorPos_x;   //!
    TBranch        *b_u2_SensorPos_x;   //!
    TBranch        *b_v2_SensorPos_x;   //!
    TBranch        *b_x3_SensorPos_x;   //!
    TBranch        *b_u3_SensorPos_x;   //!
    TBranch        *b_v3_SensorPos_x;   //!
    TBranch        *b_x4_SensorPos_x;   //!
    TBranch        *b_u4_SensorPos_x;   //!
    TBranch        *b_v4_SensorPos_x;   //!
    
    TBranch        *b_x1_SensorPos_y;   //!
    TBranch        *b_u1_SensorPos_y;   //!
    TBranch        *b_v1_SensorPos_y;   //!
    TBranch        *b_x2_SensorPos_y;   //!
    TBranch        *b_u2_SensorPos_y;   //!
    TBranch        *b_v2_SensorPos_y;   //!
    TBranch        *b_x3_SensorPos_y;   //!
    TBranch        *b_u3_SensorPos_y;   //!
    TBranch        *b_v3_SensorPos_y;   //!
    TBranch        *b_x4_SensorPos_y;   //!
    TBranch        *b_u4_SensorPos_y;   //!
    TBranch        *b_v4_SensorPos_y;   //!
    
    TBranch        *b_x1_SensorAngle;   //!
    TBranch        *b_u1_SensorAngle;   //!
    TBranch        *b_v1_SensorAngle;   //!
    TBranch        *b_x2_SensorAngle;   //!
    TBranch        *b_u2_SensorAngle;   //!
    TBranch        *b_v2_SensorAngle;   //!
    TBranch        *b_x3_SensorAngle;   //!
    TBranch        *b_u3_SensorAngle;   //!
    TBranch        *b_v3_SensorAngle;   //!
    TBranch        *b_x4_SensorAngle;   //!
    TBranch        *b_u4_SensorAngle;   //!
    TBranch        *b_v4_SensorAngle;   //!
    
    // Get position offsets (set by user in macro), kept separate from position vectors so tracking can be carried out with/without
    // 'knowing' the offset to see effect of unknown misalignment in the real data. position of sensor centre calculated
    // in GEANT4 by adding position vector to offset vector.
    TBranch        *b_x1_SensorPosOffset_x;   //!
    TBranch        *b_u1_SensorPosOffset_x;   //!
    TBranch        *b_v1_SensorPosOffset_x;   //!
    TBranch        *b_x2_SensorPosOffset_x;   //!
    TBranch        *b_u2_SensorPosOffset_x;   //!
    TBranch        *b_v2_SensorPosOffset_x;   //!
    TBranch        *b_x3_SensorPosOffset_x;   //!
    TBranch        *b_u3_SensorPosOffset_x;   //!
    TBranch        *b_v3_SensorPosOffset_x;   //!
    TBranch        *b_x4_SensorPosOffset_x;   //!
    TBranch        *b_u4_SensorPosOffset_x;   //!
    TBranch        *b_v4_SensorPosOffset_x;   //!
    
    TBranch        *b_x1_SensorPosOffset_y;   //!
    TBranch        *b_u1_SensorPosOffset_y;   //!
    TBranch        *b_v1_SensorPosOffset_y;   //!
    TBranch        *b_x2_SensorPosOffset_y;   //!
    TBranch        *b_u2_SensorPosOffset_y;   //!
    TBranch        *b_v2_SensorPosOffset_y;   //!
    TBranch        *b_x3_SensorPosOffset_y;   //!
    TBranch        *b_u3_SensorPosOffset_y;   //!
    TBranch        *b_v3_SensorPosOffset_y;   //!
    TBranch        *b_x4_SensorPosOffset_y;   //!
    TBranch        *b_u4_SensorPosOffset_y;   //!
    TBranch        *b_v4_SensorPosOffset_y;   //!
    
    // Get stereo angle offsets (set by user in macro), kept separate from position vectors so tracking can be carried out with/without
    // 'knowing' the offset to see effect of unknown misalignment in the real data. position of sensor centre calculated
    // in GEANT4 by adding position vector to offset vector.
    TBranch        *b_x1_SensorAngleOffset;   //!
    TBranch        *b_u1_SensorAngleOffset;   //!
    TBranch        *b_v1_SensorAngleOffset;   //!
    TBranch        *b_x2_SensorAngleOffset;   //!
    TBranch        *b_u2_SensorAngleOffset;   //!
    TBranch        *b_v2_SensorAngleOffset;   //!
    TBranch        *b_x3_SensorAngleOffset;   //!
    TBranch        *b_u3_SensorAngleOffset;   //!
    TBranch        *b_v3_SensorAngleOffset;   //!
    TBranch        *b_x4_SensorAngleOffset;   //!
    TBranch        *b_u4_SensorAngleOffset;   //!
    TBranch        *b_v4_SensorAngleOffset;   //!
    
    
    
    t_geom->SetBranchAddress("noOfSensorStrips", &strips, &b_noOfSensorStrips);
    t_geom->SetBranchAddress("teleStripPitch", &s_pitch, &b_teleStripPitch);
    t_geom->SetBranchAddress("sensorStripLength", &s_length, &b_sensorStripLength);
    t_geom->SetBranchAddress("sensorThickness", &thickness, &b_sensorStripThickness);
    
    t_geom->SetBranchAddress("inter_plane_dist", &inter_plane_dist, &b_inter_plane_dist);
    t_geom->SetBranchAddress("inter_module_dist", &inter_module_dist, &b_inter_module_dist);
    t_geom->SetBranchAddress("phantom_gap", &phantom_gap, &b_phantom_gap);
    t_geom->SetBranchAddress("phantom_zShift", &phantom_zShift, &b_phantom_zShift);
    
    t_geom->SetBranchAddress("halfPhantomSizeZ", &halfPhantomSizeZ, &b_halfPhantomSizeZ);
    t_geom->SetBranchAddress("halfCalorimeterSizeZ", &halfCalorimeterSizeZ, &b_halfCalorimeterSizeZ);
    t_geom->SetBranchAddress("calorimeter_gap", &calorimeter_gap, &b_calorimeter_gap);
    
    t_geom->SetBranchAddress("shield_zShift", &shield_zShift, &b_shield_zShift);//****************************************
    t_geom->SetBranchAddress("halfShieldSizeZ", &halfShieldSizeZ, &b_halfShieldSizeZ);//*************************************
    
    // Above method doesn't work for retrieving variables set as vectors
    b_x1_SensorPos_x = t_geom->GetBranch("x1_SensorPos_x");
    b_x1_SensorPos_x->SetAddress(&temp);
    b_x1_SensorPos_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x1_pos.SetX(temp); temp = 0;
    
    b_u1_SensorPos_x = t_geom->GetBranch("u1_SensorPos_x");
    b_u1_SensorPos_x->SetAddress(&temp);
    b_u1_SensorPos_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u1_pos.SetX(temp); temp = 0;
    
    b_v1_SensorPos_x = t_geom->GetBranch("v1_SensorPos_x");
    b_v1_SensorPos_x->SetAddress(&temp);
    b_v1_SensorPos_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v1_pos.SetX(temp); temp = 0;
    
    
    
    b_x2_SensorPos_x = t_geom->GetBranch("x2_SensorPos_x");
    b_x2_SensorPos_x->SetAddress(&temp);
    b_x2_SensorPos_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x2_pos.SetX(temp); temp = 0;
    
    b_u2_SensorPos_x = t_geom->GetBranch("u2_SensorPos_x");
    b_u2_SensorPos_x->SetAddress(&temp);
    b_u2_SensorPos_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u2_pos.SetX(temp); temp = 0;
    
    b_v2_SensorPos_x = t_geom->GetBranch("v2_SensorPos_x");
    b_v2_SensorPos_x->SetAddress(&temp);
    b_v2_SensorPos_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v2_pos.SetX(temp); temp = 0;
    
    
    
    b_x3_SensorPos_x = t_geom->GetBranch("x3_SensorPos_x");
    b_x3_SensorPos_x->SetAddress(&temp);
    b_x3_SensorPos_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x3_pos.SetX(temp); temp = 0;
    
    b_u3_SensorPos_x = t_geom->GetBranch("u3_SensorPos_x");
    b_u3_SensorPos_x->SetAddress(&temp);
    b_u3_SensorPos_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u3_pos.SetX(temp); temp = 0;
    
    b_v3_SensorPos_x = t_geom->GetBranch("v3_SensorPos_x");
    b_v3_SensorPos_x->SetAddress(&temp);
    b_v3_SensorPos_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v3_pos.SetX(temp); temp = 0;
    
    
    
    b_x4_SensorPos_x = t_geom->GetBranch("x4_SensorPos_x");
    b_x4_SensorPos_x->SetAddress(&temp);
    b_x4_SensorPos_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x4_pos.SetX(temp); temp = 0;
    
    b_u4_SensorPos_x = t_geom->GetBranch("u4_SensorPos_x");
    b_u4_SensorPos_x->SetAddress(&temp);
    b_u4_SensorPos_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u4_pos.SetX(temp); temp = 0;
    
    b_v4_SensorPos_x = t_geom->GetBranch("v4_SensorPos_x");
    b_v4_SensorPos_x->SetAddress(&temp);
    b_v4_SensorPos_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v4_pos.SetX(temp); temp = 0;
    
    
    
//    t_geom->SetBranchAddress("x1_SensorPos_x", &temp, &b_x1_SensorPos_x);
//    x1_pos.SetX( temp );
//    t_geom->SetBranchAddress("u1_SensorPos_x", &temp, &b_u1_SensorPos_x);
//    u1_pos.SetX( temp );
//    t_geom->SetBranchAddress("v1_SensorPos_x", &temp, &b_v1_SensorPos_x);
//    v1_pos.SetX( temp );
    
//    t_geom->SetBranchAddress("x2_SensorPos_x", &temp, &b_x2_SensorPos_x);
//    x2_pos.SetX( temp );
//    t_geom->SetBranchAddress("u2_SensorPos_x", &temp, &b_u2_SensorPos_x);
//    u2_pos.SetX( temp );
//    t_geom->SetBranchAddress("v2_SensorPos_x", &temp, &b_v2_SensorPos_x);
//    v2_pos.SetX( temp );
    
//    t_geom->SetBranchAddress("x3_SensorPos_x", &temp, &b_x3_SensorPos_x);
//    x3_pos.SetX( temp );
//    t_geom->SetBranchAddress("u3_SensorPos_x", &temp, &b_u3_SensorPos_x);
//    u3_pos.SetX( temp );
//    t_geom->SetBranchAddress("v3_SensorPos_x", &temp, &b_v3_SensorPos_x);
//    v3_pos.SetX( temp );
    
//    t_geom->SetBranchAddress("x4_SensorPos_x", &temp, &b_x4_SensorPos_x);
//    x4_pos.SetX( temp );
//    t_geom->SetBranchAddress("u4_SensorPos_x", &temp, &b_u4_SensorPos_x);
//    u4_pos.SetX( temp );
//    t_geom->SetBranchAddress("v4_SensorPos_x", &temp, &b_v4_SensorPos_x);
//    v4_pos.SetX( temp );
    

    b_x1_SensorPos_y = t_geom->GetBranch("x1_SensorPos_y");
    b_x1_SensorPos_y->SetAddress(&temp);
    b_x1_SensorPos_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x1_pos.SetY(temp); temp = 0;
    
    b_u1_SensorPos_y = t_geom->GetBranch("u1_SensorPos_y");
    b_u1_SensorPos_y->SetAddress(&temp);
    b_u1_SensorPos_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u1_pos.SetY(temp); temp = 0;
    
    b_v1_SensorPos_y = t_geom->GetBranch("v1_SensorPos_y");
    b_v1_SensorPos_y->SetAddress(&temp);
    b_v1_SensorPos_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v1_pos.SetY(temp); temp = 0;
    
    
    
    b_x2_SensorPos_y = t_geom->GetBranch("x2_SensorPos_y");
    b_x2_SensorPos_y->SetAddress(&temp);
    b_x2_SensorPos_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x2_pos.SetY(temp); temp = 0;
    
    b_u2_SensorPos_y = t_geom->GetBranch("u2_SensorPos_y");
    b_u2_SensorPos_y->SetAddress(&temp);
    b_u2_SensorPos_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u2_pos.SetY(temp); temp = 0;
    
    b_v2_SensorPos_y = t_geom->GetBranch("v2_SensorPos_y");
    b_v2_SensorPos_y->SetAddress(&temp);
    b_v2_SensorPos_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v2_pos.SetY(temp); temp = 0;
    
    
    
    b_x3_SensorPos_y = t_geom->GetBranch("x3_SensorPos_y");
    b_x3_SensorPos_y->SetAddress(&temp);
    b_x3_SensorPos_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x3_pos.SetY(temp); temp = 0;
    
    b_u3_SensorPos_y = t_geom->GetBranch("u3_SensorPos_y");
    b_u3_SensorPos_y->SetAddress(&temp);
    b_u3_SensorPos_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u3_pos.SetY(temp); temp = 0;
    
    b_v3_SensorPos_y = t_geom->GetBranch("v3_SensorPos_y");
    b_v3_SensorPos_y->SetAddress(&temp);
    b_v3_SensorPos_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v3_pos.SetY(temp); temp = 0;
    
    
    
    b_x4_SensorPos_y = t_geom->GetBranch("x4_SensorPos_y");
    b_x4_SensorPos_y->SetAddress(&temp);
    b_x4_SensorPos_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x4_pos.SetY(temp); temp = 0;
    
    b_u4_SensorPos_y = t_geom->GetBranch("u4_SensorPos_y");
    b_u4_SensorPos_y->SetAddress(&temp);
    b_u4_SensorPos_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u4_pos.SetY(temp); temp = 0;
    
    b_v4_SensorPos_y = t_geom->GetBranch("v4_SensorPos_y");
    b_v4_SensorPos_y->SetAddress(&temp);
    b_v4_SensorPos_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v4_pos.SetY(temp); temp = 0;
    
    
//    t_geom->SetBranchAddress("x1_SensorPos_y", &temp, &b_x1_SensorPos_y);
//    x1_pos.SetY( temp );
//    t_geom->SetBranchAddress("u1_SensorPos_y", &temp, &b_u1_SensorPos_y);
//    u1_pos.SetY( temp );
//    t_geom->SetBranchAddress("v1_SensorPos_y", &temp, &b_v1_SensorPos_y);
//    v1_pos.SetY( temp );
//    
//    t_geom->SetBranchAddress("x2_SensorPos_y", &temp, &b_x2_SensorPos_y);
//    x2_pos.SetY( temp );
//    t_geom->SetBranchAddress("u2_SensorPos_y", &temp, &b_u2_SensorPos_y);
//    u2_pos.SetY( temp );
//    t_geom->SetBranchAddress("v2_SensorPos_y", &temp, &b_v2_SensorPos_y);
//    v2_pos.SetY( temp );
//    
//    t_geom->SetBranchAddress("x3_SensorPos_y", &temp, &b_x3_SensorPos_y);
//    x3_pos.SetY( temp );
//    t_geom->SetBranchAddress("u3_SensorPos_y", &temp, &b_u3_SensorPos_y);
//    u3_pos.SetY( temp );
//    t_geom->SetBranchAddress("v3_SensorPos_y", &temp, &b_v3_SensorPos_y);
//    v3_pos.SetY( temp );
//    
//    t_geom->SetBranchAddress("x4_SensorPos_y", &temp, &b_x4_SensorPos_y);
//    x4_pos.SetY( temp );
//    t_geom->SetBranchAddress("u4_SensorPos_y", &temp, &b_u4_SensorPos_y);
//    u4_pos.SetY( temp );
//    t_geom->SetBranchAddress("v4_SensorPos_y", &temp, &b_v4_SensorPos_y);
//    v4_pos.SetY( temp );
    
    
    t_geom->SetBranchAddress("x1_SensorAngle", &stereo_x1, &b_x1_SensorAngle);
    t_geom->SetBranchAddress("u1_SensorAngle", &stereo_u1, &b_u1_SensorAngle);
    t_geom->SetBranchAddress("v1_SensorAngle", &stereo_v1, &b_v1_SensorAngle);
    t_geom->SetBranchAddress("x2_SensorAngle", &stereo_x2, &b_x2_SensorAngle);
    t_geom->SetBranchAddress("u2_SensorAngle", &stereo_u2, &b_u2_SensorAngle);
    t_geom->SetBranchAddress("v2_SensorAngle", &stereo_v2, &b_v2_SensorAngle);
    t_geom->SetBranchAddress("x3_SensorAngle", &stereo_x3, &b_x3_SensorAngle);
    t_geom->SetBranchAddress("u3_SensorAngle", &stereo_u3, &b_u3_SensorAngle);
    t_geom->SetBranchAddress("v3_SensorAngle", &stereo_v3, &b_v3_SensorAngle);
    t_geom->SetBranchAddress("x4_SensorAngle", &stereo_x4, &b_x4_SensorAngle);
    t_geom->SetBranchAddress("u4_SensorAngle", &stereo_u4, &b_u4_SensorAngle);
    t_geom->SetBranchAddress("v4_SensorAngle", &stereo_v4, &b_v4_SensorAngle);
    
    
    
    b_x1_SensorPosOffset_x = t_geom->GetBranch("x1_SensorPosOffset_x");
    b_x1_SensorPosOffset_x->SetAddress(&temp);
    b_x1_SensorPosOffset_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x1_pos_offset.SetX(temp); temp = 0;
    
    b_u1_SensorPosOffset_x = t_geom->GetBranch("u1_SensorPosOffset_x");
    b_u1_SensorPosOffset_x->SetAddress(&temp);
    b_u1_SensorPosOffset_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u1_pos_offset.SetX(temp); temp = 0;
    
    b_v1_SensorPosOffset_x = t_geom->GetBranch("v1_SensorPosOffset_x");
    b_v1_SensorPosOffset_x->SetAddress(&temp);
    b_v1_SensorPosOffset_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v1_pos_offset.SetX(temp); temp = 0;
    
    
    
    b_x2_SensorPosOffset_x = t_geom->GetBranch("x2_SensorPosOffset_x");
    b_x2_SensorPosOffset_x->SetAddress(&temp);
    b_x2_SensorPosOffset_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x2_pos_offset.SetX(temp); temp = 0;
    
    b_u2_SensorPosOffset_x = t_geom->GetBranch("u2_SensorPosOffset_x");
    b_u2_SensorPosOffset_x->SetAddress(&temp);
    b_u2_SensorPosOffset_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u2_pos_offset.SetX(temp); temp = 0;
    
    b_v2_SensorPosOffset_x = t_geom->GetBranch("v2_SensorPosOffset_x");
    b_v2_SensorPosOffset_x->SetAddress(&temp);
    b_v2_SensorPosOffset_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v2_pos_offset.SetX(temp); temp = 0;
    
    
    
    b_x3_SensorPosOffset_x = t_geom->GetBranch("x3_SensorPosOffset_x");
    b_x3_SensorPosOffset_x->SetAddress(&temp);
    b_x3_SensorPosOffset_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x3_pos_offset.SetX(temp); temp = 0;
    
    b_u3_SensorPosOffset_x = t_geom->GetBranch("u3_SensorPosOffset_x");
    b_u3_SensorPosOffset_x->SetAddress(&temp);
    b_u3_SensorPosOffset_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u3_pos_offset.SetX(temp); temp = 0;
    
    b_v3_SensorPosOffset_x = t_geom->GetBranch("v3_SensorPosOffset_x");
    b_v3_SensorPosOffset_x->SetAddress(&temp);
    b_v3_SensorPosOffset_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v3_pos_offset.SetX(temp); temp = 0;
    
    
    
    b_x4_SensorPosOffset_x = t_geom->GetBranch("x4_SensorPosOffset_x");
    b_x4_SensorPosOffset_x->SetAddress(&temp);
    b_x4_SensorPosOffset_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x4_pos_offset.SetX(temp); temp = 0;
    
    b_u4_SensorPosOffset_x = t_geom->GetBranch("u4_SensorPosOffset_x");
    b_u4_SensorPosOffset_x->SetAddress(&temp);
    b_u4_SensorPosOffset_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u4_pos_offset.SetX(temp); temp = 0;
    
    b_v4_SensorPosOffset_x = t_geom->GetBranch("v4_SensorPosOffset_x");
    b_v4_SensorPosOffset_x->SetAddress(&temp);
    b_v4_SensorPosOffset_x->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v4_pos_offset.SetX(temp); temp = 0;
    
    
    
    b_x1_SensorPosOffset_y = t_geom->GetBranch("x1_SensorPosOffset_y");
    b_x1_SensorPosOffset_y->SetAddress(&temp);
    b_x1_SensorPosOffset_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x1_pos_offset.SetY(temp); temp = 0;
    
    b_u1_SensorPosOffset_y = t_geom->GetBranch("u1_SensorPosOffset_y");
    b_u1_SensorPosOffset_y->SetAddress(&temp);
    b_u1_SensorPosOffset_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u1_pos_offset.SetY(temp); temp = 0;
    
    b_v1_SensorPosOffset_y = t_geom->GetBranch("v1_SensorPosOffset_y");
    b_v1_SensorPosOffset_y->SetAddress(&temp);
    b_v1_SensorPosOffset_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v1_pos_offset.SetY(temp); temp = 0;
    
    
    
    b_x2_SensorPosOffset_y = t_geom->GetBranch("x2_SensorPosOffset_y");
    b_x2_SensorPosOffset_y->SetAddress(&temp);
    b_x2_SensorPosOffset_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x2_pos_offset.SetY(temp); temp = 0;
    
    b_u2_SensorPosOffset_y = t_geom->GetBranch("u2_SensorPosOffset_y");
    b_u2_SensorPosOffset_y->SetAddress(&temp);
    b_u2_SensorPosOffset_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u2_pos_offset.SetY(temp); temp = 0;
    
    b_v2_SensorPosOffset_y = t_geom->GetBranch("v2_SensorPosOffset_y");
    b_v2_SensorPosOffset_y->SetAddress(&temp);
    b_v2_SensorPosOffset_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v2_pos_offset.SetY(temp); temp = 0;
    
    
    
    b_x3_SensorPosOffset_y = t_geom->GetBranch("x3_SensorPosOffset_y");
    b_x3_SensorPosOffset_y->SetAddress(&temp);
    b_x3_SensorPosOffset_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x3_pos_offset.SetY(temp); temp = 0;
    
    b_u3_SensorPosOffset_y = t_geom->GetBranch("u3_SensorPosOffset_y");
    b_u3_SensorPosOffset_y->SetAddress(&temp);
    b_u3_SensorPosOffset_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u3_pos_offset.SetY(temp); temp = 0;
    
    b_v3_SensorPosOffset_y = t_geom->GetBranch("v3_SensorPosOffset_y");
    b_v3_SensorPosOffset_y->SetAddress(&temp);
    b_v3_SensorPosOffset_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v3_pos_offset.SetY(temp); temp = 0;
    
    
    
    b_x4_SensorPosOffset_y = t_geom->GetBranch("x4_SensorPosOffset_y");
    b_x4_SensorPosOffset_y->SetAddress(&temp);
    b_x4_SensorPosOffset_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    x4_pos_offset.SetY(temp); temp = 0;
    
    b_u4_SensorPosOffset_y = t_geom->GetBranch("u4_SensorPosOffset_y");
    b_u4_SensorPosOffset_y->SetAddress(&temp);
    b_u4_SensorPosOffset_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    u4_pos_offset.SetY(temp); temp = 0;
    
    b_v4_SensorPosOffset_y = t_geom->GetBranch("v4_SensorPosOffset_y");
    b_v4_SensorPosOffset_y->SetAddress(&temp);
    b_v4_SensorPosOffset_y->SetAutoDelete(kTRUE);
    t_geom->GetEntry(0);
    v4_pos_offset.SetY(temp); temp = 0;
    
    
//    t_geom->SetBranchAddress("x1_SensorPosOffset_x", &temp, &b_x1_SensorPosOffset_x);
//    x1_pos_offset.SetX( temp );
//    t_geom->SetBranchAddress("u1_SensorPosOffset_x", &temp, &b_u1_SensorPosOffset_x);
//    u1_pos_offset.SetX( temp );
//    t_geom->SetBranchAddress("v1_SensorPosOffset_x", &temp, &b_v1_SensorPosOffset_x);
//    v1_pos_offset.SetX( temp );
    
//    t_geom->SetBranchAddress("x2_SensorPosOffset_x", &temp, &b_x2_SensorPosOffset_x);
//    x2_pos_offset.SetX( temp );
//    t_geom->SetBranchAddress("u2_SensorPosOffset_x", &temp, &b_u2_SensorPosOffset_x);
//    u2_pos_offset.SetX( temp );
//    t_geom->SetBranchAddress("v2_SensorPosOffset_x", &temp, &b_v2_SensorPosOffset_x);
//    v2_pos_offset.SetX( temp );
//    
//    t_geom->SetBranchAddress("x3_SensorPosOffset_x", &temp, &b_x3_SensorPosOffset_x);
//    x3_pos_offset.SetX( temp );
//    t_geom->SetBranchAddress("u3_SensorPosOffset_x", &temp, &b_u3_SensorPosOffset_x);
//    u3_pos_offset.SetX( temp );
//    t_geom->SetBranchAddress("v3_SensorPosOffset_x", &temp, &b_v3_SensorPosOffset_x);
//    v3_pos_offset.SetX( temp );
//    
//    t_geom->SetBranchAddress("x4_SensorPosOffset_x", &temp, &b_x4_SensorPosOffset_x);
//    x4_pos_offset.SetX( temp );
//    t_geom->SetBranchAddress("u4_SensorPosOffset_x", &temp, &b_u4_SensorPosOffset_x);
//    u4_pos_offset.SetX( temp );
//    t_geom->SetBranchAddress("v4_SensorPosOffset_x", &temp, &b_v4_SensorPosOffset_x);
//    v4_pos_offset.SetX( temp );
    
    
//    t_geom->SetBranchAddress("x1_SensorPosOffset_y", &temp, &b_x1_SensorPosOffset_y);
//    x1_pos_offset.SetY( temp );
//    t_geom->SetBranchAddress("u1_SensorPosOffset_y", &temp, &b_u1_SensorPosOffset_y);
//    u1_pos_offset.SetY( temp );
//    t_geom->SetBranchAddress("v1_SensorPosOffset_y", &temp, &b_v1_SensorPosOffset_y);
//    v1_pos_offset.SetY( temp );
//    
//    t_geom->SetBranchAddress("x2_SensorPosOffset_y", &temp, &b_x2_SensorPosOffset_y);
//    x2_pos_offset.SetY( temp );
//    t_geom->SetBranchAddress("u2_SensorPosOffset_y", &temp, &b_u2_SensorPosOffset_y);
//    u2_pos_offset.SetY( temp );
//    t_geom->SetBranchAddress("v2_SensorPosOffset_y", &temp, &b_v2_SensorPosOffset_y);
//    v2_pos_offset.SetY( temp );
//    
//    t_geom->SetBranchAddress("x3_SensorPosOffset_y", &temp, &b_x3_SensorPosOffset_y);
//    x3_pos_offset.SetY( temp );
//    t_geom->SetBranchAddress("u3_SensorPosOffset_y", &temp, &b_u3_SensorPosOffset_y);
//    u3_pos_offset.SetY( temp );
//    t_geom->SetBranchAddress("v3_SensorPosOffset_y", &temp, &b_v3_SensorPosOffset_y);
//    v3_pos_offset.SetY( temp );
//    
//    t_geom->SetBranchAddress("x4_SensorPosOffset_y", &temp, &b_x4_SensorPosOffset_y);
//    x4_pos_offset.SetY( temp );
//    t_geom->SetBranchAddress("u4_SensorPosOffset_y", &temp, &b_u4_SensorPosOffset_y);
//    u4_pos_offset.SetY( temp );
//    t_geom->SetBranchAddress("v4_SensorPosOffset_y", &temp, &b_v4_SensorPosOffset_y);
//    v4_pos_offset.SetY( temp );
    
    
    t_geom->SetBranchAddress("x1_SensorAngleOffset", &stereo_x1_offset, &b_x1_SensorAngleOffset);
    t_geom->SetBranchAddress("u1_SensorAngleOffset", &stereo_u1_offset, &b_u1_SensorAngleOffset);
    t_geom->SetBranchAddress("v1_SensorAngleOffset", &stereo_v1_offset, &b_v1_SensorAngleOffset);
    t_geom->SetBranchAddress("x2_SensorAngleOffset", &stereo_x2_offset, &b_x2_SensorAngleOffset);
    t_geom->SetBranchAddress("u2_SensorAngleOffset", &stereo_u2_offset, &b_u2_SensorAngleOffset);
    t_geom->SetBranchAddress("v2_SensorAngleOffset", &stereo_v2_offset, &b_v2_SensorAngleOffset);
    t_geom->SetBranchAddress("x3_SensorAngleOffset", &stereo_x3_offset, &b_x3_SensorAngleOffset);
    t_geom->SetBranchAddress("u3_SensorAngleOffset", &stereo_u3_offset, &b_u3_SensorAngleOffset);
    t_geom->SetBranchAddress("v3_SensorAngleOffset", &stereo_v3_offset, &b_v3_SensorAngleOffset);
    t_geom->SetBranchAddress("x4_SensorAngleOffset", &stereo_x4_offset, &b_x4_SensorAngleOffset);
    t_geom->SetBranchAddress("u4_SensorAngleOffset", &stereo_u4_offset, &b_u4_SensorAngleOffset);
    t_geom->SetBranchAddress("v4_SensorAngleOffset", &stereo_v4_offset, &b_v4_SensorAngleOffset);
    
    t_geom->GetEntry(0);
    
    
    // Printout all variables for debug
//    std::cout << "\nPrinting geom params.." << std::endl;
//     std::cout << "s_pitch = " << s_pitch << std::endl;
//     std::cout << "s_length = " << s_length << std::endl;
//     std::cout << "strips = " << strips << std::endl;
//     
//     std::cout << "halfPhantomSizeZ = " << halfPhantomSizeZ << std::endl;
//     std::cout << "halfCalorimeterSizeZ = " << halfCalorimeterSizeZ << std::endl;
//     std::cout << "calorimeter_gap = " << calorimeter_gap << std::endl;
//     
//     std::cout << "plane_dist = " << inter_plane_dist << std::endl;
//     std::cout << "module_dist = " << inter_module_dist << std::endl;
//     std::cout << "phantom_gap = " << phantom_gap << std::endl;
//     std::cout << "phantom_zShift = " << phantom_zShift << std::endl;
//     
//     std::cout << "x1_pos = (" << x1_pos.X() << "," << x1_pos.Y() << "," << x1_pos.Z() << ")" << std::endl;
//     std::cout << "u1_pos = (" << u1_pos.X() << "," << u1_pos.Y() << "," << u1_pos.Z() << ")" << std::endl;
//     std::cout << "v1_pos = (" << v1_pos.X() << "," << v1_pos.Y() << "," << v1_pos.Z() << ")" << std::endl;
//     
//     std::cout << "x2_pos = (" << x2_pos.X() << "," << x2_pos.Y() << "," << x2_pos.Z() << ")" << std::endl;
//     std::cout << "u2_pos = (" << u2_pos.X() << "," << u2_pos.Y() << "," << u2_pos.Z() << ")" << std::endl;
//     std::cout << "v2_pos = (" << v2_pos.X() << "," << v2_pos.Y() << "," << v2_pos.Z() << ")" << std::endl;
//     
//     std::cout << "x3_pos = (" << x3_pos.X() << "," << x3_pos.Y() << "," << x3_pos.Z() << ")" << std::endl;
//     std::cout << "u3_pos = (" << u3_pos.X() << "," << u3_pos.Y() << "," << u3_pos.Z() << ")" << std::endl;
//     std::cout << "v3_pos = (" << v3_pos.X() << "," << v3_pos.Y() << "," << v3_pos.Z() << ")" << std::endl;
//     
//     std::cout << "x4_pos = (" << x4_pos.X() << "," << x4_pos.Y() << "," << x4_pos.Z() << ")" << std::endl;
//     std::cout << "u4_pos = (" << u4_pos.X() << "," << u4_pos.Y() << "," << u4_pos.Z() << ")" << std::endl;
//     std::cout << "v4_pos = (" << v4_pos.X() << "," << v4_pos.Y() << "," << v4_pos.Z() << ")" << std::endl;
//     
//     std::cout << "x1_pos_offset = (" << x1_pos_offset.X() << "," << x1_pos_offset.Y() << "," << x1_pos_offset.Z() << ")" << std::endl;
//     std::cout << "u1_pos_offset = (" << u1_pos_offset.X() << "," << u1_pos_offset.Y() << "," << u1_pos_offset.Z() << ")" << std::endl;
//     std::cout << "v1_pos_offset = (" << v1_pos_offset.X() << "," << v1_pos_offset.Y() << "," << v1_pos_offset.Z() << ")" << std::endl;
//     
//     std::cout << "x2_pos_offset = (" << x2_pos_offset.X() << "," << x2_pos_offset.Y() << "," << x2_pos_offset.Z() << ")" << std::endl;
//     std::cout << "u2_pos_offset = (" << u2_pos_offset.X() << "," << u2_pos_offset.Y() << "," << u2_pos_offset.Z() << ")" << std::endl;
//     std::cout << "v2_pos_offset = (" << v2_pos_offset.X() << "," << v2_pos_offset.Y() << "," << v2_pos_offset.Z() << ")" << std::endl;
//     
//     std::cout << "x3_pos_offset = (" << x3_pos_offset.X() << "," << x3_pos_offset.Y() << "," << x3_pos_offset.Z() << ")" << std::endl;
//     std::cout << "u3_pos_offset = (" << u3_pos_offset.X() << "," << u3_pos_offset.Y() << "," << u3_pos_offset.Z() << ")" << std::endl;
//     std::cout << "v3_pos_offset = (" << v3_pos_offset.X() << "," << v3_pos_offset.Y() << "," << v3_pos_offset.Z() << ")" << std::endl;
//     
//     std::cout << "x4_pos_offset = (" << x4_pos_offset.X() << "," << x4_pos_offset.Y() << "," << x4_pos_offset.Z() << ")" << std::endl;
//     std::cout << "u4_pos_offset = (" << u4_pos_offset.X() << "," << u4_pos_offset.Y() << "," << u4_pos_offset.Z() << ")" << std::endl;
//     std::cout << "v4_pos_offset = (" << v4_pos_offset.X() << "," << v4_pos_offset.Y() << "," << v4_pos_offset.Z() << ")" << std::endl;
//     
//     std::cout << "x1_stereo angle = " << stereo_x1 << ", the offset = " << stereo_x1_offset << std::endl;
//     std::cout << "u1_stereo angle = " << stereo_u1 << ", the offset = " << stereo_u1_offset << std::endl;
//     std::cout << "v1_stereo angle = " << stereo_v1 << ", the offset = " << stereo_v1_offset << std::endl;
//     
//     std::cout << "x2_stereo angle = " << stereo_x2 << ", the offset = " << stereo_x2_offset << std::endl;
//     std::cout << "u2_stereo angle = " << stereo_u2 << ", the offset = " << stereo_u2_offset << std::endl;
//     std::cout << "v2_stereo angle = " << stereo_v2 << ", the offset = " << stereo_v2_offset << std::endl;
//     
//     std::cout << "x3_stereo angle = " << stereo_x3 << ", the offset = " << stereo_x3_offset << std::endl;
//     std::cout << "u3_stereo angle = " << stereo_u3 << ", the offset = " << stereo_u3_offset << std::endl;
//     std::cout << "v3_stereo angle = " << stereo_v3 << ", the offset = " << stereo_v3_offset << std::endl;
//     
//     std::cout << "x4_stereo angle = " << stereo_x4 << ", the offset = " << stereo_x4_offset << std::endl;
//     std::cout << "u4_stereo angle = " << stereo_u4 << ", the offset = " << stereo_u4_offset << std::endl;
//     std::cout << "v4_stereo angle = " << stereo_v4 << ", the offset = " << stereo_v4_offset << std::endl;
    
    
//    std::cout << "\nInitParameters .root END" << std::endl;
//    std::cout << "x1_pos = (" << get_x1_pos().X() << "," << get_x1_pos().Y() << "," << get_x1_pos().Z() << ")" << std::endl;
//    std::cout << "u1_pos = (" << get_u1_pos().X() << "," << get_u1_pos().Y() << "," << get_u1_pos().Z() << ")" << std::endl;
//    std::cout << "v1_pos = (" << get_v1_pos().X() << "," << get_v1_pos().Y() << "," << get_v1_pos().Z() << ")" << std::endl;
//    
//    std::cout << "x2_pos = (" << get_x2_pos().X() << "," << get_x2_pos().Y() << "," << get_x2_pos().Z() << ")" << std::endl;
//    std::cout << "u2_pos = (" << get_u2_pos().X() << "," << get_u2_pos().Y() << "," << get_u2_pos().Z() << ")" << std::endl;
//    std::cout << "v2_pos = (" << get_v2_pos().X() << "," << get_v2_pos().Y() << "," << get_v2_pos().Z() << ")" << std::endl;
//    
//    std::cout << "x3_pos = (" << get_x3_pos().X() << "," << get_x3_pos().Y() << "," << get_x3_pos().Z() << ")" << std::endl;
//    std::cout << "u3_pos = (" << get_u3_pos().X() << "," << get_u3_pos().Y() << "," << get_u3_pos().Z() << ")" << std::endl;
//    std::cout << "v3_pos = (" << get_v3_pos().X() << "," << get_v3_pos().Y() << "," << get_v3_pos().Z() << ")" << std::endl;
//    
//    std::cout << "x4_pos = (" << get_x4_pos().X() << "," << get_x4_pos().Y() << "," << get_x4_pos().Z() << ")" << std::endl;
//    std::cout << "u4_pos = (" << get_u4_pos().X() << "," << get_u4_pos().Y() << "," << get_u4_pos().Z() << ")" << std::endl;
//    std::cout << "v4_pos = (" << get_v4_pos().X() << "," << get_v4_pos().Y() << "," << get_v4_pos().Z() << ")" << std::endl;
//    
//    std::cout << "x1_pos_offset = (" << get_x1_pos_offset().X() << "," << get_x1_pos_offset().Y() << "," << get_x1_pos_offset().Z() << ")" << std::endl;
//    std::cout << "u1_pos_offset = (" << get_u1_pos_offset().X() << "," << get_u1_pos_offset().Y() << "," << get_u1_pos_offset().Z() << ")" << std::endl;
//    std::cout << "v1_pos_offset = (" << get_v1_pos_offset().X() << "," << get_v1_pos_offset().Y() << "," << get_v1_pos_offset().Z() << ")" << std::endl;
//
//    std::cout << "x2_pos_offset = (" << get_x2_pos_offset().X() << "," << get_x2_pos_offset().Y() << "," << get_x2_pos_offset().Z() << ")" << std::endl;
//    std::cout << "u2_pos_offset = (" << get_u2_pos_offset().X() << "," << get_u2_pos_offset().Y() << "," << get_u2_pos_offset().Z() << ")" << std::endl;
//    std::cout << "v2_pos_offset = (" << get_v2_pos_offset().X() << "," << get_v2_pos_offset().Y() << "," << get_v2_pos_offset().Z() << ")" << std::endl;
//    
//    std::cout << "x3_pos_offset = (" << get_x3_pos_offset().X() << "," << get_x3_pos_offset().Y() << "," << get_x3_pos_offset().Z() << ")" << std::endl;
//    std::cout << "u3_pos_offset = (" << get_u3_pos_offset().X() << "," << get_u3_pos_offset().Y() << "," << get_u3_pos_offset().Z() << ")" << std::endl;
//    std::cout << "v3_pos_offset = (" << get_v3_pos_offset().X() << "," << get_v3_pos_offset().Y() << "," << get_v3_pos_offset().Z() << ")" << std::endl;
//    
//    std::cout << "x4_pos_offset = (" << get_x4_pos_offset().X() << "," << get_x4_pos_offset().Y() << "," << get_x4_pos_offset().Z() << ")" << std::endl;
//    std::cout << "u4_pos_offset = (" << get_u4_pos_offset().X() << "," << get_u4_pos_offset().Y() << "," << get_u4_pos_offset().Z() << ")" << std::endl;
//    std::cout << "v4_pos_offset = (" << get_v4_pos_offset().X() << "," << get_v4_pos_offset().Y() << "," << get_v4_pos_offset().Z() << ")" << std::endl;
//    
//    
//    std::cout << "The angle of x1 = " << stereo_x1 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of u1 = " << stereo_u1 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of v1 = " << stereo_v1 * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The angle of x2 = " << stereo_x2 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of u2 = " << stereo_u2 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of v2 = " << stereo_v2 * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The angle of x3 = " << stereo_x3 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of u3 = " << stereo_u3 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of v3 = " << stereo_v3 * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The angle of x4 = " << stereo_x4 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of u4 = " << stereo_u4 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of v4 = " << stereo_v4 * TMath::RadToDeg() << " deg." << std::endl;
//    
//    
//    std::cout << "The offset in angle of x1 = " << stereo_x1_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of u1 = " << stereo_u1_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of v1 = " << stereo_v1_offset * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The offset in angle of x2 = " << stereo_x2_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of u2 = " << stereo_u2_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of v2 = " << stereo_v2_offset * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The offset in angle of x3 = " << stereo_x3_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of u3 = " << stereo_u3_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of v3 = " << stereo_v3_offset * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The offset in angle of x4 = " << stereo_x4_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of u4 = " << stereo_u4_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of v4 = " << stereo_v4_offset * TMath::RadToDeg() << " deg." << std::endl;
}

// This function computes the tracker geometry
void TrackerGeometry::ComputeParameters()
{
    det_width = strips*s_pitch;
    // distance from origin to corner of det.
    diag = ( sqrt( det_width*det_width + s_length*s_length ) )*0.5;
    // angle between vector from origin (det. centre) to det. corner and x axis
    origin_angle = atan( (s_length*0.5) / (det_width*0.5) );
    
    //******************************************* Compute positions of geometry in z ***************************************************//
    
    // We know compute the geometry of the entire setup using the same method used in DetectorConstrucion.cc, this is used by the charge
    // sharing algorithm and DiffusionGenerator class in GEANT4 but it is mainly to allow the tracking routines that are external to GEANT4
    // to have access to the geometry of the system
    
    // The phantom is placed at isocentre all other geometry is relative to this a shift along z can be supplied with the phantom_zShift
    // parameter which has a default value of 0. This is used in case another part of the system has been put at the isocentre
    // e.g. in SuperSimulation in which case everything must be shifted forward or back. This shift is applied to DetectorConstruction
    // geometry by the DetectorMessenger class. xShift and yShift are also availiable in the DetectorMessenger but are not currently used
    phantom_pos.SetXYZ( 0.0, 0.0, (0.0 + phantom_zShift) );
    shield_pos.SetXYZ( 0.0, 0.0, (0.0 + shield_zShift) );//*******************************************

    // Protons are reconstructed at the front surface and edges of the phantom on the planes below
    // to represent tracking for the centre and edges of the phantom.
    phantom_centre_plane.SetXYZ( 0, 0, phantom_pos.Z() );
    phantom_middle_plane.SetXYZ( 0, 0, phantom_pos.Z() - 0.5*halfPhantomSizeZ );
    phantom_surface_plane.SetXYZ( 0, 0, phantom_pos.Z() - halfPhantomSizeZ );
    
    // Compensator is aligned in x/y with phantom
    compensator_pos.SetXYZ( phantom_pos.X(), phantom_pos.Y(),
                            phantom_pos.Z() -0.5*phantom_gap - inter_plane_dist*5 - inter_module_dist - halfPhantomSizeZ );
    
    shield_centre_plane.SetXYZ( 0, 0, shield_pos.Z() );//********************************
    shield_middle_plane.SetXYZ( 0, 0, shield_pos.Z() - 0.5*halfShieldSizeZ );//**********************
    shield_surface_plane.SetXYZ( 0, 0, shield_pos.Z() - halfShieldSizeZ );//****************************
                       
    // Detector z positions are computed here relative to the phantom (isocentre) and other detector planes.
    // Any offsets in x/y/stereo angle that have been specified are also applied here.
                       
    //**** Incoming tracker ****//
    
    x1_pos.SetZ( phantom_pos.Z() - 0.5*phantom_gap - inter_plane_dist*4 - inter_module_dist );
    x1_pos += x1_pos_offset;  // add offset to get misalignment in x/y
    u1_pos.SetZ( phantom_pos.Z() - 0.5*phantom_gap - inter_plane_dist*3 - inter_module_dist );
    u1_pos += u1_pos_offset;  // add offset to get misalignment in x/y
    v1_pos.SetZ( phantom_pos.Z() - 0.5*phantom_gap - inter_plane_dist*2 - inter_module_dist );
    v1_pos += v1_pos_offset;  // add offset to get misalignment in x/y
    
    x2_pos.SetZ( phantom_pos.Z() - 0.5*phantom_gap - inter_plane_dist*2 );
    x2_pos += x2_pos_offset;  // add offset to get misalignment in x/y
    u2_pos.SetZ( phantom_pos.Z() - 0.5*phantom_gap - inter_plane_dist );
    u2_pos += u2_pos_offset;  // add offset to get misalignment in x/y
    v2_pos.SetZ( phantom_pos.Z() - 0.5*phantom_gap );
    v2_pos += v2_pos_offset;  // add offset to get misalignment in x/y
    
    // Incoming truth planes fixed at 1mm after last tracker plane from phantom and 1mm before phantom surface
    TP1_pos.SetZ( phantom_pos.Z() - 0.5*phantom_gap + 1.0 );       // 1mm after v2 plane
    TP1_pos += v2_pos_offset;    // add offset to track misalignment in x/y for v2 plane (avoids TP missing events if offset is applied to detector)
    TP2_pos.SetZ( phantom_pos.Z() - halfPhantomSizeZ - 1.0 );      // 1mm from phantom surface
    TP2_pos += v2_pos_offset;    // add offset to track misalignment in x/y for v2 plane (avoids TP missing events if offset is applied to detector)
    
    //**** Outgoing tracker ****//
    
    // Outgoing truth planes fixed at 1mm from phantom surface and 1mm before next tracker plane after phantom
    TP3_pos.SetZ( phantom_pos.Z() + halfPhantomSizeZ + 1.0 );      // 1mm from phantom surface
    TP3_pos += x3_pos_offset;    // add offset to track misalignment in x/y for x3 plane (avoids TP missing events if offset is applied to detector)
    TP4_pos.SetZ( phantom_pos.Z() + 0.5*phantom_gap - 1.0 );       // 1mm before x3 plane
    TP4_pos += x3_pos_offset;    // add offset to track misalignment in x/y for x3 plane (avoids TP missing events if offset is applied to detector)
    
    x3_pos.SetZ( phantom_pos.Z() + 0.5*phantom_gap );
    x3_pos += x3_pos_offset;  // add offset to get misalignment in x/y
    u3_pos.SetZ( phantom_pos.Z() + 0.5*phantom_gap + inter_plane_dist );
    u3_pos += u3_pos_offset;  // add offset to get misalignment in x/y
    v3_pos.SetZ( phantom_pos.Z() + 0.5*phantom_gap + inter_plane_dist*2 );
    v3_pos += v3_pos_offset;  // add offset to get misalignment in x/y
    
    x4_pos.SetZ( phantom_pos.Z() + 0.5*phantom_gap + inter_plane_dist*2 + inter_module_dist );
    x4_pos += x4_pos_offset;  // add offset to get misalignment in x/y
    u4_pos.SetZ( phantom_pos.Z() + 0.5*phantom_gap + inter_plane_dist*3 + inter_module_dist );
    u4_pos += u4_pos_offset;  // add offset to get misalignment in x/y
    v4_pos.SetZ( phantom_pos.Z() + 0.5*phantom_gap + inter_plane_dist*4 + inter_module_dist );
    v4_pos += v4_pos_offset;  // add offset to get misalignment in x/y
    
    // Final truth plane 1mm before calorimeter position to check acceptance of tracker events
    TP5_pos.SetZ( phantom_pos.Z() + 0.5*phantom_gap + inter_plane_dist*4 + inter_module_dist + calorimeter_gap - 1.0 );
    TP5_pos += v4_pos_offset;    // add offset to track misalignment in x/y for v4 plane (avoids TP missing events if offset is applied to detector)
    
    // Range telescope (distance between last strip detector and first range telescope plane set by calorimeter_gap)
    // Calorimeter is aligned in x/y with phantom
    calorimeter_pos.SetXYZ( phantom_pos.X(), phantom_pos.Y(),
                            phantom_pos.Z() + 0.5*phantom_gap + inter_plane_dist*4 + inter_module_dist + calorimeter_gap + halfCalorimeterSizeZ );
    
    // Add offsets to angles for detectors
    stereo_x1 += stereo_x1_offset;
    stereo_u1 += stereo_u1_offset;
    stereo_v1 += stereo_v1_offset;
    
    stereo_x2 += stereo_x2_offset;
    stereo_u2 += stereo_u2_offset;
    stereo_v2 += stereo_v2_offset;
    
    stereo_x3 += stereo_x3_offset;
    stereo_u3 += stereo_u3_offset;
    stereo_v3 += stereo_v3_offset;
    
    stereo_x4 += stereo_x4_offset;
    stereo_u4 += stereo_u4_offset;
    stereo_v4 += stereo_v4_offset;
    
    //******************************************* x1 plane of the tracker ***************************************************//
    
    //   obtain position of the det. corners using polar co-ordinates.
                       
    // (x,y,z) positions of bottom left corner wrt phantom centre (world co-ordinates)
    A_x1.SetX( -1*diag*cos(origin_angle - stereo_x1) + x1_pos_offset.X() );
    A_x1.SetY( -1*diag*sin(origin_angle - stereo_x1) + x1_pos_offset.Y() );
    A_x1.SetZ( x1_pos.Z() );

    // (x,y,z) positions of top left corner wrt phantom centre (world co-ordinates)
    B_x1.SetX( -1*diag*cos(origin_angle + stereo_x1) + x1_pos_offset.X() );
    B_x1.SetY( diag*sin(origin_angle + stereo_x1) + x1_pos_offset.Y() );
    B_x1.SetZ( x1_pos.Z() );

    // (x,y,z) positions of top right corner wrt phantom centre (world co-ordinates)
    C_x1.SetX( diag*cos(origin_angle - stereo_x1) + x1_pos_offset.X() );
    C_x1.SetY( diag*sin(origin_angle - stereo_x1) + x1_pos_offset.Y() );
    C_x1.SetZ( x1_pos.Z() );

    // (x,y,z) positions of bottom right corner wrt phantom centre (world co-ordinates)
    D_x1.SetX( diag*cos(origin_angle + stereo_x1) + x1_pos_offset.X() );
    D_x1.SetY( -1*diag*sin(origin_angle + stereo_x1) + x1_pos_offset.Y() );
    D_x1.SetZ( x1_pos.Z() );
    
    //******************************************* u1 plane of the tracker ***************************************************//
    
    A_u1.SetX( -1*diag*cos(origin_angle - stereo_u1) + u1_pos_offset.X() );
    A_u1.SetY( -1*diag*sin(origin_angle - stereo_u1) + u1_pos_offset.Y() );
    A_u1.SetZ( u1_pos.Z() );
    
    B_u1.SetX( -1*diag*cos(origin_angle + stereo_u1) + u1_pos_offset.X() );
    B_u1.SetY( diag*sin(origin_angle + stereo_u1) + u1_pos_offset.Y() );
    B_u1.SetZ( u1_pos.Z() );
    
    C_u1.SetX( diag*cos(origin_angle - stereo_u1) + u1_pos_offset.X() );
    C_u1.SetY( diag*sin(origin_angle - stereo_u1) + u1_pos_offset.Y() );
    C_u1.SetZ( u1_pos.Z() );
    
    D_u1.SetX( diag*cos(origin_angle + stereo_u1) + u1_pos_offset.X() );
    D_u1.SetY( -1*diag*sin(origin_angle + stereo_u1) + u1_pos_offset.Y() );
    D_u1.SetZ( u1_pos.Z() );
    
    //******************************************* v1 plane of the tracker ***************************************************//
    
    A_v1.SetX( -1*diag*cos(origin_angle - stereo_v1) + v1_pos_offset.X() );
    A_v1.SetY( -1*diag*sin(origin_angle - stereo_v1) + v1_pos_offset.Y() );
    A_v1.SetZ( v1_pos.Z() );
    
    B_v1.SetX( -1*diag*cos(origin_angle + stereo_v1) + v1_pos_offset.X() );
    B_v1.SetY( diag*sin(origin_angle + stereo_v1) + v1_pos_offset.Y() );
    B_v1.SetZ( v1_pos.Z() );
    
    C_v1.SetX( diag*cos(origin_angle - stereo_v1) + v1_pos_offset.X() );
    C_v1.SetY( diag*sin(origin_angle - stereo_v1) + v1_pos_offset.Y() );
    C_v1.SetZ( v1_pos.Z() );
    
    D_v1.SetX( diag*cos(origin_angle + stereo_v1) + v1_pos_offset.X() );
    D_v1.SetY( -1*diag*sin(origin_angle + stereo_v1) + v1_pos_offset.Y() );
    D_v1.SetZ( v1_pos.Z() );
    
    //******************************************* x2 plane of the tracker ***************************************************//
    
    A_x2.SetX( -1*diag*cos(origin_angle - stereo_x2) + x2_pos_offset.X() );
    A_x2.SetY( -1*diag*sin(origin_angle - stereo_x2) + x2_pos_offset.Y() );
    A_x2.SetZ( x2_pos.Z() );
    
    B_x2.SetX( -1*diag*cos(origin_angle + stereo_x2) + x2_pos_offset.X() );
    B_x2.SetY( diag*sin(origin_angle + stereo_x2) + x2_pos_offset.Y() );
    B_x2.SetZ( x2_pos.Z() );
    
    C_x2.SetX( diag*cos(origin_angle - stereo_x2) + x2_pos_offset.X() );
    C_x2.SetY( diag*sin(origin_angle - stereo_x2) + x2_pos_offset.Y() );
    C_x2.SetZ( x2_pos.Z() );
    
    D_x2.SetX( diag*cos(origin_angle + stereo_x2) + x2_pos_offset.X() );
    D_x2.SetY( -1*diag*sin(origin_angle + stereo_x2) + x2_pos_offset.Y() );
    D_x2.SetZ( x2_pos.Z() );
    
    //******************************************* u2 plane of the tracker ***************************************************//
    
    A_u2.SetX( -1*diag*cos(origin_angle - stereo_u2) + u2_pos_offset.X() );
    A_u2.SetY( -1*diag*sin(origin_angle - stereo_u2) + u2_pos_offset.Y() );
    A_u2.SetZ( u2_pos.Z() );

    B_u2.SetX( -1*diag*cos(origin_angle + stereo_u2) + u2_pos_offset.X() );
    B_u2.SetY( diag*sin(origin_angle + stereo_u2) + u2_pos_offset.Y() );
    B_u2.SetZ( u2_pos.Z() );
    
    C_u2.SetX( diag*cos(origin_angle - stereo_u2) + u2_pos_offset.X() );
    C_u2.SetY( diag*sin(origin_angle - stereo_u2) + u2_pos_offset.Y() );
    C_u2.SetZ( u2_pos.Z() );
    
    D_u2.SetX( diag*cos(origin_angle + stereo_u2) + u2_pos_offset.X() );
    D_u2.SetY( -1*diag*sin(origin_angle + stereo_u2) + u2_pos_offset.Y() );
    D_u2.SetZ( u2_pos.Z() );
    
    //******************************************* v2 plane of the tracker ***************************************************//
    
    A_v2.SetX( -1*diag*cos(origin_angle - stereo_v2) + v2_pos_offset.X() );
    A_v2.SetY( -1*diag*sin(origin_angle - stereo_v2) + v2_pos_offset.Y() );
    A_v2.SetZ( v2_pos.Z() );
    
    B_v2.SetX( -1*diag*cos(origin_angle + stereo_v2) + v2_pos_offset.X() );
    B_v2.SetY( diag*sin(origin_angle + stereo_v2) + v2_pos_offset.Y() );
    B_v2.SetZ( v2_pos.Z() );
    
    C_v2.SetX( diag*cos(origin_angle - stereo_v2) + v2_pos_offset.X() );
    C_v2.SetY( diag*sin(origin_angle - stereo_v2) + v2_pos_offset.Y() );
    C_v2.SetZ( v2_pos.Z() );
    
    D_v2.SetX( diag*cos(origin_angle + stereo_v2) + v2_pos_offset.X() );
    D_v2.SetY( -1*diag*sin(origin_angle + stereo_v2) + v2_pos_offset.Y() );
    D_v2.SetZ( v2_pos.Z() );
    
    //******************************************* x3 plane of the tracker ***************************************************//
    
    A_x3.SetX( -1*diag*cos(origin_angle - stereo_x3) );
    A_x3.SetY( -1*diag*sin(origin_angle - stereo_x3) );
    A_x3.SetZ( x3_pos.Z() );
    
    B_x3.SetX( -1*diag*cos(origin_angle + stereo_x3) );
    B_x3.SetY( diag*sin(origin_angle + stereo_x3) );
    B_x3.SetZ( x3_pos.Z() );
    
    C_x3.SetX( diag*cos(origin_angle - stereo_x3) );
    C_x3.SetY( diag*sin(origin_angle - stereo_x3) );
    C_x3.SetZ( x3_pos.Z() );
    
    D_x3.SetX( diag*cos(origin_angle + stereo_x3) );
    D_x3.SetY( -1*diag*sin(origin_angle + stereo_x3) );
    D_x3.SetZ( x3_pos.Z() );
    
    //******************************************* u3 plane of the tracker ***************************************************//
    
    A_u3.SetX( -1*diag*cos(origin_angle - stereo_u3) );
    A_u3.SetY( -1*diag*sin(origin_angle - stereo_u3) );
    A_u3.SetZ( u3_pos.Z() );
    
    B_u3.SetX( -1*diag*cos(origin_angle + stereo_u3) );
    B_u3.SetY( diag*sin(origin_angle + stereo_u3) );
    B_u3.SetZ( u3_pos.Z() );
    
    C_u3.SetX( diag*cos(origin_angle - stereo_u3) );
    C_u3.SetY( diag*sin(origin_angle - stereo_u3) );
    C_u3.SetZ( u3_pos.Z() );
    
    D_u3.SetX( diag*cos(origin_angle + stereo_u3) );
    D_u3.SetY( -1*diag*sin(origin_angle + stereo_u3) );
    D_u3.SetZ( u3_pos.Z() );
    
    //******************************************* v3 plane of the tracker ***************************************************//
    
    A_v3.SetX( -1*diag*cos(origin_angle - stereo_v3) );
    A_v3.SetY( -1*diag*sin(origin_angle - stereo_v3) );
    A_v3.SetZ( v3_pos.Z() );
    
    B_v3.SetX( -1*diag*cos(origin_angle + stereo_v3) );
    B_v3.SetY( diag*sin(origin_angle + stereo_v3) );
    B_v3.SetZ( v3_pos.Z() );
    
    C_v3.SetX( diag*cos(origin_angle - stereo_v3) );
    C_v3.SetY( diag*sin(origin_angle - stereo_v3) );
    C_v3.SetZ( v3_pos.Z() );
    
    D_v3.SetX( diag*cos(origin_angle + stereo_v3) );
    D_v3.SetY( -1*diag*sin(origin_angle + stereo_v3) );
    D_v3.SetZ( v3_pos.Z() );
    
    //******************************************* x4 plane of the tracker ***************************************************//
    
    A_x4.SetX( -1*diag*cos(origin_angle - stereo_x4) );
    A_x4.SetY( -1*diag*sin(origin_angle - stereo_x4) );
    A_x4.SetZ( x4_pos.Z() );
    
    B_x4.SetX( -1*diag*cos(origin_angle + stereo_x4) );
    B_x4.SetY( diag*sin(origin_angle + stereo_x4) );
    B_x4.SetZ( x4_pos.Z() );
    
    C_x4.SetX( diag*cos(origin_angle - stereo_x4) );
    C_x4.SetY( diag*sin(origin_angle - stereo_x4) );
    C_x4.SetZ( x4_pos.Z() );
    
    D_x4.SetX( diag*cos(origin_angle + stereo_x4) );
    D_x4.SetY( -1*diag*sin(origin_angle + stereo_x4) );
    D_x4.SetZ( x4_pos.Z() );
    
    //******************************************* u4 plane of the tracker ***************************************************//
    
    A_u4.SetX( -1*diag*cos(origin_angle - stereo_u4) );
    A_u4.SetY( -1*diag*sin(origin_angle - stereo_u4) );
    A_u4.SetZ( u4_pos.Z() );
    
    B_u4.SetX( -1*diag*cos(origin_angle + stereo_u4) );
    B_u4.SetY( diag*sin(origin_angle + stereo_u4) );
    B_u4.SetZ( u4_pos.Z() );
    
    C_u4.SetX( diag*cos(origin_angle - stereo_u4) );
    C_u4.SetY( diag*sin(origin_angle - stereo_u4) );
    C_u4.SetZ( u4_pos.Z() );
    
    D_u4.SetX( diag*cos(origin_angle + stereo_u4) );
    D_u4.SetY( -1*diag*sin(origin_angle + stereo_u4) );
    D_u4.SetZ( u4_pos.Z() );
    
    //******************************************* v4 plane of the tracker ***************************************************//
    
    A_v4.SetX( -1*diag*cos(origin_angle - stereo_v4) );
    A_v4.SetY( -1*diag*sin(origin_angle - stereo_v4) );
    A_v4.SetZ( v4_pos.Z() );
    
    B_v4.SetX( -1*diag*cos(origin_angle + stereo_v4) );
    B_v4.SetY( diag*sin(origin_angle + stereo_v4) );
    B_v4.SetZ( v4_pos.Z() );
    
    C_v4.SetX( diag*cos(origin_angle - stereo_v4) );
    C_v4.SetY( diag*sin(origin_angle - stereo_v4) );
    C_v4.SetZ( v4_pos.Z() );
    
    D_v4.SetX( diag*cos(origin_angle + stereo_v4) );
    D_v4.SetY( -1*diag*sin(origin_angle + stereo_v4) );
    D_v4.SetZ( v4_pos.Z() );
    
    // call PrintGeometry() for debug
    //PrintGeometry();
    
    
//    std::cout << "\nComputeParameters() END" << std::endl;
//    std::cout << "x1_pos = (" << get_x1_pos().X() << "," << get_x1_pos().Y() << "," << get_x1_pos().Z() << ")" << std::endl;
//    std::cout << "u1_pos = (" << get_u1_pos().X() << "," << get_u1_pos().Y() << "," << get_u1_pos().Z() << ")" << std::endl;
//    std::cout << "v1_pos = (" << get_v1_pos().X() << "," << get_v1_pos().Y() << "," << get_v1_pos().Z() << ")" << std::endl;
//    
//    std::cout << "x2_pos = (" << get_x2_pos().X() << "," << get_x2_pos().Y() << "," << get_x2_pos().Z() << ")" << std::endl;
//    std::cout << "u2_pos = (" << get_u2_pos().X() << "," << get_u2_pos().Y() << "," << get_u2_pos().Z() << ")" << std::endl;
//    std::cout << "v2_pos = (" << get_v2_pos().X() << "," << get_v2_pos().Y() << "," << get_v2_pos().Z() << ")" << std::endl;
//    
//    std::cout << "x3_pos = (" << get_x3_pos().X() << "," << get_x3_pos().Y() << "," << get_x3_pos().Z() << ")" << std::endl;
//    std::cout << "u3_pos = (" << get_u3_pos().X() << "," << get_u3_pos().Y() << "," << get_u3_pos().Z() << ")" << std::endl;
//    std::cout << "v3_pos = (" << get_v3_pos().X() << "," << get_v3_pos().Y() << "," << get_v3_pos().Z() << ")" << std::endl;
//    
//    std::cout << "x4_pos = (" << get_x4_pos().X() << "," << get_x4_pos().Y() << "," << get_x4_pos().Z() << ")" << std::endl;
//    std::cout << "u4_pos = (" << get_u4_pos().X() << "," << get_u4_pos().Y() << "," << get_u4_pos().Z() << ")" << std::endl;
//    std::cout << "v4_pos = (" << get_v4_pos().X() << "," << get_v4_pos().Y() << "," << get_v4_pos().Z() << ")" << std::endl;
//    
//    std::cout << "x1_pos_offset = (" << get_x1_pos_offset().X() << "," << get_x1_pos_offset().Y() << "," << get_x1_pos_offset().Z() << ")" << std::endl;
//    std::cout << "u1_pos_offset = (" << get_u1_pos_offset().X() << "," << get_u1_pos_offset().Y() << "," << get_u1_pos_offset().Z() << ")" << std::endl;
//    std::cout << "v1_pos_offset = (" << get_v1_pos_offset().X() << "," << get_v1_pos_offset().Y() << "," << get_v1_pos_offset().Z() << ")" << std::endl;
//    
//    std::cout << "x2_pos_offset = (" << get_x2_pos_offset().X() << "," << get_x2_pos_offset().Y() << "," << get_x2_pos_offset().Z() << ")" << std::endl;
//    std::cout << "u2_pos_offset = (" << get_u2_pos_offset().X() << "," << get_u2_pos_offset().Y() << "," << get_u2_pos_offset().Z() << ")" << std::endl;
//    std::cout << "v2_pos_offset = (" << get_v2_pos_offset().X() << "," << get_v2_pos_offset().Y() << "," << get_v2_pos_offset().Z() << ")" << std::endl;
//    
//    std::cout << "x3_pos_offset = (" << get_x3_pos_offset().X() << "," << get_x3_pos_offset().Y() << "," << get_x3_pos_offset().Z() << ")" << std::endl;
//    std::cout << "u3_pos_offset = (" << get_u3_pos_offset().X() << "," << get_u3_pos_offset().Y() << "," << get_u3_pos_offset().Z() << ")" << std::endl;
//    std::cout << "v3_pos_offset = (" << get_v3_pos_offset().X() << "," << get_v3_pos_offset().Y() << "," << get_v3_pos_offset().Z() << ")" << std::endl;
//    
//    std::cout << "x4_pos_offset = (" << get_x4_pos_offset().X() << "," << get_x4_pos_offset().Y() << "," << get_x4_pos_offset().Z() << ")" << std::endl;
//    std::cout << "u4_pos_offset = (" << get_u4_pos_offset().X() << "," << get_u4_pos_offset().Y() << "," << get_u4_pos_offset().Z() << ")" << std::endl;
//    std::cout << "v4_pos_offset = (" << get_v4_pos_offset().X() << "," << get_v4_pos_offset().Y() << "," << get_v4_pos_offset().Z() << ")" << std::endl;
//    
//    
//    std::cout << "The angle of x1 = " << stereo_x1 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of u1 = " << stereo_u1 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of v1 = " << stereo_v1 * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The angle of x2 = " << stereo_x2 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of u2 = " << stereo_u2 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of v2 = " << stereo_v2 * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The angle of x3 = " << stereo_x3 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of u3 = " << stereo_u3 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of v3 = " << stereo_v3 * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The angle of x4 = " << stereo_x4 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of u4 = " << stereo_u4 * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The angle of v4 = " << stereo_v4 * TMath::RadToDeg() << " deg." << std::endl;
//    
//    
//    std::cout << "The offset in angle of x1 = " << stereo_x1_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of u1 = " << stereo_u1_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of v1 = " << stereo_v1_offset * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The offset in angle of x2 = " << stereo_x2_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of u2 = " << stereo_u2_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of v2 = " << stereo_v2_offset * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The offset in angle of x3 = " << stereo_x3_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of u3 = " << stereo_u3_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of v3 = " << stereo_v3_offset * TMath::RadToDeg() << " deg." << std::endl;
//    
//    std::cout << "The offset in angle of x4 = " << stereo_x4_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of u4 = " << stereo_u4_offset * TMath::RadToDeg() << " deg." << std::endl;
//    std::cout << "The offset in angle of v4 = " << stereo_v4_offset * TMath::RadToDeg() << " deg." << std::endl;
    
}


void TrackerGeometry::PrintGeometry()
{
    std::cout << "\nTrackerGeometry object created with the following parameters.." << std::endl;
    std::cout << "Detectors have: " << strips << " strips with a pitch of: " << s_pitch << " mm" << std::endl;
    std::cout << "The length of the strips (detector) is: " << s_length << " mm, and the width of the detector is: " << det_width << " mm" << std::endl;
    std::cout << "The detector has a thickness of: " << thickness << " mm" << std::endl;
    
    std::cout << "\nThe silicon tracker planes and phantom are positioned as follows..\n" << std::endl;
    
    std::cout << "The x1 det plane is at: (" << x1_pos.X() << "," << x1_pos.Y() << "," << x1_pos.Z() << ") mm" << std::endl;
    std::cout << "The u1 det plane is at: (" << u1_pos.X() << "," << u1_pos.Y() << "," << u1_pos.Z() << ") mm" << std::endl;
    std::cout << "The v1 det plane is at: (" << v1_pos.X() << "," << v1_pos.Y() << "," << v1_pos.Z() << ") mm" << std::endl;
    
    std::cout << "The x2 det plane is at: (" << x2_pos.X() << "," << x2_pos.Y() << "," << x2_pos.Z() << ") mm" << std::endl;
    std::cout << "The u2 det plane is at: (" << u2_pos.X() << "," << u2_pos.Y() << "," << u2_pos.Z() << ") mm" << std::endl;
    std::cout << "The v2 det plane is at: (" << v2_pos.X() << "," << v2_pos.Y() << "," << v2_pos.Z() << ") mm" << std::endl;
    
    std::cout << "The x3 det plane is at: (" << x3_pos.X() << "," << x3_pos.Y() << "," << x3_pos.Z() << ") mm" << std::endl;
    std::cout << "The u3 det plane is at: (" << u3_pos.X() << "," << u3_pos.Y() << "," << u3_pos.Z() << ") mm" << std::endl;
    std::cout << "The v3 det plane is at: (" << v3_pos.X() << "," << v3_pos.Y() << "," << v3_pos.Z() << ") mm" << std::endl;
    
    std::cout << "The x4 det plane is at: (" << x4_pos.X() << "," << x4_pos.Y() << "," << x4_pos.Z() << ") mm" << std::endl;
    std::cout << "The u4 det plane is at: (" << u4_pos.X() << "," << u4_pos.Y() << "," << u4_pos.Z() << ") mm" << std::endl;
    std::cout << "The v4 det plane is at: (" << v4_pos.X() << "," << v4_pos.Y() << "," << v4_pos.Z() << ") mm" << std::endl;
    
    std::cout << "\nThe truth planes track the positions of the silicon planes..\n" << std::endl;
    
    std::cout << "Truth plane 1 is at: (" << TP1_pos.X() << "," << TP1_pos.Y() << "," << TP1_pos.Z() << ") mm" << std::endl;
    std::cout << "Truth plane 2 is at: (" << TP2_pos.X() << "," << TP2_pos.Y() << "," << TP2_pos.Z() << ") mm" << std::endl;
    std::cout << "Truth plane 3 is at: (" << TP3_pos.X() << "," << TP3_pos.Y() << "," << TP3_pos.Z() << ") mm" << std::endl;
    std::cout << "Truth plane 4 is at: (" << TP4_pos.X() << "," << TP4_pos.Y() << "," << TP4_pos.Z() << ") mm" << std::endl;
    std::cout << "Truth plane 5 is at: (" << TP5_pos.X() << "," << TP5_pos.Y() << "," << TP5_pos.Z() << ") mm" << std::endl;
    
    std::cout << "\nThe comensator, phantom and reconstruction planes and calorimeter are positioned as follows ..\n" << std::endl;
    
    std::cout << "The compensator is at (" << compensator_pos.X() << "," << compensator_pos.Y() << "," << compensator_pos.Z() << ") mm" << std::endl;
    
    std::cout << "The phantom surface plane is at: ";
    std::cout << "(" << phantom_surface_plane.X() << "," << phantom_surface_plane.Y() << "," << phantom_surface_plane.Z() << ") mm" << std::endl;
    std::cout << "The phantom middle plane is at: ";
    std::cout << "(" << phantom_middle_plane.X() << "," << phantom_middle_plane.Y() << "," << phantom_middle_plane.Z() << ") mm" << std::endl;
    std::cout << "The phantom centre plane is at: ";
    std::cout << "(" << phantom_centre_plane.X() << "," << phantom_centre_plane.Y() << "," << phantom_centre_plane.Z() << ") mm\n" << std::endl;
    
    //**************************************SHIELD Adding**************************
    std::cout << "The shield surface plane is at: ";
    std::cout << "(" << shield_surface_plane.X() << "," << shield_surface_plane.Y() << "," << shield_surface_plane.Z() << ") mm" << std::endl;
    std::cout << "The shield middle plane is at: ";
    std::cout << "(" << shield_middle_plane.X() << "," << shield_middle_plane.Y() << "," << shield_middle_plane.Z() << ") mm" << std::endl;
    std::cout << "The shield centre plane is at: ";
    std::cout << "(" << shield_centre_plane.X() << "," << shield_centre_plane.Y() << "," << shield_centre_plane.Z() << ") mm\n" << std::endl;
    //*********************************************END*******************************


    std::cout << "The calorimeter is at (" << calorimeter_pos.X() << "," << calorimeter_pos.Y() << "," << calorimeter_pos.Z() << ") mm\n" << std::endl;
    
    std::cout << "The centre of the phantom is placed at the origin (0,0,0) mm" << std::endl;
    std::cout << "Det. centres have default positions at (0,0,z) mm" << std::endl;
    std::cout << "Any lateral offsets applied will change this to (x,y,z) mm" << std::endl;
    std::cout << "Default rotations for planes is (x,u,v) = (0,60,-60) deg" << std::endl;
    std::cout << "Any angular offsets applied will be added to this" << std::endl;
    std::cout << "Rotation for u-planes in clockwise direction" << std::endl;
    std::cout << "Rotation for v-planes in anti-clockwise direction" << std::endl;
    std::cout << "Detector corners labelled as follows..\n" << std::endl;
    
    std::cout << "B ******************* C" << std::endl;
    std::cout << "  *                 *  " << std::endl;
    std::cout << "  *                 *  " << std::endl;
    std::cout << "  *                 *  " << std::endl;
    std::cout << "  *        *        *  " << std::endl;
    std::cout << "  *                 *  " << std::endl;
    std::cout << "  *                 *  " << std::endl;
    std::cout << "  *                 *  " << std::endl;
    std::cout << "A ******************* D" << std::endl;
    
    //*************************************************** x1 plane of the tracker *************************************************************//
    
    // Position of det. corners wrt origin (world co-ordinates)
    std::cout << "\nFor the x1 plane.." << std::endl;
    std::cout << "The position of A_x1 = (" << A_x1.X() << "," << A_x1.Y() << "," << A_x1.Z() << ") mm" << std::endl;
    std::cout << "The position of B_x1 = (" << B_x1.X() << "," << B_x1.Y() << "," << B_x1.Z() << ") mm" << std::endl;
    std::cout << "The position of C_x1 = (" << C_x1.X() << "," << C_x1.Y() << "," << C_x1.Z() << ") mm" << std::endl;
    std::cout << "The position of D_x1 = (" << D_x1.X() << "," << D_x1.Y() << "," << D_x1.Z() << ") mm" << std::endl;
    std::cout << "The angle of rotation = " << stereo_x1 * TMath::RadToDeg() << " deg.\n" << std::endl;
    
    //*************************************************** u1 plane of the tracker *************************************************************//
    
    // Position of det. corners wrt origin (world co-ordinates)
    std::cout << "\nFor the u1 plane.." << std::endl;
    std::cout << "The position of A_u1 = (" << A_u1.X() << "," << A_u1.Y() << "," << A_u1.Z() << ") mm" << std::endl;
    std::cout << "The position of B_u1 = (" << B_u1.X() << "," << B_u1.Y() << "," << B_u1.Z() << ") mm" << std::endl;
    std::cout << "The position of C_u1 = (" << C_u1.X() << "," << C_u1.Y() << "," << C_u1.Z() << ") mm" << std::endl;
    std::cout << "The position of D_u1 = (" << D_u1.X() << "," << D_u1.Y() << "," << D_u1.Z() << ") mm" << std::endl;
    std::cout << "The angle of rotation = " << stereo_u1 * TMath::RadToDeg() << " deg.\n" << std::endl;
    
    //*************************************************** v1 plane of the tracker *************************************************************//
    
    // Position of det. corners wrt origin (world co-ordinates)
    std::cout << "\nFor the v1 plane.." << std::endl;
    std::cout << "The position of A_v1 = (" << A_v1.X() << "," << A_v1.Y() << "," << A_v1.Z() << ") mm" << std::endl;
    std::cout << "The position of B_v1 = (" << B_v1.X() << "," << B_v1.Y() << "," << B_v1.Z() << ") mm" << std::endl;
    std::cout << "The position of C_v1 = (" << C_v1.X() << "," << C_v1.Y() << "," << C_v1.Z() << ") mm" << std::endl;
    std::cout << "The position of D_v1 = (" << D_v1.X() << "," << D_v1.Y() << "," << D_v1.Z() << ") mm" << std::endl;
    std::cout << "The angle of rotation = " << stereo_v1 * TMath::RadToDeg() << " deg.\n" << std::endl;
    
    
    //*************************************************** x2 plane of the tracker *************************************************************//
    
    // Position of det. corners wrt origin (world co-ordinates)
    std::cout << "\nFor the x2 plane.." << std::endl;
    std::cout << "The position of A_x2 = (" << A_x2.X() << "," << A_x2.Y() << "," << A_x2.Z() << ") mm" << std::endl;
    std::cout << "The position of B_x2 = (" << B_x2.X() << "," << B_x2.Y() << "," << B_x2.Z() << ") mm" << std::endl;
    std::cout << "The position of C_x2 = (" << C_x2.X() << "," << C_x2.Y() << "," << C_x2.Z() << ") mm" << std::endl;
    std::cout << "The position of D_x2 = (" << D_x2.X() << "," << D_x2.Y() << "," << D_x2.Z() << ") mm" << std::endl;
    std::cout << "The angle of rotation = " << stereo_x2 * TMath::RadToDeg() << " deg.\n" << std::endl;
    
    //*************************************************** u2 plane of the tracker *************************************************************//
    
    // Position of det. corners wrt origin (world co-ordinates)
    std::cout << "\nFor the u2 plane.." << std::endl;
    std::cout << "The position of A_u2 = (" << A_u2.X() << "," << A_u2.Y() << "," << A_u2.Z() << ") mm" << std::endl;
    std::cout << "The position of B_u2 = (" << B_u2.X() << "," << B_u2.Y() << "," << B_u2.Z() << ") mm" << std::endl;
    std::cout << "The position of C_u2 = (" << C_u2.X() << "," << C_u2.Y() << "," << C_u2.Z() << ") mm" << std::endl;
    std::cout << "The position of D_u2 = (" << D_u2.X() << "," << D_u2.Y() << "," << D_u2.Z() << ") mm" << std::endl;
    std::cout << "The angle of rotation = " << stereo_u2 * TMath::RadToDeg() << " deg.\n" << std::endl;
    
    //*************************************************** v2 plane of the tracker *************************************************************//
    
    // Position of det. corners wrt origin (world co-ordinates)
    std::cout << "\nFor the v2 plane.." << std::endl;
    std::cout << "The position of A_v2 = (" << A_v2.X() << "," << A_v2.Y() << "," << A_v2.Z() << ") mm" << std::endl;
    std::cout << "The position of B_v2 = (" << B_v2.X() << "," << B_v2.Y() << "," << B_v2.Z() << ") mm" << std::endl;
    std::cout << "The position of C_v2 = (" << C_v2.X() << "," << C_v2.Y() << "," << C_v2.Z() << ") mm" << std::endl;
    std::cout << "The position of D_v2 = (" << D_v2.X() << "," << D_v2.Y() << "," << D_v2.Z() << ") mm" << std::endl;
    std::cout << "The angle of rotation = " << stereo_v2 * TMath::RadToDeg() << " deg.\n" << std::endl;
    
    
    //*************************************************** x3 plane of the tracker *************************************************************//
    
    // Position of det. corners wrt origin (world co-ordinates)
    std::cout << "\nFor the x3 plane.." << std::endl;
    std::cout << "The position of A_x3 = (" << A_x3.X() << "," << A_x3.Y() << "," << A_x3.Z() << ") mm" << std::endl;
    std::cout << "The position of B_x3 = (" << B_x3.X() << "," << B_x3.Y() << "," << B_x3.Z() << ") mm" << std::endl;
    std::cout << "The position of C_x3 = (" << C_x3.X() << "," << C_x3.Y() << "," << C_x3.Z() << ") mm" << std::endl;
    std::cout << "The position of D_x3 = (" << D_x3.X() << "," << D_x3.Y() << "," << D_x3.Z() << ") mm" << std::endl;
    std::cout << "The angle of rotation = " << stereo_x3 * TMath::RadToDeg() << " deg.\n" << std::endl;
    
    //*************************************************** u3 plane of the tracker *************************************************************//
    
    // Position of det. corners wrt origin (world co-ordinates)
    std::cout << "\nFor the u3 plane.." << std::endl;
    std::cout << "The position of A_u3 = (" << A_u3.X() << "," << A_u3.Y() << "," << A_u3.Z() << ") mm" << std::endl;
    std::cout << "The position of B_u3 = (" << B_u3.X() << "," << B_u3.Y() << "," << B_u3.Z() << ") mm" << std::endl;
    std::cout << "The position of C_u3 = (" << C_u3.X() << "," << C_u3.Y() << "," << C_u3.Z() << ") mm" << std::endl;
    std::cout << "The position of D_u3 = (" << D_u3.X() << "," << D_u3.Y() << "," << D_u3.Z() << ") mm" << std::endl;
    std::cout << "The angle of rotation = " << stereo_u3 * TMath::RadToDeg() << " deg.\n" << std::endl;
    
    //*************************************************** v3 plane of the tracker *************************************************************//
    
    // Position of det. corners wrt origin (world co-ordinates)
    std::cout << "\nFor the v3 plane.." << std::endl;
    std::cout << "The position of A_v3 = (" << A_v3.X() << "," << A_v3.Y() << "," << A_v3.Z() << ") mm" << std::endl;
    std::cout << "The position of B_v3 = (" << B_v3.X() << "," << B_v3.Y() << "," << B_v3.Z() << ") mm" << std::endl;
    std::cout << "The position of C_v3 = (" << C_v3.X() << "," << C_v3.Y() << "," << C_v3.Z() << ") mm" << std::endl;
    std::cout << "The position of D_v3 = (" << D_v3.X() << "," << D_v3.Y() << "," << D_v3.Z() << ") mm" << std::endl;
    std::cout << "The angle of rotation = " << stereo_v3 * TMath::RadToDeg() << " deg.\n" << std::endl;
    
    
    //*************************************************** x4 plane of the tracker *************************************************************//
    
    // Position of det. corners wrt origin (world co-ordinates)
    std::cout << "\nFor the x4 plane.." << std::endl;
    std::cout << "The position of A_x4 = (" << A_x4.X() << "," << A_x4.Y() << "," << A_x4.Z() << ") mm" << std::endl;
    std::cout << "The position of B_x4 = (" << B_x4.X() << "," << B_x4.Y() << "," << B_x4.Z() << ") mm" << std::endl;
    std::cout << "The position of C_x4 = (" << C_x4.X() << "," << C_x4.Y() << "," << C_x4.Z() << ") mm" << std::endl;
    std::cout << "The position of D_x4 = (" << D_x4.X() << "," << D_x4.Y() << "," << D_x4.Z() << ") mm" << std::endl;
    std::cout << "The angle of rotation = " << stereo_x4 * TMath::RadToDeg() << " deg.\n" << std::endl;
    
    //*************************************************** u4 plane of the tracker *************************************************************//
    
    // Position of det. corners wrt origin (world co-ordinates)
    std::cout << "\nFor the u4 plane.." << std::endl;
    std::cout << "The position of A_u4 = (" << A_u4.X() << "," << A_u4.Y() << "," << A_u4.Z() << ") mm" << std::endl;
    std::cout << "The position of B_u4 = (" << B_u4.X() << "," << B_u4.Y() << "," << B_u4.Z() << ") mm" << std::endl;
    std::cout << "The position of C_u4 = (" << C_u4.X() << "," << C_u4.Y() << "," << C_u4.Z() << ") mm" << std::endl;
    std::cout << "The position of D_u4 = (" << D_u4.X() << "," << D_u4.Y() << "," << D_u4.Z() << ") mm" << std::endl;
    std::cout << "The angle of rotation = " << stereo_u4 * TMath::RadToDeg() << " deg.\n" << std::endl;
    
    //*************************************************** v4 plane of the tracker *************************************************************//
    
    // Position of det. corners wrt origin (world co-ordinates)
    std::cout << "\nFor the v4 plane.." << std::endl;
    std::cout << "The position of A_v4 = (" << A_v4.X() << "," << A_v4.Y() << "," << A_v4.Z() << ") mm" << std::endl;
    std::cout << "The position of B_v4 = (" << B_v4.X() << "," << B_v4.Y() << "," << B_v4.Z() << ") mm" << std::endl;
    std::cout << "The position of C_v4 = (" << C_v4.X() << "," << C_v4.Y() << "," << C_v4.Z() << ") mm" << std::endl;
    std::cout << "The position of D_v4 = (" << D_v4.X() << "," << D_v4.Y() << "," << D_v4.Z() << ") mm" << std::endl;
    std::cout << "The angle of rotation = " << stereo_v4 * TMath::RadToDeg() << " deg.\n" << std::endl;
    
    //*****************************************************************************************************************************************//
    
}

TVector3 TrackerGeometry::get_strip_origin(TVector3 det_origin, int strip_no, double stereo_angle)
{
    TVector3 strip_origin;
    
    // obtain position of first strip (det. corner) using
    // polar co-ordinates wrt the det. centre.
    float r_origin_x = det_origin.X();
    float r_origin_y = det_origin.Y();
    
    // obtain position of the chosen strip wrt to the first strip (det. corner)
    // using polar co-ordinates.
    float r_x_strips = (strip_no*s_pitch - 0.5*s_pitch) * cos(stereo_angle);
    float r_y_strips = (-1*strip_no*s_pitch + 0.5*s_pitch) * sin(stereo_angle);
    
    // using the position of the first strip convert from det. co-ordinates to world
    // co-ordinates
    strip_origin.SetX(r_x_strips + r_origin_x);
    strip_origin.SetY(r_y_strips + r_origin_y);
    strip_origin.SetZ(det_origin.Z());
    
    return strip_origin;
}

// return co-ordinates at the top of strip using co-ordinates of bottom
TVector3 TrackerGeometry::build_strip(TVector3 btm, double stereo_angle)
{
    TVector3 top;
    top.SetXYZ(btm.X() + s_length*sin(stereo_angle), btm.Y() + s_length*cos(stereo_angle), btm.Z());
    return top;
}

// used for drawing geometry and hits, fills three pre-defined Tline objects for a strip that represent the strip edges and centre
void TrackerGeometry::draw_strip(TLine * x_low, TLine * x_mid, TLine * x_high, TVector3 strip_centre_origin, double stereo_angle)
{
    TVector3 top, btm;
    
    btm.SetXYZ( strip_centre_origin.x(), strip_centre_origin.y(), strip_centre_origin.z() );
    top.SetXYZ(btm.X() + s_length*sin(stereo_angle), btm.Y() + s_length*cos(stereo_angle), btm.Z());
    x_mid->SetX1(btm.X());
    x_mid->SetY1(btm.Y());
    x_mid->SetX2(top.X());
    x_mid->SetY2(top.Y());
    
    btm.SetXYZ( strip_centre_origin.x() - 0.5*s_pitch*cos(stereo_angle), strip_centre_origin.y() + 0.5*s_pitch*sin(stereo_angle), strip_centre_origin.z() );
    top.SetXYZ(btm.X() + s_length*sin(stereo_angle), btm.Y() + s_length*cos(stereo_angle), btm.Z());
    x_low->SetX1(btm.X());
    x_low->SetY1(btm.Y());
    x_low->SetX2(top.X());
    x_low->SetY2(top.Y());
    
    btm.SetXYZ( strip_centre_origin.x() + 0.5*s_pitch*cos(stereo_angle), strip_centre_origin.y() - 0.5*s_pitch*sin(stereo_angle), strip_centre_origin.z() );
    top.SetXYZ(btm.X() + s_length*sin(stereo_angle), btm.Y() + s_length*cos(stereo_angle), btm.Z());
    x_high->SetX1(btm.X());
    x_high->SetY1(btm.Y());
    x_high->SetX2(top.X());
    x_high->SetY2(top.Y());
}

// used for defining pixel boundaries of hits, fills six pre-defined TVector3 objects for a strip that represent the top and bottom of the strip edge
// and centre vectors.
void TrackerGeometry::draw_strip(TVector3 * x_low_b, TVector3 * x_mid_b, TVector3 * x_high_b, TVector3 * x_low_t, TVector3 * x_mid_t, TVector3 * x_high_t,
                                 TVector3 strip_centre_origin, double stereo_angle)
{
    TVector3 top, btm;
    
    btm.SetXYZ( strip_centre_origin.x(), strip_centre_origin.y(), strip_centre_origin.z() );
    top.SetXYZ( btm.X() + s_length*sin(stereo_angle), btm.Y() + s_length*cos(stereo_angle), btm.Z() );
    x_mid_b->SetXYZ( btm.X(), btm.Y(), btm.Z());
    //cout << "x_mid_b: (" << x_mid_b->X() << "," << x_mid_b->Y() << "," << x_mid_b->Z() << ")" << endl;
    x_mid_t->SetXYZ( top.X(), top.Y(), top.Z());
    //cout << "x_mid_t: (" << x_mid_t->X() << "," << x_mid_t->Y() << "," << x_mid_t->Z() << ")" << endl;
    
    btm.SetXYZ( strip_centre_origin.x() - 0.5*s_pitch*cos(stereo_angle), strip_centre_origin.y() + 0.5*s_pitch*sin(stereo_angle), strip_centre_origin.z() );
    top.SetXYZ(btm.X() + s_length*sin(stereo_angle), btm.Y() + s_length*cos(stereo_angle), btm.Z());
    x_low_b->SetXYZ( btm.X(), btm.Y(), btm.Z());
    //cout << "x_low_b: (" << x_low_b->X() << "," << x_low_b->Y() << "," << x_low_b->Z() << ")" << endl;
    x_low_t->SetXYZ( top.X(), top.Y(), top.Z());
    //cout << "x_low_t: (" << x_low_t->X() << "," << x_low_t->Y() << "," << x_low_t->Z() << ")" << endl;
    
    
    btm.SetXYZ( strip_centre_origin.x() + 0.5*s_pitch*cos(stereo_angle), strip_centre_origin.y() - 0.5*s_pitch*sin(stereo_angle), strip_centre_origin.z() );
    top.SetXYZ(btm.X() + s_length*sin(stereo_angle), btm.Y() + s_length*cos(stereo_angle), btm.Z());
    x_high_b->SetXYZ( btm.X(), btm.Y(), btm.Z());
    //cout << "x_high_b: (" << x_high_b->X() << "," << x_high_b->Y() << "," << x_high_b->Z() << ")" << endl;
    x_high_t->SetXYZ( top.X(), top.Y(), top.Z());
    //cout << "x_high_t: (" << x_high_t->X() << "," << x_high_t->Y() << "," << x_high_t->Z() << ")" << endl;
}

// Transform 3D hit location (x,y,z) into 1D det. co-ordinate (u or v)
double TrackerGeometry::world_2_det_transform(TVector3 det_origin, TVector3 det_opp_origin, TVector3 hit)
{
    // u or v axis constructed between two rotated detector corners
    TVector3 det_axis = det_opp_origin - det_origin;
    // Transform 3D vectors to 2D ie must make sure that hit vector
    // has same z component as detector corners so that it's magnitude
    // and angle wrt to det_axis vector corresponds to x/y components only.
    hit.SetZ(det_origin.Z());
    // vector from origin to hit location.
    TVector3 dist_to_hit = hit - det_origin;
    // Change from 2D position to 1D ie distance of hit along det axis
    // 1D used since detector is only segmented in one dimension.
    double r = dist_to_hit.Mag()*cos(det_axis.Angle(dist_to_hit));
    return r;
}

// Checks if a hit is inside a given strip
bool TrackerGeometry::IsInsideStrip(TVector3 h, TVector3 origin, double stereo_angle)
{
    
    // see here for how to do it analytically:
    // http://math.stackexchange.com/questions/190111/how-to-check-if-a-point-is-inside-a-rectangle
    // probably not much faster than using the IsInside() function that belongs to the TGraph object in root
    // which probably uses a similar algorithm
    
    TVector3 top, btm, A,B,C,D;
    
    btm.SetXYZ(origin.X() - 0.5*s_pitch*cos(stereo_angle), origin.Y() + 0.5*s_pitch*sin(stereo_angle), origin.Z());
    top.SetXYZ(btm.X() + s_length*sin(stereo_angle), btm.Y() + s_length*cos(stereo_angle), btm.Z());
    A.SetXYZ(btm.X(),btm.Y(),btm.Z());
    B.SetXYZ(top.X(),top.Y(),top.Z());
    
    btm.SetXYZ(origin.X() + 0.5*s_pitch*cos(stereo_angle), origin.Y() - 0.5*s_pitch*sin(stereo_angle), origin.Z());
    top.SetXYZ(btm.X() + s_length*sin(stereo_angle), btm.Y() + s_length*cos(stereo_angle), btm.Z());
    D.SetXYZ(btm.X(),btm.Y(),btm.Z());
    C.SetXYZ(top.X(),top.Y(),top.Z());
    
    bool IsInside = true;
    float _X[5];
    float _Y[5];
    TGraph * g;
    
    _X[0] = A.X();
    _Y[0] = A.Y();
    _X[1] = B.X();
    _Y[1] = B.Y();
    _X[2] = C.X();
    _Y[2] = C.Y();
    _X[3] = D.X();
    _Y[3] = D.Y();
    _X[4] = A.X();
    _Y[4] = A.Y();
    
    g = new TGraph(5,_X,_Y);
    if( !g->IsInside( h.X(),h.Y() ) ) { IsInside = false; }
    delete g;
    
	return IsInside;
}

// Returns a vector which points along the centre of a cluster
TVector3 TrackerGeometry::get_average_strip_pos(TVector3 det_origin, std::vector<int> hit_strips, double stereo_angle)
{
    TVector3 average(0,0,0);
    for(size_t i=0; i<hit_strips.size(); i++){average += get_strip_origin(det_origin, hit_strips.at(i), stereo_angle);}
    return TVector3( average.x()/hit_strips.size(), average.y()/hit_strips.size(), average.z()/hit_strips.size());
}

// Find average pos between two points. if z is the same it will remain constant
TVector3 TrackerGeometry::get_average_pos(TVector3 p1, TVector3 p2)
{
    return TVector3( 0.5*(p1.x() + p2.x()), 0.5*(p1.y() + p2.y()), 0.5*(p1.z() + p2.z()));
}

// Find average pos from list of points. if z is the same it will remain constant
TVector3 TrackerGeometry::get_average_pos(std::vector<TVector3> points)
{
    TVector3 average(0,0,0);
    for(size_t i=0; i<points.size(); i++){average += points.at(i);}
    return TVector3( average.x()/points.size(), average.y()/points.size(), average.z()/points.size());
}

// Convert a distance in detector co-ordinates (x,u or v) strips into a distance in world
// co-ordinates (x,y) mm. This function is similar to the get_strip_origin() function except
// that it returns a distance vector rather than the distance from the centre of the selected
// strip to the detector origin (corner) ie a position vector.
// if no. of strips (strip_distance) is extracted by fitting the mean then it will
// not be an int therefore use double.
TVector3 TrackerGeometry::get_xy_distance(double strip_distance, double stereo_angle)
{
    TVector3 xy_distance;

    // obtain distance using polar co-ordinates.
    float r_x_strips = strip_distance * s_pitch * cos(stereo_angle);
    float r_y_strips = -1 * strip_distance * s_pitch * sin(stereo_angle);
    
    // convert 1D distance in det. co-ordinates to 2D distance in world co-ordinates
    xy_distance.SetX( r_x_strips );
    xy_distance.SetY( r_y_strips );
    xy_distance.SetZ( 0.0 );
    
    return xy_distance;
}

