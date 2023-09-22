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
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include "G4EqMagElectricField.hh"
#include "G4UniformElectricField.hh"

class G4FieldManager;
class G4ChordFinder;
class G4EquationOfMotion;
class G4Mag_EqRhs;
class G4EqMagElectricField;
class G4MagIntegratorStepper;
class G4MagInt_Driver;

class G4VPhysicalVolume;
class G4LogicalVolume;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  class DetectorMessenger;

  //****************************************************************************
  /// Detector construction class to define materials and geometry.
  ///
  //****************************************************************************
  class DetectorConstruction : public G4VUserDetectorConstruction
  {
    public:
      DetectorConstruction();
      ~DetectorConstruction() override;

      // methods from base class
      G4VPhysicalVolume* Construct() override;
      // G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
      void ConstructSDandField() override;
      
      // set methods
      void SetOverlapFlag( G4bool flag ) {fCheckOverlaps = flag;};          

      //Setup the required objects of the FIRST local field
      G4ElectricField*        fEMfield_1;
      G4EqMagElectricField*   fEquation_1;
      G4MagIntegratorStepper* fStepper_1;
      G4FieldManager*         fFieldMgr_1;
      G4double                fMinStep_1;
      G4ChordFinder*          fChordFinder_1;

      //Setup the required objects of the SECOND local field
      G4ElectricField*        fEMfield_2;
      G4EqMagElectricField*   fEquation_2;
      G4MagIntegratorStepper* fStepper_2;
      G4FieldManager*         fFieldMgr_2;
      G4double                fMinStep_2;
      G4ChordFinder*          fChordFinder_2;

      //world electric field
      G4ElectricField*        fEMfield_WORLD;
      G4EqMagElectricField*   fEquation_WORLD;
      G4MagIntegratorStepper* fStepper_WORLD;
      G4FieldManager*         fFieldMgr_WORLD;
      G4double                fMinStep_WORLD;
      G4ChordFinder*          fChordFinder_WORLD;

    private:
      // methods
      void DefineMaterials();
      G4VPhysicalVolume* DefineVolumes();

      DetectorMessenger* fDetMessenger = nullptr;   // messenger

      G4bool fCheckOverlaps = false; 

      // G4LogicalVolume* fScoringVolume = nullptr;
  };
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif

