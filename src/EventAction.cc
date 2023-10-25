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
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace {
  //****************************************************************************
  // Utility function which finds a hit collection with the given Id
  // and print warnings if not found
  //****************************************************************************
  G4VHitsCollection* GetHitCollection(const G4Event* event, G4int hcID ) 
  {
    auto hce = event->GetHCofThisEvent();
    if (!hce) {
        G4ExceptionDescription msg;
        msg << "No hits collection of this event found." << G4endl;
        G4Exception("EventAction::GetHitsCollection()",
                    "Code001", JustWarning, msg);
        return nullptr;
    }
    
    auto hc = hce->GetHC(hcID);
    if ( ! hc) {
      G4ExceptionDescription msg;
      msg << "Cannot access hitsCollection ID " << hcID; 
      G4Exception("EventAction::GetHitsCollection()",
        "Code0001", JustWarning, msg);
    }         

    return hc;
  }    
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  //****************************************************************************
  // Constructor
  //****************************************************************************
  EventAction::EventAction()
  {}

  //****************************************************************************
  // Destructor
  //****************************************************************************
  EventAction::~EventAction()
  {}

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void EventAction::BeginOfEventAction(const G4Event* /*event*/)
  {}

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void EventAction::EndOfEventAction(const G4Event* event)
  {  
    auto sdManager = G4SDManager::GetSDMpointer();

    // Get hits collections IDs (only once)
    if ( fLaminaHCID == -1 ) {
      fLaminaHCID = sdManager->GetCollectionID("LMCP_Det/LaminaCollection");
    }
    if ( fPoreHCID == -1 ) {
      fPoreHCID = sdManager->GetCollectionID("PORE_Det/PoreCollection");
    }

    // Get hits collection
    auto laminaHC = GetHitCollection( event, fLaminaHCID );
    if ( ! laminaHC ) return;
    auto poreHC = GetHitCollection( event, fPoreHCID );
    if ( ! poreHC ) return;

    // get number of hits
    auto nLaminaHits = laminaHC->GetSize();
    auto nPoreHits = poreHC->GetSize();
    auto nHits = nPoreHits + nLaminaHits;
    // G4cout << "Lamina hits: " << nLaminaHits << G4endl;
    // G4cout << "Pore hits: " << nPoreHits << G4endl;

    // Check if there were any hits in instrument
    if ( nHits > 0 ){
      // get analysis manager
      auto analysisManager = G4AnalysisManager::Instance();
      auto tuple = 1; // ntuple number
      auto col = 0; // ntuple column number

      if ( nLaminaHits > 0 ) {
        tuple = 1;
        col = 0;

        // Fill Lamina Ntuple
        analysisManager->FillNtupleIColumn(tuple, col++, event->GetEventID());
        // G4cout << "Event ID: " << event->GetEventID() << G4endl;

        // auto primaryE = event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy()/keV;
        // analysisManager->FillNtupleDColumn(col++, primaryE);

        // Add number of hits
        // analysisManager->FillNtupleIColumn(0, col++, nLaminaHits);

        // Data for each hit element
        fLaminaID = std::vector<G4int>(nLaminaHits, NAN);
        fLaminaEdep = std::vector<G4double>(nLaminaHits, NAN);
        fLaminaTime = std::vector<G4double>(nLaminaHits, NAN);
        for (unsigned long i=0; i<nLaminaHits; i++ ) {
            auto hit = static_cast<LaminaHit*>(laminaHC->GetHit(i));

            fLaminaID[i] = hit->GetCopyNo();
            fLaminaEdep[i] = hit->GetEdep()/keV;
            fLaminaTime[i] = hit->GetTime()/ns;

            // G4cout << "    Track ID: " << hit->GetTrackID() << G4endl;
            // G4cout << "    Proc: " << hit->GetProc() << G4endl;
            // G4cout << "    Position: " << hit->GetPos()/mm << G4endl;
            // G4cout << "    Edep: " << hit->GetEdep()/keV << " keV" << G4endl;
            // G4cout << "    Time: " << hit->GetTime()/ns << " ns" << G4endl;
            // G4cout << "    Copy No: " << hit->GetCopyNo() << G4endl;

        }

        // Finish event
        analysisManager->AddNtupleRow(tuple);  
      }

      if ( nPoreHits > 0 ) {
        tuple = 2;
        col = 0;

        // Fill Pore Ntuple
        analysisManager->FillNtupleIColumn(tuple, col++, event->GetEventID());

        // auto primaryE = event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy()/keV;
        // analysisManager->FillNtupleDColumn(col++, primaryE);

        // Add number of hits
        // analysisManager->FillNtupleIColumn(1, col++, nPoreHits);

        // Data for each hit element
        fPoreID = std::vector<G4int>(nPoreHits, NAN);
        fPoreCreatorProc = std::vector<std::string>(nPoreHits, "");
        fPoreEdep = std::vector<G4double>(nPoreHits, NAN);
        fPoreEKin = std::vector<G4double>(nPoreHits, NAN);
        fPoreTime = std::vector<G4double>(nPoreHits, NAN);
        fPorePosX = std::vector<G4double>(nPoreHits, NAN);
        fPorePosY = std::vector<G4double>(nPoreHits, NAN);
        fPorePosZ = std::vector<G4double>(nPoreHits, NAN);
        for (unsigned long i=0; i<nPoreHits; i++ ) {
            auto hit = static_cast<PoreHit*>(poreHC->GetHit(i));

            fPoreID[i] = hit->GetCopyNo();
            fPoreCreatorProc[i] = hit->GetCreatorProc();
            fPoreEdep[i] = hit->GetEdep()/keV;
            fPoreEKin[i] = hit->GetEKin()/keV;
            fPoreTime[i] = hit->GetTime()/ns;
            fPorePosX[i] = hit->GetLocalPos().x()/mm;
            fPorePosY[i] = hit->GetLocalPos().y()/mm;
            fPorePosZ[i] = hit->GetLocalPos().z()/mm;

            // G4cout << "    Track ID: " << hit->GetTrackID() << G4endl;
            // G4cout << "    Proc: " << hit->GetProc() << G4endl;
            // G4cout << "    Position: " << hit->GetLocalPos()/mm << G4endl;
            // G4cout << "    Global " << hit->GetGlobalPos()/mm << G4endl;
            // G4cout << "    Edep: " << hit->GetEdep()/keV << " keV" << G4endl;
            // G4cout << "    Time: " << hit->GetTime()/ns << " ns" << G4endl;
            // G4cout << "    Copy No: " << hit->GetCopyNo() << G4endl;

        }

        // Finish event
        analysisManager->AddNtupleRow(tuple);  
      }

    }

  }  
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
