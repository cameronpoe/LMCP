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
/// \file LaminaHit.hh
/// \brief Definition of the LaminaHit class

#ifndef LaminaHit_h
#define LaminaHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Threading.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
namespace lmcp
{

  //****************************************************************************
  /// Laminaimeter hit class
  ///
  /// It defines data members to store the the energy deposit and track lengths
  /// of charged particles in a selected volume:
  //****************************************************************************
  class LaminaHit : public G4VHit
  {
    public:
      LaminaHit();
      LaminaHit(const LaminaHit&) = default;
      ~LaminaHit() override;

      // operators
      LaminaHit& operator=(const LaminaHit&) = default;
      G4bool operator==(const LaminaHit&) const;

      inline void* operator new(size_t);
      inline void  operator delete(void*);

      // methods from base class
      virtual void Draw()  override;
      virtual void Print() override;

      // methods
      void SetTrackID ( G4int val ) { fTrackID = val; }
      G4int GetTrackID() const { return fTrackID; }

      void SetCopyNo ( G4int val ) { fCopyNo = val; }
      G4int GetCopyNo() const { return fCopyNo; }

      void SetTime ( G4double sec ) { fTime = sec; }
      G4double GetTime() const { return fTime; }

      void SetEdep ( G4double de ) { fEdep = de; }
      void AddEdep ( G4double de ) { fEdep += de; }
      G4double GetEdep()     const { return fEdep; }

      void SetEKin ( G4double ekin ) { fEKin = ekin; }
      G4double GetEKin()     const { return fEKin; }

      void SetPos ( G4ThreeVector xyz ) { fPos = xyz; }
      G4ThreeVector GetPos() const { return fPos; }

      void SetLogV ( G4LogicalVolume* vol ) { fPLogV = vol; }
      const G4LogicalVolume* GetLogV() const { return fPLogV; }

      void SetProc ( G4String& proc ) { fProc = proc; }
      const G4String& GetProc() const { return fProc; }
        
    private:
      // data members
      G4int    fTrackID = 0;
      G4int    fCopyNo = 0;

      G4double fTime = 0.;

      G4ThreeVector fPos;

      G4double fEdep = 0.;

      G4double fEKin = 0.;

      const G4LogicalVolume* fPLogV = nullptr;

      G4String fProc = "";
  };

  //****************************************************************************
  //
  //****************************************************************************
  using LaminaHitsCollection = G4THitsCollection<LaminaHit>;

  extern G4ThreadLocal G4Allocator<LaminaHit>* LaminaHitAllocator;

  //****************************************************************************
  // Operators
  //****************************************************************************
  inline void* LaminaHit::operator new(size_t)
  {
    if (!LaminaHitAllocator) {
      LaminaHitAllocator = new G4Allocator<LaminaHit>;
    }
    return (void *) LaminaHitAllocator->MallocSingle();
  }

  inline void LaminaHit::operator delete(void *hit)
  {
    if (!LaminaHitAllocator) {
      LaminaHitAllocator = new G4Allocator<LaminaHit>;
    }
    LaminaHitAllocator->FreeSingle((LaminaHit*) hit);
  }

  //****************************************************************************
  // Method
  //****************************************************************************
  // inline void LaminaHit::AddEdep(G4double de) {
  //   fEdep += de; 
  // }

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif
