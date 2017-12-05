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
/// \file MimosaEventActionMessenger.cc
/// \brief Implementation of the MimosaEventActionMessenger class

#include "MimosaEventActionMessenger.hh"
#include "MimosaEventAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MimosaEventActionMessenger::MimosaEventActionMessenger(MimosaEventAction* eventAction)
 : G4UImessenger(),
   fEventAction(eventAction),
   fDirectory(0),
   fSetPrintModuloCmd(0)
{
  fDirectory = new G4UIdirectory("/Mimosa/event/");
  fDirectory->SetGuidance("event control");
   
  fSetPrintModuloCmd 
    = new G4UIcmdWithAnInteger("/Mimosa/event/setPrintModulo",this);
  fSetPrintModuloCmd->SetGuidance("Print events modulo n");
  fSetPrintModuloCmd->SetParameterName("EventNb",false);
  fSetPrintModuloCmd->SetRange("EventNb>0");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MimosaEventActionMessenger::~MimosaEventActionMessenger()
{
  delete fSetPrintModuloCmd;
  delete fDirectory;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MimosaEventActionMessenger::SetNewValue(
                                        G4UIcommand* command,G4String newValue)
{ 
  if ( command == fSetPrintModuloCmd ) {
    fEventAction->SetPrintModulo(fSetPrintModuloCmd->GetNewIntValue(newValue));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
