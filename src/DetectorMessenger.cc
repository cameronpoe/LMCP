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

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp {

//****************************************************************************
// Constructor
//****************************************************************************
DetectorMessenger::DetectorMessenger(DetectorConstruction *det)
    : fDetectorConstruction(det) {
  fUSERDir = new G4UIdirectory("/user/");
  fUSERDir->SetGuidance("UI commands specific to this example.");

  fDetDir = new G4UIdirectory("/user/det/");
  fDetDir->SetGuidance("detector control.");

  fOverlapCmd = new G4UIcmdWithABool("/user/det/setOverlapChecking", this);

  fOverlapCmd->SetParameterName("flag", false);
  fOverlapCmd->SetDefaultValue(true);
  fOverlapCmd->AvailableForStates(G4State_PreInit);

  fSlabDimensionsCmd =
      new G4UIcmdWith3VectorAndUnit("/user/det/setSlabDimensions", this);
  fSlabDimensionsCmd->SetGuidance("Sets length of LMCP slab in x, y, and z.");
  fSlabDimensionsCmd->SetParameterName("dimX", "dimY", "dimZ", false);
  fSlabDimensionsCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fPoreDimensionsCmd =
      new G4UIcmdWith3VectorAndUnit("/user/det/setPoreDimensions", this);
  fPoreDimensionsCmd->SetGuidance("Sets length of pore in x, y, and z.");
  fPoreDimensionsCmd->SetParameterName("dimX", "dimY", "dimZ", false);
  fPoreDimensionsCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fWallXCmd = new G4UIcmdWithADoubleAndUnit("/user/det/setWallX", this);
  fWallXCmd->SetGuidance(
      "Sets the thickness of pore wall (substrate between pores).");
  fWallXCmd->SetParameterName("thickness", false);
  fWallXCmd->SetUnitCategory("Length");
  fWallXCmd->SetRange("thickness>0.0");
  fWallXCmd->SetDefaultValue(0.1524);
  fWallXCmd->SetDefaultUnit("mm");
  fWallXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fWallYCmd = new G4UIcmdWithADoubleAndUnit("/user/det/setWallY", this);
  fWallYCmd->SetGuidance(
      "Sets the thickness of pore wall (substrate between pores).");
  fWallYCmd->SetParameterName("thickness", false);
  fWallYCmd->SetUnitCategory("Length");
  fWallYCmd->SetRange("thickness>0.0");
  fWallYCmd->SetDefaultValue(0.1524);
  fWallYCmd->SetDefaultUnit("mm");
  fWallYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fUpdateCmd = new G4UIcmdWithoutParameter("/user/det/update", this);
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
  delete fWallXCmd;
  delete fWallYCmd;
  delete fPoreDimensionsCmd;
  delete fSlabDimensionsCmd;
  delete fOverlapCmd;
  delete fDetDir;
  delete fUSERDir;
}

//****************************************************************************
// Method from base class
//****************************************************************************
void DetectorMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
  if (command == fOverlapCmd) {
    fDetectorConstruction->SetOverlapFlag(
        fOverlapCmd->GetNewBoolValue(newValue));
  } else if (command == fSlabDimensionsCmd) {
    fDetectorConstruction->SetSlabDimensions(
        fSlabDimensionsCmd->GetNew3VectorValue(newValue));
  } else if (command == fPoreDimensionsCmd) {
    fDetectorConstruction->SetPoreDimensions(
        fPoreDimensionsCmd->GetNew3VectorValue(newValue));
  } else if (command == fWallXCmd) {
    fDetectorConstruction->SetWallX(fWallXCmd->GetNewDoubleValue(newValue));
  } else if (command == fWallYCmd) {
    fDetectorConstruction->SetWallY(fWallYCmd->GetNewDoubleValue(newValue));
  } else if (command == fUpdateCmd) {
    fDetectorConstruction->UpdateGeometry();
  }
}

} // namespace lmcp
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
