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

#ifndef MimosaGeoParameter_H
#define MimosaGeoParameter_H 


#include <string>
#include <vector>

using namespace std;

class MimosaChip{
public:
  MimosaChip(void){};
//  MimosaChip();
  ~MimosaChip(void){};

//get method
  int RowNo() const {return nPixelR;};
  int ColNo() const {return nPixelC;};
//  int64_t PixNo() const {return RowNo()*ColNo();};
  int ADCBitNo() const {return adcBitNo;};
  int ADCRange() const {return adcRange;};
  double CCE() const {return cce;};
  double ENC() const {return enc;};
  int DigiMethod() const {return _digiMethod;};
  unsigned int PixNo() const {return RowNo()*ColNo();};
  double R() const {return radius;};
  double Phi() const {return chipPhi;};
  double PitchX() const {return pixPitchX;};
  double PitchY() const {return pixPitchY;};
  double DiodeOffsetX() const{return diodeOffsetX;};
  double DiodeOffsetY() const{return diodeOffsetY;};
  double SensorThick() const {return sensorTh;};
  double SubstrateThick() const {return substrTh;};
  double AsicThick() const {return asicTh;};
  double ChipWidth() const {return chipWidth;};
  double ChipLength() const {return chipLength;};
  const string LayerName(void) const {return layerName;};

//set method
  void RowNo(int nRow) { nPixelR=nRow;};
  void ColNo(int nCol) { nPixelC=nCol;};
  void ADCBitNo(int nBit) { adcBitNo=nBit;};
  void ADCRange(int range) { adcRange=range;};
  void CCE(double c) { cce=c;};
  void ENC(double e) { enc=e;};
  void DigiMethod(int dm) { _digiMethod=dm;};
  void R(double r) { radius=r;};
  void Phi(double phi) { chipPhi=phi;};
  void PitchX(double px) { pixPitchX=px;};
  void PitchY(double py) { pixPitchY=py;};
  void DiodeOffsetX(double offsetX) { diodeOffsetX=offsetX;};
  void DiodeOffsetY(double offsetY) { diodeOffsetX=offsetY;};
  void SensorThick(double th) { sensorTh=th;};
  void SubstrateThick(double th) { substrTh=th;};
  void AsicThick(double th) { asicTh=th;};
  void ChipWidth(double size) { chipWidth=size;};
  void ChipLength(double size) { chipLength=size;};
  void SetLayerName(string nam){layerName=nam;};

private:
  int nPixelR, nPixelC;

  int adcBitNo;
  int adcRange;
  double cce; //charge collection efficiency
  double enc; //noise level
  int _digiMethod; //digitiz method

  double radius;
  double chipPhi;  //phi

  //Pitch of pixel
  double pixPitchX, pixPitchY;

  //Diode offset
  double diodeOffsetX, diodeOffsetY;

  //Thickness of sensor layer
  double sensorTh, substrTh, asicTh;

  double chipWidth, chipLength;
  
  string layerName;
};

class MimosaLadder : public MimosaChip{
public:
  MimosaLadder(void){};
  ~MimosaLadder(void){};

  //get method
  int ChipNo() const {return nChip;};
//  int64_t LadderPixNo() const {return nChip*PixNo();};
  unsigned int LadderPixNo() const {return nChip*PixNo();};
  double LadderWidth() const {return supportW;};
  double LadderLength() const {return supportL;};
  double EpoxyThick() const {return epoxyThick;};
  double KaptonThick() const {return kptThick;};
  double MetalFoilThick() const {return mfThick;};
  double CarbonFiberThick() const {return cfThick;};

  inline double TotalThick();
  double RCenter(); //radius of ladder center

  //set method
  void ChipNo(int nc) {nChip=nc;};
  void LadderWidth(double w) {supportW=w;};
  void LadderLength(double l) {supportL=l;};
  void EpoxyThick(double t) {epoxyThick=t;};
  void KaptonThick(double t) {kptThick=t;};
  void MetalFoilThick(double t) {mfThick=t;};
  void CarbonFiberThick(double t) {cfThick=t;};

private:
  int nChip;
  double supportW; //length of support structure
  double supportL;
  double epoxyThick;
  double kptThick;
  double mfThick;
  double cfThick;
};

class MimosaLayer : public MimosaLadder{
public:
  MimosaLayer(void){};
  ~MimosaLayer(void){};

  //get method
  int LadderNo() const {return nLadder;};
  int LayerChipNo() const {return nLadder*ChipNo();};
//  int64_t LayerPixNo() const {return LayerChipNo()*PixNo();};
  unsigned int LayerPixNo() const {return LayerChipNo()*PixNo();};
  double PhaseAngle() const {return phaseAngle;};

  //set method
  void LadderNo(int nL) {nLadder=nL;};
  void PhaseAngle(double angle) {phaseAngle=angle;};
  
private:
  int nLadder;
  double phaseAngle;
};


class MimosaMember{
  //class used for construct tube and endplane in MC
  public:
    MimosaMember(void){};
    ~MimosaMember(void){};

  public:
    double Length(void){return fLength;} //Full length In Z direction
    double InnerR(void)     {return fInnerR;} //From Z axes to bottom surface of those segments.
    double OutR(void) {return fOutR;}  //From Z axes to upper surface of those segments.
    double Z(void) {return fZ;}  //Offset value of segment center from Z=0 
    string Name(void) {return fName;} //Name of segment

  public:
    void Length(double x) {fLength=x;}
    void InnerR(double x) {fInnerR=x;}
    void OutR(double x)  {fOutR=x;}
    void Z(double x)  {fZ=x;}
    void Name(string x) {fName=x;}
  private:
    double fLength, fInnerR, fOutR, fZ;
    string fName;
};

class MimosaGeoParameter{
public:
  MimosaGeoParameter(void){
//    Initialize();
    InitFromFile();
  };
  ~MimosaGeoParameter(void){};

  void Initialize(void);
  void InitFromFile(void);  //Get MimosaGeoParameter from Spt.txt
//  void InitFromSvc(void);  //Get MimosaGeoParameter from SptGeomSvc
  void Dump(void);  //Output info

  //get method
  int LayerNo() const {return layerNo;};
  const MimosaLayer& Layer(int layerId) const;

  int SptSegmentNo(void) {return fSptSegmentNo;}
  MimosaMember SptSegment(int x) {return fSptSegment[x];}

  static MimosaGeoParameter* GetGeo(void);

private:
  int layerNo;
  MimosaLayer fLayer[8];

  int fSptSegmentNo; //Total number of Tube and Endplane segments.
  MimosaMember fSptSegment[100];

  static  MimosaGeoParameter * fSptGeo;

};

#endif
