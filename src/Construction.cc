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
    
    // Plasma is practically vacuum
    plasma_mat = nist->FindOrBuildMaterial("G4_Galactic");
    
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
    
    
    // Breeding blanket materials
    // Liquid Lithium-Lead
    // https://www.sciencedirect.com/science/article/pii/S0920379618307361
    // The Li in the PbLi is enriched
    G4double enrichment = 50.0; // Li6 enrichment fraction
    // G4Isotope("name", z, n, a);
    G4Isotope* Li6 = new G4Isotope("Li6", 3, 6, 6.015*g/mole);
    G4Isotope* Li7 = new G4Isotope("Li7", 3, 7, 7.016*g/mole);
    G4Element* Li = new G4Element("Lithium", "Li", 2);
    Li->AddIsotope(Li6, enrichment*perCent);
    Li->AddIsotope(Li7, (100-enrichment)*perCent);
    
    PbLi = new G4Material("PbLi", 9.8*g/cm3, 2);
    PbLi->AddElement(nist->FindOrBuildElement("Pb"), 80*perCent);
    PbLi->AddElement(Li, 20*perCent);
    
    // Heat sink of 1st wall, made of CuCrZr
    // file:///C:/Users/User/Downloads/PNA%20372%20-%20CuCrZr-C18160_EN-1.pdf
    HeatSink_mat = new G4Material("HeatSink_mat", 8.9*g/cm3, 3);
    HeatSink_mat->AddElement(nist->FindOrBuildElement("Cu"), 98.8*perCent);
    HeatSink_mat->AddElement(nist->FindOrBuildElement("Cr"), 1*perCent);
    HeatSink_mat->AddElement(nist->FindOrBuildElement("Zr"), 0.2*perCent);
    
    
    
    // Concrete for Bioshield
    Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");
    
    // Homogeneous material for filling
    // This material is defined as an average of the most used materials in the ITER tokamak
    Filling_mat = new G4Material("Filling_mat", 7.5*g/cm3, 3);
    Filling_mat->AddMaterial(SS316, 75*perCent); // Steel
    Filling_mat->AddElement(nist->FindOrBuildElement("Cu"), 15*perCent); // Copper
    Filling_mat->AddMaterial(Concrete, 10*perCent);
    
}

