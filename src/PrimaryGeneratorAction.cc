#include "PrimaryGeneratorAction.hh"

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction() {
    
    // 1 is the number of particles
    // fParticleGun = new G4ParticleGun(1);
    fParticleGun = new G4GeneralParticleSource();
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    
    // Proton
    G4ParticleDefinition* particle = particleTable->FindParticle("proton");
    // G4ParticleDefinition* particle = particleTable->FindParticle("geantino");
    
    
    // Kinematics
    G4ThreeVector pos(0.*m, 0.*m, 0.*m); // Position where particle is generated
    G4ThreeVector mom(0., 1., 0.); // Momentum direction
    
    // fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleDefinition(G4Neutron::Definition());
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
    
    if(isIon == true) {
        // Requires that the particle is a G4Geantino
        // Heavy ion
        G4int Z = 6;
        G4int A = 14;
        G4double charge = Z*eplus;
        
        G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z, A, 0*keV);
        fParticleGun->SetParticleDefinition(ion);
        fParticleGun->SetParticleCharge(charge);
    }
    
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
