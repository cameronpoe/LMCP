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

#include <G4UIcmdWithAString.hh>
#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWith3VectorAndUnit;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp {

class DetectorConstruction;

//****************************************************************************
/// Detector Messenger Class
///
//****************************************************************************
class DetectorMessenger : public G4UImessenger {
public:
  DetectorMessenger(DetectorConstruction *);
  ~DetectorMessenger() override;

  // methods from base class
  void SetNewValue(G4UIcommand *, G4String) override;

private:
  // data members
  DetectorConstruction *fDetectorConstruction = nullptr;

  G4UIdirectory *fUSERDir = nullptr;
  G4UIdirectory *fDetDir = nullptr;

  G4UIcmdWithABool *fOverlapCmd = nullptr;
  G4UIcmdWith3VectorAndUnit *fSlabDimensionsCmd = nullptr;
  G4UIcmdWith3VectorAndUnit *fPoreDimensionsCmd = nullptr;
  G4UIcmdWithADoubleAndUnit *fWallXCmd = nullptr;
  G4UIcmdWithADoubleAndUnit *fWallYCmd = nullptr;
  // G4UIcmdWithAString *fMaterialCmd = nullptr;
  G4UIcmdWithoutParameter *fUpdateCmd = nullptr;
};

} // namespace lmcp
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif
