#ifndef MIO_H
#define MIO_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include "MEvent.h"
#include "MHit.h"


using namespace std;

class MIO{
  public:
    MIO():m_fin(NULL){
      if(m_io){ cout<<"Warning::MIO is constructed twice."<<endl;; }
      m_io=this;
    };
    ~MIO(){if(m_fin) delete m_fin;};

    void OpenInputFile(string filein);
    void CloseInputFile(){m_fin->close();};
    int ReadEvent(MEvent* evt);

    void OpenOutputFile(string fileout);
    void CloseOutputFile(){m_fout->close();};
    int WriteEvent(MEvent* evt);

    static MIO* Instance();

  private:

    fstream* m_fin;
    fstream* m_fout;

    int m_evtId;
    int m_nTruth;
    int m_nDigi;

    static  MIO * m_io;
};

#endif
