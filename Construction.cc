#include "Construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
    // Geant4 material manager
    G4NistManager* nist = G4NistManager::Instance();
    
    // Dimensions of the world mother volume
    G4double xWorld = 1.0*m;
    G4double yWorld = 1.0*m;
    G4double zWorld = 1.0*m;
    
    // World solid volume
    G4Box* solidWorld = new G4Box("World", xWorld, yWorld, zWorld);
    
    // G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    
    // Logical volume of the world: comprises the material
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    
    // World Physical volume: it is the ONLY which must be PVPlacement
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    
    // Plasma torus
    G4Torus* torus = new G4Torus("torus", 0*cm, 20*cm, 40*cm, 0*deg, 360*deg);
    // G4Material* graphite = nist->FindOrBuildMaterial("G4_GRAPHITE");
    
    G4Material* plasma = new G4Material("plasma", (5*10^-4)*g/m3,2);
    
    G4LogicalVolume* logicTorus = new G4LogicalVolume(torus, graphite, "logicTorus");
    G4RotationMatrix* rot = new G4RotationMatrix;
    rot->rotateX(90*deg);
    G4VPhysicalVolume* physTorus = new G4PVPlacement(rot, G4ThreeVector(0., 0., 0.), logicTorus, "physTorus", logicWorld, false, 0, true);
    
    return physWorld;
}
