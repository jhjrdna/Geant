#include "G4ParticleGun.hh"
#include "G4Gamma.hh" //including G4Gamma Class
#include "G4SystemOfUnits.hh"

#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
{
    fPrimary = new G4ParticleGun();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fPrimary;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    fPrimary->SetParticleDefinition(G4Gamma::Definition()); // Initial particle is gamma

    fPrimary->SetParticleEnergy(0.662 * MeV); // Setting Gamma energy

    fPrimary->SetParticlePosition(G4ThreeVector()); // Setting Initial Gamma position

    fPrimary->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.)); // Setting Gamma direction

    fPrimary->GeneratePrimaryVertex(anEvent);
}
