
#ifndef TrackerGeometry_h
#define TrackerGeometry_h 1

#include <TMath.h>
#include <TVector3.h>
#include <TLine.h>
#include <TTree.h>

class TrackerGeometry
{
    
public:
    
  // Default Constructor
  TrackerGeometry();
  TrackerGeometry(std::string input_macro_file);
  TrackerGeometry(TTree * t_geom);
    
  // Destructor
  ~TrackerGeometry();
    
    // some simple set & get functions not all variables have a get function
    // as many of them are used to compute more important quantities so don't need
    // to be accessed outside the class itself.
    
    // Set functions only available for the offsets as these are the only variables
    // that need to be modified after they have been read in from external files
    // i.e. the (exact) offset values from the .mac/.root files are used by GEANT4 for
    // the charge sharing algorithm and are read in by the tracker for comparing the
    // current geometry in GEANT4 (.mac file) with the file you are about to track (.root file).
    // However, the actual tracking will use offsets that it calculates itself or that
    // the user inputs manually to simulate the real experiment but also allow comparison with
    // the exact offset values used by GEANT4.
    
    // Setters...
    TVector3 set_x1_pos_offset(const TVector3 & offset)   { return x1_pos_offset = offset; }
    TVector3 set_u1_pos_offset(const TVector3 & offset)   { return u1_pos_offset = offset; }
    TVector3 set_v1_pos_offset(const TVector3 & offset)   { return v1_pos_offset = offset; }

    TVector3 set_x2_pos_offset(const TVector3 & offset)   { return x2_pos_offset = offset; }
    TVector3 set_u2_pos_offset(const TVector3 & offset)   { return u2_pos_offset = offset; }
    TVector3 set_v2_pos_offset(const TVector3 & offset)   { return v2_pos_offset = offset; }
    
    TVector3 set_x3_pos_offset(const TVector3 & offset)   { return x3_pos_offset = offset; }
    TVector3 set_u3_pos_offset(const TVector3 & offset)   { return u3_pos_offset = offset; }
    TVector3 set_v3_pos_offset(const TVector3 & offset)   { return v3_pos_offset = offset; }
    
    TVector3 set_x4_pos_offset(const TVector3 & offset)   { return x4_pos_offset = offset; }
    TVector3 set_u4_pos_offset(const TVector3 & offset)   { return u4_pos_offset = offset; }
    TVector3 set_v4_pos_offset(const TVector3 & offset)   { return v4_pos_offset = offset; }
    
    
    double set_stereo_x1_offset(const double & offset)    { return stereo_x1_offset = offset; }
    double set_stereo_u1_offset(const double & offset)    { return stereo_u1_offset = offset; }
    double set_stereo_v1_offset(const double & offset)    { return stereo_v1_offset = offset; }
    
    double set_stereo_x2_offset(const double & offset)    { return stereo_x2_offset = offset; }
    double set_stereo_u2_offset(const double & offset)    { return stereo_u2_offset = offset; }
    double set_stereo_v2_offset(const double & offset)    { return stereo_v2_offset = offset; }
    
    double set_stereo_x3_offset(const double & offset)    { return stereo_x3_offset = offset; }
    double set_stereo_u3_offset(const double & offset)    { return stereo_u3_offset = offset; }
    double set_stereo_v3_offset(const double & offset)    { return stereo_v3_offset = offset; }
    
    double set_stereo_x4_offset(const double & offset)    { return stereo_x4_offset = offset; }
    double set_stereo_u4_offset(const double & offset)    { return stereo_u4_offset = offset; }
    double set_stereo_v4_offset(const double & offset)    { return stereo_v4_offset = offset; }
    
    
    // Getters...
    double get_s_pitch()        const  { return s_pitch; }
    double get_s_length()       const  { return s_length; }
    int get_strips()            const  { return strips; }
    double get_thickness()      const  { return thickness; }
    
    double get_det_width()      const  { return det_width; }
    double get_diag()           const  { return diag; }
    double get_origin_angle()   const  { return origin_angle; }

    
    TVector3 get_x1_pos()           const  { return x1_pos; }
    TVector3 get_u1_pos()           const  { return u1_pos; }
    TVector3 get_v1_pos()           const  { return v1_pos; }
    
