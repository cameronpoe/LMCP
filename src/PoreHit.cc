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
/// \file PoreHit.cc
/// \brief Implementation of the PoreHit class

#include "PoreHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{
  G4ThreadLocal G4Allocator<PoreHit>* PoreHitAllocator = nullptr;

  //****************************************************************************
  // Constructor
  //****************************************************************************
  PoreHit::PoreHit()
  {}

  //****************************************************************************
  // Destructor
  //****************************************************************************
  PoreHit::~PoreHit() {}

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void PoreHit::Draw()
  {}

  //****************************************************************************
  // Method from base class
  //****************************************************************************
  void PoreHit::Print()
  {}

  //****************************************************************************
  // Operator
  //****************************************************************************
  G4bool PoreHit::operator==(const PoreHit& right) const
  {
    return ( this == &right ) ? true : false;
  }

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
