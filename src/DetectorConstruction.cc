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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "LaminaSD.hh"
#include "PoreSD.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4UImanager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4AutoDelete.hh"

#include "G4UserLimits.hh"
#include "G4SDManager.hh"

#include "G4UniformElectricField.hh"
#include "G4UniformMagField.hh"
#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4EquationOfMotion.hh"
#include "G4EqMagElectricField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"

#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4DormandPrince745.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{
  // G4ThreadLocal SensitiveDetector* DetectorConstruction::SD = nullptr;
  G4ThreadLocal LaminaSD* DetectorConstruction::fLMCP_SD = nullptr;
  G4ThreadLocal PoreSD* DetectorConstruction::fPORE_SD = nullptr;


  //****************************************************************************
  // Constructor
  //****************************************************************************
  DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
  fEMfield_1(0),
  fEquation_1(0),
  fStepper_1(0),
  fFieldMgr_1(0),
  fMinStep_1(0),
  fChordFinder_1(0),
  fEMfield_2(0),
  fEquation_2(0),
  fStepper_2(0),
  fFieldMgr_2(0),
  fMinStep_2(0),
  fChordFinder_2(0),
  fEMfield_WORLD(0),
  fEquation_WORLD(0),
  fStepper_WORLD(0),
  fFieldMgr_WORLD(0),
  fMinStep_WORLD(0),
  fChordFinder_WORLD(0)
  {
    // Define materials 
    DefineMaterials();

    // G4GeometryManager::GetInstance()->SetWorldMaximumExtent(WorldExtent);
    
    // create commands for interactive definition of the geometry
    fDetMessenger = new DetectorMessenger(this);
  }

  //****************************************************************************
  // Destructor
  //****************************************************************************
  DetectorConstruction::~DetectorConstruction()
  {
    delete fDetMessenger;
  }  

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  G4VPhysicalVolume* DetectorConstruction::Construct()
  {
    // Define volumes
    return DefineVolumes();
  }

  //****************************************************************************
  // Method
  //****************************************************************************
  void DetectorConstruction::DefineMaterials()
  { 
    // Materials defined using NIST Manager
    // https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html
    auto nistManager = G4NistManager::Instance();

    //======================================================
    // define elements
    //======================================================
    auto isotopes = true;
    G4Element* eH  = nistManager -> FindOrBuildElement( "H" , isotopes ); // 1
    // G4Element* eBe = nistManager -> FindOrBuildElement( "Be", isotopes ); // 4
    G4Element* eB  = nistManager -> FindOrBuildElement( "B" , isotopes );  // 5
    G4Element* eC  = nistManager -> FindOrBuildElement( "C" , isotopes ); // 6
    G4Element* eO  = nistManager -> FindOrBuildElement( "O" , isotopes ); // 8
    // G4Element* eF  = nistManager -> FindOrBuildElement( "F" , isotopes ); // 9
    G4Element* eNa = nistManager -> FindOrBuildElement( "Na", isotopes ); // 11
    G4Element* eMg = nistManager -> FindOrBuildElement( "Mg", isotopes ); // 12
    G4Element* eAl = nistManager -> FindOrBuildElement( "Al", isotopes ); // 13
    G4Element* eSi = nistManager -> FindOrBuildElement( "Si", isotopes ); // 14
    G4Element* eTi = nistManager -> FindOrBuildElement( "Ti", isotopes ); // 22
    G4Element* eCr = nistManager -> FindOrBuildElement( "Cr", isotopes ); // 24
    G4Element* eMn = nistManager -> FindOrBuildElement( "Mn", isotopes ); // 25
    G4Element* eFe = nistManager -> FindOrBuildElement( "Fe", isotopes ); // 26
    G4Element* eCu = nistManager -> FindOrBuildElement( "Cu", isotopes ); // 29
    G4Element* eZn = nistManager -> FindOrBuildElement( "Zn", isotopes ); // 30
    // G4Element* eGa = nistManager -> FindOrBuildElement( "Ga", isotopes ); // 31
    G4Element* eSn = nistManager -> FindOrBuildElement( "Sn", isotopes ); // 50
    // G4Element* eI  = nistManager -> FindOrBuildElement( "I" , isotopes ); // 53
    // G4Element* eCs = nistManager -> FindOrBuildElement( "Cs", isotopes ); // 55
    G4Element* eW  = nistManager -> FindOrBuildElement( "W" , isotopes ); // 74
    G4Element* ePb = nistManager -> FindOrBuildElement( "Pb", isotopes ); // 82

    //======================================================
    // define materials
    //======================================================
    // Vacuum ( Nist G4_Galactic )
    nistManager -> FindOrBuildMaterial("G4_Galactic");
    nistManager -> FindOrBuildMaterial("G4_AIR");

    // Elements as materials
    nistManager -> FindOrBuildMaterial("G4_Pb");

    // Pb-Glass
    nistManager -> FindOrBuildMaterial("G4_GLASS_LEAD");

    // Schott B33 glass
    G4Material* eSiO2  = nistManager -> FindOrBuildMaterial( "G4_SILICON_DIOXIDE" , isotopes );
    G4Material* eB2O3  = nistManager -> FindOrBuildMaterial( "G4_BORON_OXIDE" , isotopes );
    G4Material* eNa2O  = nistManager -> FindOrBuildMaterial( "G4_SODIUM_MONOXIDE" , isotopes );
    G4Material* eAl2O3 = nistManager -> FindOrBuildMaterial( "G4_ALUMINUM_OXIDE" , isotopes );
    auto mB33 = new G4Material( "B33", 2.23*g/cm3, 4 );
      mB33->AddMaterial( eSiO2, 81*perCent);
      mB33->AddMaterial( eB2O3, 13*perCent);
      mB33->AddMaterial( eNa2O,  4*perCent);
      mB33->AddMaterial( eAl2O3, 2*perCent);

    // Peek chemical formula (C19H12O3)
    //  C 114/150 = 76
    //  H 12/150  = 8
    //  O 24/150  = 16
    auto mPeek = new G4Material( "PEEK", 1.31*g/cm3, 3 );
      mPeek->AddElement( eC, 76*perCent);
      mPeek->AddElement( eH,  8*perCent);
      mPeek->AddElement( eO, 16*perCent);
     
    // Ecomass
    // PEEK with W nano particles
    // 90% W by weight
    auto mECOMASS = new G4Material("ECOMASS", 11.*g/cm3, 2 );
      mECOMASS->AddMaterial( mPeek,  10*perCent);
      mECOMASS->AddElement( eW, 90*perCent);
     
     // ALUMINUM 6061
    auto mAl6061 = new G4Material( "Aluminum6061", 2.6989*g/cm3, 9 );
      mAl6061 -> AddElement( eSi,  0.60*perCent );
      mAl6061 -> AddElement( eFe,  0.70*perCent );
      mAl6061 -> AddElement( eCu,  0.30*perCent );
      mAl6061 -> AddElement( eMn,  0.15*perCent );
      mAl6061 -> AddElement( eMg,  1.00*perCent );
      mAl6061 -> AddElement( eCr,  0.20*perCent );
      mAl6061 -> AddElement( eZn,  0.25*perCent );
      mAl6061 -> AddElement( eTi,  0.15*perCent );
      mAl6061 -> AddElement( eAl, 96.65*perCent );

    // PC Baord 80% G10 circuit board, 20% generic electronics
    auto mPCB = new G4Material( "PCB", 1.8*g/cm3, 9 );
      mPCB -> AddElement( eH ,  3.0*perCent );
      mPCB -> AddElement( eC , 17.4*perCent );
      mPCB -> AddElement( eO , 39.2*perCent );
      mPCB -> AddElement( eAl, 10.0*perCent );
      mPCB -> AddElement( eSi, 24.4*perCent );
      mPCB -> AddElement( eFe,  1.0*perCent );
      mPCB -> AddElement( eCu,  3.0*perCent );
      mPCB -> AddElement( eSn,  1.0*perCent );
      mPCB -> AddElement( ePb,  1.0*perCent );

    //------------------------------------------------------
    // Print materials list
    //------------------------------------------------------
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  }

  //****************************************************************************
  // Method
  //****************************************************************************
  G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
  {
    //======================================================
    // Cleanup old geometry
    //======================================================
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    //======================================================
    // Materials
    //======================================================
    auto mVacuum = G4Material::GetMaterial("G4_Galactic");
    // auto mAir = G4Material::GetMaterial("G4_AIR");

    // auto mAl6061 = G4Material::GetMaterial( "Aluminum6061" );
    // auto mPCB = G4Material::GetMaterial( "PCB" );

    auto mLMCP = G4Material::GetMaterial( "G4_GLASS_LEAD" );
    // auto mLMCP = G4Material::GetMaterial( "B33" );
    // auto mLMCP = G4Material::GetMaterial( "PEEK" );
    // auto mLMCP = G4Material::GetMaterial( "ECOMASS" );

    //======================================================
    // Volumes
    //======================================================
    //------------------------------------------------------
    // Calculating dimensions
    //------------------------------------------------------
    auto numPores_X = static_cast<int>(fSlabDimensions[0]/(fPoreDimensions[0]+fWallThickness));
    auto numPores_Y = static_cast<int>(fSlabDimensions[1]/(fPoreDimensions[1]+fWallThickness));

    auto pore_X = fPoreDimensions[0];
    auto pore_Y = fPoreDimensions[1];
    auto pore_Z = fSlabDimensions[2];

    auto div_lamina_X = fPoreDimensions[0] + fWallThickness;
    auto div_lamina_Y = fPoreDimensions[1] + fWallThickness;
    auto div_lamina_Z = pore_Z;

    auto lamina_X = div_lamina_X * numPores_X;
    auto lamina_Y = div_lamina_Y;
    auto lamina_Z = div_lamina_Z;

    auto lmcp_X = lamina_X;
    auto lmcp_Y = div_lamina_Y * numPores_Y;
    auto lmcp_Z = lamina_Z;

    auto world_X = 2*lmcp_X;
    auto world_Y = 2*lmcp_Y;
    auto world_Z = 2*lmcp_Z;

    //------------------------------------------------------
    // World
    //------------------------------------------------------
    // SOLID VOLUME
    auto world_S = new G4Box(
                  "World_S",            // its name
                  world_X/2,            // its X dimension 
                  world_Y/2,            // its Y dimension 
                  world_Z/2);           // its Z dimension 

    // LOGICAL VOLUME
    auto world_LV = new G4LogicalVolume(
                  world_S,              // its solid
                  mVacuum,                 // its material
                  "World_LV");          // its name

    // PHYSICAL VOLUME
    auto world_PV = new G4PVPlacement(
                  0,                    // no rotation
                  G4ThreeVector(),      // at (0,0,0)
                  world_LV,             // its logical volume
                  "World_PV",           // its name
                  0,                    // its mother  volume
                  false,                // no boolean operation
                  0,                    // copy number
                  fCheckOverlaps);      // checking overlaps 
    
    //----------------------------------
    // LMCP
    //----------------------------------
    // SOLID VOLUME
    auto LMCP_S = new G4Box(
                  "LMCP_S",             // its name
                  lmcp_X/2,             // its X dimension 
                  lmcp_Y/2,             // its Y dimension
                  lmcp_Z/2);            // its Z dimension

    // LOGICAL VOLUME
    auto LMCP_LV = new G4LogicalVolume(
                  LMCP_S,               // its solid
                  mLMCP,                // its material
                  "LMCP_LV");           // its name

    // PHYSICAL VOLUME
    auto lmcp_Xpos = 0.0*mm;
    auto lmcp_Ypos = 0.0*mm;
    auto lmcp_Zpos = 0.0*mm;
    auto lmcp_Pos = G4ThreeVector( lmcp_Xpos, lmcp_Ypos, lmcp_Zpos );

    new G4PVPlacement(
                  0,                    // its rotation
                  lmcp_Pos,             // its position
                  LMCP_LV,              // its logical volume
                  "LMCP_PV",            // its name
                  world_LV,             // its mother  volume
                  false,                // no boolean operation
                  0,                    // copy number
                  fCheckOverlaps);      // checking overlaps 


    //----------------------------------
    // Lamina
    //----------------------------------
    // SOLID VOLUME
    auto lamina_S = new G4Box(
                  "Lamina_S",             // its name
                  lamina_X/2,             // its X dimension 
                  lamina_Y/2,             // its Y dimension
                  lamina_Z/2);            // its Z dimension

    // LOGICAL VOLUME
    auto lamina_LV = new G4LogicalVolume(
                  lamina_S,               // its solid
                  mLMCP,                  // its material
                  "Lamina_LV");           // its name

    new G4PVReplica(
                  "Lamina_PV",            // its name
                  lamina_LV,              // its logical volume
                  LMCP_LV,                // its mother volume
                  kYAxis,                 // its replica axis
                  numPores_Y,            // number of replicas
                  lamina_Y);              // dimension of replica

    //----------------------------------
    // Lamina divided
    //----------------------------------
    // SOLID VOLUME
    auto divLamina_S = new G4Box(
                  "Div_Lamina_S",             // its name
                  div_lamina_X/2,             // its X dimension 
                  div_lamina_Y/2,             // its Y dimension
                  div_lamina_Z/2);            // its Z dimension

    // LOGICAL VOLUME
    auto divLamina_LV = new G4LogicalVolume(
                  divLamina_S,               // its solid
                  mLMCP,                      // its material
                  "Div_Lamina_LV");           // its name

    new G4PVReplica(
                  "Div_Lamina_PV",            // its name
                  divLamina_LV,              // its logical volume
                  lamina_LV,                  // its mother volume
                  kXAxis,                     // its replica axis
                  numPores_X,                // number of replicas
                  div_lamina_X);              // dimension of replica

    //----------------------------------
    // Pore
    //----------------------------------
    // SOLID VOLUME
    auto pore_S = new G4Box(
                  "Pore_S",             // its name
                  pore_X/2,             // its X dimension 
                  pore_Y/2,             // its Y dimension
                  pore_Z/2);            // its Z dimension

    // LOGICAL VOLUME
    auto pore_LV = new G4LogicalVolume(
                  pore_S,               // its solid
                  mVacuum,              // its material
                  "Pore_LV");           // its name

    // PHYSICAL VOLUME
    auto pore_Xpos = 0.0*mm;
    auto pore_Ypos = 0.0*mm;
    auto pore_Zpos = 0.0*mm;
    auto pore_Pos = G4ThreeVector( pore_Xpos, pore_Ypos, pore_Zpos );

    new G4PVPlacement(
                  0,                    // its rotation
                  pore_Pos,             // its position
                  pore_LV,              // its logical volume
                  "Pore_PV",            // its name
                  divLamina_LV,        // its mother  volume
                  false,                // no boolean operation
                  0,                    // copy number
                  fCheckOverlaps);      // checking overlaps 

    // //----------------------------------
    // // Anode
    // //----------------------------------
    // // SOLID VOLUME
    // auto anode_X = kAnode_Xsize;
    // auto anode_Y = kAnode_Ysize;
    // auto anode_Z = kAnode_Zsize;

    // auto anode_S = new G4Box(
    //               "Anode_S",             // its name
    //               anode_X/2,             // its X dimension 
    //               anode_Y/2,             // its Y dimension
    //               anode_Z/2);            // its Z dimension

    // // LOGICAL VOLUME
    // auto anode_LV = new G4LogicalVolume(
    //               anode_S,               // its solid
    //               mPCB,             // its material
    //               "Anode_LV");           // its name

    // // PHYSICAL VOLUME
    // auto anode_Xpos = 0.0*mm;
    // auto anode_Ypos = 0.0*mm;
    // auto anode_Zpos = lmcp_Zpos - lmcp_Z/2 - kAnode_gap - anode_Z/2;
    // auto anode_Pos = G4ThreeVector( anode_Xpos, anode_Ypos, anode_Zpos );

    // new G4PVPlacement(
    //               0,                    // its rotation
    //               anode_Pos,            // its position
    //               anode_LV,             // its logical volume
    //               "Anode_PV",           // its name
    //               world_LV,             // its mother  volume
    //               false,                // no boolean operation
    //               0,                    // copy number
    //               fCheckOverlaps);      // checking overlaps 


    //======================================================
    // Visualization attributes
    //======================================================
    world_LV -> SetVisAttributes( G4VisAttributes::GetInvisible() );
    lamina_LV -> SetVisAttributes( G4VisAttributes::GetInvisible() );
    divLamina_LV -> SetVisAttributes( G4VisAttributes::GetInvisible() );

    auto metal_VisAtt = new G4VisAttributes( G4Color::Gray() );
    metal_VisAtt -> SetVisibility( true );
    pore_LV -> SetVisAttributes( metal_VisAtt );

 
    //======================================================
    // Step Limits
    //======================================================
    // auto maxStep = 0.5*kPore_Ysize;
    // auto maxStep = 0.0000005*mm;
    auto maxStep = 5*um;
    auto stepLimit = new G4UserLimits( maxStep );
    lamina_LV->SetUserLimits( stepLimit );
    pore_LV->SetUserLimits( stepLimit );

    //======================================================
    // Print some information
    //======================================================
    G4cout << G4endl;
    G4cout << "------ Wall Thickness: " << G4endl;
    G4cout << "           T: " << fWallThickness/um << " um" << G4endl;

    G4cout << G4endl;
    G4cout << "------ Pore Dimensions: " << G4endl;
    G4cout << "           X: " << pore_X/um << " um" << G4endl;
    G4cout << "           Y: " << pore_Y/um << " um" << G4endl;
    G4cout << "           (Z: " << pore_Z/mm << " mm)" << G4endl;

    G4cout << G4endl;
    G4cout << "------ LMCP Dimensions: " << G4endl;
    G4cout << "           X: " << lmcp_X/mm << " mm" << G4endl;
    G4cout << "           Y: " << lmcp_Y/mm << " mm" << G4endl;
    G4cout << "           Z: " << lmcp_Z/mm << " mm" << G4endl;

    G4cout << G4endl;
    G4cout << "------ World Dimensions: " << G4endl;
    G4cout << "           X: " << world_X/mm << " mm" << G4endl;
    G4cout << "           Y: " << world_Y/mm << " mm" << G4endl;
    G4cout << "           Z: " << world_Z/mm << " mm" << G4endl;

    G4cout << G4endl;
    G4cout << G4endl;

    //======================================================
    // Always return the physical World
    //======================================================
    return world_PV;
  }

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void DetectorConstruction::ConstructSDandField()
  {
    auto LVS = G4LogicalVolumeStore::GetInstance();

    //---------------------------------------------------------
    //This is the first local electric field(Box_1)
    fEMfield_1 = new G4UniformElectricField(G4ThreeVector(0.0, 0.0, 1.0*kilovolt/mm));
    // Create an equation of motion for this field
    fEquation_1 = new G4EqMagElectricField(fEMfield_1);

    G4int nvar1 = 8;
    // fStepper_1 = new G4ClassicalRK4(fEquation_1,nvar1);
    fStepper_1 = new G4DormandPrince745(fEquation_1,nvar1);

    auto localFieldMgr_1 = new G4FieldManager(fEMfield_1);
    LVS->GetVolume( "Pore_LV" )->SetFieldManager(localFieldMgr_1,true);

    fMinStep_1 = 0.00001*mm ; // minimal step of 0.01 microns

    auto fIntgrDriver_1 = new G4MagInt_Driver(fMinStep_1,fStepper_1,fStepper_1->GetNumberOfVariables());

    fChordFinder_1 = new G4ChordFinder(fIntgrDriver_1);
    localFieldMgr_1->SetChordFinder(fChordFinder_1);

    //----------------------------------------------------------
    //----------------------------------------------------------
    //----------------------------------------------------------
    //---------------------------------------------------------

    // //This is the second local electric field(Box_2)
    fEMfield_2 = new G4UniformElectricField(G4ThreeVector(0.0, 0.0, 1.0*kilovolt/mm));
    // Create an equation of motion for this field
    fEquation_2 = new G4EqMagElectricField(fEMfield_2);

    G4int nvar2 = 8;
    // fStepper_2 = new G4ClassicalRK4(fEquation_2,nvar2);
    fStepper_2 = new G4DormandPrince745(fEquation_2,nvar2);

    auto localFieldMgr_2 = new G4FieldManager(fEMfield_2);
    LVS->GetVolume( "LMCP_LV" )->SetFieldManager(localFieldMgr_2,true);
    LVS->GetVolume( "Lamina_LV" )->SetFieldManager(localFieldMgr_2,true);
    LVS->GetVolume( "Div_Lamina_LV" )->SetFieldManager(localFieldMgr_2,true);

    fMinStep_2 = 0.00001*mm ; // minimal step of 0.01 microns

    auto fIntgrDriver_2 = new G4MagInt_Driver(fMinStep_2,fStepper_2,fStepper_2->GetNumberOfVariables());

    fChordFinder_2 = new G4ChordFinder(fIntgrDriver_2);
    localFieldMgr_2->SetChordFinder(fChordFinder_2);
    
    //----------------------------------------------------------
    //----------------------------------------------------------
    //----------------------------------------------------------
    //----------------------------------------------------------

    //This is the world's electric field.Currently, it is set to 0
    fEMfield_WORLD = new G4UniformElectricField(G4ThreeVector(0.0, 0.0,0.0*kilovolt/m));

    // Create an equation of motion for this field
    fEquation_WORLD = new G4EqMagElectricField(fEMfield_WORLD);

    G4int nvar3 = 8;
    // fStepper_WORLD = new G4ClassicalRK4(fEquation_WORLD,nvar3);
    fStepper_WORLD = new G4DormandPrince745(fEquation_WORLD,nvar3);

    // Get the global field manager
    auto fFieldManager= G4TransportationManager::GetTransportationManager()->GetFieldManager();

    // Set this field to the global field manager
    fFieldManager->SetDetectorField(fEMfield_WORLD);

    fMinStep_WORLD = 0.010*mm ; // minimal step of 10 microns

    auto fIntgrDriver_WORLD = new G4MagInt_Driver(fMinStep_WORLD,fStepper_WORLD,fStepper_WORLD->GetNumberOfVariables());

    fChordFinder_WORLD = new G4ChordFinder(fIntgrDriver_WORLD);
    fFieldManager->SetChordFinder(fChordFinder_WORLD);

    //----------------------------------------------------------
    //----------------------------------------------------------
    //----------------------------------------------------------
    //----------------------------------------------------------

    auto SDManager = G4SDManager::GetSDMpointer();

    SDManager->SetVerboseLevel(2);

    //----------------------------------
    // Sensitive detectors
    //----------------------------------
    if (fLMCP_SD == nullptr) {
      fLMCP_SD = new LaminaSD( "LMCP_Det" );
      SDManager->AddNewDetector( fLMCP_SD );
      // LVS->GetVolume( "Lamina_LV" )->SetSensitiveDetector( fLMCP_SD );
      // LVS->GetVolume( "Div_Lamina_LV" )->SetSensitiveDetector( fLMCP_SD );
      SetSensitiveDetector( "Lamina_LV", fLMCP_SD, true );
      SetSensitiveDetector( "Div_Lamina_LV", fLMCP_SD, true );
    }

    if (fPORE_SD == nullptr) {
      fPORE_SD = new PoreSD( "PORE_Det" );
      SDManager->AddNewDetector( fPORE_SD );
      // LVS->GetVolume( "Pore_LV" )->SetSensitiveDetector( fPORE_SD );
      SetSensitiveDetector( "Pore_LV", fPORE_SD, true );
    }

  }

  void DetectorConstruction::UpdateGeometry() {
    #ifdef G4MULTITHREADED
    G4MTRunManager::GetRunManager()->ReinitializeGeometry();
    G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
    #else
    G4RunManager::GetRunManager()->ReinitializeGeometry();
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    #endif
  }

  void DetectorConstruction::SetSlabDimensions( G4ThreeVector dimensions ) {
    if ( fSlabDimensions != dimensions ) {
      fSlabDimensions = dimensions;
    }
    UpdateGeometry();
  }
  
  void DetectorConstruction::SetPoreDimensions( G4ThreeVector dimensions) {
    if ( fPoreDimensions != dimensions ) {
      fPoreDimensions = dimensions;
    }
    UpdateGeometry();
  }

  void DetectorConstruction::SetWallThickness( G4double thickness ) {
    if ( fWallThickness != thickness ) {
      fWallThickness = thickness;
    }
    UpdateGeometry();
  }

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
