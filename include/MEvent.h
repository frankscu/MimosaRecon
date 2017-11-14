#ifndef MEvent_H
#define MEvent_H

#include <iostream>
#include <vector>
#include <map>

#include "MChip.h"
#include "MCluster.h"
#include "MHit.h"

class MChip;
class MHit;
class MDigi;
class MEvent{
public:
	MEvent(int nChip);
	~MEvent(void);

	void Reset();
	
  void Reconstruct();

	int GetID() const {return m_id;};
	void SetID(int id){m_id=id;};

	int NofHit(){return m_hitVec.size();}
  MHit* GetHit(int i) const {return m_hitVec[i];};

	int NofTruth(){return m_trHitVec.size();}
  MHit* GetTruthHit(int i) const {return m_trHitVec[i];};

	int NofDigi(){return m_digiVec.size();}
  MDigi* GetDigi(int i) const {return m_digiVec[i];};

	int NofChip(){return m_chipVec.size();}
  MChip* GetChip(int i) const {return m_chipVec[i];};

  void AddTruth(int trackId,int chipId,double edep,double time,double posX,double posY,double posZ,double enterAngle);
  void AddDigi(int trackId,int chipId,int rowId,int colId,double ADC,double TDC);

	void Print();

private:

	vector<MHit*> m_hitVec;	
	vector<MHit*>::iterator m_itHit;

	vector<MHit*> m_trHitVec;	
	vector<MHit*>::iterator m_itTrHit;
  int m_trSeed;
  double m_maxEdep;

	vector<MChip*> m_chipVec;	
	vector<MChip*>::iterator m_itChip;	

	vector<MDigi*> m_digiVec;	
	vector<MDigi*>::iterator m_itDigi;	

	int m_id;
};


#endif
