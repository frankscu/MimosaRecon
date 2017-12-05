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
/// \file MimosaRunAction.hh
/// \brief Definition of the MimosaRunAction class

#ifndef MimosaHit_h
#define MimosaHit_h 1

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class MimosaEventAction;

class MimosaHit: public G4VHit 
{
    public:
        MimosaHit();
        ~MimosaHit();
        MimosaHit(const MimosaHit& right);

        void SetEdep(G4double de) 		{ edep = de; };
        void AddEdep(G4double de) 		{ edep += de;}
        G4double GetEdep() 			const   { return edep; };

        void SetTrackID(G4int stid)		{ trackID = stid; }
        G4int GetTrackID()			const   { return trackID; }

        void SetLayerID(G4int layid)		{ layerID = layid; }
        G4int GetLayerID()			const   { return layerID; }

        void SetLadderID(G4int ladid)		{ ladderID = ladid; }
        G4int GetLadderID()			const   { return ladderID; }

        void SetChipID(G4int cid)		{ chipID = cid; }
        G4int GetChipID()			const   { return chipID; }

        void SetGlobalChipID(G4int cid)		{ globalChipID = cid; }
        G4int GetGlobalChipID()			const   { return globalChipID; }

        void SetRow(G4int row)		{ rowID = row; }
        G4int GetRow()			const   { return rowID; }

        void SetCol(G4int col)		{ columnID = col; }
        G4int GetCol()			const   { return columnID; }

        void SetGlobalT(G4int gT)		{ globalT = gT; }
        G4int GetGlobalT()			const   { return globalT; }

        void SetPos(G4ThreeVector xyz)		{ pos = xyz; };
        G4ThreeVector GetPos() 		const 	{ return pos; };

        void SetPrePos(G4ThreeVector xyz)		{ pos = xyz; };
        G4ThreeVector GetPrePos() 		const 	{ return pos; };

        void SetPostPos(G4ThreeVector xyz)		{ pos = xyz; };
        G4ThreeVector GetPostPos() 		const 	{ return pos; };

        void SetPDGCode(G4double pcode)		{  pdgCode= pcode; }; //FIX ME
        G4double GetPDGCode() 		const 	{ return pdgCode; };

        void SetTheta(G4double ta) { theta = ta; };
        G4double GetTheta()     const   { return theta; };

        void SetEnterAngle(G4double eAngle) {enterAngle = eAngle;}; //FIX ME
        G4double GetEnterAngle() {return enterAngle;};

        void Print();

        const MimosaHit& operator =(const MimosaHit& right);

        G4int operator ==(const MimosaHit& right) const;

    private:
        G4int trackID;
        G4int layerID;
        G4int ladderID;
        G4int chipID;
        G4int globalChipID;
        G4int rowID;
        G4int columnID;
        G4double edep;
        G4double globalT;
        G4ThreeVector pos;
        G4double pdgCode;
        G4double theta;
        G4double enterAngle;

};

typedef G4THitsCollection<MimosaHit> MimosaHitsCollection;

#endif