    TVector3 get_x2_pos()           const  { return x2_pos; }
    TVector3 get_u2_pos()           const  { return u2_pos; }
    TVector3 get_v2_pos()           const  { return v2_pos; }
    
    TVector3 get_x3_pos()           const  { return x3_pos; }
    TVector3 get_u3_pos()           const  { return u3_pos; }
    TVector3 get_v3_pos()           const  { return v3_pos; }
    
    TVector3 get_x4_pos()           const  { return x4_pos; }
    TVector3 get_u4_pos()           const  { return u4_pos; }
    TVector3 get_v4_pos()           const  { return v4_pos; }
    
    TVector3 get_x1_pos_offset()    const  { return x1_pos_offset; }
    TVector3 get_u1_pos_offset()    const  { return u1_pos_offset; }
    TVector3 get_v1_pos_offset()    const  { return v1_pos_offset; }
    
    TVector3 get_x2_pos_offset()    const  { return x2_pos_offset; }
    TVector3 get_u2_pos_offset()    const  { return u2_pos_offset; }
    TVector3 get_v2_pos_offset()    const  { return v2_pos_offset; }
    
    TVector3 get_x3_pos_offset()    const  { return x3_pos_offset; }
    TVector3 get_u3_pos_offset()    const  { return u3_pos_offset; }
    TVector3 get_v3_pos_offset()    const  { return v3_pos_offset; }
    
    TVector3 get_x4_pos_offset()    const  { return x4_pos_offset; }
    TVector3 get_u4_pos_offset()    const  { return u4_pos_offset; }
    TVector3 get_v4_pos_offset()    const  { return v4_pos_offset; }
    
    
    TVector3 get_TP1_pos()           const  { return TP1_pos; }
    TVector3 get_TP2_pos()           const  { return TP2_pos; }
    
    TVector3 get_TP3_pos()           const  { return TP3_pos; }
    TVector3 get_TP4_pos()           const  { return TP4_pos; }
    
    TVector3 get_TP5_pos()           const  { return TP5_pos; }
  TVector3 get_shield_pos()       const  { return shield_pos; }// *************************
    
    TVector3 get_phantom_pos()       const  { return phantom_pos; }
    TVector3 get_compensator_pos()   const  { return compensator_pos; }
    TVector3 get_calorimeter_pos()   const  { return calorimeter_pos; }
    
    
    TVector3 get_phantom_centre_plane()       const  { return phantom_centre_plane; }
    TVector3 get_phantom_middle_plane()       const  { return phantom_middle_plane; }
    TVector3 get_phantom_surface_plane()      const  { return phantom_surface_plane; }
  // adding the shielding layer *******************************
  TVector3 get_shield_centre_plane()       const  { return shield_centre_plane; }
  TVector3 get_shield_middle_plane()       const  { return shield_middle_plane; }
  TVector3 get_shield_surface_plane()      const  { return shield_surface_plane; }
    
//    double get_x1_zpos()            const  { return x1_zpos; }
//    double get_u1_zpos()            const  { return u1_zpos; }
//    double get_v1_zpos()            const  { return v1_zpos; }
//    
//    double get_x2_zpos()            const  { return x2_zpos; }
//    double get_u2_zpos()            const  { return u2_zpos; }
//    double get_v2_zpos()            const  { return v2_zpos; }
//    
//    double get_x3_zpos()            const  { return x3_zpos; }
//    double get_u3_zpos()            const  { return u3_zpos; }
//    double get_v3_zpos()            const  { return v3_zpos; }
//    
//    double get_x4_zpos()            const  { return x4_zpos; }
//    double get_u4_zpos()            const  { return u4_zpos; }
//    double get_v4_zpos()            const  { return v4_zpos; }
    
//    double get_TP1_zpos()           const  { return TP1_zpos; }
//    double get_TP2_zpos()           const  { return TP2_zpos; }
//
//    double get_TP3_zpos()           const  { return TP3_zpos; }
//    double get_TP4_zpos()           const  { return TP4_zpos; }
//    
//    double get_TP5_zpos()           const  { return TP5_zpos; }
    
    
//    double get_phantom_zpos()       const  { return phantom_zpos; }
//    double get_compensator_zpos()   const  { return compensator_zpos; }
//    double get_calorimeter_zpos()   const  { return calorimeter_zpos; }
    
//    double get_phantom_centre_d()   const  { return phantom_centre_d; }
//    double get_phantom_average_d()  const  { return phantom_average_d; }
//    double get_phantom_surface_d()  const  { return phantom_surface_d; }
  double get_halfShieldSizeZ()       const  { return halfShieldSizeZ; }//******************
  double get_shield_zShift()         const  { return shield_zShift; }//*******************
    
