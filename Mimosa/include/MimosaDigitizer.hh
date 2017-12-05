//---------------------------------------------------------------------------//
//      BOOST --- BESIII Object_Oriented Simulation Tool                     //
//---------------------------------------------------------------------------//
//Description:
//Author: Yuan Ye(yuany@mail.ihep.ac.cn)
//Created: Oct. 26, 2004
//Modified:
//Comment:
//---------------------------------------------------------------------------//

#ifndef MimosaDigitizer_h
#define MimosaDigitizer_h 1

class MimosaDigitizerMessenger;
class TFile;
class TH1F;
class G4Svc;

#include "G4VDigitizerModule.hh"
#include "MimosaIdentifier.hh"
#include "MimosaHit.hh"
#include "MimosaDigi.hh"
#include "MimosaGeoParameter.hh"

#include "globals.hh"
#include <vector>

#pragma clang diagnostic ignored "-Wunused-private-field"
#pragma clang diagnostic ignored "-Wshadow"

class MimosaDigitizer : public G4VDigitizerModule
{
public:
  MimosaDigitizer(G4String modName);
  ~MimosaDigitizer();

  //necessary digi collection object must be constructed and set to 
  //G4DCofThisEvent by StoreDigiCollection protected method.
  virtual void Digitize();
  void AddNoise(void);

  int GetADC(double edep);
  int GetTDC(double time);

  void HitRealizition(MimosaHit* rawHit);
  void DiffuseE(G4double edep,G4ThreeVector hitPoint,MimosaIdentifier& mimosaId,MimosaHit* rawHit);

  void HitRealizitionEelectrode(MimosaHit* rawHit);
  double DiffuseGaussLorentzElectrode(MimosaIdentifier& mimosaId, G4ThreeVector hitPoint);
  double OverMOSDiffuseGaussLorentzElectrode(MimosaIdentifier& mimosaId, G4ThreeVector hitPoint);

  void PixelIntegration(G4double ePix,MimosaIdentifier& mimosaId,MimosaHit* rawHit);

private:


  std::vector<MimosaHit*> realizedHitsCol;
  std::map<unsigned int,G4int> realizedHitMap;
  std::map<unsigned int,G4int>::iterator itRealizedHitMap;

  MimosaDigisCollection* digisCollection;         
//  MimosaDigitizerMessenger* digitizerMessenger;
  MimosaGeoParameter* mimosaGeo;

  std::map<G4long,G4int> digiMap;
  std::map<G4long,G4int>::iterator itDigiMap;

  G4double pedestal ;  // e-
  G4double enc ;  // e- ENC
  G4double cce ;  // charge collection efficiency
  G4double adcRange;
  G4double ehpEnergy;
  G4double energyThreshold;

  G4double adcEnergyRange;
  G4int nAdcBit;

  G4double noiseFlag; //1: Add electronic noise; 0: do nothing
  G4int fakeHitFlag; //1: Add fake hit; 0: do nothing
};

#endif

