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
/// \file MimosaDetectorConstruction.cc
/// \brief Implementation of the MimosaDetectorConstruction class

#include "MimosaDetectorConstruction.hh"
#include "MimosaDetectorMessenger.hh"
#include "MimosaSD.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4UnitsTable.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4SystemOfUnits.hh"

#include <stdio.h>

#pragma clang diagnostic ignored "-Wunused-variable" //FIX ME

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MimosaDetectorConstruction::MimosaDetectorConstruction()
 : G4VUserDetectorConstruction(),
   fMessenger(0),
   fMagField(0),
   fCheckOverlaps(true)
{
   fMessenger = new MimosaDetectorMessenger(this);
   mimosaGeo = MimosaGeoParameter::GetGeo();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MimosaDetectorConstruction::~MimosaDetectorConstruction()
{ 
  delete fMagField;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* MimosaDetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MimosaDetectorConstruction::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
//  G4bool fromIsotopes = false;
  nistManager->FindOrBuildMaterial("G4_AIR");
  nistManager->FindOrBuildMaterial("G4_Si");
/*  
  // Liquid argon material
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density; 
  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
         // The argon by NIST Manager is a gas with a different density

  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);
*/
  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* MimosaDetectorConstruction::DefineVolumes()
{
  mimosaGeo->Dump();
  cout<<"MyMessage::Start Detector Construction!!!"<<endl;
/*
  // Geometry parameters
  G4double asicThickness = 10.*um;
  G4double sensorThickness =  30.*mm;
  G4double substrThickness  = 10.*um;

  G4double chipSizeX = 21.56*mm;
  G4double chipSizeY = 13.78*mm;
  G4double activeSizeX = 21.1968*mm;
  G4double activeSizeY = 10.5984*mm;

  G4double activeOffset = ((chipSizeY-activeSizeY-chipSizeX+activeSizeX)/2)*mm;

  G4double pixelSizeXY = 0.52992*mm;
//  G4double pixelSizeXY = 18.4*um;
//  G4double pixelSizeXY = 73.6*um;
*/
  // Geometry parameters
  G4double asicThickness = (mimosaGeo->Layer(0)).AsicThick() *um;
  G4double sensorThickness = (mimosaGeo->Layer(0)).SensorThick() *um;
  G4double substrThickness  = (mimosaGeo->Layer(0)).SubstrateThick() *um;

  G4int nofColumn=(mimosaGeo->Layer(0)).ColNo();
  G4int nofRow=(mimosaGeo->Layer(0)).RowNo();

  G4double pixelSizeX = (mimosaGeo->Layer(0)).PitchX() *um;
  G4double pixelSizeY = (mimosaGeo->Layer(0)).PitchY() *um;
  G4double chipSizeX = (mimosaGeo->Layer(0)).ChipLength() *mm;
  G4double chipSizeY = (mimosaGeo->Layer(0)).ChipWidth() *mm;
  G4double activeSizeX = pixelSizeX*nofColumn *mm;
  G4double activeSizeY = pixelSizeY*nofRow *mm;

  G4double activeOffset = ((chipSizeY-activeSizeY-chipSizeX+activeSizeX)/2)*mm;
  //G4double activeOffset = ((chipSizeY-activeSizeY)/2)*mm;

  G4double chipThickness = asicThickness + sensorThickness + substrThickness;
  G4double worldSizeXY =  10*chipSizeX;
  //G4double worldSizeZ  =  2*chipThickness; 
  G4double worldSizeZ  =  10*chipSizeX; 

  // Get materials
  G4Material* worldMaterial = G4Material::GetMaterial("G4_AIR");
  G4Material* asicMaterial = G4Material::GetMaterial("G4_Si");
  G4Material* sensorMaterial = G4Material::GetMaterial("G4_Si");
  G4Material* substrMaterial = G4Material::GetMaterial("G4_Si");
  
  if ( ! worldMaterial || ! asicMaterial || ! sensorMaterial || ! substrMaterial) {
    G4cerr << "Cannot retrieve materials already defined. " << G4endl;
    G4cerr << "Exiting application " << G4endl;
    exit(1);
  }  
   
  //     
  // World
  //
  G4VSolid* worldS 
    = new G4Box("World",           // its name
                 worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); // its size
                         
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 worldMaterial,  // its material
                 "World");         // its name
                                   
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
  
  //                               
  // Chip
  //  
  G4VSolid* chipS
    = new G4Box("Chip",     // its name
                 chipSizeX/2, chipSizeY/2, chipThickness/2); // its size
                         
  G4LogicalVolume* chipLV
    = new G4LogicalVolume(
                 chipS,     // its solid
                 substrMaterial,  // its material
                 "Chip");   // its name
                                   
  new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0,-activeOffset,0),  // at (0,0,0)
                 chipLV,          // its logical volume                         
                 "Chip",    // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 


  //                               
  // ASIC
  //
  G4VSolid* asicS 
    = new G4Box("ASIC",            // its name
                 chipSizeX/2, chipSizeY/2, asicThickness/2); // its size
                         
  G4LogicalVolume* asicLV
    = new G4LogicalVolume(
                 asicS,        // its solid
                 asicMaterial, // its material
                 "ASIC");          // its name
                                   
   new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0., 0., -(chipThickness-asicThickness)/2), // its position
                 asicLV,       // its logical volume                         
                 "ASIC",           // its name
                 chipLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 

  //                               
  // Sensor
  //
  G4VSolid* sensorS 
    = new G4Box("Sensor",             // its name
                 activeSizeX/2, activeSizeY/2, sensorThickness/2); // its size
                         
  G4LogicalVolume* sensorLV
    = new G4LogicalVolume(
                 sensorS,             // its solid
                 sensorMaterial,      // its material
                 "Sensor");           // its name
                                   
  new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0.,activeOffset,asicThickness-(chipThickness-sensorThickness)/2),//its position
                 sensorLV,            // its logical volume                         
                 "Sensor",            // its name
                 chipLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps

