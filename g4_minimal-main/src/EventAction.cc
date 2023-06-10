#include "G4Event.hh"
#include "G4SDManager.hh" //To use GetCollectionID function of G4SDManager class
#include "EventAction.hh"
#include "G4THitsMap.hh"  // To get HC by using ID 
#include "G4SystemOfUnits.hh" 

EventAction::EventAction()
    : G4UserEventAction(), fHCID(-1) // initializing fHCID to -1
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event *)
{
}

void EventAction::EndOfEventAction(const G4Event *anEvent)
{
    auto HCE = anEvent->GetHCofThisEvent(); // Made HCofThisEvent in this event is designated to HCE parameter
    if (!HCE)
        return;

    if(fHCID == -1)
	fHCID = G4SDManager::GetSDMpointer()->GetCollectionID("Detector/EDep");
    // only set when ID is -1 if not, if sentence is not executed

    auto hitsMap = static_cast<G4THitsMap<G4double> *>(HCE->GetHC(fHCID));
    // hitsMap is a parameter which copynumber is stored

    for (const auto &iter : *(hitsMap->GetMap()))
    {
        auto eDep = *(iter.second);
        if (eDep > 0.)
        {
    	    G4cout << "--- Energy Deposit:" << eDep / MeV << G4endl;
        }
    }

}
