#ifndef MChip_H
#define MChip_H

#include <iostream>
#include <vector>
#include <map>

#include "MDigi.h"
#include "MCluster.h"
#include "MHit.h"

class MDigi;
class MCluster;
class MHit;
class MChip
{
public:
	MChip(int id);
	~MChip(void);

	int GetID() const {return m_id;};
	void SetID(int id){m_id=id;};

	//void FindHit( int (*reconTable)[1152],map<int,MDigi*> &digiVec);
	int FindHit(vector<MHit*> &hitVec);

	void Reset();

  int nDigi() const {return m_digiVec.size();};
  void AddDigi(MDigi* digi){m_digiVec.push_back(digi);};
  MDigi* Digi(int i) const{return m_digiVec[i];};

  void Print();

private:

	vector<MDigi*> m_digiVec;	
	vector<MDigi*>::iterator m_itDigi;	

	vector<MDigi*> m_tempDigiVec;	
	vector<MDigi*>::iterator m_tempItDigi;	

	vector<MCluster*> m_clusterVec;	
	vector<MCluster*>::iterator m_itCluster;	

	int m_id;
};


#endif
