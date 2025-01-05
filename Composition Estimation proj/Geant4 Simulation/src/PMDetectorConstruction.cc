#include "PMDetectorConstruction.hh"

PMDetectorConstruction::PMDetectorConstruction()
{
}

PMDetectorConstruction::~PMDetectorConstruction()
{
}

G4VPhysicalVolume *PMDetectorConstruction::Construct()
{
    G4bool checkOverlaps = true;

    G4NistManager *nist  = G4NistManager::Instance();
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *detMat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
    G4Material *SampleMat = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

    // Define Er-155
    G4Isotope *Er155 = new G4Isotope("Er155", 68, 155, 154.933216 * g / mole);
    G4Element *elEr155 = new G4Element("Erbium-155", "F18", 1);
    elEr155->AddIsotope(Er155, 100.0 * perCent);
    G4Material *matEr155 = new G4Material("Er155Source", 9.065 * g / cm3, 1);
    matEr155->AddElement(elEr155, 100.0 * perCent);

    G4double xWorld = 1. * m;
    G4double yWorld = 1. * m;
    G4double zWorld = 1. * m;

    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicalWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    // radioactive source (Er-155)
    G4double sourceRadius = 1. * mm;

    G4Sphere *solidSource = new G4Sphere("solidSource", 0.0, sourceRadius, 0.0, 360. * deg, 0.0, 180. * deg);
    G4LogicalVolume *logicSource = new G4LogicalVolume(solidSource, matEr155, "logicSource");
    G4VPhysicalVolume *physSource = new G4PVPlacement(0, G4ThreeVector(0., 0., 5 * cm), logicSource, "physSource", logicWorld, 0, checkOverlaps);

    G4VisAttributes *sourceVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 1.0, 0.5));
    sourceVisAtt->SetForceSolid(true);
    logicSource->SetVisAttributes(sourceVisAtt);

    // Sample Info
    G4double SampleThickness = 7.6 * mm;
    G4double SampleSize = 3. * cm; // radius
    G4double SampleZPlacement= 10. * cm;
    G4Tubs *SolidSample= new G4Tubs("SolidSample",0.0 , 0.5 * SampleSize,0.5 * SampleThickness, 0.0, 360 * deg);
    G4LogicalVolume *LogicSample = new G4LogicalVolume(SolidSample, SampleMat, "LogicSample");
    G4VPhysicalVolume *PhysicalSample = new G4PVPlacement(0, G4ThreeVector(0.0 ,0.0 , SampleZPlacement+ 0.5 * SampleThickness),LogicSample,"physSample", logicWorld, false, 0, checkOverlaps);

    G4VisAttributes *SampleVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 0.5));
    SampleVisAtt->SetForceSolid(true);
    LogicSample->SetVisAttributes(SampleVisAtt);


    //Detector Physicals
    G4double detectorSize = 10.0 * cm;
    G4double detectorRadius = 5 * cm;
    G4Tubs *solidDetector = new G4Tubs("solidDetector", 0.0, 0.5 * detectorRadius, 0.5 * detectorSize,0.0 ,360.0 * deg);
    logicDetector = new G4LogicalVolume(solidDetector, detMat, "logicDetector");
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0. , 0.5 * detectorSize + SampleZPlacement + SampleThickness + 1 * mm), logicDetector, "physDetector", logicWorld, false, checkOverlaps);
    //SampleZPlacement + SampleThickness + 1 * mm
    G4VisAttributes *detVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.5));
    detVisAtt->SetForceSolid(true);
    logicDetector->SetVisAttributes(detVisAtt);

    return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
    PMSensitiveDetector *sensDet = new PMSensitiveDetector("SensitveDetector");
    logicDetector->SetSensitiveDetector(sensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}