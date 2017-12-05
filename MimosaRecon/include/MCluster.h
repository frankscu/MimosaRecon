#ifndef MCluster_H
#define MCluster_H

#include "MDigi.h"
#include "MHit.h"
#include "MGeo.h"

class MDigi;
class MHit;
class MCluster
{
public:
	MCluster(void){};
  MCluster(MDigi* m_digi);
	MCluster(int trackId,int chipId,int rowId,int colId,double ADC, double TDC);
  ~MCluster(void);

	inline int GetColId();
	inline int GetRowId();
	inline int Overflow();
	inline int Frame();
	inline int GetId();
  void SetId(int id) {m_id=id;};
  
  int GetGlobalChipId() const{return m_globalChipId;};
  int GetTrackId() const{return m_trackId;};

	inline MHit* Hit();
	inline void Hit(MHit *hit);

  void SetADC(double adc){m_adc=adc;};
  void SetTDC(double tdc){m_tdc=tdc;};
  
  void SetGlobalChipId(int chipId){m_globalChipId=chipId;};
  void SetTrackId(int trkId){m_trackId=trkId;};

  double GetADC() const {return m_adc;};
  double GetTDC() const {return m_tdc;};

	inline bool IsInHit();
	inline void IsInHit(bool isInHit);

	bool IsAdjacentTo(MCluster* digi);

private:
  int m_trackId;
	int m_globalChipId;
	int m_rowId;
	int m_colId;
	int m_overflow;
	int m_frame;
	int m_id;
  double m_adc;
  double m_tdc;
	bool m_IsInHit;
	MHit *m_hit;
};

inline int MCluster::GetColId(){
	return m_colId;
}

inline int MCluster::GetRowId(){
	return m_rowId;
}

inline int MCluster::Overflow(){
	return m_overflow;
}

inline int MCluster::Frame(){
	return m_frame;
}

inline int MCluster::GetId(){
	return m_id;
}

inline MHit* MCluster::Hit(){
	return m_hit;
}

inline void MCluster::Hit(MHit* hit){
	m_hit=hit;
}

inline void  MCluster::IsInHit(bool isInHit){
	m_IsInHit=isInHit;
}

inline bool  MCluster::IsInHit(){
	return m_IsInHit;
}


#endif
