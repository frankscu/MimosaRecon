#ifndef MDigi_H
#define MDigi_H

#include "MHit.h"
#include "MGeo.h"

class MHit;
class MDigi
{
public:
	MDigi(void){};
	MDigi(int trackId,int chipId,int rowId,int colId,double ADC, double TDC);
  ~MDigi(void);

	inline int GetColId();
	inline int GetRowId();
	inline int Overflow();
	inline int Frame();
	inline int GetId();
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

	bool IsAdjacentTo(MDigi* digi);

  bool IsAboveTHR(double threshold);

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

inline int MDigi::GetColId(){
	return m_colId;
}

inline int MDigi::GetRowId(){
	return m_rowId;
}

inline int MDigi::Overflow(){
	return m_overflow;
}

inline int MDigi::Frame(){
	return m_frame;
}

inline int MDigi::GetId(){
	return m_id;
}

inline MHit* MDigi::Hit(){
	return m_hit;
}

inline void MDigi::Hit(MHit* hit){
	m_hit=hit;
}

inline void  MDigi::IsInHit(bool isInHit){
	m_IsInHit=isInHit;
}

inline bool  MDigi::IsInHit(){
	return m_IsInHit;
}


#endif
