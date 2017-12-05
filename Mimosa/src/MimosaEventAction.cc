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
/// \file MimosaEventAction.cc
/// \brief Implementation of the MimosaEventAction class

#include "MimosaEventAction.hh"
#include "MimosaEventActionMessenger.hh"
#include "MimosaSD.hh"
#include "MimosaHit.hh"
#include "MimosaAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "MimosaDigi.hh"
#include "MimosaDigitizer.hh"
#include "G4DigiManager.hh"

#include "MimosaWriter.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  MimosaEventAction::MimosaEventAction()
: G4UserEventAction(),
  fMessenger(0),
  fPrintModulo(1)
{
  fMessenger = new MimosaEventActionMessenger(this);
  G4DigiManager * fDM = G4DigiManager::GetDMpointer();
  MimosaDigitizer * mimosaDTZ = new MimosaDigitizer( "MimosaDigitizer" );
  fDM->AddNewModule(mimosaDTZ);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MimosaEventAction::~MimosaEventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MimosaHitsCollection* 
MimosaEventAction::GetHitsCollection(const G4String& hcName,
    const G4Event* event) const
{
  G4int HCID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(hcName);

  MimosaHitsCollection* hitsCollection 
    = static_cast<MimosaHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(HCID));


  if ( ! hitsCollection ) {
    G4cerr << "MimosaEventAction :: Cannot access hitsCollection " << hcName << G4endl;
    exit(1);
  }         

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MimosaEventAction::PrintEventStatistics(G4double hitEdep, G4double truthEdep) const
{
  // print event statistics
  G4cout
    << "   Hit: total energy: " 
    << std::setw(7) << G4BestUnit(hitEdep, "Energy")
    << G4endl
    << "   Truth: total energy: " 
    << std::setw(7) << G4BestUnit(truthEdep, "Energy")
    << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MimosaEventAction::BeginOfEventAction(const G4Event* event)
{  

  G4int eventID = event->GetEventID();
  if ( eventID % fPrintModulo == 0 )  { 
    G4cout << "\n---> Begin of event: " << eventID << G4endl;
    //CLHEP::HepRandom::showEngineStatus();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MimosaEventAction::EndOfEventAction(const G4Event* event)
{ 
  //Digitize
  G4DigiManager * fDM = G4DigiManager::GetDMpointer();
  MimosaDigitizer * mimosaDTZ = (MimosaDigitizer*)fDM->FindDigitizerModule( "MimosaDigitizer" );
  mimosaDTZ->Digitize();
  G4int myDigiCollID = fDM->GetDigiCollectionID("MimosaDigisCollection");
  MimosaDigisCollection* digiC = (MimosaDigisCollection*)fDM->GetDigiCollection( myDigiCollID );

  // Get hits collections
  MimosaHitsCollection* truthC
    = GetHitsCollection("MimosaTruthCollection", event);

  if(truthC->entries()==0){
    G4cerr << "MimosaEventAction :: There is no hits in MimosaHitsCollections!!! " << G4endl;
    //exit(1);
    return;
  }

  MimosaWriter* mWriter = MimosaWriter::Instance();

  mWriter->WriteEventTag(event->GetEventID());

  //Write McTruth
  G4int nofMc=truthC->entries();
  mWriter->WriteMcTag(nofMc); 
  for(G4int iMc=0;iMc<nofMc;iMc++){
    MimosaHit* truthHit = (*truthC)[iMc];
    mWriter->WriteMc(truthHit);          
  }

  G4int nofDigi = digiC->entries();
  mWriter->WriteDigiTag(nofDigi);          
  for(G4int iDigi=0;iDigi<nofDigi;iDigi++){
    MimosaDigi* digiHit = (*digiC)[iDigi];
    mWriter->WriteDigi(digiHit);          
  }


  // Print per event (modulo n)   
  G4int eventID = event->GetEventID();
  if ( eventID % fPrintModulo == 0) {
    G4cout << "---> End of event: " << eventID << G4endl;     
    //      PrintEventStatistics(newHit->GetEdep(),truthHit->GetEdep());
  }    

}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

