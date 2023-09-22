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
/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class

#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "G4SystemOfUnits.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysicsGS.hh"
#include "G4EmStandardPhysicsSS.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmLivermorePolarizedPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmParameters.hh"

#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronHElasticPhysics.hh"
#include "G4ChargeExchangePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4NeutronCrossSectionXS.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4IonPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonElasticPhysics.hh"

#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsFTFP_BERT_TRV.hh"
#include "G4HadronPhysicsFTF_BIC.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_FTFP_BERT.hh"
#include "G4HadronPhysicsQGS_BIC.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"

#include "G4SystemOfUnits.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  //****************************************************************************
  // Constructor
  //****************************************************************************
  PhysicsList::PhysicsList()
  {
    fPhyMessenger = new PhysicsListMessenger(this);

    verboseLevel = 1;
    SetVerboseLevel(verboseLevel);

    // default
    PhysicsList0();
  }

  //****************************************************************************
  // Destructor
  //****************************************************************************
  PhysicsList::~PhysicsList()
  {
    delete fPhyMessenger;

    delete fParticleList;
    delete fEmPhysicsList;  
    for(size_t i=0; i<fHadronPhys.size(); i++) {
      delete fHadronPhys[i];
    }    
  }

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void PhysicsList::ConstructParticle()
  {
    fParticleList->ConstructParticle();
  }

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void PhysicsList::ConstructProcess()
  {
    AddTransportation();
    fEmPhysicsList->ConstructProcess();
    fParticleList->ConstructProcess();
    for(size_t i=0; i<fHadronPhys.size(); i++) {
      fHadronPhys[i]->ConstructProcess();
    }
  }

  //****************************************************************************
  // Method
  //****************************************************************************
  void PhysicsList::SetPhysicsList(const G4String& name)
  {
    if (verboseLevel>0) {
      G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
    }

    if (name == "Option0") {
      PhysicsList0();

    } else if (name == "Option1") {
      PhysicsList1();

    } else if (name == "Option2") {
      PhysicsList2();

    } else {

      G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
             << " is not defined"
             << G4endl;
    }
  }

  //****************************************************************************
  // Method
  //****************************************************************************
  void PhysicsList::List()
  {
    G4cout << G4endl;
    G4cout << "=======================================================================" << G4endl;
    G4cout << "======                 Physics Lists available                 ========" << G4endl;
    G4cout << "=======================================================================" << G4endl;
    G4cout << "### Option0: Electromagnetic" << G4endl;
    G4cout << "    G4DecayPhysics" << G4endl;
    G4cout << "    G4EmLivermorePolarizedPhysics" << G4endl;
    G4cout << "### Option1: Electromagnetic & Hadronic" << G4endl;
    G4cout << "    G4DecayPhysics" << G4endl;
    G4cout << "    G4EmLivermorePolarizedPhysics" << G4endl;
    // G4cout << "    G4EmStandardPhysics_option4" << G4endl;
    G4cout << "    G4HadronPhysicsFTFP_BERT" << G4endl;
    G4cout << "    G4HadronElasticPhysics" << G4endl;
    G4cout << "    G4IonPhysics" << G4endl;
    G4cout << "    G4StoppingPhysics" << G4endl;
    G4cout << "    G4EmExtraPhysics" << G4endl;
    G4cout << "### Option2: Electromagnetic & Hadronic with high precision option" << G4endl;
    G4cout << "    G4DecayPhysics" << G4endl;
    G4cout << "    G4EmLivermorePolarizedPhysics" << G4endl;
    // G4cout << "    G4EmStandardPhysics_option4" << G4endl;
    G4cout << "    G4HadronPhysicsFTFP_BERT_HP" << G4endl;
    G4cout << "    G4HadronElasticPhysicsHP" << G4endl;
    G4cout << "    G4IonPhysicsXS" << G4endl;
    G4cout << "    G4IonElasticPhysics" << G4endl;
    G4cout << "    G4StoppingPhysics" << G4endl;
    G4cout << "    G4NeutronTrackingCut" << G4endl;
    G4cout << "    G4EmExtraPhysics" << G4endl;
    G4cout << "    G4RadioactiveDecayPhysics" << G4endl;
    G4cout << G4endl;

  }

  //****************************************************************************
  // Method
  //****************************************************************************
  void PhysicsList::PhysicsList0()
  {
    //------------------------------------------//
    // Clear old physics
    //------------------------------------------//
    fHadronPhys.clear();

    //------------------------------------------//
    // Set physics
    //------------------------------------------//
    // Decay
    //
    fParticleList = new G4DecayPhysics(verboseLevel);

    // EM
    //
    // fEmPhysicsList = new G4EmStandardPhysics(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysics_option1(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysics_option2(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysics_option3(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysics_option4(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysicsGS(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysicsSS(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysicsWVI(verboseLevel);
    // fEmPhysicsList = new G4EmLowEPPHysics(verboseLevel);
    // fEmPhysicsList = new G4EmLivermorePhysics(verboseLevel);
    // fEmPhysicsList = new G4EmLivermorePolarizedPhysics(verboseLevel);
    fEmPhysicsList = new G4EmPenelopePhysics(verboseLevel);

  }

  //****************************************************************************
  // Method
  //****************************************************************************
  void PhysicsList::PhysicsList1()
  {
    //------------------------------------------//
    // Clear old physics
    //------------------------------------------//
    fHadronPhys.clear();

    //------------------------------------------//
    // Set physics
    //------------------------------------------//
    // Decay
    //
    fParticleList = new G4DecayPhysics(verboseLevel);

    // EM
    //
    // fEmPhysicsList = new G4EmStandardPhysics(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysics_option1(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysics_option2(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysics_option3(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysics_option4(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysicsGS(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysicsSS(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysicsWVI(verboseLevel);
    // fEmPhysicsList = new G4EmLowEPPHysics(verboseLevel);
    // fEmPhysicsList = new G4EmLivermorePhysics(verboseLevel);
    // fEmPhysicsList = new G4EmLivermorePolarizedPhysics(verboseLevel);
    fEmPhysicsList = new G4EmPenelopePhysics(verboseLevel);
    
    // Hadron Inelastic Physics
    //
    fHadronPhys.push_back( new G4HadronPhysicsFTFP_BERT(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronPhysicsQGSP_BIC(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronPhysicsQGSP_BIC_AllHP(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronInelasticQBBC(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronPhysicsINCLXX(verboseLevel) );
    
    // Hadron Elastic scattering
    //
    fHadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronElasticPhysicsXS(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronElasticPhysicsLEND(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronDElasticPhysics(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronHElasticPhysics(verboseLevel) );
    
    // Ion Inelastic physics
    //
    fHadronPhys.push_back( new G4IonPhysics(verboseLevel) );
    // fHadronPhys.push_back( new G4IonPhysicsXS(verboseLevel) );
    // fHadronPhys.push_back( new G4IonINCLXXPhysics(verboseLevel) );
    
    // Ion Elastic scattering
    //
    fHadronPhys.push_back( new G4IonElasticPhysics(verboseLevel) );
    
    // Stopping Particles
    //
    fHadronPhys.push_back( new G4StoppingPhysics(verboseLevel) );

    // Neutron Tracking
    //
    // fHadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel) );
        
    // Gamma-Nuclear Physics
    //   
    fHadronPhys.push_back( new G4EmExtraPhysics() );

    // Radioactive decay
    //
    fHadronPhys.push_back( new G4RadioactiveDecayPhysics(verboseLevel) );  

  }

  //****************************************************************************
  // Method
  //****************************************************************************
  void PhysicsList::PhysicsList2()
  {
    //------------------------------------------//
    // Clear old physics
    //------------------------------------------//
    fHadronPhys.clear();

    //------------------------------------------//
    // Set physics
    //------------------------------------------//
    // Decay
    //
    fParticleList = new G4DecayPhysics(verboseLevel);

    // EM
    //
    // fEmPhysicsList = new G4EmStandardPhysics(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysics_option1(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysics_option2(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysics_option3(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysics_option4(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysicsGS(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysicsSS(verboseLevel);
    // fEmPhysicsList = new G4EmStandardPhysicsWVI(verboseLevel);
    // fEmPhysicsList = new G4EmLowEPPHysics(verboseLevel);
    // fEmPhysicsList = new G4EmLivermorePhysics(verboseLevel);
    // fEmPhysicsList = new G4EmLivermorePolarizedPhysics(verboseLevel);
    fEmPhysicsList = new G4EmPenelopePhysics(verboseLevel);
    
    // Hadron Inelastic Physics
    //
    // fHadronPhys.push_back( new G4HadronPhysicsFTFP_BERT(verboseLevel) );
    fHadronPhys.push_back( new G4HadronPhysicsFTFP_BERT_HP(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronPhysicsQGSP_BIC(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronPhysicsQGSP_BIC_AllHP(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronInelasticQBBC(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronPhysicsINCLXX(verboseLevel) );
    
    // Hadron Elastic scattering
    //
    // fHadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
    fHadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronElasticPhysicsXS(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronElasticPhysicsLEND(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronDElasticPhysics(verboseLevel) );
    // fHadronPhys.push_back( new G4HadronHElasticPhysics(verboseLevel) );
    
    // Ion Inelastic physics
    //
    // fHadronPhys.push_back( new G4IonPhysics(verboseLevel) );
    fHadronPhys.push_back( new G4IonPhysicsXS(verboseLevel) );
    // fHadronPhys.push_back( new G4IonINCLXXPhysics(verboseLevel) );
    
    // Ion Elastic scattering
    //
    fHadronPhys.push_back( new G4IonElasticPhysics(verboseLevel) );
    
    // Stopping Particles
    //
    fHadronPhys.push_back( new G4StoppingPhysics(verboseLevel) );

    // Neutron Tracking
    //
    fHadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel) );
        
    // Gamma-Nuclear Physics
    //   
    fHadronPhys.push_back( new G4EmExtraPhysics() );

    // Radioactive decay
    //
    fHadronPhys.push_back( new G4RadioactiveDecayPhysics(verboseLevel) );  

  }

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void PhysicsList::SetCuts()
  {
    SetCutValue(0.*mm, "proton");
    SetCutValue(0.*mm, "e-");
    SetCutValue(0.*mm, "e+");
    SetCutValue(0.*mm, "gamma");      
  }

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

