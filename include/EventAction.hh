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
/// \file EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "globals.hh"
#include "G4UserEventAction.hh"

#include "LaminaHit.hh"
#include "PoreHit.hh"
#include "Constants.hh"

#include <array>
#include <vector>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  //****************************************************************************
  /// Event action class
  ///
  //****************************************************************************
  class EventAction : public G4UserEventAction
  {
    public:
      EventAction();
      ~EventAction() override;

      // methods from base class
      void  BeginOfEventAction( const G4Event* ) override;
      void    EndOfEventAction( const G4Event* ) override;

      // methods
      std::vector<G4int>& GetLaminaID() { return fLaminaID; }
      std::vector<G4double>& GetLaminaEdep() { return fLaminaEdep; }
      std::vector<G4double>& GetLaminaTime() { return fLaminaTime; }
      std::vector<G4int>& GetPoreID() { return fPoreID; }
      std::vector<G4int>& GetTrackID() { return fTrackID; }
      std::vector<std::string>& GetPoreCreatorProc() { return fPoreCreatorProc; }
      std::vector<G4double>& GetPoreEdep() { return fPoreEdep; }
      std::vector<G4double>& GetPoreEKin() {return fPoreEKin; }
      std::vector<G4double>& GetPoreTime() { return fPoreTime; }
      std::vector<G4double>& GetPorePosX() { return fPorePosX; }
      std::vector<G4double>& GetPorePosY() { return fPorePosY; }
      std::vector<G4double>& GetPorePosZ() { return fPorePosZ; }

    private:      
      // data members
      G4int fLaminaHCID = -1;
      G4int fPoreHCID = -1;

      std::vector<G4int> fLaminaID;
      std::vector<G4double> fLaminaEdep;
      std::vector<G4double> fLaminaTime;  
      std::vector<G4int> fPoreID;
      std::vector<G4int> fTrackID;
      std::vector<std::string> fPoreCreatorProc;
      std::vector<G4double> fPoreEdep;
      std::vector<G4double> fPoreEKin;
      std::vector<G4double> fPoreTime;  
      std::vector<G4double> fPorePosX;  
      std::vector<G4double> fPorePosY;  
      std::vector<G4double> fPorePosZ;  
  };
                     
}
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif

    
