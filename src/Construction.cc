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
    
    worldMat = nist->FindOrBuildMaterial("G4_AIR");
    // G4Material* worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    
    // G4Material* graphite = nist->FindOrBuildMaterial("G4_GRAPHITE");
    
    // Plasma as mixture of 90% D and 10% T
    plasma_mat = new G4Material("plasma_mat", (5*0.0001)*g/m3,1);
    plasma_mat->AddElement(nist->FindOrBuildElement("H"), 1); // Deuterium
    // plasma_mat->AddElement(nist->FindOrBuildElement(""), ); // Tritium
    
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
    
    // Aluminum oxide Al2O3
    Al2O3 = new G4Material("Al2O3", 3.95*g/cm3, 2);
    Al2O3->AddElement(nist->FindOrBuildElement("Al"), 2);
    Al2O3->AddElement(nist->FindOrBuildElement("O"), 3);
    
    // Tungsten for 1st wall
    W = new G4Material("W", 19.3*g/cm3, 1);
    W->AddElement(nist->FindOrBuildElement("W"), 1);
    
    // Central solenoid - density is obtained as harmonic weighted mean
    CS_mat = new G4Material("CS1", 6.72*g/cm3, 3);
    CS_mat->AddMaterial(Nb3Sn, 45*perCent);
    CS_mat->AddMaterial(Incoloy908, 50*perCent);
    CS_mat->AddMaterial(Al2O3, 5*perCent);
    
    
    // Fist wall of tungsten/Lithium
    // Wall1_mat = new G4Material();
    Wall_mat = nist->FindOrBuildMaterial("G4_Li");
    
    // Heat sink of 1st wall, made of CuCrZr
    // file:///C:/Users/User/Downloads/PNA%20372%20-%20CuCrZr-C18160_EN-1.pdf
    HeatSink_mat = new G4Material("HeatSink_mat", 8.9*g/cm3, 3);
    HeatSink_mat->AddElement(nist->FindOrBuildElement("Cu"), 98.8*perCent);
    HeatSink_mat->AddElement(nist->FindOrBuildElement("Cr"), 1*perCent);
    HeatSink_mat->AddElement(nist->FindOrBuildElement("Zr"), 0.2*perCent);
    
    // Concrete for Bioshield
    Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");
    
}

