#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"

class MySteppingAction: public G4UserSteppingAction
{
public:
    MySteppingAction() {} 
    virtual ~MySteppingAction() {} 

    virtual void UserSteppingAction(const G4Step* step) 
    {
        // get the track of current particle
        G4Track* track = step->GetTrack();
        
        // check if track is inside your primitive (e.g. a box)
        G4VPhysicalVolume* volume = track->GetVolume();
        
        if(volume->GetName() == "MAGNET_F")
        {
            // kill the track
            track->SetTrackStatus(fStopAndKill);
        }
    }
};
