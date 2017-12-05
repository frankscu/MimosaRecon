//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file MimosaSD.cc
/// \brief Implementation of the MimosaSD class

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "MimosaSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "Randomize.hh"

#pragma clang diagnostic ignored "-Wunused-variable"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MimosaSD::MimosaSD(const G4String& name)
  : G4VSensitiveDetector(name),
  hitsCollection(0)
{
  collectionName.insert("MimosaHitsCollection");
  collectionName.insert("MimosaTruthCollection");
  mimosaGeoPointer=MimosaGeoParameter::GetGeo();
  //  HitTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MimosaSD::~MimosaSD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MimosaSD::Initialize(G4HCofThisEvent* HCE)
{
  // Create hits collection
  hitsCollection 
    = new MimosaHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in HCE
  G4int HCID = -1;
  if(HCID<0) HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection( HCID, hitsCollection ); 

  //for MC truth
  truthCollection 
    = new MimosaHitsCollection(SensitiveDetectorName,collectionName[1]);

  static G4int HLID=-1;
  if(HLID<0){
    HLID = G4SDManager::GetSDMpointer() -> GetCollectionID(collectionName[1]); 
  }
  HCE->AddHitsCollection(HLID,truthCollection);

}

/*
//for MC Truth
void MimosaSD::BeginOfTruthEvent(const G4Event* evt)
{  
truthCollection = new MimosaHitsCollection
(SensitiveDetectorName,collectionName[1]); 
//  G4cout<<" begin event "<<evt->GetEventID()<<G4endl;
}

void MimosaSD::EndOfTruthEvent(const G4Event* evt)
{
static G4int HLID=-1;
if(HLID<0){
HLID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[1]); 
}
G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
HCE->AddHitsCollection(HLID,truthCollection);
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MimosaSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{

  G4Track* gTrack = aStep->GetTrack() ;
  G4double globalT=gTrack->GetGlobalTime();//Time since the event in which the track belongs is created

  //if(globalT > 2000)return false; //Mimosa T window is 2 microsecond 

  //skip neutral tracks
  G4int charge = gTrack->GetDefinition()->GetPDGCharge();
  //if (charge == 0) return false;

  G4double stepLength=aStep->GetStepLength();
  if(stepLength==0){
    //    G4cout<<"step length equal 0!!"<<G4endl;
    return false;
  }

  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;


  // get position of the track at the beginning and at the end of step
  G4StepPoint* prePoint  = aStep->GetPreStepPoint() ;
  G4StepPoint* postPoint = aStep->GetPostStepPoint() ;

  //get position coordinate
  G4ThreeVector pointIn  = prePoint->GetPosition();
  G4ThreeVector pointOut = postPoint->GetPosition();
  G4ThreeVector hitPosition = (pointIn+pointOut)/2;

  const G4VTouchable *touchable = prePoint->GetTouchable();

  G4ThreeVector preLPos = touchable->GetHistory()->GetTopTransform().TransformPoint(pointIn);
  G4ThreeVector postLPos = touchable->GetHistory()->GetTopTransform().TransformPoint(pointOut);
  G4ThreeVector hitLPos = (preLPos+postLPos)/2;
  G4ThreeVector measurePos = hitLPos;
  measurePos.setZ(hitPosition.z());


  G4int trackID = 0;
  G4int layerID = 0;
  G4int ladderID = 0;
  G4int chipID = 0;
  //  int64_t key = 0;
  unsigned int key = 0;

  // history depth :| 0:pixel; 1:column; 2:sensor; 3:chip; 4:ladder; 5:layer
  //  pixelID = touchable->GetVolume(0)->GetCopyNo();
  trackID = gTrack->GetTrackID();
  G4int primaryTrackId=1;

  chipID =  touchable->GetVolume(0)->GetCopyNo();
  //ladderID =  touchable->GetVolume(1)->GetCopyNo();
  //layerID =  touchable->GetVolume(2)->GetCopyNo();

  G4String particleName=gTrack->GetDefinition()->GetParticleName();
  G4int pdgCode = gTrack->GetDefinition()->GetPDGEncoding();
  //G4cout<<"MyMessage::ParticleName: "<<particleName<<" PdgCode: "<<pdgCode<<" Charge: "<<charge<<" TrackID: "<<trackID<<" Edep: "<<edep/eV<<" eV"<<G4endl;

  G4int nofCol=mimosaGeoPointer->Layer(layerID).ColNo();

  MimosaIdentifier mimosaId(layerID,ladderID,chipID,-1,-1);
  //mimosaId.Print();

  G4int truthLayer=layerID;
  G4int truthLadder=ladderID;
  G4int truthChip=chipID;
  G4int truthGlobalChip=mimosaId.GlobalChipID();

  //G4cout<<"MyMessage:: HitLPos.X: "<<hitLPos.x()/cm<<" Y: "<<hitLPos.y()/cm<<" Z: "<<hitLPos.z()/cm<<G4endl;
  //G4cout<<"MyMessage:: HitGPos.X: "<<hitPosition.x()/cm<<" Y: "<<hitPosition.y()/cm<<" Z: "<<hitPosition.z()/cm<<G4endl;
  //G4cout<<"MyMessage:: MeasurePos.X: "<<measurePos.x()/cm<<" Y: "<<measurePos.y()/cm<<" Z: "<<measurePos.z()/cm<<G4endl;
  //G4cout<<"MyMessage::postLPos.X: "<<postLPos.x()<<" Y: "<<postLPos.y()<<" Z: "<<postLPos.z()<<G4endl;
  //  G4cout<<"MyMessage::HitPos.Rho: "<<hitPosition.rho()<<" Phi: "<<hitPosition.phi()/deg<<" Z: "<<hitPosition.z()<<G4endl;

  G4double theta=gTrack->GetMomentumDirection().theta()/deg;

  G4ThreeVector hitLine=pointOut-pointIn;
  //G4double enterAngle=hitLine.phi()-pointIn.phi();
  G4double enterAngle=hitLine.theta();
  while(enterAngle<-pi/2.)enterAngle+=pi;
  while(enterAngle>pi/2.)enterAngle-=pi;

  G4ThreeVector hitLineLoc=postLPos-preLPos;
  //G4double enterAngleLoc=(hitLineLoc.phi()-halfpi)/deg;
  G4double enterAngleLoc=hitLineLoc.theta();
  while(enterAngleLoc<-pi/2.)enterAngleLoc+=pi;
  while(enterAngleLoc>pi/2.)enterAngleLoc-=pi;
  //  G4cout<<"MyMessage:: enterAngle: "<<enterAngleLoc<<" theta: "<<theta<<" stepLength: "<<stepLength<<G4endl;
  //  G4cout<<""<<G4endl;

  key = mimosaId.GlobalChipID();
  MimosaHit* newHit = new MimosaHit();
  newHit->SetTrackID(primaryTrackId);
  newHit->SetLayerID(truthLayer);
  newHit->SetLadderID(truthLadder);
  newHit->SetChipID(truthChip);
  newHit->SetGlobalChipID(truthGlobalChip);
  newHit->SetPrePos(preLPos);
  //newHit->SetPos(hitPosition);
  newHit->SetPos(hitLPos);
  newHit->SetPostPos(postLPos);
  newHit->SetPDGCode(pdgCode);
  newHit->SetEdep(edep);
  newHit->SetGlobalT(globalT);
  newHit->SetEnterAngle(enterAngle);

  hitsCollection->insert(newHit);    
  hitMap[key]=G4int(hitsCollection->entries())-1;


  //for MC truth  
  if(truthCollection){
    if(trackID==primaryTrackId){
      key = mimosaId.GlobalChipID();
      itTruthMap = truthMap.find(key);
      if(itTruthMap==truthMap.end()){
        MimosaHit* truthHit = new MimosaHit();
        truthHit->SetTrackID(primaryTrackId);
        truthHit->SetLayerID(truthLayer);
        truthHit->SetLadderID(truthLadder);
        truthHit->SetChipID(truthChip);
        truthHit->SetGlobalChipID(truthGlobalChip);
        truthHit->SetPrePos(pointIn);
        //truthHit->SetPos(hitPosition);
        truthHit->SetPos(hitLPos);
        truthHit->SetPostPos(pointOut);
        truthHit->SetPDGCode(pdgCode);
        truthHit->SetEdep(edep);
        truthHit->SetGlobalT(globalT);
        truthHit->SetEnterAngle(enterAngle);

        truthCollection->insert(truthHit);
        truthMap[key]=truthCollection->entries()-1;
      }
      else{
        G4int pointer=(*itTruthMap).second; 
        if(trackID==(*truthCollection)[pointer]->GetTrackID()){
          (*truthCollection)[pointer]->SetEdep((*truthCollection)[pointer]->GetEdep()+edep);
          G4double preGlobalT=(*truthCollection)[pointer]->GetGlobalT();
          if(globalT<preGlobalT){
            (*truthCollection)[pointer]->SetGlobalT(globalT);
          }
        }
        else{
          MimosaHit* truthHit = new MimosaHit();
          truthHit->SetTrackID(primaryTrackId);
          truthHit->SetLayerID(truthLayer);
          truthHit->SetLadderID(truthLadder);
          truthHit->SetChipID(truthChip);
          truthHit->SetGlobalChipID(truthGlobalChip);
          //truthHit->SetPos(hitPosition);
          truthHit->SetPos(hitLPos);
          truthHit->SetPDGCode(pdgCode);
          truthHit->SetEdep(edep);
          truthHit->SetGlobalT(globalT);
          truthHit->SetEnterAngle(enterAngle);

          truthCollection->insert(truthHit);
          truthMap[key]=truthCollection->entries()-1;  
        }
      }
    }
  }


  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MimosaSD::EndOfEvent(G4HCofThisEvent*)
{

  hitMap.clear();
  truthMap.clear();

  if ( verboseLevel>1 ) { 
    G4int nofHits = hitsCollection->entries();
    G4cout << "\n-------->Hits Collection: in this event they are " << nofHits 
      << " hits in the tracker chambers: " << G4endl;
    for ( G4int i=0; i<nofHits; i++ ) (*hitsCollection)[i]->Print();
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