// Construction of all the components of the tokamak
G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
    
    // Dimensions of the world mother volume
    G4double xWorld = 20.0*m;
    G4double yWorld = 20.0*m;
    G4double zWorld = 20.0*m;
    
    G4double alpha = 270; // Revolution Angle
    
    // World solid volume
    G4Box* solidWorld = new G4Box("World", xWorld, yWorld, zWorld);
    
    
    // World logical volume: comprises the material
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    
    // World Physical volume
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    
    // Plasma torus
    G4Torus* plasma = new G4Torus("plasma", 0*cm, 225.5*cm, 627*cm, 0*deg, alpha*deg);
    
    G4LogicalVolume* logicPlasma = new G4LogicalVolume(plasma, plasma_mat, "logicPlasma");
    // Rotation matrix for circular elements
    G4RotationMatrix* rot = new G4RotationMatrix;
    rot->rotateX(90*deg);
    G4VPhysicalVolume* physPlasma = new G4PVPlacement(rot, G4ThreeVector(0., 0., 0.), logicPlasma, "physPlasma", logicWorld, false, 0, true);
    
    // Central Solenoid
    G4Tubs* CS = new G4Tubs("CS1", 80*cm, 200*cm, 1400*cm, 0*deg, alpha*deg);
    G4LogicalVolume* logicCS = new G4LogicalVolume(CS, CS_mat, "logicCS");
    G4VPhysicalVolume* physCS = new G4PVPlacement(rot, G4ThreeVector(0,0,0), logicCS, "physCS", logicWorld, false, 0, true);
    
    // Tokamak 1st wall (Tungsten)
    G4Torus* Wall_1 = new G4Torus("Wall_1", 226*cm, 227*cm, 627*cm, 0*deg, (alpha+40)*deg);
    G4LogicalVolume* logicWall_1 = new G4LogicalVolume(Wall_1, W, "logicWall_1");
    G4VPhysicalVolume* physWall = new G4PVPlacement(rot, G4ThreeVector(0, 0, 0), logicWall_1, "physWall", logicWorld, false, 0, true);
    
    // 1st Wall heat sink
    G4Torus* Wall_1_HeatSink = new G4Torus("Wall_1_HeatSink", 227*cm, 229*cm, 627*cm, 0*deg, (alpha+30)*deg);
    G4LogicalVolume* logicWall_1_HeatSink = new G4LogicalVolume(Wall_1_HeatSink, HeatSink_mat, "logicWall_1_HeatSink");
    G4VPhysicalVolume* physWall_HeatSink = new G4PVPlacement(rot, G4ThreeVector(0, 0, 0), logicWall_1_HeatSink, "physWall", logicWorld, false, 0, true);
    
    // Blanket shield block
    G4Torus* BLK_shield = new G4Torus("BLK_shield", 229*cm, 271*cm, 627*cm, 0*deg, (alpha+20)*deg);
    G4LogicalVolume* logicBLK_shield = new G4LogicalVolume(BLK_shield, SS316, "logicBLK_shield");
    G4VPhysicalVolume* physBLK_shield = new G4PVPlacement(rot, G4ThreeVector(0, 0, 0), logicBLK_shield, "physWall", logicWorld, false, 0, true);
    
    
    // Vacuum Vessel (VV)
    G4Torus* VV = new G4Torus("VV", 270*cm, 305.7*cm, 628.5*cm, 0*deg, (alpha+10)*deg);
    G4LogicalVolume* logicVV = new G4LogicalVolume(VV, SS316, "logicVV");
    G4VPhysicalVolume* physVV = new G4PVPlacement(rot, G4ThreeVector(0,0,0), logicVV, "physVV", logicWorld, false, 0, true);
    
    // Toroidal Field Coils (TFC), which are OUTSIDE the VV
    G4Torus* TFC = new G4Torus("TFC", 307.1*cm, 407.1*cm, 627*cm, 0*deg, alpha*deg);
    G4LogicalVolume* logicTFC = new G4LogicalVolume(TFC, SS316, "logicTFC");
    G4VPhysicalVolume* physTFC = new G4PVPlacement(rot, G4ThreeVector(0,0,0), logicTFC, "physTFC", logicWorld, false, 0, true);
    
    // Test: 18 TFCs
    // G4Tubs* TFCi = new G4Tubs("TFCi", 307.1*cm, 407.1*cm, );
    
    // Cryostat
    G4Tubs* Cryostat = new G4Tubs("Cryostat", 1358.2*cm, 1413.2*cm, 1413*cm, 0*deg, alpha*deg);
    
    G4LogicalVolume* logicCryostat = new G4LogicalVolume(Cryostat, SS316, "logicCryostat");
    
    G4VPhysicalVolume* physCryostat = new G4PVPlacement(rot, G4ThreeVector(0,0,0), logicCryostat, "physCryostat", logicWorld, false, 0, true);
    
    // Biological shield (concrete) lateral
    G4Tubs* Bioshield = new G4Tubs("Bioshield", 1413.2*cm, 1613*cm, 1413*cm, 0*deg, alpha*deg);
    
    // G4Sphere* Bioshield = new G4Sphere("Bioshield", 1413.2*cm, 1613*cm, 0*deg, alpha*deg, 0*deg, 180*deg);
    G4LogicalVolume* logicBioshield = new G4LogicalVolume(Bioshield, Concrete, "logicBioshield");
    
    G4VPhysicalVolume* physBioshield = new G4PVPlacement(rot, G4ThreeVector(0,0,0), logicBioshield, "physBioshield", logicWorld, false, 0, true);
    
    // Bioshield up
    G4Tubs* Bioshield_up = new G4Tubs("Bioshield_up", 0*cm, 1613*cm, 100*cm, 0*deg, alpha*deg);
    
    G4LogicalVolume* logicBioshield_up = new G4LogicalVolume(Bioshield_up, Concrete, "logicBioshield_up");
    
    G4VPhysicalVolume* physBioshield_up = new G4PVPlacement(rot, G4ThreeVector(0,1513*cm, 0), logicBioshield_up, "physBioshield_up", logicWorld, false, 0, true);
    
    // Bioshield down
    G4Tubs* Bioshield_down = new G4Tubs("Bioshield_down", 0*cm, 1613*cm, 100*cm, 0*deg, alpha*deg);
    
    G4LogicalVolume* logicBioshield_down = new G4LogicalVolume(Bioshield_down, Concrete, "logicBioshield_down");
    
    G4VPhysicalVolume* physBioshield_down = new G4PVPlacement(rot, G4ThreeVector(0,-1513*cm, 0), logicBioshield_down, "physBioshield_down", logicWorld, false, 0, true);
    
    return physWorld;
}
