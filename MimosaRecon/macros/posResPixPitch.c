#include <iostream>
#include <fstream>
#include "string"
#include <algorithm>
#include "stdlib.h"
#include "/home/xiuql/include/MyRoot.h"

void posResPixPitch(){

  SetStyle();

  TH1F* hdx = new TH1F("hdx","hdx",100,-0.05,0.05);
  TH1F* hdy = new TH1F("hdy","hdy",100,-0.05,0.05);

  string line;
  int evtId,hitId,chipId,HitNofDigi,TrNofDigi;
  double posX,posY,hitADC,trPosX,trPosY,edep;

  double pixPitch[7]={10,15,20,25,30,35,40};
  double adcBit[8]={0,1,2,3,4,5,6,7};
  double resXADC[7][8]={0};
  double resYADC[7][8]={0};
  double resXPitch[8][7]={0};
  double resYPitch[8][7]={0};

  const int adcNP=8;
  const int pitchNP=7;
  //const int adcNP=2;
  //const int pitchNP=2;

  char fileName[200];
  for(int i=0;i<pitchNP;i++){
    for(int j=0;j<adcNP;j++){
      sprintf(fileName,"/home/xiuql/Work/Geant4/AnaMimosa/RecData/pixPitchADC/raw_pitch_%d_adcBit_%d.recdat",int(pixPitch[i]),int(adcBit[j]));
      cout<<"Processing::"<<fileName<<endl;
      fstream fin(fileName,ios::in);
      std::getline(fin,line);
      //cout<<line<<endl;

      while(fin.good()){
        fin>>evtId>>hitId>>chipId>>posX>>posY>>hitADC>>HitNofDigi>>trPosX>>trPosY>>edep>>TrNofDigi;
        fin.seekg(1,ios::cur);
        if(!fin.good()) break;

        double dx = posX-trPosX;
        double dy = posY-trPosY;

        hdx->Fill(dx);
        hdy->Fill(dy);

      }
        
      hdx->Fit("gaus");
      hdy->Fit("gaus");

      resXADC[i][j] = hdx->GetFunction("gaus")->GetParameter(2)*1000;
      resYADC[i][j] = hdy->GetFunction("gaus")->GetParameter(2)*1000;

      resXPitch[j][i] = resXADC[i][j];
      resYPitch[j][i] = resYADC[i][j];
       
      cout<<" ResX: "<<resXADC[i][j]<<", ResY: "<<resYADC[i][j]<<endl;

      hdx->Reset();
      hdy->Reset();
    }
  }
  
  int color=0;
  char legText[200];
  
  TCanvas* c1 = new TCanvas("c1","c1",960,720);
  TH2F* hA = new TH2F("hA","",100,-0.5,7.5,100,0,8.5);
  hA->Draw();
  hA->GetXaxis()->SetTitle("Number of ADC Bit");
  hA->GetYaxis()->SetTitle("Spatial Resolution [um]");

  TLegend* legADC = new TLegend(0.6,0.5,0.9,0.9);
  for(int i=0;i<pitchNP;i++){
    color=i+1;
    if(color==5) color=46;
    TGraph* grADC = new TGraph(adcNP,adcBit,resXADC[i]);
    //TGraph* grADC = new TGraph(8,adcBit,resYADC[i]);
    grADC->Draw("PLSame");
    grADC->SetMarkerStyle(20+i);
    grADC->SetMarkerColor(color);
    grADC->SetLineColor(color);
    sprintf(legText,"Pixel Pitch: %d",int(pixPitch[i]));
    legADC->AddEntry(grADC,legText,"lp");
  }
  legADC->Draw("Same");
    
  TCanvas* c2 = new TCanvas("c2","c2",960,720);
  TLegend* legPitch = new TLegend(0.12,0.56,0.45,0.92);
  double pixResDigi[7];
  for(int i=0;i<pitchNP;i++){
    pixResDigi[i]=pixPitch[i]/TMath::Sqrt(12);
  }
  TGraph* grDigi = new TGraph(pitchNP,pixPitch,pixResDigi);
  grDigi->Draw("AL");
  grDigi->SetTitle("");
  grDigi->SetLineColor(9);
  grDigi->SetLineWidth(2);
  grDigi->SetLineStyle(2);
  grDigi->GetHistogram()->SetMaximum(12);
  grDigi->GetHistogram()->SetMinimum(0);
  grDigi->GetXaxis()->SetTitle("Pixel Pitch [um]");
  grDigi->GetYaxis()->SetTitle("Spatial Resolution [um]");
  legPitch->AddEntry(grDigi,"Theoritical digital resolution","l");

  for(int i=0;i<adcNP;i++){
    color=i+1;
    if(color==5) color=46;
    TGraph* grPitch = new TGraph(pitchNP,pixPitch,resXPitch[i]);
    //TGraph* grPitch = new TGraph(7,pixPitch,resYPitch[i]);
    grPitch->Draw("PLSame");
    grPitch->SetMarkerStyle(20+i);
    grPitch->SetMarkerColor(color);
    grPitch->SetLineColor(color);
    sprintf(legText,"ADC Bit: %d",int(adcBit[i]));
    legPitch->AddEntry(grPitch,legText,"lp");
  }
  legPitch->Draw("same");
}
