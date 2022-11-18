
#include "SiDigi_pix.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

// -- one more nasty trick for new and delete operator overloading:
G4Allocator<SiDigi_pix> SiDigi_pixAllocator;

SiDigi_pix::SiDigi_pix(const int& pix_pn, const int& pix_n) :
		charge(0) ,
		pixelNumber(pix_n) ,
		planeNumber(pix_pn) ,
		isPrimary(false)
{

}

void SiDigi_pix::Print()
{
  //Add +1 to the plane & strip no. since they start at 0 but det./strip no starts at 1
  //Called by EventAction class with the command: digits->PrintAllDigi();
  //if(charge>0)
      G4cout << "Digit: Plane = "<< planeNumber+1 << " Pixel = " << pixelNumber << " with Charge = " << charge << " electrons" << G4endl;
}
