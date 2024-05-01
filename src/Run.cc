#include "Run.hh"

MyRunAction::MyRunAction() {
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->CreateNtuple("T_Energy", "T_Energy");
    man->CreateNtupleDColumn("E");
    man->FinishNtuple(0);
}

MyRunAction::~MyRunAction()
{}

// Function to create an output file with a progressive index
void MyRunAction::BeginOfRunAction(const G4Run* run) {
    
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    
    // Progressive index of the output .root file
    G4int runID = run->GetRunID();
    std::stringstream strRunID; // Convert id to string
    strRunID << runID;
    
    man->OpenFile("output"+strRunID.str()+".root");
    
}

// Close .root file
void MyRunAction::EndOfRunAction(const G4Run*) {
    
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    
    man->Write();
    man->CloseFile();
    
}
