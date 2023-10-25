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
/// \file PoreSD.cc
/// \brief Implementation of the PoreSD class

#include "PoreSD.hh"
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

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  //****************************************************************************
  // Constructor
  //****************************************************************************
  PoreSD::PoreSD(G4String name)
   : G4VSensitiveDetector(name)
  {
    collectionName.insert( "PoreCollection" );
  }

  //****************************************************************************
  // Destructor
  //****************************************************************************
  PoreSD::~PoreSD()
  {}

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void PoreSD::Initialize(G4HCofThisEvent* hce)
  {
    // Create hits collection
    fHitsCollection = new PoreHitsCollection( SensitiveDetectorName, collectionName[0] );

    // Add this collection in hce
    if (fHCID<0) {
      fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    }
  
    hce->AddHitsCollection(fHCID,fHitsCollection);
  }

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  G4bool PoreSD::ProcessHits(G4Step* step, G4TouchableHistory*)
  {  

    auto track = step->GetTrack();
    auto preStepPoint = step->GetPreStepPoint();
    auto postStepPoint = step->GetPostStepPoint();
    auto preTouch = preStepPoint->GetTouchable();
    auto postTouch = postStepPoint->GetTouchable();

    // if ( track->GetParticleDefinition() == G4Electron::Definition() ) {
        // if ( step->IsFirstStepInVolume() ) {
        //   G4cout << "***ENTERING" << G4endl;
        //   if (preStepPoint->GetStepStatus() == postStepPoint->GetStepStatus())G4cout << "    TRUE" << G4endl;
        //   G4cout << "    Particle: " << track->GetParticleDefinition()->GetParticleName() << G4endl;
        //   G4cout << "    Track ID: " << track->GetTrackID() << G4endl;
        //   G4cout << "    PreStep Vol: " << preStepPoint->GetTouchable()->GetVolume()->GetName() << G4endl;
        //   G4cout << "    PostStep Vol: " << postStepPoint->GetTouchable()->GetVolume()->GetName() << G4endl;
        // }
        // else if (postStepPoint->GetStepStatus() == fGeomBoundary ) {
        //   G4cout << "***EXITING" << G4endl;
        //   if (preStepPoint->GetStepStatus() == postStepPoint->GetStepStatus())G4cout << "    TRUE" << G4endl;
        //   G4cout << "    " << preStepPoint->GetTouchable()->GetVolume()->GetName() << G4endl;
        //   G4cout << "    " << postStepPoint->GetTouchable()->GetVolume()->GetName() << G4endl;
        // }
        // else {
        //   G4cout << "***OTHER" << G4endl;
        //   if (preStepPoint->GetStepStatus() == postStepPoint->GetStepStatus())G4cout << "    TRUE" << G4endl;
        //   G4cout << "    Particle: " << track->GetParticleDefinition()->GetParticleName() << G4endl;
        //   G4cout << "    Track ID: " << track->GetTrackID() << G4endl;
        //   G4cout << "    PreStep Vol: " << preStepPoint->GetTouchable()->GetVolume()->GetName() << G4endl;
        //   G4cout << "    PostStep Vol: " << postStepPoint->GetTouchable()->GetVolume()->GetName() << G4endl;

        // }
          // G4cout << "***OTHER" << G4endl;
          // if (preStepPoint->GetStepStatus() == postStepPoint->GetStepStatus())G4cout << "    TRUE" << G4endl;
          // G4cout << "    Particle: " << track->GetParticleDefinition()->GetParticleName() << G4endl;
          // G4cout << "    Track ID: " << track->GetTrackID() << G4endl;
          // G4cout << "    PreStep Vol: " << preStepPoint->GetTouchable()->GetVolume()->GetName() << G4endl;
          // G4cout << "    PostStep Vol: " << postStepPoint->GetTouchable()->GetVolume()->GetName() << G4endl;
          // auto preCoord = preStepPoint->GetPosition();
          // G4AffineTransform preTransform = preTouch->GetHistory()->GetTopTransform();
          // G4cout << "    PreStep Pos: " << preTransform.TransformPoint(preCoord) << G4endl;
          // auto postCoord = postStepPoint->GetPosition();
          // G4AffineTransform postTransform = postTouch->GetHistory()->GetTopTransform();
          // G4cout << "    PostStep Pos: " << postTransform.TransformPoint(postCoord) << G4endl;


      // }

    // check if electrons hitting the wall
    if ( track->GetParticleDefinition() == G4Electron::Definition() ) {

        auto hit = new PoreHit();

        // get track ID
        auto trackID = track->GetTrackID();
        hit->SetTrackID( trackID );

        // get copy number 
        // This is currently wrong, need mother and grandmother
        auto copyNo = preTouch->GetCopyNumber(0);
        hit->SetCopyNo( copyNo );

        // get track's creator process
        auto trackCreatorProcess = track->GetCreatorProcess()->GetProcessName();
        hit->SetCreatorProc( trackCreatorProcess );

        // get global time of hit
        auto hitTime = postStepPoint->GetGlobalTime();
        hit->SetTime(hitTime);

        // get the process name
        auto procName = postStepPoint->GetProcessDefinedStep()->GetProcessName();
        hit->SetProc( procName );
        
        // get global position
        G4AffineTransform transform1 = postTouch->GetHistory()->GetTopTransform();
        transform1.Invert();
        // hit->SetRot(transform1.NetRotation());
        hit->SetGlobalPos( transform1.NetTranslation() );

        // get local position
        auto coord = postStepPoint->GetPosition();
        G4AffineTransform transform2 = postTouch->GetHistory()->GetTopTransform();
        auto localPosition = transform2.TransformPoint(coord);
        hit->SetLocalPos( localPosition );

        // get energy deposited
        // auto edep = step->GetTotalEnergyDeposit();
        auto edep = postStepPoint->GetKineticEnergy() - preStepPoint->GetKineticEnergy();
        hit->SetEdep(edep);

        auto e_kin = postStepPoint->GetKineticEnergy();
        hit->SetEKin(e_kin);


        // insert the hit
        fHitsCollection->insert(hit);

        track->SetTrackStatus(fStopAndKill); 

        return true;
    } 
    else return false;
  }

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void PoreSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
  {}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}

