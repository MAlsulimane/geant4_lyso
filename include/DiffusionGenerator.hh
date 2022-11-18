
//Class to simulate charge diffusion

#ifndef DIFFUSIONGENERATOR_HH_
#define DIFFUSIONGENERATOR_HH_

#include <TMath.h>

#include "TrackerGeometry.hh"

//simulates charge diffusion between strips

class DiffusionGenerator
{
    
public:

    //Constructor
    DiffusionGenerator();

    //Constructor with bias voltage value
    DiffusionGenerator(const double& bias);
    
    //Destructor
    ~DiffusionGenerator();
    
    //some simple get functions
    double get_diffusion_sigma() const { return sigma; }
    double get_hit_depth()       const { return d; }
    double get_bias_voltage()    const { return Vb; }
    double get_det_thickness()   const { return det_thickness; }

    double get_charge_frac(double limits[]);
    double* get_igral_limits(double limits[], double det_strip_origin, double det_hit_pos, double s_pitch);
    
private:

    double sigma, k, e, d, det_thickness;
    float T, Vb;
    //The object that handles the tracker geometry used in charge diffusion
    //And is used by the MakeDiffusion() function.
    TrackerGeometry tracker;
    
};

#endif /* DIFFUSIONGENERATOR_HH_ */
