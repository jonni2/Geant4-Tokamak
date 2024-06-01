// Main file of the Geant4 simulation

// Include Geant4 libraries
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

// Include header files
#include "Construction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv) {
    
    // Create the G4 RunManager instance: heart of Geant4
    G4RunManager* runManager = new G4RunManager;
    
    // set mandatory initialization classes
    runManager->SetUserInitialization(new MyDetectorConstruction);
    runManager->SetUserInitialization(new MyPhysicsList);
    runManager->SetUserInitialization(new MyActionInitialization);
    
    // Initialize Geant4 kernel i.e. construct detector, create physics processes...
    runManager->Initialize();
    
    G4UIExecutive* ui = 0;
    
    // If one executes without arguments (only ./Tokamak_Breeding.exe) the graphics is enabled
    if(argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }
    
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();
    
    // Get the pointer to the UI manager, which is a class automatically created when the G4RunManager is created
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    // Execute setup.mac macro: setup neutron source
    UImanager->ApplyCommand("/control/execute setup.mac");
    
    if(ui) {
        // Graphics enabled
        
        // Execute the vis.mac macro: setup visualization
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
    
    } else {
        // Graphics disabled
        
        // Run external run.mac macro
        UImanager->ApplyCommand("/control/execute run.mac");
        
    }
    
    
    delete runManager;
    delete visManager;
}
