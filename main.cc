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
    
    // If one executes without arguments (only ./a.out) the graphics is enabled
    
    std::cout << "\n\nEnable graphics for the simulation? (yes/no)\n";
    
    std::string ans;
    
    std::cin >> ans;
    
    if(ans == "yes" || ans == "y") {
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
        
        std::cout << "\n\nGraphics disabled. Do you want to execute the external run.mac macro? (yes/no)\n";
        
        std::cin >> ans;
        
        if(ans == "yes" || ans == "y") {
            
            // Run external run.mac macro
            UImanager->ApplyCommand("/control/execute run.mac");
            
        } else {
            std::cout << "The simulation will generate N neutrons inside the tokamak's vessel and evaluate their interaction with the breeding blanket to produce tritium. The simulation will output the Tritium Breeding Ratio (TBR).\nInert the number of neutrons to generate:\n";
            
            G4int N;
            
            std::cin >> N;
            
            G4String command = "/run/beamOn ";
            UImanager->ApplyCommand(command+std::to_string(N));
        }
    }
    
    
    delete runManager;
    delete visManager;
}
