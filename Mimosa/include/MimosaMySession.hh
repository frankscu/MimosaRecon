#ifndef MimosaMySession_H
#define MimosaMySession_H 1

#include "G4UIsession.hh"
#include "fstream"
#include "iostream"

#pragma clang diagnostic ignored "-Woverloaded-virtual"

class MimosaMySession : public G4UIsession
{
    public:
        ofstream logFile, logFile1;
        virtual G4int ReceiveG4cout(G4String coutString);
        virtual G4int ReceiveG4cerr(G4String cerrString);
        
        MimosaMySession(){
            logFile.open("results.log", ios::app);
            logFile1.open("errors.log", ios::app);
        }
        
        ~MimosaMySession(){
            logFile.close();
            logFile1.close();
        }
};

G4int MimosaMySession::ReceiveG4cout(G4String coutString)
{
    logFile << coutString << flush;
    return 0;
}

G4int MimosaMySession::ReceiveG4cerr(G4String cerrString)
{
    logFile1 << cerrString << flush;
    return 0;
}
#endif
