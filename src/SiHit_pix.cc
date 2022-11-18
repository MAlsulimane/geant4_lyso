
#include "SiHit_pix.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

// -- one more nasty trick for new and delete operator overloading:
G4Allocator<SiHit_pix> SiHit_pixAllocator;

SiHit_pix::SiHit_pix(const G4int pixel, const G4int plane, const G4bool primary, G4int track)
  : pixelNumber(pixel), planeNumber(plane), trackNumber(track), isPrimary(primary) // <<-- note BTW this is the only way to initialize a "const" member
{
  eDep     = 0.0;
  ni_eDep  = 0.0;
}

SiHit_pix::~SiHit_pix()
{
}

void SiHit_pix::Print()
{
    //Add +1 to the plane & strip no. since they start at 0 but det./strip no starts at 1
	G4cout << "Hit: Plane = " << planeNumber+1 << ", PIXEL = " << pixelNumber+1 << ", Edep = " << eDep/MeV << " MeV, NI_Edep = "
    << ni_eDep/MeV << " MeV, t = " << hit_time/s << " sec, isPrimary = " << (isPrimary?"true":"false") << ", track no: " << trackNumber << G4endl;
}