    double get_halfPhantomSizeZ()       const  { return halfPhantomSizeZ; }
    double get_halfCalorimeterSizeZ()   const  { return halfCalorimeterSizeZ; }
    double get_inter_plane_dist()       const  { return inter_plane_dist; }
    double get_inter_module_dist()      const  { return inter_module_dist; }
    double get_phantom_gap()            const  { return phantom_gap; }
    double get_phantom_zShift()         const  { return phantom_zShift; }
    double get_calorimeter_gap()        const  { return calorimeter_gap; }
    
    TVector3 get_A_x1() const  { return A_x1; }
    TVector3 get_B_x1() const  { return B_x1; }
    TVector3 get_C_x1() const  { return C_x1; }
    TVector3 get_D_x1() const  { return D_x1; }
    
    TVector3 get_A_u1() const  { return A_u1; }
    TVector3 get_B_u1() const  { return B_u1; }
    TVector3 get_C_u1() const  { return C_u1; }
    TVector3 get_D_u1() const  { return D_u1; }
    
    TVector3 get_A_v1() const  { return A_v1; }
    TVector3 get_B_v1() const  { return B_v1; }
    TVector3 get_C_v1() const  { return C_v1; }
    TVector3 get_D_v1() const  { return D_v1; }
    
    TVector3 get_A_x2() const  { return A_x2; }
    TVector3 get_B_x2() const  { return B_x2; }
    TVector3 get_C_x2() const  { return C_x2; }
    TVector3 get_D_x2() const  { return D_x2; }
    
    TVector3 get_A_u2() const  { return A_u2; }
    TVector3 get_B_u2() const  { return B_u2; }
    TVector3 get_C_u2() const  { return C_u2; }
    TVector3 get_D_u2() const  { return D_u2; }
    
    TVector3 get_A_v2() const  { return A_v2; }
    TVector3 get_B_v2() const  { return B_v2; }
    TVector3 get_C_v2() const  { return C_v2; }
    TVector3 get_D_v2() const  { return D_v2; }
    
    TVector3 get_A_x3() const  { return A_x3; }
    TVector3 get_B_x3() const  { return B_x3; }
    TVector3 get_C_x3() const  { return C_x3; }
    TVector3 get_D_x3() const  { return D_x3; }
    
    TVector3 get_A_u3() const  { return A_u3; }
    TVector3 get_B_u3() const  { return B_u3; }
    TVector3 get_C_u3() const  { return C_u3; }
    TVector3 get_D_u3() const  { return D_u3; }
    
    TVector3 get_A_v3() const  { return A_v3; }
    TVector3 get_B_v3() const  { return B_v3; }
    TVector3 get_C_v3() const  { return C_v3; }
    TVector3 get_D_v3() const  { return D_v3; }
    
    TVector3 get_A_x4() const  { return A_x4; }
    TVector3 get_B_x4() const  { return B_x4; }
    TVector3 get_C_x4() const  { return C_x4; }
    TVector3 get_D_x4() const  { return D_x4; }
    
    TVector3 get_A_u4() const  { return A_u4; }
    TVector3 get_B_u4() const  { return B_u4; }
    TVector3 get_C_u4() const  { return C_u4; }
    TVector3 get_D_u4() const  { return D_u4; }
    
    TVector3 get_A_v4() const  { return A_v4; }
    TVector3 get_B_v4() const  { return B_v4; }
    TVector3 get_C_v4() const  { return C_v4; }
    TVector3 get_D_v4() const  { return D_v4; }
    
