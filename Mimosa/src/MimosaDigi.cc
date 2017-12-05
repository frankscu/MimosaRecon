//---------------------------------------------------------------------------//
//      BOOST --- BESIII Object_Oriented Simulation Tool                     //
//---------------------------------------------------------------------------//
//Description:
//Author: Yuan Ye(yuany@mail.ihep.ac.cn)
//Created:  Oct.26, 2004
//Modified:
//Comment:
//---------------------------------------------------------------------------//

#include "MimosaDigi.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<MimosaDigi> MimosaDigiAllocator;

MimosaDigi::MimosaDigi()
{
  trackID   = 0;
  layerID   = 0;
  ladderID  = 0;
  chipID    = 0;
  rowID    = 0;
  columnID    = 0;
  edep=0;
  globalT=0;
}

MimosaDigi::~MimosaDigi() {}

MimosaDigi::MimosaDigi(const MimosaDigi& right)
:G4VDigi()
{
  trackID   = right.trackID;
  layerID   = right.layerID;
  ladderID  = right.ladderID;
  chipID    = right.chipID;
  rowID    = right.rowID;
  columnID    = right.columnID;
  edep      = right.edep;
  globalT    = right.globalT;
}


const MimosaDigi& MimosaDigi::operator=(const MimosaDigi& right)
{
  trackID   = right.trackID;
  layerID   = right.layerID;
  ladderID  = right.ladderID;
  chipID    = right.chipID;
  rowID    = right.rowID;
  columnID    = right.columnID;
  edep      = right.edep;
  globalT    = right.globalT;
  return *this;
}


int MimosaDigi::operator==(const MimosaDigi& right) const
{
  return (this==&right) ? 1 : 0;
}

/*
void MimosaDigi::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos);
    circle.SetScreenSize(2.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}
*/

void MimosaDigi::Print()
{
  G4cout << "  trackID: " << trackID << "  layerID: " << layerID
	 << "  layerID: " << layerID
	 << "  ladderID:  " << ladderID
	 << "  chipID:  " << chipID
	 << "  row:  " << rowID
	 << "  column:  " << columnID
         << "  energy deposit: " << G4BestUnit(edep,"Energy")
         << "  globalT: " << G4BestUnit(globalT,"Time")<<G4endl;
}


