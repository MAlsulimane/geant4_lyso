
#include "MeV2ChargeConverter.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

MeV2ChargeConverter::MeV2ChargeConverter(const G4double& value) : MeV2Q(value)
{
}

G4double MeV2ChargeConverter::operator()( G4double input ) const
{
  //Very simple!
  return input*MeV2Q;
}
