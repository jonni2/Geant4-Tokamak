#ifndef PRIMARYGENERATORACTION_HH
#define PRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4Neutron.hh"
#include "G4IonTable.hh"
#include "G4Geantino.hh"
#include "G4GeneralParticleSource.hh"

class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    MyPrimaryGeneratorAction();
    ~MyPrimaryGeneratorAction();
    
    virtual void GeneratePrimaries(G4Event*);    
private:
    
    G4bool isIon = false;
    
    // G4ParticleGun is of the abstract class G4VPrimaryGenerator, the others being G4GeneralParticleSource and G4HEPEvtInterface
    // G4ParticleGun* fParticleGun;
    G4GeneralParticleSource* fParticleGun;
    
};

#endif
