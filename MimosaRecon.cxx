// MimosaRecon.cpp
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <time.h>

#include "MIO.h"
#include "MEvent.h"
#include "MDigi.h"
#include "MHit.h"
#include "MChip.h"
#include "MSuppress.h"

using namespace std;


int main(int argc, char* argv[])
{
  time_t startSup,endSup;
  time(&startSup);


  MSuppress* mimosaSup = MSuppress::Instance();
  mimosaSup->OpenInputFile("../../Mimosa/work/Mimosa.rawdat");
  mimosaSup->OpenOutputFile("./data/Mimosa.supdat");
  mimosaSup->SetTHR(50);

  int nChipSup=1;
  MEvent* evtSup = new MEvent(nChipSup);

  int nEvtSup=0;
  while(mimosaSup->ReadEvent(evtSup)>-1){

    mimosaSup->WriteEvent(evtSup);

    nEvtSup++;
    if(nEvtSup%1000==0) cout<<nEvtSup<<" events are processed!"<<endl;

    evtSup->Reset();
  }

  delete evtSup;
  delete mimosaSup;

  time(&endSup);

  double tSup = difftime(endSup,startSup);
  cout<<"*****************************************************************"<<endl;
  cout<<"Time Cost: "<<tSup<<endl;
  cout<<"Total Event: "<<nEvtSup<<"! \t Event Rate: "<<nEvtSup/tSup<<"!"<<endl;

  time_t start,end;
  time(&start);
  MIO* mimosaIO = MIO::Instance();
  mimosaIO->OpenInputFile("./data/Mimosa.supdat");
  mimosaIO->OpenOutputFile("./data/Mimosa.recdat");

  int nChip=1;
  MEvent* evt = new MEvent(nChip);

  int nEvt=0;
  while(mimosaIO->ReadEvent(evt)>-1){

    evt->Reconstruct();
    mimosaIO->WriteEvent(evt);

    nEvt++;
    if(nEvt%1000==0) cout<<nEvt<<" events are processed!"<<endl;

    evt->Reset();
  }

  delete evt;
  delete mimosaIO;

  time(&end);
  double t = difftime(end,start);
  cout<<"*****************************************************************"<<endl;
  cout<<"Time Cost: "<<t<<endl;
  cout<<"Total Event: "<<nEvt<<"! \t Event Rate: "<<nEvt/t<<"!"<<endl;


  //getchar();
  return 0;

}
