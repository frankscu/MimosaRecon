#include "MHit.h"

MHit::~MHit(void)
{
}


void MHit::Reconstruct(){
  MGeo* _mGeo = MGeo::GetGeo();
  int nofCol = _mGeo->Layer(0).ColNo();
  int nofRow = _mGeo->Layer(0).RowNo();
  double pixPitchX = _mGeo->Layer(0).PitchX() *um;
  double pixPitchY = _mGeo->Layer(0).PitchY() *um;
  double _tmpX=0;
  double _tmpY=0;
  vector<MDigi*>::iterator m_itDigi;
  m_edep=0;
  for(m_itDigi=m_digiVec.begin();m_itDigi<m_digiVec.end();m_itDigi++){
    m_edep += (*m_itDigi)->GetADC();
    _tmpX += ((1.*(*m_itDigi)->GetColId()+0.5)*pixPitchX)*(*m_itDigi)->GetADC();
    _tmpY += ((1.*(*m_itDigi)->GetRowId()+0.5)*pixPitchY)*(*m_itDigi)->GetADC();
  }

  _tmpX /= m_edep;
  _tmpX += (-nofCol*pixPitchX/2);
  _tmpY /= m_edep;
  _tmpY += (-nofRow*pixPitchY/2);

  m_pos.set(_tmpX,_tmpY,0);
  m_time = (m_digiVec[0])->GetTDC();
  m_trackId = (m_digiVec[0])->GetTrackId();
  m_chipId = (m_digiVec[0])->GetGlobalChipId();
}

/*
   void MHit::Print(){
   cout<<""<<endl;
   cout<<"+++++++++ Number of digis in hit "<<m_id<<" :: "<<m_nDigi<<" ++++++++++++"<<endl;
   for(m_itDigi=m_digiVec.begin();m_itDigi<m_digiVec.end();m_itDigi++){
   cout<<"Row: "<<(*m_itDigi)->Row()<<"   Col: "<<(*m_itDigi)->Col()<<"   ID: "<<(*m_itDigi)->ID()<<"   nDigi: "<<(*m_itDigi)->nDigi()<<endl;
   }
   }
   */
