#include "PrimaryGeneratorAction.hh"

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction() {
    
    // 1 is the number of particles
    // fParticleGun = new G4ParticleGun(1);
    fParticleGun = new G4GeneralParticleSource();
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    
    // Set characteristics of the generated particles
    fParticleGun->SetParticleDefinition(G4Neutron::Definition());
    
    // The following characteristics are set in the setup.mac file
    // fParticleGun->SetParticlePosition(pos);
    // fParticleGun->SetParticleMomentumDirection(mom);
    // fParticleGun->SetParticleMomentum(0*eV);
    // fParticleGun->SetParticleEnergy(10*GeV);
    
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction() {
    delete fParticleGun;
}

// Here we can invoke the generation also of particles with RANDOMIZED Energy, momentum... (which by default are NOT randomized)
void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
