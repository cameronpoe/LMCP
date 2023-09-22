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
/// \file PrimaryGeneratorAction.hh
/// \brief Definition of the PrimaryGeneratorAction class

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ThreeVector.hh"

class G4GeneralParticleSource;
class G4Event;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  //****************************************************************************
  /// The primary generator action class with general particle source.
  ///
  //****************************************************************************
  class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
  {
    public:
      PrimaryGeneratorAction();
      ~PrimaryGeneratorAction() override;

      // methods from base class
      void GeneratePrimaries(G4Event* event) override;

      // get methods
      G4GeneralParticleSource* GetParticleGun() { return fGPS; };
      G4double       GetSrcEnergy() const { return fSrcEnergy; }
      G4ThreeVector  GetSrcPos()    const { return fSrcPos; }

    private:
      // data members
      G4GeneralParticleSource* 	fGPS = nullptr;

      G4double		  fSrcEnergy = 0.;
      G4ThreeVector fSrcPos = G4ThreeVector();
      G4ThreeVector fSrcMomentum = G4ThreeVector();
      G4ThreeVector fSrcPolar = G4ThreeVector();
  };

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif
