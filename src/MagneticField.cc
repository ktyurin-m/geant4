#include "MagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

namespace project
{

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  MagneticField::MagneticField()
  {
    // define commands for this class
    // DefineCommands();
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  MagneticField::~MagneticField()
  {

  }

  void MagneticField::GetFieldValue(const G4double point[4], double *bField) const
  {
    G4double Hm = 9882.18390*gauss;
    G4double k = Hm/(95.);
    // if (std::abs(point[0])<=60.)
    // {
    //   bField[1] = -Hm;
    // }
    // if (point[0]>60.){ //(+x)
    //   bField[1] = k*(point[0]-155);
    // }
    // if (point[0]<-60.){
    //   bField[1] = -( k*(point[0]+155));
    // }
    // // bField[1] = Hm;
    bField[0] = 0.;
    
    bField[2] = 0;
    bField[1] = -Hm; 
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void MagneticField::DefineCommands()
  {
    // Define /B5/field command directory using generic messenger class
    // fMessenger = new G4GenericMessenger(this,
    //                                     "/B5/field/",
    //                                     "Field control");

    // // fieldValue command
    // auto &valueCmd = fMessenger->DeclareMethodWithUnit("value", "tesla",
    //                                                    &MagneticField::SetField,
    //                                                    "Set field strength.");
    // valueCmd.SetParameterName("field", true);
    // valueCmd.SetDefaultValue("1.");
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
