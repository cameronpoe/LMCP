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
/// \file Constants.hh
/// \brief Definition of the Constants class


#ifndef Constants_h
#define Constants_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{
  //****************************************************************************
  /// Constants to define geometry.
  /// X-axis is lamina length
  /// Y-axis is lamina thickness
  /// Z-axis is pore length
  ///
  //****************************************************************************
  // constexpr auto kNumKeep = 8;
  // constexpr auto kThickness = 0.1524*mm;    // 6 mils
  constexpr auto kThickness = 5*um;


  //--------------------------------------------------------
  // Pores
  //--------------------------------------------------------
  constexpr auto kPore_Xsize = 2.54*mm;     // 100 mils
  constexpr auto kPore_Ysize = 0.0508*mm;   // 2 mils
  constexpr auto kPore_Zsize = 25.4*mm;     // 1 inch

  //--------------------------------------------------------
  // Lamina
  //--------------------------------------------------------
  constexpr auto kLamina_Xsize = kThickness + kPore_Xsize;
  constexpr auto kLamina_Ysize = kThickness + kPore_Ysize;
  constexpr auto kLamina_Zsize = kPore_Zsize;

  //--------------------------------------------------------
  // LMCP
  //--------------------------------------------------------
  constexpr auto kNumPores_X = 10;
  // constexpr auto kNumPores_Y = 124;
  constexpr auto kNumPores_Y = 455;

  constexpr auto kLMCP_Xsize = kNumPores_X * kLamina_Xsize;
  constexpr auto kLMCP_Ysize = kNumPores_Y * kLamina_Ysize;
  constexpr auto kLMCP_Zsize = kLamina_Zsize;

  //--------------------------------------------------------
  // Floor (Pb Plate under the instrument)
  //--------------------------------------------------------
  // constexpr auto kAnode_Gap = 2.*mm;
  // constexpr auto kAnode_Xsize = kLMCP_Xsize;
  // constexpr auto kAnode_Ysize = kLMCP_Ysize;
  // constexpr auto kAnode_Zsize = 1.*mm;

  //--------------------------------------------------------
  // WORLD
  //--------------------------------------------------------
  constexpr auto kWorld_Xsize = 2*kLMCP_Xsize;
  constexpr auto kWorld_Ysize = 2*kLMCP_Ysize;
  constexpr auto kWorld_Zsize = 2*kLMCP_Zsize;

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif
