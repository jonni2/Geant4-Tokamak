#include "PhysicsList.hh"

MyPhysicsList::MyPhysicsList()
{
    // Electromagnetic physics
    RegisterPhysics(new G4EmStandardPhysics());
    
    // Hadronic elastic scattering
    RegisterPhysics(new G4HadronElasticPhysics());
    RegisterPhysics(new G4HadronPhysicsFTFP_BERT_TRV());
    
    // Decay physics
    // RegisterPhysics(new G4DecayPhysics());
    // RegisterPhysics(new G4RadioactiveDecayPhysics()); //crash
    
    // Neutron physics
    RegisterPhysics(new NeutronHPphysics("neutronHP"));
    
    // Photon physics
    // RegisterPhysics(new G4OpticalPhysics);
}

MyPhysicsList::~MyPhysicsList()
{}

void MyPhysicsList::SetCuts()
{}
