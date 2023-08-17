#ifndef PHYSICSLIST_HH
#define PHYSICSLIST_HH

#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT_TRV.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "globals.hh"
#include "G4ParticleHPManager.hh"
#include "NeutronHPphysics.hh"

// class MyPhysicsList : public G4VUserPhysicsList {
class MyPhysicsList : public G4VModularPhysicsList {
public:
    
    MyPhysicsList();
    ~MyPhysicsList();
    
    // virtual void ConstructParticle();
    // virtual void ConstructProcess();
    virtual void SetCuts();
    
};

#endif
