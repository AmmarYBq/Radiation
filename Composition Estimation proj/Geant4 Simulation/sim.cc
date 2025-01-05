#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "PMPhysicsList.hh"
#include "PMDetectorConstruction.hh"
#include "PMActionInitialization.hh"

int main(int argc, char** argv)
{
    #ifdef G4MULTITHREADED
        G4MTRunManager *runManager = new G4MTRunManager;
    #else
        G4RunManager *runManager = new G4RunManager;
    #endif

    // Physics list
    runManager->SetUserInitialization(new PMPhysicsList());

    // Detector construction
    runManager->SetUserInitialization(new PMDetectorConstruction());

    // Action initialization
    runManager->SetUserInitialization(new PMActionInitialization());

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // Check if running in interactive or batch mode
    if (argc == 1) // Interactive mode
    {
        G4UIExecutive *ui = new G4UIExecutive(argc, argv);

        // Initialize visualization manager
        G4VisManager *visManager = new G4VisExecutive();
        visManager->Initialize();

        UImanager->ApplyCommand("/control/execute vis.mac"); // Use default visualization macro
        ui->SessionStart();
        
        delete ui;
        delete visManager;
    }
    else // Batch mode
    {
        // Execute the provided macro file
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    delete runManager;
    return 0;
}
