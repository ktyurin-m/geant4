
/// \file RunAction.hh
/// \brief Definition of the B2::RunAction class

#ifndef B2RunAction_h
#define B2RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

namespace project
{

/// Run action class

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction() override;

    void BeginOfRunAction(const G4Run* run) override;
    void   EndOfRunAction(const G4Run* run) override;
};

}

#endif
