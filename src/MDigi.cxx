#include "MDigi.h"
#include <cmath>


MDigi::MDigi(int trackId,int chipId,int rowId,int colId,double ADC, double TDC):m_trackId(trackId),m_globalChipId(chipId),m_rowId(rowId),m_colId(colId),m_adc(ADC),m_tdc(TDC),m_IsInHit(false){
  MGeo* _mGeo = MGeo::GetGeo();
  int nofCol = _mGeo->Layer(0).ColNo();
  int nofRow = _mGeo->Layer(0).RowNo();
  m_id = m_globalChipId*nofCol*nofRow+nofCol*m_rowId+m_colId;
};

MDigi::~MDigi(void)
{
}

bool MDigi::IsAdjacentTo(MDigi* digi){
	int dX=abs(m_colId - digi->GetColId());
	int dY=abs(m_rowId - digi->GetRowId());
	if(dX<2&&dY<2){
		return true;
	}
	else{
		return false;
	}
}

bool MDigi::IsAboveTHR(double threshold){
	if(m_adc>threshold){
		return true;
	}
	else{
		return false;
	}
}