    double get_stereo_x1() const  { return stereo_x1; }
    double get_stereo_u1() const  { return stereo_u1; }
    double get_stereo_v1() const  { return stereo_v1; }
    
    double get_stereo_x2() const  { return stereo_x2; }
    double get_stereo_u2() const  { return stereo_u2; }
    double get_stereo_v2() const  { return stereo_v2; }
    
    double get_stereo_x3() const  { return stereo_x3; }
    double get_stereo_u3() const  { return stereo_u3; }
    double get_stereo_v3() const  { return stereo_v3; }
    
    double get_stereo_x4() const  { return stereo_x4; }
    double get_stereo_u4() const  { return stereo_u4; }
    double get_stereo_v4() const  { return stereo_v4; }
    
    double get_stereo_x1_offset() const  { return stereo_x1_offset; }
    double get_stereo_u1_offset() const  { return stereo_u1_offset; }
    double get_stereo_v1_offset() const  { return stereo_v1_offset; }
    
    double get_stereo_x2_offset() const  { return stereo_x2_offset; }
    double get_stereo_u2_offset() const  { return stereo_u2_offset; }
    double get_stereo_v2_offset() const  { return stereo_v2_offset; }
    
    double get_stereo_x3_offset() const  { return stereo_x3_offset; }
    double get_stereo_u3_offset() const  { return stereo_u3_offset; }
    double get_stereo_v3_offset() const  { return stereo_v3_offset; }
    
    double get_stereo_x4_offset() const  { return stereo_x4_offset; }
    double get_stereo_u4_offset() const  { return stereo_u4_offset; }
    double get_stereo_v4_offset() const  { return stereo_v4_offset; }
    
    void ComputeParameters();
    void PrintGeometry();
    TVector3 get_strip_origin(TVector3 det_origin, int strip_no, double stereo_angle);
    TVector3 build_strip(TVector3 btm, double stereo_angle);
    void draw_strip(TLine * x_low, TLine * x_mid, TLine * x_high, TVector3 strip_centre_origin, double stereo_angle);
    void draw_strip(TVector3 *x_low_b, TVector3 *x_mid_b, TVector3 *x_high_b, TVector3 *x_low_t, TVector3 *x_mid_t, TVector3 *x_high_t,
                    TVector3 strip_centre_origin, double stereo_angle);
    double world_2_det_transform(TVector3 det_origin, TVector3 det_opp_origin, TVector3 hit);
    bool IsInsideStrip(TVector3 h, TVector3 origin, double stereo_angle);
    TVector3 get_average_strip_pos(TVector3 det_origin, std::vector<int> strips, double stereo_angle);
    TVector3 get_average_pos(TVector3 p1, TVector3 p2);
    TVector3 get_average_pos(std::vector<TVector3> points);
    TVector3 get_xy_distance(double strip_distance, double stereo_angle);

private:

    // initialize geometry parameters
    void InitParameters();
    void InitParameters(std::string input_macro_file);
    void InitParameters(TTree * geom_tree);
    
    // detector parameters
    double s_pitch;                 // Strip pitch in mm
    double s_length;                // Length of strips in mm, slightly longer than width (pitch*strips)
    int strips;                     // No. of strips on each detector
    double thickness;               // Det thickness in mm
    
    double det_width;
    double diag;                    // distance from origin to corner of det.
    double origin_angle;            // angle between vector from origin (det. centre) to det. corner and x axis

    // lengths and spacing between geometry elements
  double halfShieldSizeZ, shield_zShift ; //************************************


    double halfPhantomSizeZ;
    double halfCalorimeterSizeZ;
    double inter_plane_dist;
    double inter_module_dist;
    double phantom_gap, phantom_zShift;
    double calorimeter_gap;
    
    Double_t temp;
//    Double_t temp_x1, temp_u1, temp_v1;
//    Double_t temp_x2, temp_u2, temp_v2;
//    Double_t temp_x3, temp_u3, temp_v3;
//    Double_t temp_x4, temp_u4, temp_v4;
    
    TVector3 x1_pos, u1_pos, v1_pos;
    TVector3 x2_pos, u2_pos, v2_pos;
    TVector3 x3_pos, u3_pos, v3_pos;
    TVector3 x4_pos, u4_pos, v4_pos;
    
