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
        // if (track->GetVolume()->GetName() == "Magnet_2")
        // {
        //     track->SetTrackStatus(fStopAndKill);
        // }
        
        G4double x, y, z;
        x = track->GetPosition().getX();
        y = track->GetPosition().getY();
        z = track->GetPosition().getZ();

        // // // check if track is inside your primitive (e.g. a box)
        G4double R = sqrt(pow(y,2) + pow(z,2));
        if(R>2*500 && x < 27000)
        {
            // kill the track
        // G4cout << "y: " << y <<", z: " << z<< G4endl;
           track->SetTrackStatus(fStopAndKill);
        }
    }
};
