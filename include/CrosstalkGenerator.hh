
#ifndef CROSSTALKGENERATOR_HH_
#define CROSSTALKGENERATOR_HH_


// Define crosstalk simulation


//#include "CLHEP/Matrix/SymMatrix.h"
//#include "CLHEP/Matrix/Matrix.h"
//#include "CLHEP/Matrix/Vector.h"
//#include "CLHEP/Vector/ThreeVector.h"

/* Crosstalk generator
 *
 * This class simulates the crosstalk between strips.
 * In the default implementation the crosstalks is applied in a first neighbour
 * apporximation.
 * For a strip i the charge is modified to be:
 * Q'(i) = (1-2*f)*Q(i) + f*Q(i+1) + f*Q(i-1)
 */

/*
class CrosstalkGenerator
{
public:
	//Default constructor
    //Creates a Crosstalk generator object
    //xtalk : the fraction of charge leaking
    //dimension : the number of elements (Si strips)
	 CrosstalkGenerator(const double& xtalk , const int& dimension=1024);
 
	//Default destructor
	 virtual ~CrosstalkGenerator() {};
	
    //Simulate crosstalk
    //The crosstalk is applied to the input.
    //Note that the input vector has to be ordered.

	 virtual CLHEP::HepVector operator()( const CLHEP::HepVector& input ) const { return xtalk*input; }
    
private:
	// crosstalk parameter for first neighbours
	double firstNearXtalk;
    
	// parameter defining the number of elements
	int dimension;
    
	//crosstalk matrix representation
	 CLHEP::HepMatrix xtalk;
    
	//Initializes xtalk
    //This method can be overwritten to simulate
    //more complex crosstalk patterns.
    
	 virtual void Init();
};
*/

 
#endif /* CROSSTALKGENERATOR_HH_ */
