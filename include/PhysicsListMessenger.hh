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
/// \file PhysicsListMessenger.hh
/// \brief Definition of the PhysicsListMessenger class

#ifndef PhysicsListMessenger_h
#define PhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  class PhysicsList;

  //****************************************************************************
  /// Physics List Messenger Class
  ///
  //****************************************************************************
  class PhysicsListMessenger: public G4UImessenger
  {
    public:
      PhysicsListMessenger( PhysicsList* );
      ~PhysicsListMessenger() override;
        
      // methods from base class
      void SetNewValue(G4UIcommand*, G4String) override;
      
    private:
      // data members
      PhysicsList* fPhysicsList = nullptr;
      
      G4UIdirectory* fPhyDir = nullptr;

      G4UIcmdWithAString* fPListCmd = nullptr;

      G4UIcmdWithoutParameter* fListCmd = nullptr;  
  };

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif

