
/// \file PrimaryGeneratorAction.hh
/// \brief Definition of the B2::PrimaryGeneratorAction class

#ifndef B2PrimaryGeneratorAction_h
#define B2PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;

namespace project
{

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the Tracker
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class
/// (see the macros provided with this example).

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event* ) override;

    G4GeneralParticleSource* GetParticleGun() {return fParticleGun;}

    // Set methods
    void SetRandomFlag(G4bool );
    G4double angle = 0;
  private:
    G4GeneralParticleSource* fParticleGun = nullptr; // G4 particle gun
};

}

#endif
