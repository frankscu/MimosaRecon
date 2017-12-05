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
/// \file MimosaPrimaryGeneratorAction.cc
/// \brief Implementation of the MimosaPrimaryGeneratorAction class

#include "MimosaPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GeneralParticleSource.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

#pragma clang diagnostic ignored "-Wunused-variable"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MimosaPrimaryGeneratorAction::MimosaPrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
    fParticleGun(0),fParticleSource(0),
    m_particleType("mu-"),m_gunPosXYSpread(0.5),m_gunPosZ(1.),m_gunEnergy(20*GeV)
{

    m_PrimaryGeneratorActionMessenger=new MimosaPrimaryGeneratorActionMessenger(this);


    G4int nofParticles = 1;
    fParticleGun = new G4ParticleGun(nofParticles);
    fParticleSource = new G4GeneralParticleSource();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MimosaPrimaryGeneratorAction::~MimosaPrimaryGeneratorAction()
{
    delete fParticleGun;
    delete fParticleSource;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MimosaPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // This function is called at the begining of event

    // In order to avoid dependence of PrimaryGeneratorAction
    // on DetectorConstruction class we get world volume
    // from G4LogicalVolumeStore
    //
    G4ParticleDefinition* particleDefinition 
        = G4ParticleTable::GetParticleTable()->FindParticle(m_particleType);
    fParticleGun->SetParticleDefinition(particleDefinition);

    G4double worldZHalfLength = 0;
    G4LogicalVolume* worldLV
        = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    G4Box* worldBox = 0;
    if ( worldLV) worldBox = dynamic_cast< G4Box*>(worldLV->GetSolid()); 
    if ( worldBox ) {
        worldZHalfLength = worldBox->GetZHalfLength();  
    }
    else  {
        G4cerr << "World volume of box not found." << G4endl;
        G4cerr << "Perhaps you have changed geometry." << G4endl;
        G4cerr << "The gun will be place in the center." << G4endl;
    } 


    G4double pPosX= m_gunPosXYSpread*2*(0.5-G4UniformRand());
    G4double pPosY= m_gunPosXYSpread*2*(0.5-G4UniformRand());
    //G4double pPosX= 0;
    //G4double pPosY= 0;
    //G4double pPosX= 10+2*m_gunPosXYSpread*(0.5-G4UniformRand());
    //G4double pPosY= 10+2*m_gunPosXYSpread*(0.5-G4UniformRand());
    // Set gun position
    G4double pPosZ= -m_gunPosZ;
    fParticleGun
        ->SetParticlePosition(G4ThreeVector(pPosX, pPosY, pPosZ));

    G4double mDirX = 5*(0.5-G4UniformRand()) *mm;
    G4double mDirY = 5*(0.5-G4UniformRand()) *mm;
    //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,mDirY,fabs(pPosZ)));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,fabs(pPosZ)));

    fParticleGun->SetParticleEnergy(m_gunEnergy);

    fParticleGun->GeneratePrimaryVertex(anEvent);

    //Particle Source
    fParticleSource->GeneratePrimaryVertex(anEvent);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

