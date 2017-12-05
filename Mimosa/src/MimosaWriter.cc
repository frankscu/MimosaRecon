//---------------------------------------------------------------------------//
//      BOOST --- BESIII Object_Oriented Simulation Tool                     //
//---------------------------------------------------------------------------//
//Description: Handle database I/O and user interface 
//             for MDC geometry parameters
//Author: Yuan Ye(yuany@mail.ihep.ac.cn)
//Created: 4 Dec, 2003
//Modified:
//Comment: Used in "Mimosa" now, should be insert in framwork later
//         The units are "mm" and "rad". 
//         Datum plane is the East Endplane of MDC.
//---------------------------------------------------------------------------//

#include <iostream>
#include <fstream>

#include <CLHEP/Units/PhysicalConstants.h>
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4UnitsTable.hh"

#include "MimosaHit.hh"
#include "MimosaDigi.hh"
#include "G4ThreeVector.hh"

#include "MimosaWriter.hh"
#include "globals.hh"
#include <math.h>
#include <cstdlib>

MimosaWriter * MimosaWriter::m_mimosaWriter=NULL;

MimosaWriter* MimosaWriter::Instance(){
  if (! m_mimosaWriter) m_mimosaWriter = new MimosaWriter();
  return m_mimosaWriter;
}


void MimosaWriter::WriteEventTag(int id){
  (*m_fout)<<""<<endl;  
  (*m_fout)<<"EventId: "<<id<<endl;    
}

void MimosaWriter::WriteMcTag(int n){
  (*m_fout)<<"McTruth Hit No: "<<n<<endl;    
}

void MimosaWriter::WriteMc(MimosaHit* truthHit){
  G4ThreeVector trPos = truthHit->GetPos();
  (*m_fout)<<truthHit->GetTrackID()<<"\t"<<truthHit->GetGlobalChipID()<<"\t"<<truthHit->GetEdep()<<"\t"<<truthHit->GetGlobalT()<<"\t"<<trPos.x()<<"\t"<<trPos.y()<<"\t"<<trPos.z()<<"\t"<<truthHit->GetEnterAngle()<<endl;
}

void MimosaWriter::WriteDigiTag(int n){
  (*m_fout)<<"Digi Hit No: "<<n<<endl;    
}

void MimosaWriter::WriteDigi(MimosaDigi* digiHit){
  (*m_fout)<<digiHit->GetTrackID()<<"\t"<<digiHit->GetGlobalChipID()<<"\t"<<digiHit->GetRow()<<"\t"<<digiHit->GetCol()<<"\t"<<digiHit->GetADC()<<"\t"<<digiHit->GetTDC()<<endl;
}


