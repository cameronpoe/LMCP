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
/// \file PoreHit.hh
/// \brief Definition of the PoreHit class

#ifndef PoreHit_h
#define PoreHit_h 1

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
  /// Pore hit class
  ///
  /// It defines data members to store the the energy deposit and track lengths
  /// of charged particles in a selected volume:
  //****************************************************************************
  class PoreHit : public G4VHit
  {
    public:
      PoreHit();
      PoreHit(const PoreHit&) = default;
      ~PoreHit() override;

      // operators
      PoreHit& operator=(const PoreHit&) = default;
      G4bool operator==(const PoreHit&) const;

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

      void SetLocalPos ( G4ThreeVector xyz ) { fLocalPos = xyz; }
      G4ThreeVector GetLocalPos() const { return fLocalPos; }

      void SetGlobalPos ( G4ThreeVector xyz ) { fGlobalPos = xyz; }
      G4ThreeVector GetGlobalPos() const { return fGlobalPos; }

      void SetLogV ( G4LogicalVolume* vol ) { fPLogV = vol; }
      const G4LogicalVolume* GetLogV() const { return fPLogV; }

      void SetProc ( G4String& proc ) { fProc = proc; }
      const G4String& GetProc() const { return fProc; }
        
    private:
      // data members
      G4int    fTrackID = 0;
      G4int    fCopyNo = 0;

      G4double fTime = 0.;

      G4ThreeVector fLocalPos;
      G4ThreeVector fGlobalPos;

      G4double fEdep = 0.;

      G4double fEKin = 0.;

      const G4LogicalVolume* fPLogV = nullptr;

      G4String fProc = "";
  };

  //****************************************************************************
  //
  //****************************************************************************
  using PoreHitsCollection = G4THitsCollection<PoreHit>;

  extern G4ThreadLocal G4Allocator<PoreHit>* PoreHitAllocator;

  //****************************************************************************
  // Operators
  //****************************************************************************
  inline void* PoreHit::operator new(size_t)
  {
    if (!PoreHitAllocator) {
      PoreHitAllocator = new G4Allocator<PoreHit>;
    }
    return (void *) PoreHitAllocator->MallocSingle();
  }

  inline void PoreHit::operator delete(void *hit)
  {
    if (!PoreHitAllocator) {
      PoreHitAllocator = new G4Allocator<PoreHit>;
    }
    PoreHitAllocator->FreeSingle((PoreHit*) hit);
  }

  //****************************************************************************
  // Method
  //****************************************************************************
  // inline void PoreHit::AddEdep(G4double de) {
  //   fEdep += de; 
  // }

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif
