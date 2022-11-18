
#ifndef SIDIGI_HH_
#define SIDIGI_HH_


#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"

#include <TVector3.h>

/* Definition of a digit
 *
 * This class defines the structure of a digit.
 * A digit is the output of the detector as recorded by the R/O
 * A digit is defined by an identifier: the
 * (plane number , strip number) and the measurement,
 * in this case the collected charge.
 * Hits are collected in a collection of hits: SiDigiCollection
 */

class SiDigi : public G4VDigi
{
public:
  //constructor
  SiDigi(const int& planeNum , const int& stripNum);
  //Empty destructor
  virtual ~SiDigi() {}

    //Add a charge to the digit
    inline void Add( const G4double& aValue ) { charge+= aValue; }
  /*
   * Print a digit
   *
   * Inherited method. Print some information on the
   * digit
   */
  void Print();
  /*
   * Draw a digit
   *
   * Inherited method, empty: do not draw digits
   */
  void Draw() {}
  //some simple set & get functions
  inline void     SetPlaneNumber( const G4int& aPlane ) { planeNumber = aPlane; }
  inline G4int    GetPlaneNumber( ) const { return planeNumber; }
  inline void     SetStripNumber( const G4int& aStrip) { stripNumber = aStrip; }
  inline G4int    GetStripNumber( ) const { return stripNumber; }
  inline void     SetCharge( const G4double& aCharge ) { charge = aCharge; }
  inline G4double GetCharge( ) const { return charge; }
  inline void       SetPos( const TVector3& position ) { hit_pos = position; }
  inline TVector3   GetPos( ) const { return hit_pos; }
    
  // Memory management methods
  // Equality operator
  /*
   * Two digits are the same if they belong to the same detector
   * i.e. plane and Strip number
   * note that no check is done on the charge, since the logic is that
   * each strip can make a single measurement (the hit).
   */
  inline G4int operator==(const SiDigi& aDigi) const
  { return ( ( planeNumber == aDigi.GetPlaneNumber() ) && ( stripNumber == aDigi.GetStripNumber() ) ); }
  // The new operator
  /*
   * This operator creates efficiently a new hit.
   * Overwriting the default new operators allows for the use
   * of the G4Allocator functionalities.
   *
   * See 3.2.4 "General management classes" paragraph in Application Developer G4 manual
   *
   * \note The use of allocators is not mandatory but recommended (to improve performances).
   * You can safely skip this part.
   */
  inline void* operator new(size_t);
  // Delete operator
  /* operator new() */
  inline void  operator delete(void* aDigi);

private:
  // Collected charge
  G4double charge;
  // Strip number
  G4int stripNumber;
  // Plane Number
  G4int planeNumber;
  // flag if hit is from primary
  G4bool isPrimary;
  // position of hit within strip
  TVector3 hit_pos;
    
};

/*
 * A container of digitis
 */
typedef G4TDigiCollection<SiDigi> SiDigiCollection;

/*
 * Allocator
 *
 * Creating this objects allows for an efficient use of memory.
 * Operators new and delete for the SiDigi objects have to be
 * defined
 */
extern G4Allocator<SiDigi> SiDigiAllocator;

//It's not very nice to have these two in .hh and not in .cc
//But if we move these to the correct place we receive a warning at compilation time
//Also these have to appear below the previous declaration of the allocator
//This should be cleaned somehow...
void* SiDigi::operator new(size_t)
{
  return static_cast<void*>( SiDigiAllocator.MallocSingle() );
}

void SiDigi::operator delete(void* aDigi)
{
  SiDigiAllocator.FreeSingle( static_cast<SiDigi*>(aDigi) );
}

#endif /* SIDIGI_HH_ */
