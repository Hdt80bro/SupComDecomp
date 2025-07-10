
void Moho::CIntelPosHandle::UpdatePos(Wm3::Vector3f *newPos, int curTick) {
    if (this->isEnabled) {
        float dz = newPos->x - this->lastPos.x;
        float dy = newPos->y - this->lastPos.y;
        float dz = newPos->z - this->lastPos.z;
        float updateDist = this->radius * 0.333;
        if (dx*dx + dy*dy + dz*dz >= updateDist*updateDist || curTick - this->lastTickUpdated > /* 30 */ 5) {
            this->lastTickUpdated = curTick;
            this->Update(newPos);
        }
    } else {
        this->lastPos = *newPos;
    }
}

void Moho::CIntelPosHandle::Update(Wm3::Vector3f *pos) {
    if (pos->compare(&this->lastPos) != 0) {
        this->SubViz();
        this->lastPos = *pos;
        this->AddViz();
    }
}

void Moho::CIntelPosHandle::SubViz() {
    if (this->isEnabled && this->radius != 0.0 && this->grid != nullptr) {
        this->grid->SubtractCircle(&this->lastPos, this->radius);
    }
}
  
void Moho::CIntelPosHandle::AddViz() {
    if (this->isEnabled && this->radius != 0.0 && this->grid != nullptr) {
        this->grid->AddCircle(&this->lastPos, this->radius);
    }
}
  