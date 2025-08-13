#include "IntelCounterHandle.h"

void Moho::CIntelCounterHandle::SubViz() {
    if (this->mEnabled && this->mRadius != 0.0) {
        std::vector<Moho::IArmy> *p_armiesList = &this->mSim->armiesList;
        for (int i = 0; i < p_armiesList->size(); ++i) {
            Moho::IAiReconDB *reconDB = p_armiesList[i]->GetReconDB();
            if (this->mReconDB == reconDB) {
                continue;
            }
            switch (this->mType) {
                case INTELCOUNTER_RadarStealthField: {
                    Moho::WeakPtr<Moho::CIntelGrid> rciGrid = reconDB->ReconGetRCIGrid();
                    rciGrid->SubtractCircle(&this->mLastPos, this->mRadius);
                    break;
                }
                case INTELCOUNTER_SonarStealthField: {
                    Moho::WeakPtr<Moho::CIntelGrid> rsiGrid = reconDB->ReconGetSCIGrid();
                    rsiGrid->SubtractCircle(&this->mLastPos, this->mRadius);
                    break;
                }
                case INTELCOUNTER_CloakField: {
                    Moho::WeakPtr<Moho::CIntelGrid> vsiGrid = reconDB->ReconGetVCIGrid();
                    vsiGrid->SubtractCircle(&this->mLastPos, this->mRadius);
                    break;
                }
                default: {
                    gpg::Warnf("Moho::CIntelCounterHandle::SubViz: unsupported counter intel type %i", this->type);
                    break;
                }
            }
        }
    }
}

void Moho::CIntelCounterHandle::AddViz() {
    if (this->mEnabled && this->mRadius != 0.0) {
        std::vector<Moho::IArmy> *p_armiesList = &this->mSim->armiesList;
        for (int i = 0; i < p_armiesList->size(); ++i) {
            Moho::IAiReconDB *reconDB = p_armiesList[i]->GetReconDB();
            if (this->mReconDB == reconDB) {
                continue;
            }
            switch (this->mType) {
                case INTELCOUNTER_RadarStealthField:
                    Moho::WeakPtr<Moho::CIntelGrid> rciGrid = reconDB->ReconGetRCIGrid();
                    rciGrid->AddCircle(&this->mLastPos, this->mRadius);
                    break;
                case INTELCOUNTER_SonarStealthField:
                    Moho::WeakPtr<Moho::CIntelGrid> rsiGrid = reconDB->ReconGetSCIGrid();
                    rsiGrid->AddCircle(&this->mLastPos, this->mRadius);
                    break;
                case INTELCOUNTER_CloakField:
                    Moho::WeakPtr<Moho::CIntelGrid> vsiGrid = reconDB->ReconGetVCIGrid();
                    vsiGrid->AddCircle(&this->mLastPos, this->mRadius);
                    break;
                default:
                    gpg::Warnf("Moho::CIntelCounterHandle::AddViz: unsupported counter intel type %i", this->type);
                    break;
            }
        }
    }
}
