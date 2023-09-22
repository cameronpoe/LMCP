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
/// \file LaminaSD.cc
/// \brief Implementation of the LaminaSD class

#include "LaminaSD.hh"
#include "Constants.hh"
#include "EventAction.hh"

#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4LossTableManager.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4ProcessType.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  //****************************************************************************
  // Constructor
  //****************************************************************************
  LaminaSD::LaminaSD(G4String name)
   : G4VSensitiveDetector(name)
  {
    collectionName.insert( "LaminaCollection" );
  }

  //****************************************************************************
  // Destructor
  //****************************************************************************
  LaminaSD::~LaminaSD()
  {}

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void LaminaSD::Initialize(G4HCofThisEvent* hce)
  {
    // Create hits collection
    fHitsCollection = new LaminaHitsCollection( SensitiveDetectorName, collectionName[0] );

    // Add this collection in hce
    if (fHCID<0) {
      fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    }
  
    hce->AddHitsCollection(fHCID,fHitsCollection);

  }

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  G4bool LaminaSD::ProcessHits(G4Step* step, G4TouchableHistory*)
  {  

    auto track = step->GetTrack();
    auto preStepPoint = step->GetPreStepPoint();
    auto postStepPoint = step->GetPostStepPoint();

    // check if gamma and has process defined step
    //// photon processes (phot,comp,conv) happen at the end of the step
    // if ( track->GetParticleDefinition() == G4Gamma::Definition() && postStepPoint->GetProcessDefinedStep() ) {
    if ( track->GetParticleDefinition() == G4Gamma::Definition() ) {
      auto preTouch = preStepPoint->GetTouchable();
      auto postTouch = postStepPoint->GetTouchable();
 
      // if ( postStepPoint->GetProcessDefinedStep()->GetProcessType() == 2 ) { // electromagnetic process
      auto edep = -1 * ( postStepPoint->GetKineticEnergy() - preStepPoint->GetKineticEnergy() );
      if ( edep > 0. ) {
        auto hit = new LaminaHit();

        // get track ID
        auto trackID = track->GetTrackID();
        hit->SetTrackID( trackID );

        // get copy number 
        // This is currently wrong, need mother and grandmother
        auto copyNo = preTouch->GetCopyNumber(0);
        hit->SetCopyNo( copyNo );

        // get global time of hit
        auto hitTime = postStepPoint->GetGlobalTime();
        hit->SetTime(hitTime);

        // get the process name
        auto procName = postStepPoint->GetProcessDefinedStep()->GetProcessName();
        hit->SetProc( procName );
        
        // get global position
        G4AffineTransform transform = postTouch->GetHistory()->GetTopTransform();
        transform.Invert();
        // hit->SetRot(transform.NetRotation());
        hit->SetPos( transform.NetTranslation() );

        // get energy deposited
        // auto edep = step->GetTotalEnergyDeposit();
        hit->SetEdep(edep);

        // insert the hit
        fHitsCollection->insert(hit);

        return true;
      } 
      else return false;
      
    } 
    else return false;
  }

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void LaminaSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
  {}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}

