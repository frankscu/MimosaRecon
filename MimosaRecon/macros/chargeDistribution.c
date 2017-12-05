#include <iostream>
#include <fstream>
#include "string"
#include <algorithm>
#include "stdlib.h"
TH1F* HIron();

void chargeDistribution(){

  TH1F* hdx = new TH1F("hdx","hdx",100,-0.05,0.05);
  TH1F* hdy = new TH1F("hdy","hdy",100,-0.05,0.05);

  TH1F* hisADC = new TH1F("hisADC","hisADC",3000,0,28000);
  string line;
  int evtId,hitId,chipId,HitNofDigi,TrNofDigi;
  double posX,posY,hitADC,trPosX,trPosY,edep;
  int nlines;
  double THR=0;
  double pedestal=4000;
  double distance[20044],hADC[20044];
  fstream fin("/Users/chenliejian/Documents/Code/MimosaDigitizer/AnaMimosa/MimosaRecon/data/Jadepix_Iron55.recdat",ios::in);
  std::getline(fin,line);
  cout<<line<<endl;

  while(fin.good()){
    fin>>evtId>>hitId>>chipId>>posX>>posY>>hitADC>>HitNofDigi>>trPosX>>trPosY>>edep>>TrNofDigi;
    fin.seekg(1,ios::cur);
    if(!fin.good()) break;

    //cout<<" posX: "<<posX<<" trPosX: "<<trPosX<<endl;
    double dx = posX-trPosX;
    double dy = posY-trPosY;

    nlines++;
    distance[nlines] = 1e3*sqrt(dx*dx+dy*dy);
    hADC[nlines] = hitADC;
    hdx->Fill(dx);
    hdy->Fill(dy);
    if(hitADC>THR) hisADC->Fill(hitADC+pedestal);
    cout << nlines << "  " << distance[nlines] << "  " << hADC[nlines] << endl;
  }
  fin.close();

  TCanvas* c1 = new TCanvas("c1","c1",1440,720);
  c1->Divide(2,1);
  c1->cd(1);
  hdx->Draw();
  //hdx->Fit("gaus");
  c1->cd(2);
  hdy->Draw();
  //hdy->Fit("gaus");
  TCanvas* c2 = new TCanvas("c2","c2",600,600);
  TGraph *tg = new TGraph(double(nlines),distance,hADC);
  tg->Draw("AP");

  TCanvas* c3 = new TCanvas("c3","c3",600,600);
  hisADC->Draw();

  /* TCanvas* c4 = new TCanvas("c4","c4",600,600); */
  /* TH1F* hIron; */
  /* hIron=HIron(); */
  /* hIron->Draw(); */

  /* TCanvas* c5 = new TCanvas("c5","c5",600,600); */
  /* gStyle->SetOptStat(0); */
  /* gStyle->SetOptTitle(0); */
  /* c5->SetLogy(); */
  /* hIron->SetFillColor(kBlue); */
  /* hIron->SetLineColor(kBlue); */
  /* hIron->GetXaxis()->SetRangeUser(0,28000); */
  /* hIron->DrawNormalized("HIST"); */

  hisADC->SetFillColor(kYellow);
  hisADC->SetLineColor(kYellow);
  TGaxis::SetMaxDigits(3);
  hisADC->DrawNormalized("HIST,same");

  TLegend* lg= new TLegend(0.5,0.7,0.9,0.9);
  lg->AddEntry(hisADC,"Simulation","F2");
  //lg->AddEntry(hIron,"Data","F2");
  lg->Draw();
  /* c5->SaveAs("Spectrum.root"); */
  /* c5->SaveAs("Spectrum.png"); */
}

TH1F* HIron(){
  double ironADC;
  double preAmpGain=20; // 20 e-/mV
  double factor=20;
  string csvline;

  TH1F* hisIron = new TH1F("hisIron","hisIron",150*factor,0,70*preAmpGain*factor);

  fstream csvfin("/Users/chenliejian/Documents/Code/MimosaDigitizer/AnaMimosa/MimosaRecon/data/OverMOS1A_AC_LargePixels.csv",ios::in);
  std::getline(csvfin,csvline);
  cout<<csvline<<endl;
  while(csvfin.good()){
    csvfin>>ironADC;
    csvfin.seekg(1,ios::cur);
    if(!csvfin.good()) break;
    hisIron->Fill(ironADC*preAmpGain*factor);
  }
  return hisIron;
}
