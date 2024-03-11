/// \file TrackerSD.cc
/// \brief Implementation of the B2::TrackerSD class

#include "TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "G4TrajectoryContainer.hh"
namespace project
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerSD::TrackerSD(const G4String& name,
                     const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerSD::~TrackerSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsCollection
    = new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]);
  G4cout << "SensDetName: " << SensitiveDetectorName << G4endl;
  // Add this collection in hce
  
  G4int hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool TrackerSD::ProcessHits(G4Step* aStep,
                                     G4TouchableHistory*)
{
  G4String par = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
  // if (par != "gamma")
  // { 
  //   return false;
  // }
  
 //G4double edep = aStep->GetTotalEnergyDeposit();
  G4double En = aStep->GetPreStepPoint()->GetKineticEnergy();

  
  // G4cout << "Particle: " << par << G4endl;
  // G4cout << "Energy: " << En << G4endl;
    // G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  // auto event = 
  TrackerHit* newHit = new TrackerHit();
  auto touchable = aStep->GetPreStepPoint()->GetTouchable();
  auto trackID = aStep->GetTrack()->GetTrackID();
  auto parentID = aStep->GetTrack()->GetParentID();
  auto transform = touchable->GetHistory()->GetTopTransform();
  auto worldPos = aStep->GetPreStepPoint()->GetPosition();
  auto localPos
    = touchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);
  newHit->SetPos (
    localPos
    );
    newHit->SetParticle(par);
 // G4cout << "localX: " << localPos.getX() << G4endl;
  newHit->SetEdep( En );
  newHit->SetTrackID(trackID);
  // newHit->SeParentID(1);
  // newHit->SetNameDetector()
  fHitsCollection->insert( newHit );
  G4cout << "Parent ID: " << parentID << ", Tracker ID: " << trackID << ", Particle: " << par << ", Energy: " << En << G4endl;
  // G4cout << "NAME det: "<< fHitsCollection->GetSDname() << G4endl; 
  // newHit->AddEdep(edep);
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  G4int Hits = fHitsCollection->entries();
  
  
  auto analysisManager = G4AnalysisManager::Instance();
  for (int i = 0; i < Hits; i++)
  {
      analysisManager->FillNtupleDColumn(0, (*fHitsCollection)[i]->GetPos().getX());
      analysisManager->FillNtupleDColumn(1, (*fHitsCollection)[i]->GetPos().getY());
      analysisManager->FillNtupleDColumn(2, (*fHitsCollection)[i]->GetPos().getZ());  
      analysisManager->FillNtupleDColumn(3, (*fHitsCollection)[i]->GetEdep());
      analysisManager->FillNtupleSColumn(4, (*fHitsCollection)[i]->GetParticle());
      analysisManager->FillNtupleSColumn(5, SensitiveDetectorName);
      analysisManager->FillNtupleIColumn(6, (*fHitsCollection)[i]->GetTrackID());
      // analysisManager->FillNtupleIColumn(7, (*fHitsCollection)[i]->GetParentID());
      analysisManager->AddNtupleRow();
  }
  

   

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

