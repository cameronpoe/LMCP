//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//
//  Laminated Microchannel Plate (LMCP)
//  
//  Created: August 25, 2023 by Camden Ertley
//
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//
/// \file RunAction.hh
/// \brief Definition of the RunAction class

#ifndef RunAction_h
#define RunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"

class G4Run;
class G4Timer;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  class EventAction;

  //****************************************************************************
  /// Run action class
  ///
  //****************************************************************************
  class RunAction : public G4UserRunAction
  {
    public:
      RunAction( EventAction* eventAct );
      ~RunAction() override;

      // methods from base class
      void BeginOfRunAction(const G4Run* run) override;
      void   EndOfRunAction(const G4Run* run) override;

      void AddEdep (G4double edep);

    private:
      // data members
      G4Timer* fTimer = nullptr;

      EventAction* fEventAction = nullptr;

  
  };

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif

