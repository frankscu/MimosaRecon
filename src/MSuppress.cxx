#include "MSuppress.h"


MSuppress * MSuppress::m_suppress=NULL;

MSuppress* MSuppress::Instance(){
  if (! m_suppress) m_suppress = new MSuppress();
  return m_suppress;
}

void MSuppress::OpenInputFile(string filein){
  m_fin = new fstream(filein.c_str(),ios::in);
  if(!m_fin){
    cout<<""<<endl;
    cout<<"Error::"<<filein<<" can NOT be opened!!!"<<endl;
  }
}

void MSuppress::OpenOutputFile(string fileout){
  m_fout = new fstream(fileout.c_str(),ios::out);
  m_fout = new fstream(fileout.c_str(),ios::out);
  m_fout->precision(9);
  //m_fout->setf(ios::scientific);
  m_fout->setf(ios::left);
  (*m_fout)<<"TrkId \t ChId \t Edep \t\t time \t\t posX \t\t posY \t\t posZ \t enterAngle"<<endl;
  (*m_fout)<<"TrkId \t ChId \t RowId \t ColId \t ADC \t TDC"<<endl;
  (*m_fout)<<"***********************************************************************"<<endl;
}

int MSuppress::ReadEvent(MEvent* evt){

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

int MSuppress::WriteEvent(MEvent* evt){
  WriteEventTag(m_evtId);
  //Write McTruth
  int nofMc=evt->NofTruth();
  WriteMcTag(nofMc);
  for(int iMc=0;iMc<nofMc;iMc++){
    MHit* truthHit = evt->GetTruthHit(iMc);
    WriteMc(truthHit);
  }

  int nofDigi = evt->NofDigi();
  int nofDigiOverTHR = 0;
  //WriteDigiTag(nofDigi);
  for(int iDigi=0;iDigi<nofDigi;iDigi++){
    MDigi* digi = evt->GetDigi(iDigi);
    if(digi->GetADC()>m_thr) {
      //WriteDigi(digi);
      nofDigiOverTHR++;
    }
  }

  WriteDigiTag(nofDigiOverTHR);
  for(int iDigi=0;iDigi<nofDigi;iDigi++){
    MDigi* digi = evt->GetDigi(iDigi);
    if(digi->GetADC()>m_thr) {
      WriteDigi(digi);
    }
  }

  return 0;
}

void MSuppress::WriteEventTag(int id){
  (*m_fout)<<""<<endl;
  (*m_fout)<<"EventId: "<<id<<endl;
}

void MSuppress::WriteMc(MHit* truthHit){
  Hep3Vector trPos = truthHit->GetPos();
  (*m_fout)<<truthHit->GetTrackId()<<"\t"<<truthHit->GetChipId()<<"\t"<<truthHit->GetADC()<<"\t"<<truthHit->GetTDC()<<"\t"<<trPos.x()<<"\t"<<trPos.y()<<"\t"<<trPos.z()<<"\t"<<truthHit->GetEnterAngle()<<endl;
}

void MSuppress::WriteMcTag(int n){
  (*m_fout)<<"McTruth Hit No: "<<n<<endl;
}


void MSuppress::WriteDigiTag(int n){
  (*m_fout)<<"Digi Hit No: "<<n<<endl;
}

void MSuppress::WriteDigi(MDigi* digi){
  (*m_fout)<<digi->GetTrackId()<<"\t"<<digi->GetGlobalChipId()<<"\t"<<digi->GetRowId()<<"\t"<<digi->GetColId()<<"\t"<<digi->GetADC()<<"\t"<<digi->GetTDC()<<endl;
}


