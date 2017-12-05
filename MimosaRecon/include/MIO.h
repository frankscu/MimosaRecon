#ifndef MIO_H
#define MIO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include "MEvent.h"
#include "MHit.h"
#include <cstdio>

using namespace std;
typedef unsigned char Byte;

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
    void OpenBinaryFile(string fileout);
    void CloseOutputFile(){m_fout->close();};
    void CloseBinaryFile(){m_bfout->close();};
    int WriteEvent(MEvent* evt);
    void WriteEmptyBinary(MEvent* evt);
    void WriteBinary(MEvent* evt);

    static MIO* Instance();

  private:

    fstream* m_fin;
    fstream* m_fout;
    fstream* m_bfout; 

    int m_evtId;
    int m_nTruth;
    int m_nDigi;

    static  MIO * m_io;
};

#endif
