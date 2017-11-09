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

#include <iostream>
#include <fstream>

#include "MGeo.h"
#include <math.h>
#include <cstdlib>

MGeo * MGeo::fMimosaGeo=0;

MGeo * MGeo::GetGeo(void){
      if (! fMimosaGeo) fMimosaGeo = new MGeo();
           return fMimosaGeo;
	   }

void MGeo::Initialize(void){
  layerNo=1;
}

void MGeo::InitFromFile(){

  std::string geometryFilePath = "/Users/chenliejian/Documents/Code/MimosaDigitizer/Mimosa";
  geometryFilePath += "/model/Spt.txt";
  std::ifstream fin(geometryFilePath.c_str());

  if(!fin.good()){
    cout<<"Error::spt parameters file not exist"<<endl;
    return;
  }

  int rowNo, colNo;
  double chipL,chipW,asicTh,sensorTh,substrTh,pitchR,pitchC,diodeOffsetX,diodeOffsetY;
  int ladderNo,chipNo;
  double ssL,ssW,epoxyT,kptT,mfT,cfT,_R,_Phi,_PhaseAngle;
  double segInnR,segOutR,segL,segZ;

  string name,line;

  std::getline(fin,line);
  fin>>layerNo;
  cout<<"Layer Number: "<<layerNo<<endl;



  fin.seekg(1,ios::cur);
  std::getline(fin, line);
  fin.seekg(1,ios::cur);
  std::getline(fin, line);
  fin.seekg(1,ios::cur);
  std::getline(fin, line);

  fin>>chipL>>chipW>>asicTh>>sensorTh>>substrTh>>pitchR>>pitchC>>diodeOffsetX>>diodeOffsetY>>rowNo>>colNo;
  cout<<"L: "<<chipL<<" W: "<<chipW<<" asTh: "<<asicTh<<" senTh: "<<sensorTh<<" subTh: "<<substrTh<<" pitchR: "<<pitchR<<" pitchC: "<<pitchC<<" rowNo: "<<rowNo<<" colNo: "<<colNo<<endl;

  fin.seekg(1,ios::cur);
  getline(fin, line);
  getline(fin, line);
  getline(fin, line);

  fin>>ssW>>epoxyT>>kptT>>mfT>>cfT;
  cout<<ssW<<"\t"<<epoxyT<<"\t"<<kptT<<"\t"<<mfT<<"\t"<<cfT<<endl;

  fin.seekg(1,ios::cur);
  getline(fin, line);
  getline(fin, line);
  getline(fin, line);

  for(int i=0; i<layerNo; i++){
    fin>>name>>ladderNo>>ssL>>chipNo>>_R>>_Phi>>_PhaseAngle;
    cout<<name<<"\t"<<ladderNo<<"\t"<<ssL<<"\t"<<chipNo<<"\t"<<_R<<"\t"<<_Phi<<"\t"<<_PhaseAngle<<endl;
    getline(fin, line);


  //Consistent
    double activeSizeX=rowNo*pitchR *um;
    double phAngle=_PhaseAngle *deg;
    double chipR=_R *mm;
    double segmentSpanAngle = activeSizeX*cos(phAngle)/chipR;
    int nofLad=ceil(twopi/segmentSpanAngle);
    segmentSpanAngle = twopi/ladderNo;
    double conPhA=acos(segmentSpanAngle*chipR/activeSizeX)/deg;
    cout<<"Consistent Config::Least Ladder="<<nofLad<<"\t"<<"PhaseAngle="<<conPhA<<endl;


  //Chip
    fLayer[i].RowNo(rowNo);
    fLayer[i].ColNo(colNo);
    fLayer[i].PitchX(pitchC);  //*um
    fLayer[i].PitchY(pitchR);  //*um
    fLayer[i].ChipLength(chipL);   //*mm
    fLayer[i].ChipWidth(chipW);   //*mm
    fLayer[i].AsicThick(asicTh);     //*um
    fLayer[i].SensorThick(sensorTh);     //*um
    fLayer[i].SubstrateThick(substrTh);     //*um
    fLayer[i].R(_R);	      //*mm
    fLayer[i].Phi(_Phi);	      
    //fLayer[i].SetLayerName(name);
  //Ladder
    fLayer[i].ChipNo(chipNo);    
    fLayer[i].LadderLength(ssL);   //*mm
    fLayer[i].LadderWidth(ssW);   //*mm
    fLayer[i].EpoxyThick(epoxyT);    //*mm
    fLayer[i].KaptonThick(kptT);    //*mm
    fLayer[i].MetalFoilThick(mfT);    //*mm
    fLayer[i].CarbonFiberThick(cfT);    //*mm
  //Layer
    fLayer[i].LadderNo(ladderNo);
    fLayer[i].PhaseAngle(_PhaseAngle);	//*deg

  }

  fin.seekg(1,ios::cur);
  getline(fin, line);
  getline(fin, line);
  fin>>fMimosaSegmentNo;
  cout<<"MimosaSegment Number: "<<fMimosaSegmentNo<<endl;

  fin.seekg(1,ios::cur);
  getline(fin, line);
  getline(fin, line);

  for(int j=0; j<fMimosaSegmentNo; j++){
    fin>>name>>segInnR>>segOutR>>segL>>segZ;
    cout<<name<<"\t"<<segInnR<<"\t"<<segOutR<<"\t"<<segL<<"\t"<<segZ<<endl;
    getline(fin, line);

    fMimosaSegment[j].InnerR(segInnR);
    fMimosaSegment[j].OutR(segOutR);
    fMimosaSegment[j].Length(segL);
    fMimosaSegment[j].Z(segZ);
  }

  fin.close();

}

const MimosaLayer& MGeo::Layer(int layerId) const {
  if(layerId<0 || layerId>=layerNo){
  cout<<"MGeo::Error: Wrong layer No"<<endl;
  }
  return fLayer[layerId];
}

void MGeo::Dump(void){
  cout<<""<<endl;
  cout<<"***************** MGeo Information *******************"<<endl;
  cout<<"Layer No: "<<layerNo<<endl;
  for(int i=0;i<layerNo;++i){
    cout<<"Layer["<<i<<"]: "
//	<<"Name: "<<fLayer[i].LayerName()
	<<"! Ladder Number: "<<fLayer[i].LadderNo()
	<<"! Chip Number: "<<fLayer[i].LayerChipNo()
	<<"! Pixel Number: "<<fLayer[i].LayerPixNo()<<endl;
    cout<<""<<endl;
  }
  cout<<"*******************************************************************"<<endl;
}
