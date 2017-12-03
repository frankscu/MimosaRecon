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

  std::string line;

  int trackId,chipId,rowId,colId;
  double edep,time,posX,posY,posZ,enterAngle,ADC,TDC;

  while(m_fin->good()){
    std::getline(*m_fin,line);
    if(!m_fin->good()){
      return -1;
    }


    if(line.find("EventId") != std::string::npos){
      //cout<<line<<endl;
      std::string valStr=line.substr(line.find_last_of(" ")+1);
      m_evtId=atoi(valStr.c_str());
      //cout<<"EventId: "<<m_evtId<<endl;
    }

    if(line.find("McTruth") != std::string::npos){
      //cout<<line<<endl;
      std::string valStr=line.substr(line.find_last_of(" ")+1);
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
  return 0;
}

void MIO::OpenBinaryFile(string fileout){
  m_bfout = new fstream(fileout.c_str(),ios::out | ios::binary);
  if(!m_bfout){
    cout<<""<<endl;
    cout<<"Error::"<<fileout<<" can NOT be opened!!!"<<endl;
  }
  time_t t=time(0);
  char tmp[64];
  strftime(tmp, sizeof(tmp), "**********%Y%m%d%H%M%S**********", localtime(&t));
  (*m_bfout)<<tmp<<"\r\n";
}

void MIO::WriteEmptyBinary(MEvent* evt){
  string EventHeader = "\xAA\xAA\xAA\xAA";
  string EventTrailer = "\xF0\xF0\xF0\xF0";
  string RowHeaderFirst = "\x57\x53";
  string RowTrilerFirst = "\x97\x98";

  // Row Head:  0x07F4
  string RowHeaderSecondA[16]={"\x07","\x17","\x27","\x37","\x47","\x57","\x67","\x77","\x87","\x97","\xA7","\xB7","\xC7","\xD7","\xE7","\xF7"};
  string RowHeaderSecondB[3]={"\xF4","\xF5","\xF6"};

  // Row Head:  0xE407
  string RowTrilerSecondA[3]={"\xE4","\xE5","\xE6"};
  string RowTrilerSecondB[16]={"\x07","\x17","\x27","\x37","\x47","\x57","\x67","\x77","\x87","\x97","\xA7","\xB7","\xC7","\xD7","\xE7","\xF7"};

  (*m_bfout)<<EventHeader;

  for(int row=0; row<48; row++){
    (*m_bfout)<<RowHeaderFirst<<std::hex<<RowHeaderSecondA[row%16]<<std::hex<<RowHeaderSecondB[int(row/16)];
    for(int col=0; col<16; col++){
      (*m_bfout)<<std::hex<<std::setfill('\x00')<<std::setw(2)<<'\x00';
    }
    (*m_bfout)<<RowTrilerFirst<<std::hex<<RowTrilerSecondA[int(row/16)]<<std::hex<<RowTrilerSecondB[row%16];
  }
  (*m_bfout)<<EventTrailer;
}

void MIO::WriteBinary(MEvent* evt){
  string EventHeader = "\xAA\xAA\xAA\xAA";
  string EventTrailer = "\xF0\xF0\xF0\xF0";
  string RowHeaderFirst = "\x57\x53";
  string RowTrilerFirst = "\x97\x98";

  // Row Head:  0x07F4
  string RowHeaderSecondA[16]={"\x07","\x17","\x27","\x37","\x47","\x57","\x67","\x77","\x87","\x97","\xA7","\xB7","\xC7","\xD7","\xE7","\xF7"};
  string RowHeaderSecondB[3]={"\xF4","\xF5","\xF6"};

  // Row Head:  0xE407
  string RowTrilerSecondA[3]={"\xE4","\xE5","\xE6"};
  string RowTrilerSecondB[16]={"\x07","\x17","\x27","\x37","\x47","\x57","\x67","\x77","\x87","\x97","\xA7","\xB7","\xC7","\xD7","\xE7","\xF7"};

  (*m_bfout)<<EventHeader;

  int nDigi=evt->NofDigi();
  for(int row=0; row<48; row++){
    (*m_bfout)<<RowHeaderFirst<<std::hex<<RowHeaderSecondA[row%16]<<std::hex<<RowHeaderSecondB[int(row/16)];
    int counts1=0;
    int counts2=0;
    for(int col=0; col<16; col++){
      bool IsThereData=false;
      for(int iDigi=0;iDigi<nDigi;iDigi++){
        MDigi* _digi = evt->GetDigi(iDigi);
        int rowId = _digi->GetRowId();
        int colId = _digi->GetColId();
        int adc = _digi->GetADC();
        if(rowId==row && colId==col){
          //cout << "rowId: "<<rowId<<"  colId:  "<<colId<<"  adc: "<<std::setfill('0')<<std::setw(2)<<std::hex<<adc<< endl;
          //std::stringstream ssA, ssB;
          //ssA << std::hex<<setfill('0')<<setw(1)<<int(adc/16);
          //ssB << std::hex<<setfill('0')<<setw(1)<<int(adc%16);
          //string adcstr = ssA.str() + ssB.str();
          //string adcstrA = "\\x" + ssA.str();
          //string adcstrB = "\\x" + ssB.str();
          //std::cout << "ADC string:  " << adcstr<< std::endl;
          //std::cout << "ADC Hex string:  " << std::hex << adcstr<< std::endl;
          //(*m_bfout) <<std::hex<<adcstr;
          (*m_bfout) << std::setfill('\x00')<<std::setw(2)<<Byte(adc);
          IsThereData=true;
          counts1++;
        }
      }

      if(IsThereData==false){
        (*m_bfout)<<std::hex<<std::setfill('\x00')<<std::setw(2)<<'\x00';
        counts2++;
      }
    }
    // std::cout << "Counts1: " << counts1 << std::endl;
    // std::cout << "Counts2: " << counts2 << std::endl;
    (*m_bfout)<<RowTrilerFirst<<std::hex<<RowTrilerSecondA[int(row/16)]<<std::hex<<RowTrilerSecondB[row%16];
  }
  (*m_bfout)<<EventTrailer;

}
