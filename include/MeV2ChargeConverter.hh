
#ifndef MEV2CHARGECONVERTER_HH_
#define MEV2CHARGECONVERTER_HH_
#include "G4Types.hh"


/* MeV to Q converter
 *
 * This class is used to convert the true energy deposits (in MeV), usually from hits
 * to a different units (i.e. charge) this is done to simulate a very simple ADC.
 * You can inherit from this class to implement your own more realistic
 * ADC. This class does not deal with electronic noise.
 * \sa SiDigitizer
 */
class MeV2ChargeConverter
{
public:
  //default constructor
  MeV2ChargeConverter(const G4double& value);
    
  //default desctructor
  virtual ~MeV2ChargeConverter() {}
    
  /* Conversion operator
  Can be overloaded to define your own conversion.
  input should be in MeV. Can be overloaded to define your own conversion
   */
  virtual G4double operator()( G4double input_in_MeV ) const;
    
 private:
  //conversion factor Charge/MeV: output = MeV2Q * input
  G4double MeV2Q;
};

#endif /* MEV2CHARGECONVERTER_HH_ */
