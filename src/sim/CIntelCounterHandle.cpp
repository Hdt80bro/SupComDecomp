

void Moho::CIntelCounterHandle::SubViz() {
    if (this->enabled && this->radius != 0.0) {
        std::vector<Moho::CArmyImpl> *p_armiesList = &this->sim->armiesList;
        for (int i = 0; i < p_armiesList->size(); ++i) {
            Moho::CAiReconDBImpl *reconDB = p_armiesList[i]->GetReconDB();
            if (this->reconDB == reconDB) {
                continue;
            }
            switch (this->type) {
                case INTELCOUNTER_RadarStealthField:
                    Moho::WeakPtr<Moho::CIntelGrid> rciGrid = reconDB->ReconGetRCIGrid();
                    rciGrid->SubtractCircle(&this->lastPos, this->radius);
                    break;
                case INTELCOUNTER_SonarStealthField:
                    Moho::WeakPtr<Moho::CIntelGrid> rsiGrid = reconDB->ReconGetSCIGrid();
                    rsiGrid->SubtractCircle(&this->lastPos, this->radius);
                    break;
                case INTELCOUNTER_CloakField:
                    Moho::WeakPtr<Moho::CIntelGrid> vsiGrid = reconDB->ReconGetVCIGrid();
                    vsiGrid->SubtractCircle(&this->lastPos, this->radius);
                    break;
                default:
                    gpg::Warnf("Moho::CIntelCounterHandle::SubViz: unsupported counter intel type %i", this->type);
                    break;
            }
        }
    }
}

void Moho::CIntelCounterHandle::AddViz() {
    if (this->enabled && this->radius != 0.0) {
        std::vector<Moho::CArmyImpl> *p_armiesList = &this->sim->armiesList;
        for (int i = 0; i < p_armiesList->size(); ++i) {
            Moho::CAiReconDBImpl *reconDB = p_armiesList[i]->GetReconDB();
            if (this->reconDB == reconDB) {
                continue;
            }
            switch (this->type) {
                case INTELCOUNTER_RadarStealthField:
                    Moho::WeakPtr<Moho::CIntelGrid> rciGrid = reconDB->ReconGetRCIGrid();
                    rciGrid->AddCircle(&this->lastPos, this->radius);
                    break;
                case INTELCOUNTER_SonarStealthField:
                    Moho::WeakPtr<Moho::CIntelGrid> rsiGrid = reconDB->ReconGetSCIGrid();
                    rsiGrid->AddCircle(&this->lastPos, this->radius);
                    break;
                case INTELCOUNTER_CloakField:
                    Moho::WeakPtr<Moho::CIntelGrid> vsiGrid = reconDB->ReconGetVCIGrid();
                    vsiGrid->AddCircle(&this->lastPos, this->radius);
                    break;
                default:
                    gpg::Warnf("Moho::CIntelCounterHandle::AddViz: unsupported counter intel type %i", this->type);
                    break;
            }
        }
    }
}