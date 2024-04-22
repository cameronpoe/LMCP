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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "EventAction.hh"
// #include "Constants.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Timer.hh"
// #include "G4UIcommand.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  //****************************************************************************
  // Constructor
  //****************************************************************************
  RunAction::RunAction( EventAction* eventAct )
   : fEventAction( eventAct )
  { 
    // set printing event number per each event
    G4RunManager::GetRunManager()->SetPrintProgress(100);     

    // Create analysis manager
    // The choice of the output format is done via the specified
    // file extension.
    auto analysisManager = G4AnalysisManager::Instance();


    // Create directories
    //
    //analysisManager->SetHistoDirectoryName("histograms");
    //analysisManager->SetNtupleDirectoryName("ntuple");

    // Default Settings
    //
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetDefaultFileType("root");
      // Note: If the filename extension is not provided, the default file 
      // type (root) will be used for all files specified without extension.
    analysisManager->SetNtupleMerging(true);
      // Note: merging ntuples is available only with Root output

    // Default output file
    //
    G4String fileName = "data/Test_ntuple.root";
    // Other supported output types:
    // G4String fileName = "data/Test_ntuple.csv";
    // G4String fileName = "data/Test_ntuple.hdf5";
    // G4String fileName = "data/Test_ntuple.xml";
    analysisManager->SetFileName(fileName);

    analysisManager->SetFirstNtupleId(1);

    //Declare ntuples
    //
    // Create 1st ntuple (id = 1)
    //
    analysisManager->CreateNtuple("lamina", "Lamina Data");
    analysisManager->CreateNtupleIColumn("EventNumber");
    analysisManager->CreateNtupleIColumn("TrackID", fEventAction->GetLaminaTrackID());
    analysisManager->CreateNtupleIColumn("ID", fEventAction->GetLaminaID());
    analysisManager->CreateNtupleDColumn("Edep", fEventAction->GetLaminaEdep());
    analysisManager->CreateNtupleDColumn("EKin", fEventAction->GetLaminaEKin());
    analysisManager->CreateNtupleDColumn("Time", fEventAction->GetLaminaTime());
    analysisManager->FinishNtuple();

    //
    // Create 2nd ntuple (id = 2)
    //
    analysisManager->CreateNtuple("pore", "Pore Data");
    analysisManager->CreateNtupleIColumn("EventNumber");
    analysisManager->CreateNtupleIColumn("TrackID", fEventAction->GetPoreTrackID());
    analysisManager->CreateNtupleIColumn("ID", fEventAction->GetPoreID());
    analysisManager->CreateNtupleIColumn("PDGID", fEventAction->GetPDGID());
    analysisManager->CreateNtupleSColumn("CreatorProc", fEventAction->GetPoreCreatorProc());
    analysisManager->CreateNtupleDColumn("Edep", fEventAction->GetPoreEdep());
    analysisManager->CreateNtupleDColumn("EKin", fEventAction->GetPoreEKin());
    analysisManager->CreateNtupleDColumn("Time", fEventAction->GetPoreTime());
    analysisManager->CreateNtupleDColumn("PosX", fEventAction->GetPorePosX());
    analysisManager->CreateNtupleDColumn("PosY", fEventAction->GetPorePosY());
    analysisManager->CreateNtupleDColumn("PosZ", fEventAction->GetPorePosZ());
    analysisManager->FinishNtuple();

    // Create run timer
    if (isMaster) {
      fTimer = new G4Timer;
    }
  }

  //****************************************************************************
  // Destructor
  //****************************************************************************
  RunAction::~RunAction()
  {
    delete fTimer;
  }

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void RunAction::BeginOfRunAction(const G4Run* /*run*/)
  { 
    // start timer
    //
    if (isMaster) {
      fTimer->Start();
    }

    // inform the runManager to save random number seed
    // G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    
    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Open an output file
    // The default file name is set in RunAction::RunAction(),
    // it can be overwritten in a macro
    analysisManager->OpenFile();

  }

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void RunAction::EndOfRunAction(const G4Run* /*run*/)
  {
    auto analysisManager = G4AnalysisManager::Instance();

    // save histograms & ntuple
    //
    analysisManager->Write();
    analysisManager->CloseFile();

    // stop timer and print stats
    //
    if (isMaster) {
      fTimer->Stop();
      G4cout << G4endl;
      // G4cout << "  Events: " << iEvents << G4endl;
      // G4cout << "  Hits: " << iHits << G4endl;
      G4cout << "### Complete ###" << G4endl;
      G4cout << "  Timer: " << fTimer->GetRealElapsed() << " s" << G4endl;
      G4cout << G4endl;
    }

  }

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
