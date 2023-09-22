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
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4SystemOfUnits.hh"

#include "Randomize.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  //****************************************************************************
  // Constructor
  //****************************************************************************
  PrimaryGeneratorAction::PrimaryGeneratorAction()
  {
    fGPS = new G4GeneralParticleSource();
  }

  //****************************************************************************
  // Destructor
  //****************************************************************************
  PrimaryGeneratorAction::~PrimaryGeneratorAction()
  {
    delete fGPS;
  }

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
  {
    //------------------------------------------//
    // Create Event                             //
    //------------------------------------------//
    fGPS -> GeneratePrimaryVertex( anEvent ); 

    //------------------------------------------//
    // Get Event Properties                     //
    //------------------------------------------//
    fSrcEnergy = fGPS -> GetParticleEnergy() ;
    fSrcPos = fGPS -> GetParticlePosition(); 
    fSrcMomentum = fGPS -> GetParticleMomentumDirection(); 
    fSrcPolar = fGPS -> GetParticlePolarization(); 

    // G4cout << G4endl;
    // G4cout << G4endl << fSrcEnergy/keV << G4endl;
    // G4cout << fSrcPos.getX() << "  ";
    // G4cout << fSrcPos.getY() << "  ";
    // G4cout << fSrcPos.getZ() << G4endl;
    // G4cout << fSrcMomentum.getX() << "  ";
    // G4cout << fSrcMomentum.getY() << "  ";
    // G4cout << fSrcMomentum.getZ() << G4endl;
    // G4cout << fSrcPolar.getX() << "  ";
    // G4cout << fSrcPolar.getY() << "  ";
    // G4cout << fSrcPolar.getZ() << G4endl;

  }

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
