// Main file of the Geant4 simulation

// Include Geant4 libraries
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4SDManager.hh"

// Include header files
#include "Construction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "ActionInitialization.hh"

int main(int argc, char* argv[]) {
    
    // Create the G4 RunManager instance: heart of Geant4
    G4RunManager* runManager = new G4RunManager;
    
    G4double Li_enrichment = 50;
    
    // ./Tokamak_Breeding run.mac enrich
    // Enrichment inputted by user
    if(argc == 3) {
        Li_enrichment = atof(argv[2]);
    }
    
    MyDetectorConstruction* Detector = new MyDetectorConstruction(Li_enrichment);
    MySensitiveDetector* SensitiveDetector = new MySensitiveDetector("Tritium_SD");
    
    G4UIExecutive* ui = 0;
    
    // If one executes without arguments (only ./Tokamak_Breeding.exe) the graphics is ENABLED
    if(argc == 1) {
        ui = new G4UIExecutive(argc, argv);
        Detector->Set_angle(270); // Set angle for better visualization
    }
    
    // set mandatory initialization classes
    runManager->SetUserInitialization(Detector);
    // runManager->SetUserInitialization(SensitiveDetector);
    runManager->SetUserInitialization(new MyPhysicsList);
    runManager->SetUserInitialization(new MyActionInitialization);
    
    // Initialize Geant4 kernel i.e. construct detector, create physics processes...
    runManager->Initialize();
    
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
        
        // Check for existence of outfile.txt
        std::ifstream is("outfile.txt");
        if(is.is_open() == true) {
            // outfile.txt already exists, do nothing.
            // The results will be appended to the existing ones
        } else {
            // outfile.txt does not exist: create a new one with a header
            std::ofstream os("outfile.txt");
            os << "Li_enr\t# neutr\t# trit\tTBR\n";
            os.close();
        }
        is.close();
        
        is.open("run.mac");
        std::string line, word;
        
        // Sensitive Detector of the simulation
        MySensitiveDetector* SD = Detector->Get_SD();
        
        // Read each command line of run.mac
        while(std::getline(is, line)) {
            // Retrieve number of input neutrons
            G4double N_Neutron;
            
            std::stringstream str(line);
            std::getline(str, word, ' '); // "/run/beamOn"
            if(word != "/run/beamOn") {
                // Don't start simulation if the specified command is not /run/beamOn
                
                std::cout << "ATTENTION. The only accepted command is \"/run/beamOn\" \n";
                
            } else {
                str >> N_Neutron;
                
                // Reset the number of tritium atoms to 0
                SD->Reset_Tritium();
                
                // Run commands from external macro run.mac
                UImanager->ApplyCommand(line);
                
                // Retrieve the number of Tritium atoms produced
                G4int N_Tritium = SD->Get_Tritium();
                
                // Obtain Tritium Breeding Ratio (TBR)
                G4double TBR = N_Tritium/N_Neutron;
                
                
                // Print (append) data to outfile.txt
                std::ofstream outfile;
                outfile.open("outfile.txt", std::ios_base::app);
                outfile << Li_enrichment << '\t' << N_Neutron << '\t' << N_Tritium << '\t' << TBR << '\n';
                outfile.close();
                
                std::cout << "\n\n\nResults printed on outfile.txt\n\n";
            }
            
        }
        
        is.close();
        
    }
    
    
    delete runManager;
    delete visManager;
}
