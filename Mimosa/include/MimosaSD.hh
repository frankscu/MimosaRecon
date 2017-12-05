#ifndef MimosaAnalysis_h
#define MimosaAnalysis_h 1

#include "MimosaSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "MimosaHit.hh"
#include "MimosaGeoParameter.hh"
#include "MimosaIdentifier.hh"

class G4Step;
class G4TouchableHistory;
class G4HCofThisEvent;
class MimosaGeoParameter;
class MimosaIdentifier;

class MimosaSD : public G4VSensitiveDetector
{
    public:

        MimosaSD(const G4String&);
        ~MimosaSD();

        void Initialize(G4HCofThisEvent* HCE);
        G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent* );

    private:
        MimosaHitsCollection* hitsCollection;
        MimosaHitsCollection* truthCollection;
        MimosaGeoParameter* mimosaGeoPointer;
        G4String SensitiveDetectorName;
        std::map<G4int, G4int> hitMap; //FIX ME
        std::map<G4int, G4int> truthMap; //FIX ME
        std::map<G4int, G4int>::iterator itTruthMap; //FIX ME

};

#endif
