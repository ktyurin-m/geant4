
/// \file DetectorConstruction.hh
/// \brief Definition of the B2a::DetectorConstruction class

#ifndef B2aDetectorConstruction_h
#define B2aDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "tls.hh"
#include "MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4MagneticField.hh"
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;
class G4GlobalMagFieldMessenger;

namespace project
{

class DetectorMessenger;
class MagneticField;

/// Detector construction class to define materials, geometry
/// and global uniform magnetic field.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

  public:
    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    // Set methods
    void SetTargetMaterial (G4String );
    void SetChamberMaterial(G4String );
    void SetMaxStep (G4double );
    void SetCheckOverlaps(G4bool );

  private:
    // methods
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
    // static G4ThreadLocal MagneticField* fMagneticField;
    static G4ThreadLocal G4FieldManager* fFieldMgr;
    static G4ThreadLocal MagneticField* magField;
    // static data members
    // static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger;
                                         // magnetic field messenger
    // data members
    G4int fNbOfChambers = 0;

    G4LogicalVolume*  fLogicTarget = nullptr;  // pointer to the logical Target
    G4LogicalVolume*  Box = nullptr;
    G4LogicalVolume*  Convertor = nullptr;
    G4LogicalVolume** fLogicChamber = nullptr; // pointer to the logical Chamber
    G4LogicalVolume* Magnet = nullptr;
    G4LogicalVolume* Gap = nullptr;

    G4Material*       Air = nullptr;  // pointer to the target  material
    G4Material*       Vacuum = nullptr;
    // G4Material*       fChamberMaterial = nullptr; // pointer to the chamber material
    G4Material*       Fe = nullptr;
    G4Material*       CONCRETE = nullptr;

    G4UserLimits* fStepLimit = nullptr; // pointer to user step limits



    G4bool fCheckOverlaps = true; // option to activate checking of volumes overlaps
};

}

#endif
