#include "Construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    // Define the materials once for all
    DefineMaterials();
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

// Function used to define all the materials of the simulation
void MyDetectorConstruction::DefineMaterials()
{
    // Geant4 material manager
    G4NistManager* nist = G4NistManager::Instance();
    
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    // G4Material* worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    
    // G4Material* graphite = nist->FindOrBuildMaterial("G4_GRAPHITE");
    
    // Plasma as mixture of 90% D and 10% T
    G4Material* plasma = new G4Material("plasma", (5*0.0001)*g/m3,1);
    plasma->AddElement(nist->FindOrBuildElement("H"), 1); // Deuterium
    // plasma->AddElement(nist->FindOrBuildElement(""), ); // Tritium
    
    // Superconductor niobium tin: Nb3Sn
    // https://www.americanelements.com/niobium-tin-alloy-12035-04-0
    Nb3Sn = new G4Material("Nb3Sn", 6*g/cm3, 2);
    Nb3Sn->AddElement(nist->FindOrBuildElement("Nb"), 3);
    Nb3Sn->AddElement(nist->FindOrBuildElement("Sn"), 1);
    
    // Incoloy908: alloy of nickel iron
    // https://www.azom.com/article.aspx?ArticleID=9505
    Incoloy908 = new G4Material("Incoloy908", 8.17*g/cm3, 4);
    Incoloy908->AddElement(nist->FindOrBuildElement("Ni"), 50*perCent);
    Incoloy908->AddElement(nist->FindOrBuildElement("Fe"), 40*perCent);
    Incoloy908->AddElement(nist->FindOrBuildElement("Cr"), 5*perCent);
    Incoloy908->AddElement(nist->FindOrBuildElement("Nb"), 5*perCent);
    
    // SS316 (Stainless Steel with Molybdenum)
    // https://www.iqsdirectory.com/articles/stainless-steel/stainless-steel-316.html
    SS316 = new G4Material("SS316", 8*g/cm3,4);
    SS316->AddElement(nist->FindOrBuildElement("Fe"), 70*perCent);
    SS316->AddElement(nist->FindOrBuildElement("Cr"), 17*perCent);
    SS316->AddElement(nist->FindOrBuildElement("Ni"), 11*perCent);
    SS316->AddElement(nist->FindOrBuildElement("Mo"), 2*perCent);
    
}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
    
    // Dimensions of the world mother volume
    G4double xWorld = 10.0*m;
    G4double yWorld = 10.0*m;
    G4double zWorld = 10.0*m;
    
    // World solid volume
    G4Box* solidWorld = new G4Box("World", xWorld, yWorld, zWorld);
    
    
    // Logical volume of the world: comprises the material
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    
    // World Physical volume: it is the ONLY which must be PVPlacement
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    
    // Plasma torus
    G4Torus* torus = new G4Torus("torus", 0*cm, 100*cm, 400*cm, 0*deg, 360*deg);
    
    G4LogicalVolume* logicTorus = new G4LogicalVolume(torus, plasma, "logicTorus");
    G4RotationMatrix* rot = new G4RotationMatrix;
    rot->rotateX(90*deg);
    G4VPhysicalVolume* physTorus = new G4PVPlacement(rot, G4ThreeVector(0., 0., 0.), logicTorus, "physTorus", logicWorld, false, 0, true);
    
    // Central Solenoid
    G4Tubs* CS = new G4Tubs("CS1", 80*cm, 200*cm, 600*cm, 0*deg, 360*deg);
    G4LogicalVolume* logicCS = new G4LogicalVolume(CS, CSMat, "logicCS");
    G4VPhysicalVolume* physCS = new G4PVPlacement(rot, G4ThreeVector(0,0,0), logicCS, "physCS", logicWorld, false, 0, true);
    
    // Tokamak 1st wall
    G4Sphere* wall = new G4Sphere("wall", 620*cm, 630*cm, 0*deg, 360*deg, 0*deg, 180*deg);
    G4Material* wallMat = nist->FindOrBuildMaterial("G4_Li");
    G4LogicalVolume* logicWall = new G4LogicalVolume(wall, wallMat, "logicWall");
    G4VPhysicalVolume* physWall = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicWall, "physWall", logicWorld, false, 0, true);
    
    return physWorld;
}
