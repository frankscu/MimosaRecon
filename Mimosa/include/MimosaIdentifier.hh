
#ifndef MimosaIdentifier_h
#define MimosaIdentifier_h 1

#include "MimosaGeoParameter.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

class MimosaIdentifier
{
public:
  MimosaIdentifier(void){};
  MimosaIdentifier(int layer,int ladder,int chip,int column,int row);
  ~MimosaIdentifier(void){};

  //get method
  int LayerID() const {return m_layerId;};
  int LadderID() const {return m_ladderId;};
  int ChipID() const {return m_chipId;};
  int ColID() const {return m_columnId;};
  int RowID() const {return m_rowId;};
//  int64_t PixelID() const {return m_pixelId;};
  int GlobalChipID() const {return m_globalChipId;};
  unsigned int PixelID() const {return m_pixelId;};
  void WhichPixel(double posX, double posY);

  G4ThreeVector PixelPos();
  std::vector<G4ThreeVector> PixelDiode();

  void Print();

private:
  int m_layerId,m_ladderId,m_chipId,m_columnId,m_rowId;
  std::vector<G4ThreeVector> posDiode;
//  int64_t m_pixelId;
  unsigned int m_pixelId;
  int m_globalChipId;
  MimosaGeoParameter* mimosaGeo;
};

#endif

