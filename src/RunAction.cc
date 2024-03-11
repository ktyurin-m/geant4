
/// \file RunAction.cc
/// \brief Implementation of the B2::RunAction class

#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

namespace project
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
  // set printing event number per each 100 events
  // G4RunManager::GetRunManager()->SetPrintProgress(1000);
  auto analysisManager = G4AnalysisManager::Instance(); //using ROOT
  analysisManager->SetVerboseLevel( 1 );
  analysisManager->SetNtupleMerging( 1 );    
  analysisManager->CreateNtuple("Data", "Data");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("Energy");
  analysisManager->CreateNtupleSColumn("Particle");
  analysisManager->CreateNtupleSColumn("Detector");
  analysisManager->CreateNtupleIColumn("ID");
  analysisManager->CreateNtupleIColumn("event"); // = 7
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile( "out.root" );
  // analysisManager->FillNtupleIColumn(7, 2);
    // auto event = G4RunManager::GetRunManager()->GetCurrentEvent
    
  // G4cout << "Event from runman: " << event  <<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* )
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

