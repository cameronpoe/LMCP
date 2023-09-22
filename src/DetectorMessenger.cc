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
/// \file DetectorMessenger.cc
/// \brief Definition of the DetectorMessenger class

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  //****************************************************************************
  // Constructor
  //****************************************************************************
  DetectorMessenger::DetectorMessenger( DetectorConstruction* det )
  : fDetectorConstruction(det)
  { 
    fUSERDir = new G4UIdirectory( "/user/" );
    fUSERDir -> SetGuidance( "UI commands specific to this example." );
    
    fDetDir = new G4UIdirectory( "/user/det/" );
    fDetDir -> SetGuidance( "detector control." );
    
    fOverlapCmd = new G4UIcmdWithABool("/user/det/setOverlapChecking",this);  
    fOverlapCmd->SetGuidance("Set overlap checking flag.");
    fOverlapCmd->SetParameterName("flag",false);
    fOverlapCmd->SetDefaultValue(true);
    fOverlapCmd->AvailableForStates(G4State_PreInit);

  }

  //****************************************************************************
  // Destructor
  //****************************************************************************
  DetectorMessenger::~DetectorMessenger() {

    delete fOverlapCmd;
    delete fDetDir;
    delete fUSERDir;
  }

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void DetectorMessenger::SetNewValue( G4UIcommand* command,G4String newValue) 
  {
    if ( command == fOverlapCmd )
        { fDetectorConstruction->SetOverlapFlag( fOverlapCmd->GetNewBoolValue(newValue)); }
    }

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%