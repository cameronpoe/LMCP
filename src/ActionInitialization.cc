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
/// \file ActionInitialization.cc
/// \brief Implementation of the ActionInitialization class

#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

#include "G4GeneralParticleSource.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{


  //****************************************************************************
  // Constructor
  //****************************************************************************
  ActionInitialization::ActionInitialization()
  {}


  //****************************************************************************
  // Destructor
  //****************************************************************************
  ActionInitialization::~ActionInitialization()
  {}


  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void ActionInitialization::BuildForMaster() const
  {
    auto eventAct = new EventAction();
    auto runAct = new RunAction( eventAct );
    SetUserAction( runAct );
  }


  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void ActionInitialization::Build() const
  {
    auto priGenAct = new PrimaryGeneratorAction();
    SetUserAction( priGenAct );

    auto eventAct = new EventAction();
    SetUserAction( eventAct );

    auto runAct = new RunAction( eventAct );
    SetUserAction( runAct );

  }  

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
