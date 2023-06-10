#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSEnergyDeposit.hh"


#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    // materials
    auto nist = G4NistManager::Instance();
    auto matCu = nist->FindOrBuildMaterial("G4_Cu");
    auto matPt = nist->FindOrBuildMaterial("G4_Pt");
    auto matCo = nist->FindOrBuildMaterial("G4_Co");
    auto matAir = nist->FindOrBuildMaterial("G4_Air");

    //LiPON
    auto nist = G4NistManager::Instance();
    std::vector<G4String> elLiPON = {"Li", "P", "O", "N"};
    std::vector<G4double> weightLiPON = {21.11, 26.15, 44.58, 8.16 };
    auto matLiPON = nist->ConstructNewMaterial("LiPON", elLiPON, weightLiPON, 2.33 * g / cm3, true, kStateSolid);

    //LiCo
    auto nist = G4NistManager::Instance();
    std::vector<G4String> elLiCo = {"Li", "Co", "O",};
    std::vector<G4double> nbLiCo = {1, 1, 2};
    auto matLiCo = nist->ConstructNewMaterial("LiCo", elLiCo, nbLiCo, 4.9 * g / cm3, true, kStateSolid);

    // World
    auto worldSize = 1. * cm;
    auto worldSol = new G4Box("World", .5 * worldSize, .5 * worldSize, .5 * worldSize);
    auto worldLog = new G4LogicalVolume(worldSol, matAir, "World");
    auto worldPhy = new G4PVPlacement(0, G4ThreeVector(), worldLog, "World", nullptr, false, 0);
   
    // LiPON Cell
    auto LiPONXLength = 1. * cm;
    auto LiPONYLength = .5 * cm; //set
    auto LiPONZLength = .03 * micrometer; //thickness
    auto LiPONPos = G4ThreeVector(0. * cm, 0. *cm, 0. * micrometer);
    auto LiPONSol = new G4Box("LiPON", .5 * LiPONXLength, .5 * LiPONYLength, .5 *LiPONZLength);
    
    //Cu Cell
    auto CuXLength = 1. * cm;
    auto CuYLength = .5 * cm;  // set
    auto CuZLength = .2 * micrometer; //thickness
    auto CuPos = G4ThreeVector(0. * cm, 0. * cm, -.13 * micrometer);
    auto CuSol = new G4Box("Cu", .5 * CuXLength, .5 * CuYLength, .5 *CuZLength);

    // SE LiPON
    auto SEXLength = 1. * cm;
    auto SEYLength = .5 * cm;
    auto SEZLength = 3.2 * micrometer;
    auto SEPos = G4ThreeVector(0. * cm, 0. * cm, -1.83 * micrometer);
    auto SESol = new G4Box("SELiPON", .5 * SEXLength, .5 * SEYLength, .5 *SEZLength);

    //LiCoO2
    auto LiCoXLength = 1. * cm;
    auto LiCoYLength = .5 * cm;
    auto LiCoZLength = 3.1 * micrometer;
    auto LiCoPos = G4ThreeVector(0. * cm, 0. * cm, -4.98 * micrometer);
    auto LiCoSol = new G4Box("LiCo", .5 * LiCoXLength, .5 * LiCoYLength, .5 *LiCoZLength);

    //Pt
    auto PtXLength = 1. * cm;
    auto PtYLength = .5 * cm;
    auto PtZLength = .2 * micrometer; //thickness
    auto PtPos = G4ThreeVector(0. * cm, 0. * cm, -6.63 * micrometer);
    auto PtSol = new G4Box("Pt", .5 * PtXLength, .5 * PtYLength, .5 *PtZLength);

    //Co 
    auto CoXLength = 1. * cm;
    auto CoYLength = .5 * cm;
    auto CoZLength = .03 * micrometer;
    auto CoPos = G4ThreeVector(0. * cm, 0. * cm, -6.745 * micrometer);
    auto CoSol = new G4Box("Co", .5 * CoXLength, .5 * CoYLength, .5 *CoZLength);
 
    // Logical
    auto LiPONLog = new G4LogicalVolume(LiPONSol, matLiPON, "LiPON");
    auto CuLog = new G4LogicalVolume(CuSol, matCu, "Cu");
    auto SELog = new G4LogicalVolume(SESol, matLiPON, "SELiPON");
    auto LiCoLog = new G4LogicalVolume(LiCoSol, matLiCo, "LiCo");
    auto PtLog = new G4LogicalVolume(PtSol, matPt, "Pt");
    auto CoLog = new G4LogicalVolume(CoSol, matCo, "Co");

    // LiPON physical
    new G4PVPlacement(nullptr, LiPONPos, LiPONLog, "LiPON", worldLog, false, 0);
    new G4PVPlacement(nullptr, CuPos, CuLog, "Cu", worldLog, false, 0);
    new G4PVPlacement(nullptr, SEPos, SELog, "SELiPON", worldLog, false, 0);
    new G4PVPlacement(nullptr, LiCoPos, LiCoLog, "LiCo", worldLog, false, 0);
    new G4PVPlacement(nullptr, PtPos, PtLog, "Pt", worldLog, false, 0);
    new G4PVPlacement(nullptr, CoPos, CoLog, "Co", worldLog, false, 0);
    return worldPhy;
}

void DetectorConstruction::ConstructSDandField()
{
    //Creating MFD
    auto mfd = new G4MultiFunctionalDetector("Detector"); // creating object named mfd 
    G4SDManager::GetSDMpointer()->AddNewDetector(mfd);
    auto psEDep = new G4PSEnergyDeposit("Edp"); // creating G4PSEnergyDeposit class object named psEDep 
    mfd->RegisterPrimitive(psEDep);
    SetSensitiveDetector("LiPON", mfd);
}
