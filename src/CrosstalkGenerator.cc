
/*
#include "CrosstalkGenerator.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


CrosstalkGenerator::CrosstalkGenerator(const double& xtalk , const int& dim) :
	firstNearXtalk(xtalk) ,
	dimension(dim) ,
	xtalk()
{
	Init();
}

void CrosstalkGenerator::Init() {
	//Construct the crosstalk symmetric matrix.
	//If dimension is 1 (no of strips) or xtalk==0 exit as soon as possible
	if ( dimension == 1 ) return;
	xtalk = CLHEP::HepSymMatrix(dimension,1);
	if ( firstNearXtalk == 0 ) return;
	CLHEP::HepSymMatrix diag(dimension,1);
	diag *= (-2*firstNearXtalk );
	xtalk += diag;
	CLHEP::HepSymMatrix ofdiag(dimension,0);
	for ( int i = 0;i<dimension-1;++i) ofdiag[i][i+1] = firstNearXtalk;
	xtalk += ofdiag;
}
*/