// Construction of all the components of the tokamak
G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
    
    // Dimensions of the world mother volume
    G4double xWorld = 18.0*m;
    G4double yWorld = 18.0*m;
    G4double zWorld = 18.0*m;
    
    G4double alpha = 270; // Revolution Angle
    
    G4double R = 627; // Internal radius of tokamak
    
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
    
    // Central Solenoid, 13m high (650 cm half z)
    G4Tubs* CS = new G4Tubs("CS1", 80*cm, 200*cm, 650*cm, 0*deg, alpha*deg);
    G4LogicalVolume* logicCS = new G4LogicalVolume(CS, CS_mat, "logicCS");
    G4VPhysicalVolume* physCS = new G4PVPlacement(rot, G4ThreeVector(0,0,0), logicCS, "physCS", logicWorld, false, 0, true);
    
    // Tokamak 1st wall (Tungsten)
    G4Torus* Wall_1 = new G4Torus("Wall_1", 226*cm, 227*cm, 627*cm, 0*deg, (alpha+40)*deg);
    G4LogicalVolume* logicWall_1 = new G4LogicalVolume(Wall_1, W, "logicWall_1");
    G4VPhysicalVolume* physWall = new G4PVPlacement(rot, G4ThreeVector(0, 0, 0), logicWall_1, "physWall", logicWorld, false, 0, true);
    
    // 1st Wall heat sink
    G4Torus* Wall_1_HeatSink = new G4Torus("Wall_1_HeatSink", 227*cm, 229*cm, 627*cm, 0*deg, (alpha+30)*deg);
    G4LogicalVolume* logicWall_1_HeatSink = new G4LogicalVolume(Wall_1_HeatSink, HeatSink_mat, "logicWall_1_HeatSink");
    G4VPhysicalVolume* physWall_HeatSink = new G4PVPlacement(rot, G4ThreeVector(0, 0, 0), logicWall_1_HeatSink, "physWall_HeatSink", logicWorld, false, 0, true);
    
    
    // BREEDING BLANKET for TRITIUM PRODUCTION ////////////////////////
    // Blanket shield block
    //G4Torus* BLK_shield = new G4Torus("BLK_shield", 229*cm, 270*cm, 627*cm, 0*deg, (alpha+20)*deg);
    // Only steel blanket
    //G4LogicalVolume* logicBLK_shield = new G4LogicalVolume(BLK_shield, SS316, "logicBLK_shield");
    
    // The blanket is made of 2 LAYERS: the BREEDER and the SUPPORT
    // Water Cooled Lithium Lead (WCLL)
    
    // Breeder
    G4Torus* BLK_breeder = new G4Torus("BLK_breeder", 229*cm, 260*cm, 627*cm, 0*deg, (alpha+20)*deg);
    
    G4LogicalVolume* logicBLK_breeder = new G4LogicalVolume(BLK_breeder, PbLi, "logicBLK_breeder");
    
    G4VPhysicalVolume* physBLK_breeder = new G4PVPlacement(rot, G4ThreeVector(0, 0, 0), logicBLK_breeder, "physBLK_breeder", logicWorld, false, 0, true);
    
    // Support
    G4Torus* BLK_support = new G4Torus("BLK_support", 260*cm, 270*cm, 627*cm, 0*deg, (alpha+20)*deg);
    
    G4LogicalVolume* logicBLK_support = new G4LogicalVolume(BLK_support, SS316, "logicBLK_support");
    
    G4VPhysicalVolume* physBLK_support = new G4PVPlacement(rot, G4ThreeVector(0, 0, 0), logicBLK_support, "physBLK_support", logicWorld, false, 0, true);
    ///////////////////////////////////////////////////////////////////
    
    // Vacuum Vessel (VV)
    G4Torus* VV = new G4Torus("VV", 270*cm, 305.7*cm, 627*cm, 0*deg, (alpha+10)*deg);
    G4LogicalVolume* logicVV = new G4LogicalVolume(VV, SS316, "logicVV");
    G4VPhysicalVolume* physVV = new G4PVPlacement(rot, G4ThreeVector(0,0,0), logicVV, "physVV", logicWorld, false, 0, true);
    
    // Toroidal Field Coils (TFC), which are OUTSIDE the VV
    // G4Torus* TFC = new G4Torus("TFC", 307.1*cm, 407.1*cm, 627*cm, 0*deg, alpha*deg);
    // G4LogicalVolume* logicTFC = new G4LogicalVolume(TFC, SS316, "logicTFC");
    // G4VPhysicalVolume* physTFC = new G4PVPlacement(rot, G4ThreeVector(0,0,0), logicTFC, "physTFC", logicWorld, false, 0, true);
    
    
    // 18 Toroidal Field Coils (TFCs)
    G4Tubs* TFCi = new G4Tubs("TFCi", 307.1*cm, 407.1*cm, 30*cm, 0*deg, 360*deg);
    G4LogicalVolume* logicTFCi = new G4LogicalVolume(TFCi, SS316, "logicTFCi");
    
    G4VPhysicalVolume* physTFCi;
    
    G4RotationMatrix* rot_TFC; //= new G4RotationMatrix;
    for(int i = 0; i != 18; ++i) {        
        
        G4double angle = i*360/18;
        G4double xTFC = R*std::cos(angle*2*M_PI/360);
        G4double yTFC = R*std::sin(angle*2*M_PI/360);
        
        rot_TFC = new G4RotationMatrix;
        rot_TFC->rotateY(angle*deg);
        physTFCi = new G4PVPlacement(rot_TFC, G4ThreeVector(xTFC*cm,0,yTFC*cm), logicTFCi, "physTFCi", logicWorld, false, i, true);
        
    }
    
    // Cryostat
    G4Tubs* Cryostat = new G4Tubs("Cryostat", 1358.2*cm, 1413.2*cm, 1213*cm, 0*deg, alpha*deg);
    
    G4LogicalVolume* logicCryostat = new G4LogicalVolume(Cryostat, SS316, "logicCryostat");
    
    G4VPhysicalVolume* physCryostat = new G4PVPlacement(rot, G4ThreeVector(0,0,0), logicCryostat, "physCryostat", logicWorld, false, 0, true);
    
    // Space filling between bioshield (up/down) and Central Solenoid
    G4Tubs* Filling = new G4Tubs("Filling_up", 0*cm, 1358.2*cm, 281.5*cm, 0*deg, alpha*deg);
    
    G4LogicalVolume* logicFilling = new G4LogicalVolume(Filling, Filling_mat, "logicFilling");
    
    G4VPhysicalVolume* physFilling_up = new G4PVPlacement(rot, G4ThreeVector(0, 931.5*cm, 0), logicFilling, "physFilling_up", logicWorld, false, 0, true);
    
    G4VPhysicalVolume* physFilling_down = new G4PVPlacement(rot, G4ThreeVector(0, -931.5*cm, 0), logicFilling, "physFilling_down", logicWorld, false, 0, true);
    
    // Biological shield (concrete) lateral
    G4Tubs* Bioshield = new G4Tubs("Bioshield", 1413.2*cm, 1613*cm, 1213*cm, 0*deg, alpha*deg);
    
    // G4Sphere* Bioshield = new G4Sphere("Bioshield", 1413.2*cm, 1613*cm, 0*deg, alpha*deg, 0*deg, 180*deg);
    G4LogicalVolume* logicBioshield_lat = new G4LogicalVolume(Bioshield, Concrete, "logicBioshield_lat");
    
    G4VPhysicalVolume* physBioshield = new G4PVPlacement(rot, G4ThreeVector(0,0,0), logicBioshield_lat, "physBioshield_lat", logicWorld, false, 0, true);
    
    // Bioshield up
    G4Tubs* Bioshield_updown = new G4Tubs("Bioshield_updown", 0*cm, 1613*cm, 100*cm, 0*deg, alpha*deg);
    
    G4LogicalVolume* logicBioshield_updown = new G4LogicalVolume(Bioshield_updown, Concrete, "logicBioshield_updown");
    
    G4VPhysicalVolume* physBioshield_up = new G4PVPlacement(rot, G4ThreeVector(0,1313*cm, 0), logicBioshield_updown, "physBioshield_up", logicWorld, false, 0, true);
    
    G4VPhysicalVolume* physBioshield_down = new G4PVPlacement(rot, G4ThreeVector(0,-1313*cm, 0), logicBioshield_updown, "physBioshield_down", logicWorld, false, 0, true);
    
    return physWorld;
}

// Function used to define the Sensitive Detector (SD) which is used
// to detect and SCORE the hits
void MyDetectorConstruction::ConstructSDandField() {
    
    
}
