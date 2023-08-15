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
    
    // Photon physics
    // RegisterPhysics(new G4OpticalPhysics);
}

MyPhysicsList::~MyPhysicsList()
{}

// here we register all the particles required in the simulation
// void MyPhysicsList::ConstructParticle()
// {
//     // G4Proton::ProtonDefinition();
//     // G4Geantino::GeantinoDefinition();
// }
// 
// // Here we register all the physical processes required in the simulation
// void MyPhysicsList::ConstructProcess()
// {
//     // Transportation, provided in the base G4VUserPhysicsList class
//     AddTransportation();
//     // Electromagnetic process
//     // ConstructEM();
// }
