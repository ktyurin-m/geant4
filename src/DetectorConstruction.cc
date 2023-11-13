
#include "DetectorConstruction.hh"
#include "TrackerSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "MagneticField.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

#include "G4FieldManager.hh"
#include "G4EqMagElectricField.hh"
#include "CADMesh.hh"
using namespace project;

namespace project
{

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField = 0;
  G4ThreadLocal G4FieldManager *DetectorConstruction::fFieldMgr = 0;
  G4ThreadLocal MagneticField *DetectorConstruction::magField = 0;

  DetectorConstruction::DetectorConstruction()
  {
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  DetectorConstruction::~DetectorConstruction()
  {
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  G4VPhysicalVolume *DetectorConstruction::Construct()
  {
    DefineMaterials();
    return DefineVolumes();
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void DetectorConstruction::DefineMaterials()
  {
    G4NistManager *nistManager = G4NistManager::Instance();
    Vacuum = nistManager->FindOrBuildMaterial("G4_Galactic");
    Air = nistManager->FindOrBuildMaterial("G4_AIR");
    Fe = nistManager->FindOrBuildMaterial("G4_Fe");
    CONCRETE = nistManager->FindOrBuildMaterial("G4_CONCRETE");
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  G4VPhysicalVolume *DetectorConstruction::DefineVolumes()
  {

    // World

    G4double boxsize = 9 * m;
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(boxsize);
    G4Box *worldS = new G4Box("world",                    // its name
                              boxsize, boxsize, boxsize); // its size

    worldLV = new G4LogicalVolume(
        worldS,   // its solid
        Air,      // its material
        "World"); // its name

    G4VPhysicalVolume *worldPV = new G4PVPlacement(
        0,               // no rotation
        G4ThreeVector(), // at (0,0,0)
        worldLV,         // its logical volume
        "World",         // its name
        0,               // its mother  volume
        false,           // no boolean operations
        0,               // copy number
        fCheckOverlaps); // checking overlaps

    auto posFreeCad = G4ThreeVector(470 * mm, -420 * mm, -450 * mm);
    auto mesh = CADMesh::TessellatedMesh::FromOBJ("./testbeam.obj");
    //   //-------------------------------------------------------------
    //         auto Staff_only = new G4LogicalVolume( mesh->GetSolid("Cut005")
    //                                          , CONCRETE
    //                                          , "logic"
    //                                          , 0, 0, 0
    //         );
    //         new G4PVPlacement( 0
    //                          , posFreeCad
    //                          , Staff_only
    //                          , "Staff"
    //                          , worldLV
    //                          , false, fCheckOverlaps
    //         );
    // //------------------------------------------------------------------------

    //         auto Tube1 = new G4LogicalVolume( mesh->GetSolid("Tube001")
    //                                          , Fe
    //                                          , "logic"
    //                                          , 0, 0, 0
    //         );
    //         new G4PVPlacement( 0
    //                          , posFreeCad
    //                          , Tube1
    //                          , "Tube"
    //                          , worldLV
    //                          , false, fCheckOverlaps
    //         );
    // //----------------------------------------------------------

    // Visualization attributes

    // detector

    // Vacuum box
    G4double length_vac = 8.7 * m;
    G4Box *boxvac1 = new G4Box("vaccuum", length_vac / 2, 2 * m, 2 * m);
    G4LogicalVolume *Boxvac1 = new G4LogicalVolume(boxvac1, Vacuum, "vacc");
    new G4PVPlacement(0,
                      G4ThreeVector(length_vac / 2, 0, 0),
                      Boxvac1,
                      "vac",
                      worldLV,
                      false,
                      0,
                      fCheckOverlaps);

    G4double magnet_l = 2.3 * m;
    G4Box *MagnetBox = new G4Box("Magnet_2", magnet_l / 2, 0.9 * m / 2, 0.84 * m / 2);
    Magnet_2 = new G4LogicalVolume(MagnetBox, Fe, "Magnet_2");
    new G4PVPlacement(0,
                      G4ThreeVector(-(length_vac / 2 - magnet_l / 2 - 0.47 * m), 0, 0),
                      Magnet_2,
                      "MAGNET_F",
                      Boxvac1,
                      false,
                      0,
                      fCheckOverlaps);
    G4RotationMatrix *rot = new G4RotationMatrix();
    rot->rotateY(90 * deg);
    G4Tubs *tubik1 = new G4Tubs("tubil", 4 * cm, 6 * cm, 8.7* m / 2, 0, 360 * deg);
    Tubik1 = new G4LogicalVolume(tubik1, Fe, "tubik");
    new G4PVPlacement(rot,
                      G4ThreeVector(0, 0, 0),
                      Tubik1,
                      "tubik",
                      Boxvac1,
                      false,
                      0,
                      fCheckOverlaps);

    G4Tubs *tubik2 = new G4Tubs("tubil", 0, 4 * cm, 2.3 * m / 2, 0, 360 * deg);
    G4LogicalVolume *Tubik2 = new G4LogicalVolume(tubik2, Vacuum, "tubik");
    new G4PVPlacement(rot,
                      G4ThreeVector(0, 0, 0),
                      Tubik2,
                      "tubik",
                      Magnet_2,
                      false,
                      0,
                      fCheckOverlaps);
    auto mesh1 = CADMesh::TessellatedMesh::FromOBJ("./tube-Tube.obj");
    //====================================================================================================================
    // cylinder = new G4LogicalVolume(mesh1->GetSolid("Cylinder"), Vacuum, "logic", 0, 0, 0); //Вакуумная труба

    // new G4PVPlacement(rot, G4ThreeVector(length_vac / 2 + 2 * mm, 0, 0), cylinder, "Staff", Boxvac1, false, 2, fCheckOverlaps);

    // auto tubem1 = new G4LogicalVolume(mesh1->GetSolid("Tube"), Fe, "logic", 0, 0, 0);
    // new G4PVPlacement(rot, G4ThreeVector(length_vac / 2, 0, 0), tubem1, "Staff", Boxvac1, false, 2, fCheckOverlaps);

    // auto tubem2 = new G4LogicalVolume(mesh1->GetSolid("Tube"), Fe, "logic", 0, 0, 0);
    // new G4PVPlacement(rot, G4ThreeVector(length_vac / 2, 0, 0), tubem2, "Staff", Boxvac1, false, 2, fCheckOverlaps);

    G4Box *boxvac2 = new G4Box("vaccuum", 1 * m, 1 * m, 1 * m);
    G4LogicalVolume *Boxvac2 = new G4LogicalVolume(boxvac2, Vacuum, "vacc");
    new G4PVPlacement(0,
                      G4ThreeVector(-1 * m, 0, 0),
                      Boxvac2,
                      "vac",
                      worldLV,
                      false,
                      0,
                      fCheckOverlaps);
    G4Box *solidbox = new G4Box("Box", 2 * mm, 6 * cm, 6 * cm); //
    Box = new G4LogicalVolume(solidbox, Vacuum, "Box");           // detector
    new G4PVPlacement(0,
                      G4ThreeVector(8.7*m + 1*mm, 0, 0),
                      Box,
                      "Box",
                      worldLV,
                      false,
                      3,
                      true);
    // test
    G4double l = 1.76 * 0.1 * cm;
    G4Box *conv = new G4Box("conv", l / 2, 10 * cm, 10 * cm);
    Convertor = new G4LogicalVolume(conv, Fe, "conv");
    new G4PVPlacement(0,
                      G4ThreeVector(1 * m - l / 2, 0, 0),
                      Convertor,
                      "conv",
                      Boxvac2,
                      false,
                      2,
                      true);
    //==========================================================================
    // magnit
    // G4double lengthM = 1600 / 2 * mm;
    // G4double widthM = 500 / 2 * mm;
    // G4double heightM = 280 / 2 * mm;

    // G4Box *magnet = new G4Box("Box1", widthM, heightM, lengthM);

    // Magnet = new G4LogicalVolume(magnet, Fe, "Box1");

    // new G4PVPlacement(0,
    //                   G4ThreeVector(0, 0, 2 * m),
    //                   Magnet,
    //                   "Box1",
    //                   worldLV,
    //                   false,
    //                   0,
    //                   fCheckOverlaps);

    // // gap
    // G4double lengthG = 1600 / 2 * mm;
    // G4double widthG = 310 / 2 * mm;
    // G4double heightG = 20 / 2 * mm;
    // G4Box *gap = new G4Box("Box2", widthG, heightG, lengthG + 1 * mm);

    // Gap = new G4LogicalVolume(gap, Air, "Box2");

    // new G4PVPlacement(0,
    //                   G4ThreeVector(0, 0, 2 * m),
    //                   Gap,
    //                   "Box2",
    //                   worldLV,
    //                   false,
    //                   2,
    //                   fCheckOverlaps);

    G4double maxStep = 1 * mm;
    fStepLimit = new G4UserLimits(maxStep);
    worldLV->SetUserLimits(fStepLimit);

    return worldPV;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void DetectorConstruction::ConstructSDandField()
  {
    // Sensitive detectors
    G4String trackerChamberSDname = "/TrackerChamberSD";
    TrackerSD *aTrackerSD = new TrackerSD(trackerChamberSDname,
                                          "TrackerHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
    // Setting aTrackerSD to all logical volumes with the same name
    // of "Chamber_LV".
    SetSensitiveDetector("Box", aTrackerSD, false);

    // Create global magnetic field messenger.
    // Uniform magnetic field is then created automatically if
    // the field value is not zero.

    // MAGNETIC FIELD
    magField = new MagneticField();
    fFieldMgr = new G4FieldManager();
    fFieldMgr->SetDetectorField(magField);
    fFieldMgr->CreateChordFinder(magField);
    
    Magnet_2->SetFieldManager(fFieldMgr, false);

    // Register the field messenger for deleting
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void DetectorConstruction::SetMaxStep(G4double maxStep)
  {
    if ((fStepLimit) && (maxStep > 0.))
      fStepLimit->SetMaxAllowedStep(maxStep);
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void DetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
  {
    fCheckOverlaps = checkOverlaps;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
