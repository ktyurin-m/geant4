#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include <CLHEP/Units/SystemOfUnits.h>
#include "globals.hh"

class MySteppingAction: public G4UserSteppingAction
{
public:
    MySteppingAction() {} 
    virtual ~MySteppingAction() {} 

    virtual void UserSteppingAction(const G4Step* step) 
    {
        // get the track of current particle
        G4Track* track = step->GetTrack();
        G4double x, y, z;
        x = track->GetPosition().getX();
        y = track->GetPosition().getY();
        z = track->GetPosition().getZ();

        // check if track is inside your primitive (e.g. a box)

        if(abs(y) > 350 || abs(z) > 350)
        {
            // kill the track
            track->SetTrackStatus(fStopAndKill);
        }
    }
};
