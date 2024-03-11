
/// \file EventAction.cc
/// \brief Implementation of the B2::EventAction class

#include "EventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "TrackerSD.hh"
#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
// class G4Run;
namespace project
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(7, event->GetEventID());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void EventAction::EndOfEventAction(const G4Event* event)
{
  // get number of stored trajectories

  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  G4Run* run = static_cast<G4Run*>( G4RunManager::GetRunManager()->GetNonConstCurrentRun() );
  G4int nOfEvents = run->GetNumberOfEventToBeProcessed();
  // periodic printing

  G4int eventID = event->GetEventID();

  if (eventID % 1000 == 0) {
    // time_t my_time = time(NULL);
    // tm *ltm = localtime(&my_time);
    G4double status = (100*(eventID/double(nOfEvents)));
    G4cout << ">>> Event: " << eventID << " | (" << status << " %" << ")"  << G4endl;
    // if ( trajectoryContainer ) {
    //   G4cout << "    " << n_trajectories
    //          << " trajectories stored in this event." << G4endl;
    // }
    // G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
    // G4cout << "    "
    //        << hc->GetSize() << " hits stored in this event" << G4endl;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

