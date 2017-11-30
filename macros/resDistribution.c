#include <iostream>
#include <fstream>
#include "string"
#include <algorithm>
#include "stdlib.h"

void resDistribution(){

  TH1F* hdx = new TH1F("hdx","hdx",200,-0.05,0.05);
  TH1F* hdy = new TH1F("hdy","hdy",200,-0.05,0.05);
  TH2F* digiHisHit = new TH2F("digiHisHit","Digital Hit Map",200,-1,1,200,-1,1);
  TH2F* truthHisHit = new TH2F("truthHisHit","Truth Hit Map",200,-1,1,200,-1,1);

  string line;
  int nlines=0;
  double THR=100;
  int evtId,hitId,chipId,HitNofDigi,TrNofDigi;
  double posX,posY,hitADC,trPosX,trPosY,edep;
  ostringstream strTHR;
  strTHR << THR;
  string basePath="/Users/chenliejian/Documents/Code/MimosaDigitizer/AnaMimosa/MimosaRecon/data/";
  string filename="Mimosa_thr"+strTHR.str();
  string fullPath=basePath+filename+".recdat";

  //fstream fin("/Users/chenliejian/Documents/Code/MimosaDigitizer/AnaMimosa/MimosaRecon/data/Mimosa_thr50.recdat",ios::in);
  fstream fin(fullPath,ios::in);

  std::getline(fin,line);
  cout<<line<<endl;

  while(fin.good()){
    fin>>evtId>>hitId>>chipId>>posX>>posY>>hitADC>>HitNofDigi>>trPosX>>trPosY>>edep>>TrNofDigi;
    fin.seekg(1,ios::cur);
    if(!fin.good()) break;

    if(nlines%1000==0) cout<<" posX: "<<posX<<" trPosX: "<<trPosX<<endl;
    double dx = posX-trPosX;
    double dy = posY-trPosY;

    if(hitADC>THR) {
      digiHisHit->Fill(posX,posY);
      hdx->Fill(dx);
      hdy->Fill(dy);
    }
    truthHisHit->Fill(trPosX,trPosY);

    nlines++;
  }

  TCanvas* c1 = new TCanvas("c1","c1",1440,720);
  c1->Divide(2,1);
  c1->cd(1);
  hdx->Draw();
  //hdx->Fit("gaus");
  c1->cd(2);
  hdy->Draw();
  //hdy->Fit("gaus");
  TCanvas* c2 = new TCanvas("c2","c2",1440,720);
  c2->Divide(2,1);
  c2->cd(1);
  digiHisHit->Draw("colz");
  c2->cd(2);
  truthHisHit->Draw("colz");

  string rootfilename=filename+".root";
  c2->SaveAs(rootfilename.c_str());
}
