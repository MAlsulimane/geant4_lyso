
/*
 * NoiseGenerator.hh
 * Brief simulates electronic noise.
 */

#ifndef NOISEGENERATOR_HH_
#define NOISEGENERATOR_HH_


#include "G4Types.hh"
#include "Randomize.hh"

/* simulates electronic noise
 * This class simulates gaussian noise around 0
 */
class NoiseGenerator
{
public:
  // Default constructor
  NoiseGenerator(const G4double& value);
  // Default destructor
  virtual ~NoiseGenerator() {};
  /* Generate noise
   * By default generate gaussian noise
   * if sigma<0 do not smear
   */
  virtual G4double operator() ();
  /* copy and assignement operators
   * These methods are needed since
   * randomGauss should not be copied
   */
  //
  // assignement operator
  inline NoiseGenerator& operator= (const NoiseGenerator& rhs)
  {
  	sigma = rhs.sigma;
  	return *this;
  }
  // copy constructor
  NoiseGenerator(const NoiseGenerator& rhs);
  
private:
  // Noise standard deviation
  G4double sigma;
  // Gaussian Random number
  G4RandGauss randomGauss;
};

#endif /* NOISEGENERATOR_HH_ */