//set sensitive detector		 
  MimosaSD* sensorSD = new MimosaSD("SensorSD");
  G4SDManager::GetSDMpointer()->AddNewDetector(sensorSD );
  sensorLV->SetSensitiveDetector(sensorSD);

/*
  // Column
  //
  G4VSolid* columnS 
    = new G4Box("Column",             // its name
                 pixelSizeX/2, activeSizeY/2, sensorThickness/2); // its size
                         
  G4LogicalVolume* columnLV
    = new G4LogicalVolume(
                 columnS,             // its solid
                 sensorMaterial,      // its material
                 "Column");           // its name

  // Pixel
  //
  G4VSolid* pixelS 
    = new G4Box("Pixel",             // its name
                 pixelSizeX/2, pixelSizeY/2, sensorThickness/2); // its size
                         
  G4LogicalVolume* pixelLV
    = new G4LogicalVolume(
                 pixelS,             // its solid
                 sensorMaterial,      // its material
                 "Pixel");           // its name

//set sensitive detector		 
  MimosaSD* pixelSD = new MimosaSD("PixelSD");
  G4SDManager::GetSDMpointer()->AddNewDetector(pixelSD );
  pixelLV->SetSensitiveDetector(pixelSD);

//add pixel to column
  new G4PVReplica(
		  "Pixel",          // its name
		  pixelLV,          // its logical volume
		  columnLV,          // its mother
		  kYAxis,           // axis of replication
		  nofRow,        // number of replica
		  pixelSizeY);  // witdth of replica

//put Column in Sensor
  new G4PVReplica(
		  "Column",          // its name
		  columnLV,          // its logical volume
		  sensorLV,          // its mother
		  kXAxis,           // axis of replication
		  nofColumn,        // number of replica
		  pixelSizeX);  // witdth of replica

*/
 
  //                               
  // Substrate
  //
  G4VSolid* substrateS 
    = new G4Box("Substrate",             // its name
                 chipSizeX/2, chipSizeY/2, substrThickness/2); // its size
                         
  G4LogicalVolume* substrateLV
    = new G4LogicalVolume(
                 substrateS,             // its solid
                 substrMaterial,      // its material
                 "Substrate");           // its name
                                   
  new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0., 0., (chipThickness-substrThickness)/2 ), // its position
                 substrateLV,            // its logical volume                         
                 "Substrate",            // its name
                 chipLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps


  
  //                                        
  // Visualization attributes
  //
  worldLV->SetVisAttributes (G4VisAttributes::Invisible);
  //                                        

  G4VisAttributes* white= new G4VisAttributes(G4Colour::White());
  G4VisAttributes* green= new G4VisAttributes(G4Colour::Green());
  G4VisAttributes* yellow= new G4VisAttributes(G4Colour::Yellow());
  G4VisAttributes* blue= new G4VisAttributes(G4Colour::Blue());
  G4VisAttributes* red= new G4VisAttributes(G4Colour::Red());
  G4VisAttributes* cyan= new G4VisAttributes(G4Colour::Cyan());
  G4VisAttributes* magenta= new G4VisAttributes(G4Colour::Magenta());

  //chipLV->SetVisAttributes(green);
  chipLV->SetVisAttributes(G4VisAttributes::Invisible);
  //sensorLV->SetVisAttributes(G4VisAttributes::Invisible);
  asicLV->SetVisAttributes(cyan);
  //columnLV->SetVisAttributes(G4VisAttributes::Invisible);
  //pixelLV->SetVisAttributes(green);
  sensorLV->SetVisAttributes(magenta);
  substrateLV->SetVisAttributes(blue);
 
  cout<<"MyMessage::Detector Construction Finished!!!"<<endl;

  //
  // Always return the physical World
  //
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MimosaDetectorConstruction::SetMagField(G4double fieldValue)
{
  // Apply a global uniform magnetic field along X axis
  G4FieldManager* fieldManager
    = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  // Delete the existing magnetic field
  if ( fMagField )  delete fMagField; 

  if ( fieldValue != 0. ) {
    // create a new one if not null
   
//    fMagField 
//      = new G4UniformMagField(G4ThreeVector(fieldValue, 0., 0.));
    fMagField 
      = new G4UniformMagField(G4ThreeVector(0., 0., 0.));
      
    fieldManager->SetDetectorField(fMagField);
    fieldManager->CreateChordFinder(fMagField);
  } 
  else {
    fMagField = 0;
    fieldManager->SetDetectorField(fMagField);
  }
}