    TVector3 x1_pos_offset, u1_pos_offset, v1_pos_offset;
    TVector3 x2_pos_offset, u2_pos_offset, v2_pos_offset;
    TVector3 x3_pos_offset, u3_pos_offset, v3_pos_offset;
    TVector3 x4_pos_offset, u4_pos_offset, v4_pos_offset;
    
    TVector3  TP1_pos, TP2_pos, TP3_pos, TP4_pos, TP5_pos;
    
    TVector3 phantom_pos, compensator_pos, calorimeter_pos;
  TVector3 shield_pos; //************************************

    TVector3 phantom_centre_plane, phantom_middle_plane, phantom_surface_plane;
  TVector3 shield_centre_plane, shield_middle_plane, shield_surface_plane;//**************
    
    // geometry positions along x axis
    /*double x1_xpos, u1_xpos, v1_xpos, x2_xpos, u2_xpos, v2_xpos;
    double x1_xpos_offset, u1_xpos_offset, v1_xpos_offset, x2_xpos_offset, u2_xpos_offset, v2_xpos_offset;
    double x3_xpos, u3_xpos, v3_xpos, x4_xpos, u4_xpos, v4_xpos;
    double x3_xpos_offset, u3_xpos_offset, v3_xpos_offset, x4_xpos_offset, u4_xpos_offset, v4_xpos_offset;
    double TP1_xpos, TP2_xpos, TP3_xpos, TP4_xpos, TP5_xpos;
    
    // geometry positions along y axis
    double x1_ypos, u1_ypos, v1_ypos, x2_ypos, u2_ypos, v2_ypos;
    double x1_ypos_offset, u1_ypos_offset, v1_ypos_offset, x2_ypos_offset, u2_ypos_offset, v2_ypos_offset;
    double x3_ypos, u3_ypos, v3_ypos, x4_ypos, u4_ypos, v4_ypos;
    double x3_ypos_offset, u3_ypos_offset, v3_ypos_offset, x4_ypos_offset, u4_ypos_offset, v4_ypos_offset;
    double TP1_ypos, TP2_ypos, TP3_ypos, TP4_ypos, TP5_ypos;*/
    
    // geometry positions along z axis
    //double x1_zpos, u1_zpos, v1_zpos, x2_zpos, u2_zpos, v2_zpos;
    //double x3_zpos, u3_zpos, v3_zpos, x4_zpos, u4_zpos, v4_zpos;
//    double TP1_zpos, TP2_zpos, TP3_zpos, TP4_zpos, TP5_zpos;
//    double phantom_zpos, compensator_zpos, calorimeter_zpos;
//    double phantom_centre_d, phantom_average_d, phantom_surface_d;
    
    // position vectors for each detector corner
    TVector3 A_x1, B_x1, C_x1, D_x1, A_u1, B_u1, C_u1, D_u1, A_v1, B_v1, C_v1, D_v1;
    TVector3 A_x2, B_x2, C_x2, D_x2, A_u2, B_u2, C_u2, D_u2, A_v2, B_v2, C_v2, D_v2;
    TVector3 A_x3, B_x3, C_x3, D_x3, A_u3, B_u3, C_u3, D_u3, A_v3, B_v3, C_v3, D_v3;
    TVector3 A_x4, B_x4, C_x4, D_x4, A_u4, B_u4, C_u4, D_u4, A_v4, B_v4, C_v4, D_v4;
    
    // stereo angles and offsets for silicon planes
    double stereo_x1, stereo_u1, stereo_v1;
    double stereo_x2, stereo_u2, stereo_v2;
    double stereo_x3, stereo_u3, stereo_v3;
    double stereo_x4, stereo_u4, stereo_v4;
    
    double stereo_x1_offset, stereo_u1_offset, stereo_v1_offset;
    double stereo_x2_offset, stereo_u2_offset, stereo_v2_offset;
    double stereo_x3_offset, stereo_u3_offset, stereo_v3_offset;
    double stereo_x4_offset, stereo_u4_offset, stereo_v4_offset;
    
};

#endif
