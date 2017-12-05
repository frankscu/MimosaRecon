//---------------------------------------------------------------------------//
//      BOOST --- BESIII Object_Oriented Simulation Tool                     //
//---------------------------------------------------------------------------//
//Description: Handle database I/O and user interface 
//             for MDC geometry parameters
//Author: Yuan Ye(yuany@mail.ihep.ac.cn)
//Created: 4 Dec, 2003
//Modified:
//Comment: Used in "Mimosa" now, should be insert in framwork later
//         The units are "mm" and "rad". 
//         Datum plane is the East Endplane of MDC.
//---------------------------------------------------------------------------//

#ifndef MimosaWriter_H
#define MimosaWriter_H 


#include <string>
#include <vector>

#include "MimosaHit.hh"
#include "MimosaDigi.hh"

using namespace std;


class MimosaWriter{
  public:
    MimosaWriter():m_fout(NULL){
      if(m_mimosaWriter){ G4cout<<"Warning::MimosaWriter is constructed twice."<<endl;; }
      m_mimosaWriter=this;
    };
    ~MimosaWriter(){if(m_fout) delete m_fout;};

    void OpenFile(string fileout){
      m_fout = new fstream(fileout.c_str(),ios::out);
      m_fout->precision(9);
      //m_fout->setf(ios::scientific);
      m_fout->setf(ios::left);
      (*m_fout)<<"TrkId \t ChId \t Edep \t\t time \t\t posX \t\t posY \t\t posZ \t enterAngle"<<endl;
      (*m_fout)<<"TrkId \t ChId \t RowId \t ColId \t ADC \t TDC"<<endl;
      (*m_fout)<<"***********************************************************************"<<endl;
    };

    void WriteEventTag(int id);
    void WriteMcTag(int n);
    void WriteDigiTag(int n);

    void WriteMc(MimosaHit* truthHit);
    void WriteDigi(MimosaDigi* digiHit);

    void CloseFile(){m_fout->close();};

    static MimosaWriter* Instance();

  private:
    
    fstream* m_fout;

    static  MimosaWriter * m_mimosaWriter;

};

#endif
