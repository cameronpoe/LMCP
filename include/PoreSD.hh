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
/// \file PoreSD.hh
/// \brief Definition of the PoreSD class

#ifndef PoreSD_h
#define PoreSD_h 1

#include "G4VSensitiveDetector.hh"

#include "PoreHit.hh"

class G4Step;
class G4HCofThisEvent;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  class EventAction;

  //****************************************************************************
  /// Pore sensitive detector class
  ///
  /// In Initialize(), it creates one hit for each Pore 
  ///
  //****************************************************************************
  class PoreSD : public G4VSensitiveDetector
  {
    public:
      PoreSD(G4String name);
      ~PoreSD() override;
    
      // methods from base class
      void   Initialize(G4HCofThisEvent* hitCollection) override;
      G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
      void   EndOfEvent(G4HCofThisEvent* hitCollection) override;

    private:
      // data members
      PoreHitsCollection* fHitsCollection = nullptr;
      G4int fHCID = -1;

  };

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif

