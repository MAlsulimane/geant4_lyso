
#ifndef SIDIGI_PIX_HH_
#define SIDIGI_PIX_HH_

#include <TVector3.h>

#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"


// Definition of a digit
// This class defines the structure of a digit.
// A digit is the output of the detector as recorded by the R/O
// A digit is defined by an identifier: the
// (plane number , strip number) and the measurement,
// in this case the collected charge.
// Hits are collected in a collection of hits: SiDigiCollection

class SiDigi_pix : public G4VDigi
{
public:
    
    // constructor
    SiDigi_pix(const int& pixelplaneNum , const int& pixelNum);

    // Empty destructor
    virtual ~SiDigi_pix() {}

    // Add a charge to the digit
    inline void Add( const G4double& aValue ) { charge+= aValue; }

    // Print a digit, inherited method. Print some information on the digit
    void Print();

    // Draw a digit, inherited method, empty: do not draw digits
    void Draw() {}
    
    // some simple set & get functions
    inline void     SetCharge( const G4double& aCharge ) { charge = aCharge; }
    inline G4double GetCharge( ) const { return charge; }
    inline void       SetPos( const TVector3& position ) { hit_pos = position; }
    inline TVector3   GetPos( ) const { return hit_pos; }
    
    inline void     SetPlaneNumber( const G4int& aPlane ) { planeNumber = aPlane; }
    inline G4int    GetPlaneNumber( ) const { return planeNumber; }
    inline void     SetPixelNumber( const G4int& aPixel) { pixelNumber = aPixel; }
    inline G4int    GetPixelNumber( ) const { return pixelNumber; }
    
    // Memory management methods
    // Equality operator

    // Two digits are the same if they belong to the same detector i.e. plane and pixel number
    // note that no check is done on the charge, since the logic is that each strip can make a single measurement (the hit).
    inline G4int operator==(const SiDigi_pix& aDigi) const
    { return ( ( planeNumber == aDigi.GetPlaneNumber() ) && ( pixelNumber == aDigi.GetPixelNumber() ) ); }

    // The new operator
    // This operator creates efficiently a new hit. Overwriting the default new operators allows for the use
    // of the G4Allocator functionalities. See 3.2.4 "General management classes" paragraph in Application Developer G4 manual
    // note The use of allocators is not mandatory but recommended (to improve performances). You can safely skip this part.
    inline void* operator new(size_t);

    // Delete operator
    inline void  operator delete(void* aDigi);

private:
    
    // Collected charge
    G4double charge;

    // position of hit within strip
    TVector3 hit_pos;
    
    G4int pixelNumber;
    G4int planeNumber;

    // flag if hit is from primary
    G4bool isPrimary;
    
};

// A container of digits
typedef G4TDigiCollection<SiDigi_pix> SiDigi_pixCollection;


// Allocator
// Creating this objects allows for an efficient use of memory.
// Operators new and delete for the SiDigi objects have to be defined
extern G4Allocator<SiDigi_pix> SiDigi_pixAllocator;

// It's not very nice to have these two in .hh and not in .cc
// But if we move these to the correct place we receive a warning at compilation time
// Also these have to appear below the previous declaration of the allocator
// This should be cleaned somehow...
void * SiDigi_pix::operator new(size_t)
{
  return static_cast<void*>( SiDigi_pixAllocator.MallocSingle() );
}

void SiDigi_pix::operator delete(void* aDigi)
{
  SiDigi_pixAllocator.FreeSingle( static_cast<SiDigi_pix*>(aDigi) );
}

#endif /* SIDIGI_PIX_HH_ */
