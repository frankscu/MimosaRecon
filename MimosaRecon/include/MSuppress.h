#ifndef MSuppress_H
#define MSuppress_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include "MEvent.h"
#include "MHit.h"
#include "MDigi.h"


using namespace std;

class MSuppress{
  public:
    MSuppress():m_fin(NULL){
      if(m_suppress){ cout<<"Warning::MSuppress is constructed twice."<<endl;; }
      m_suppress=this;
    };
    ~MSuppress(){if(m_fin) delete m_fin;};

    void OpenInputFile(string filein);
    void CloseInputFile(){m_fin->close();};
    int ReadEvent(MEvent* evt);

    void OpenOutputFile(string fileout);
    void CloseOutputFile(){m_fout->close();};
    int WriteEvent(MEvent* evt);

    void WriteEventTag(int id);
    void WriteMc(MHit* truthHit);
    void WriteMcTag(int n);

    void SetTHR(double thr){m_thr = thr;};
    void WriteDigiTag(int n);
    void WriteDigi(MDigi* digi);
    
    static MSuppress* Instance();

  private:

    fstream* m_fin;
    fstream* m_fout;

    int m_evtId;
    int m_nTruth;
    int m_nDigi;

    double m_thr;

    static  MSuppress * m_suppress;
};

#endif
