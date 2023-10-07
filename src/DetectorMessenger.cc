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
#include "G4UIcmdWith3VectorAndUnit.hh"

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

    fSlabDimensionsCmd = new G4UIcmdWith3VectorAndUnit("/user/det/setSlabDimensions",this);
    fSlabDimensionsCmd->SetGuidance("Sets length of LMCP slab in x, y, and z.");
    fSlabDimensionsCmd->SetParameterName("dimX", "dimY", "dimZ", false);
    fSlabDimensionsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fPoreDimensionsCmd = new G4UIcmdWith3VectorAndUnit("/user/det/setPoreDimensions",this);
    fPoreDimensionsCmd->SetGuidance("Sets length of pore in x, y, and z.");
    fPoreDimensionsCmd->SetParameterName("dimX", "dimY", "dimZ", false);
    fPoreDimensionsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fWallThicknessCmd = new G4UIcmdWithADoubleAndUnit("/user/det/setWallThickness",this);
    fWallThicknessCmd->SetGuidance("Sets the thickness of pore wall (substrate between pores).");
    fWallThicknessCmd->SetParameterName("thickness", false);
    fWallThicknessCmd->SetUnitCategory("Length");
    fWallThicknessCmd->SetRange("thickness>0.0");
    fWallThicknessCmd->SetDefaultValue(0.1524);
    fWallThicknessCmd->SetDefaultUnit("mm");
    fWallThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fUpdateCmd = new G4UIcmdWithoutParameter("/user/det/update",this);
    fUpdateCmd->SetGuidance("Update geometry.");
    fUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
    fUpdateCmd->SetGuidance("if you changed geometrical value(s).");
    fUpdateCmd->AvailableForStates(G4State_Idle);
  }

  //****************************************************************************
  // Destructor
  //****************************************************************************
  DetectorMessenger::~DetectorMessenger() {

    delete fUpdateCmd;
    delete fWallThicknessCmd;
    delete fPoreDimensionsCmd;
    delete fSlabDimensionsCmd;
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
    else if ( command == fSlabDimensionsCmd )
        { fDetectorConstruction->SetSlabDimensions( fSlabDimensionsCmd->GetNew3VectorValue(newValue));}
    else if ( command == fPoreDimensionsCmd )
        { fDetectorConstruction->SetPoreDimensions( fPoreDimensionsCmd->GetNew3VectorRawValue(newValue));}
    else if ( command == fWallThicknessCmd )
        { fDetectorConstruction->SetWallThickness( fWallThicknessCmd->GetNewDoubleValue(newValue));}
    else if ( command == fUpdateCmd ) 
        { fDetectorConstruction->UpdateGeometry();}
  }
    

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%