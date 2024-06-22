#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include <cmath>
// Include Geant4 libraries
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Torus.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"

// Used to build the Sensitive Detector (SD)
#include "Detector.hh"

// Class called in main() to construct the whole world, detectors and shapes
class MyDetectorConstruction : public G4VUserDetectorConstruction {
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    
    // Set the geometrical revolution angle to visualize the tokamak
    void Set_angle(G4double angle);
    
    // Get the SensitiveDetector to obtain the Tritium atoms generated
    MySensitiveDetector* Get_SD();
    
private:
    
    // Revolution Angle (to see tokamak's interior)
    G4double alpha; 
    
    // Materials of the various components
    G4Material *worldMat, *SS316, *plasma_mat, *PbLi, *Concrete, *CS_mat, *BLK_breeder_mat, *HeatSink_mat, *W, *Filling_mat;
    
    // Used to define the SensitiveDetector to detect Tritium breeding
    G4LogicalVolume* logicBLK_breeder;
    
    void DefineMaterials();
    
    // Method used to build the Sensitive Detector (SD) which collects and processes hits (Tritium production)
    virtual void ConstructSDandField();
    
    // SensitiveDetector used to track particles and tritium generation
    MySensitiveDetector* sensDet;
    
};

#endif
