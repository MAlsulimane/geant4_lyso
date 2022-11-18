

//Class to simulate charge diffusion

#include "DiffusionGenerator.hh"
#include <iostream>
#include <assert.h>
#include <algorithm>

#include "TrackerGeometry.hh"

DiffusionGenerator::DiffusionGenerator()
{
    //Calculate charge diffusion sigma
    k = 1.3806488e-23;
    T = 300;                            // temp
    Vb = 30;                            // bias voltage default
    e = 1.6e-19;
    det_thickness = 0.150;              // default det. thickness
    d = det_thickness*det_thickness;    // depth, set as det thickness^2 as good approximation
    sigma = sqrt((2*k*T*d*d)/(e*Vb));
}

// Use dedicated tracker geom object instead of passing one from SiDigitizer
// as this allows the diffusion generator to be initialised with a single
// variable from the messenger classes and macros compare with noise,
// pedestal and cross talk functions
DiffusionGenerator::DiffusionGenerator(const double& bias) : Vb(bias),
tracker( std::string("tracker_geom.mac") )
{
    if ( bias <= 0. ){sigma = 0;}
    else
    {
        //Calculate charge diffusion sigma
        k = 1.3806488e-23;
        T = 300;
        e = 1.6e-19;
        det_thickness = tracker.get_thickness();
        // depth, set as det thickness^2 as good approximation
        d = det_thickness*det_thickness;
        sigma = sqrt((2*k*T*d*d)/(e*Vb));
    }
}

DiffusionGenerator::~DiffusionGenerator()
{
    
}

double* DiffusionGenerator::get_igral_limits(double limits[], double det_strip_origin, double det_hit_pos, double s_pitch)
{
    //integral intial limit from det_strip_origin - 0.5*P - det_hit_pos
    limits[0] = det_strip_origin - 0.5*s_pitch - det_hit_pos;
    //integral final limit from det_strip_origin + -0.5*P - det_hit_pos
    limits[1] = det_strip_origin + 0.5*s_pitch - det_hit_pos;
     
    return limits;
}

double DiffusionGenerator::get_charge_frac(double limits[])
{
    //Integrate the charge distribution (gaussian) over the limits set
    //by the strip pitch to determine what fraction of charge belongs
    //on the strip. The integral of a gaussian is the error function,
    //constants added so that it runs from 0 - 1.
    
    double val_l, val_h, erf_val_l, erf_val_h, fraction, thr;
    
    //Set charge sharing threshold so that computation time is reduced
    //prevents calculation for insignifcant amounts of charge sharing
    //ie for a thr = 1e-4 , signals of 50ke- to 100ke- will leave a
    //minimun of ~10e- on a strip.
    thr = 1e-4;
    
    val_l = (limits[0])/(sigma*sqrt(2));
    val_h = (limits[1])/(sigma*sqrt(2));
    erf_val_l = 0.5*(1+TMath::Erf(val_l));
    erf_val_h = 0.5*(1+TMath::Erf(val_h));
    
    fraction = ( (erf_val_h - erf_val_l) >= thr ) ? (erf_val_h - erf_val_l) : 0;
    
    //Use for debug
    /*std::cout << "\nlimits[0] = " << limits[0] << " mm" << std::endl;
    std::cout << "limits[1] = " << limits[1] << " mm" << std::endl;
    std::cout << "val_l  = " << val_l  << std::endl;
    std::cout << "val_h  = " << val_h  << std::endl;
    std::cout << "erf_val_l   = " << erf_val_l   << std::endl;
    std::cout << "erf_val_h  = " << erf_val_h  << std::endl;
    std::cout << "fraction of seed charge on current strip is: " << fraction << std::endl;*/
    
    return fraction;
}


