#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

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

// Class called in main() to construct the whole world, detectors and shapes
class MyDetectorConstruction : public G4VUserDetectorConstruction {
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    
private:
    
    // Basic material
    G4Material *worldMat, *Nb3Sn, *Incoloy908, *SS316, *Al2O3, *plasma_mat, *Concrete;
    
    // Materials for tokamak components
    G4Material *CS_mat, *Wall_mat;
    G4Element *W;
    void DefineMaterials();
    
};

#endif
