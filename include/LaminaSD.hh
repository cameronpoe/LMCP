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
/// \file LaminaSD.hh
/// \brief Definition of the LaminaSD class

#ifndef LaminaSD_h
#define LaminaSD_h 1

#include "G4VSensitiveDetector.hh"

#include "LaminaHit.hh"

class G4Step;
class G4HCofThisEvent;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  class EventAction;

  //****************************************************************************
  /// Lamina sensitive detector class
  ///
  /// In Initialize(), it creates one hit for each Lamina 
  ///
  //****************************************************************************
  class LaminaSD : public G4VSensitiveDetector
  {
    public:
      LaminaSD(G4String name);
      ~LaminaSD() override;
    
      // methods from base class
      void   Initialize(G4HCofThisEvent* hitCollection) override;
      G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
      void   EndOfEvent(G4HCofThisEvent* hitCollection) override;

    private:
      // data members
      LaminaHitsCollection* fHitsCollection = nullptr;
      G4int fHCID = -1;

  };

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif

