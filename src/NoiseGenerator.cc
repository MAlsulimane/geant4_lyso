
#include "NoiseGenerator.hh"
#include <assert.h>
#include <algorithm>

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

NoiseGenerator::NoiseGenerator(const G4double& value) :
  sigma(value) ,
  //Passing the engine as reference it prevents deletion by G4GaussRandom
  randomGauss( *(CLHEP::HepRandom::getTheEngine()) , 0.0 , 1.0 )
{
}

NoiseGenerator::NoiseGenerator(const NoiseGenerator& rhs ) :
	sigma(rhs.sigma) ,
	randomGauss( *(CLHEP::HepRandom::getTheEngine()) , 0.0 , 1.0 )
{
}

G4double NoiseGenerator::operator()()
{
	//Noise Generator uses underlying G4RandGauss to generate random numbers
   if ( sigma > 0. )
     return randomGauss.fire( 0.0 , sigma );
   else
     return 0.;
}


