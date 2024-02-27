
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
#include "G4Mag_SpinEqRhs.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4Mag_EqRhs.hh"
#include "G4SubtractionSolid.hh"
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

    G4double WorldSize = 9 * m;
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(WorldSize);
    G4Box *worldS = new G4Box("world",                               // its name
                              4 * WorldSize, WorldSize, WorldSize); // its size

    worldLV = new G4LogicalVolume(
        worldS,   // its solid
        Air,   // its material
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

    auto posFreeCad = G4ThreeVector(470 * mm, -450 * mm, +420 * mm);
    auto mesh = CADMesh::TessellatedMesh::FromOBJ("./testbeam.obj");
    //   //-------------------------------------------------------------
    auto Staff_only = new G4LogicalVolume(mesh->GetSolid("Cut005"), CONCRETE, "logic", 0, 0, 0);
    auto st = new G4RotationMatrix();
    st->rotateX(90*deg);
    new G4PVPlacement(st, posFreeCad, Staff_only, "Staff_Only", worldLV, false, fCheckOverlaps);

    // Space_1
    G4double length_space = 8.7 * m, width_space = 4 * m, height = 4 * m;
    G4Box *space_1 = new G4Box("space_1", length_space / 2, width_space / 2, height / 2);
    G4LogicalVolume *Space_1 = new G4LogicalVolume(space_1, Vacuum, "space_1");
    new G4PVPlacement(0,
                      G4ThreeVector(length_space / 2, 0, 0),
                      Space_1,
                      "Sector_VEPP4",
                      worldLV,
                      false,
                      0,
                      fCheckOverlaps);
    //=======================================================================================
    G4double length_magnet = 2.3 * m, width_magnet = 0.9 * m, height_magnet = 0.84 * m,
             width_aper = 26 * cm, height_aper = 27 * cm;
    G4Box *MagnetBox = new G4Box("R_Magnet_VEPP4", length_magnet / 2, width_magnet / 2, height_magnet / 2);
    //=========================================================================================
    G4Box *aper = new G4Box("aper", length_magnet / 2, width_aper / 2, height_aper / 2);
    Aper = new G4LogicalVolume(aper, Air, "aper");
    new G4PVPlacement(0,
                      G4ThreeVector(-(length_space / 2 - length_magnet / 2 - 0.47 * m), 0, 0),
                      Aper,
                      "Aper",
                      Space_1,
                      false,
                      0,
                      fCheckOverlaps);
    //=========================================================================================
    G4SubtractionSolid *substraction = new G4SubtractionSolid("Box - Aper", MagnetBox, aper);
    R_Magnet_VEPP4 = new G4LogicalVolume(substraction, Fe, "R_Magnet_VEPP4");
    new G4PVPlacement(0,
                      G4ThreeVector(-(length_space / 2 - length_magnet / 2 - 0.47 * m), 0, 0),
                      R_Magnet_VEPP4,
                      "R_Magnet_VEPP4",
                      Space_1,
                      false,
                      0,
                      fCheckOverlaps);

    //======================================================================================
    G4RotationMatrix *rot = new G4RotationMatrix();
    rot->rotateY(90 * deg);
    G4Tubs *tube1 = new G4Tubs("tube1", 4.5 * cm, 5 * cm, length_magnet / 2, 0, 360 * deg); // tubes in aper
    Tube1 = new G4LogicalVolume(tube1, Fe, "tube1");
    new G4PVPlacement(rot,
                      G4ThreeVector(0, 0, 0),
                      Tube1,
                      "tube1",
                      Aper,
                      false,
                      0,
                      fCheckOverlaps);
    //======================================================================================
    G4Tubs *tube2 = new G4Tubs("tube2", 0, 4.5 * cm, length_magnet / 2, 0, 360 * deg);
    Tube2 = new G4LogicalVolume(tube2, Vacuum, "tube2");
    new G4PVPlacement(rot,
                      G4ThreeVector(0, 0, 0),
                      Tube2,
                      "tube2",
                      Aper,
                      false,
                      0,
                      fCheckOverlaps);
    //=============================================================================================
    G4Tubs *tube4 = new G4Tubs("tube4", 4.5 * cm, 5 * cm, length_space / 2, 0, 360 * deg);
    G4LogicalVolume *Tube4 = new G4LogicalVolume(tube4, Fe, "tube4");
    new G4PVPlacement(rot,
                      G4ThreeVector(0, 0, 0),
                      Tube4,
                      "tube4",
                      Space_1,
                      false,
                      0,
                      fCheckOverlaps);
        //======================================================================================
    G4Tubs *Transport_Pipe = new G4Tubs("Transport_Pipe", 31*cm/2, 33/2 * cm, 12.9 / 2*m, 0, 360 * deg);
    G4LogicalVolume *TubeL = new G4LogicalVolume(Transport_Pipe, Fe, "Transport_Pipe");
    new G4PVPlacement(rot,
                      G4ThreeVector(21.49*m, 0, 0),
                      TubeL,
                      "Transport_Pipe",
                      worldLV,
                      false,
                      0,
                      fCheckOverlaps);
      //=======================================================================================================
    G4Box *folga = new G4Box("folga", 0.5*mm/2, width_aper / 2, height_aper / 2); //folga -- material between vaccuum and air
    auto Folga = new G4LogicalVolume(folga, Fe, "folga");
    new G4PVPlacement(0,
                      G4ThreeVector(8.7*m + 0.5*mm/2, 0, 0),
                      Folga,
                      "Aper",
                      worldLV,
                      false,
                      0,
                      fCheckOverlaps);
    //====================================================================================================================
    G4Box *boxvac2 = new G4Box("vaccuum", 1 * m, 1 * m, 1 * m);
    G4LogicalVolume *Boxvac2 = new G4LogicalVolume(boxvac2, Vacuum, "vacc");
    new G4PVPlacement(0,
                      G4ThreeVector(-1 * m, 0, 0),
                      Boxvac2,
                      "Sector_VEPP4",
                      worldLV,
                      false,
                      0,
                      fCheckOverlaps);
    //===================================================================================
    G4Box *detector1 = new G4Box("Box", 2 * mm, 0.5 * m, 0.5 * m);     //
    Detector1 = new G4LogicalVolume(detector1, Vacuum, "Detector1"); // detector
    new G4PVPlacement(0,
                      G4ThreeVector(27.94*m + 0.5*m, 0, 0),
                      Detector1,
                      "Detector1",
                      worldLV,
                      false,
                      3,
                      true);
    //=========================================================================================
    G4Box *detector2 = new G4Box("Box", 2 * mm, 0.5 * m, 0.5 * m);     //
    Detector2 = new G4LogicalVolume(detector2, Vacuum, "Detector2"); // detector
    new G4PVPlacement(0,
                      G4ThreeVector(27.94*m + 4*m, 200*mm, 0),
                      Detector2,
                      "Detector2",
                      worldLV,
                      false,
                      3,
                      true);
    // // convertor
    G4double X0 = 1.76 * 0.1 * cm;
    G4Box *conv = new G4Box("conv", X0 / 2, 10 * cm, 10 * cm);
    Convertor = new G4LogicalVolume(conv, Fe, "conv");
    new G4PVPlacement(0,
                      G4ThreeVector(1 * m - X0 / 2 - 1 * cm, 0, 0),
                      Convertor,
                      "conv",
                      Boxvac2,
                      false,
                      2,
                      true);

    //==========================================================================
    G4double conv_m_l = 1.76 * 0.4 * cm;
    G4Box *conv_mish = new G4Box("convers_mishen", conv_m_l / 2, 1 * cm/2, 1 * cm/2);
    G4LogicalVolume* Conver_mish = new G4LogicalVolume(conv_mish, Fe, "convers_mishen");
    new G4PVPlacement(0,
                      G4ThreeVector(27.94*m + 0.2*m, 0, 0),
                      Conver_mish,
                      "convers_mishen",
                      worldLV,
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

    G4double maxStep = 0.5 * mm;
    fStepLimit = new G4UserLimits(maxStep);
    worldLV->SetUserLimits(fStepLimit);

    return worldPV;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void DetectorConstruction::ConstructSDandField()
  {
    // Sensitive detectors
    G4String detector_1 = "/det1";
    TrackerSD *aTrackerSD1 = new TrackerSD(detector_1,
                                          "TrackerHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD1);
    SetSensitiveDetector("Detector1", aTrackerSD1, false);

    G4String detector_2 = "/det2";
    TrackerSD *aTrackerSD2 = new TrackerSD(detector_2,
                                          "TrackerHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD2);
    SetSensitiveDetector("Detector2", aTrackerSD2, false);
    

    // MAGNETIC FIELD
    magField = new MagneticField();
    fFieldMgr = new G4FieldManager();
    fFieldMgr->SetDetectorField(magField);

    fFieldMgr->CreateChordFinder(magField);
    Aper->SetFieldManager(fFieldMgr, true);
    // Tube1->SetFieldManager(fFieldMgr, true);
    // Tube2->SetFieldManager(fFieldMgr, true);

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
