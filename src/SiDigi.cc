
#include "SiDigi.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

// -- one more nasty trick for new and delete operator overloading:
G4Allocator<SiDigi> SiDigiAllocator;

SiDigi::SiDigi(const int& pn, const int& sn) :
		charge(0) ,
		stripNumber(sn) ,
		planeNumber(pn) ,
		isPrimary(false)
{

}

void SiDigi::Print()
{
  //Add +1 to the plane & strip no. since they start at 0 but det./strip no starts at 1
  //Called by EventAction class with the command: digits->PrintAllDigi();
  //if(charge>0) G4cout << "Digit: Plane = "<< planeNumber+1 << " Strip = " << stripNumber << " with Charge = " << charge << " electrons" << G4endl;
}
