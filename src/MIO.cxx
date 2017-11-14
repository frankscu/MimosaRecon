#include "MIO.h"


MIO * MIO::m_io=NULL;

MIO* MIO::Instance(){
  if (! m_io) m_io = new MIO();
  return m_io;
}

void MIO::OpenInputFile(string filein){
  m_fin = new fstream(filein.c_str(),ios::in);
  if(!m_fin){
    cout<<""<<endl;
    cout<<"Error::"<<filein<<" can NOT be opened!!!"<<endl;
  }
}

int MIO::ReadEvent(MEvent* evt){

  bool _isGood=true;
  string line;

  int trackId,chipId,rowId,colId;
  double edep,time,posX,posY,posZ,enterAngle,ADC,TDC;

  while(m_fin->good()){
    std::getline(*m_fin,line);
    if(!m_fin->good()){
      return -1;
    }


    if(line.find("EventId") != std::string::npos){
      //cout<<line<<endl;
      string valStr=line.substr(line.find_last_of(" ")+1);
      m_evtId=atoi(valStr.c_str());
      //cout<<"EventId: "<<m_evtId<<endl;
    }

    if(line.find("McTruth") != std::string::npos){
      //cout<<line<<endl;
      string valStr=line.substr(line.find_last_of(" ")+1);
      //cout<<"TruthNo: "<<valStr<<endl;
      m_nTruth=atoi(valStr.c_str());

      for(int i=0;i<m_nTruth;i++){
        (*m_fin)>>trackId>>chipId>>edep>>time>>posX>>posY>>posZ>>enterAngle;
        m_fin->seekg(1,ios::cur);
        //cout<<rowId<<" "<<colId<<" "<<edep<<" "<<posX<<" "<<enterAngle<<endl;
        evt->AddTruth(trackId,chipId,edep,time,posX,posY,posZ,enterAngle);
      }
    }

    if(line.find("Digi") != std::string::npos){
      //cout<<line<<endl;
      string valStr=line.substr(line.find_last_of(" ")+1);
      //cout<<"DigiNo: "<<valStr<<endl;
      m_nDigi=atoi(valStr.c_str());

      for(int j=0;j<m_nDigi;j++){
        (*m_fin)>>trackId>>chipId>>rowId>>colId>>ADC>>TDC;
        m_fin->seekg(1,ios::cur);
        //cout<<rowId<<" "<<colId<<" "<<ADC<<endl;
        evt->AddDigi(trackId,chipId,rowId,colId,ADC,TDC);
      }
      break;
    }

  }

  return m_evtId;
}


void MIO::OpenOutputFile(string fileout){
  m_fout = new fstream(fileout.c_str(),ios::out);
  m_fout->precision(6);
  m_fout->setf(ios::scientific);
  m_fout->setf(ios::right, ios::adjustfield);
  if(!m_fout){
    cout<<""<<endl;
    cout<<"Error::"<<fileout<<" can NOT be opened!!!"<<endl;
  }
  (*m_fout)<<"EvtId\t\tHitId\tChId\tHPosX\t\tHPosY\t\tHADC\tHDigiN\tTPosX\t\tTPosY\t\tTEdep\t\tTDigiN"<<endl;
}


int MIO::WriteEvent(MEvent* evt){
  int nHit=evt->NofHit();
  for(int iHit=0;iHit<nHit;iHit++){
    MHit* _hit = evt->GetHit(iHit);
    Hep3Vector pos = _hit->GetPos();
    MHit* _trHit = _hit->GetTruth(0);
    Hep3Vector trpos = _trHit->GetPos();

    (*m_fout)<<m_evtId<<"\t\t"<<_hit->GetId()<<"\t"<<_hit->GetChipId()<<"\t"<<pos.x()<<"\t"<<pos.y()<<"\t"<<int(_hit->GetADC())<<"\t"<<_hit->GetNofDigi()<<"\t"<<trpos.x()<<"\t"<<trpos.y()<<"\t"<<_trHit->GetADC()<<"\t"<<_trHit->GetNofDigi()<<endl;
  }
}


