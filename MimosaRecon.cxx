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

using namespace std;


int main(int argc, char* argv[])
{
  time_t start,end;
  time(&start);

  MIO* mimosaIO = MIO::Instance();
  mimosaIO->OpenInputFile("./data/Mimosa.rawdat");
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


