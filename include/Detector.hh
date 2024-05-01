// This file is used to create the SENSITIVE DETECTORS that can SCORE HITS
// This file is used to COUNT the number of tritium atoms generated, according to the breeding reaction: n+Li6->He4+T3

#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include <iostream>
#include <fstream>

class MySensitiveDetector : public G4VSensitiveDetector {
    
public:
    MySensitiveDetector(G4String);
    ~MySensitiveDetector();
    
    // Method used to count Tritium breeding reactions.
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    
private:
    
    int N_Tritium = 0; // Count the number of T breeding reactions
    
};


#endif
