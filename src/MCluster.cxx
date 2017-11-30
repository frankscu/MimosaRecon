#include "MCluster.h"
#include <cmath>

MCluster::MCluster(MDigi* m_digi){
  m_trackId = m_digi->GetTrackId();
  m_globalChipId = m_digi->GetGlobalChipId();
  m_rowId = m_digi->GetRowId();
  m_colId = m_digi->GetColId();
  m_adc = m_digi->GetADC();
  m_tdc = m_digi->GetTDC();
  m_IsInHit = m_digi->IsInHit();

}

MCluster::MCluster(int trackId,int chipId,int rowId,int colId,double ADC, double TDC):m_trackId(trackId),m_globalChipId(chipId),m_rowId(rowId),m_colId(colId),m_adc(ADC),m_tdc(TDC),m_IsInHit(false){

};

MCluster::~MCluster(void)
{
}

bool MCluster::IsAdjacentTo(MCluster* digi){
	int dX=abs(m_colId - digi->GetColId());
	int dY=abs(m_rowId - digi->GetRowId());
	if(dX<2&&dY<2){
		return true;
	}
	else{
		return false;
	}
}

