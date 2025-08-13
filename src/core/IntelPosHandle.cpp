#include "IntelPosHandle.h"

void Moho::CIntelPosHandle::UpdatePos(Wm3::Vector3f *newPos, int curTick) {
    if (this->mEnabled) {
        float dx = newPos->X() - this->mLastPos.X();
        float dy = newPos->Y() - this->mLastPos.Y();
        float dz = newPos->Z() - this->mLastPos.Z();
        float updateDist = this->mRadius * 0.333;
        if (dx*dx + dy*dy + dz*dz >= updateDist*updateDist || curTick - this->lastTickUpdated > /* 30 */ 5) {
            this->mLastTickUpdated = curTick;
            this->Update(newPos);
        }
    } else {
        this->mLastPos = *newPos;
    }
}

void Moho::CIntelPosHandle::Update(Wm3::Vector3f *pos) {
    if (*pos != this->mLastPos) {
        this->SubViz();
        this->mLastPos = *pos;
        this->AddViz();
    }
}

void Moho::CIntelPosHandle::SubViz() {
    if (this->mEnabled && this->mRadius != 0.0 && this->mGrid()) {
        this->mGrid->SubtractCircle(&this->mLastPos, this->mRadius);
    }
}

void Moho::CIntelPosHandle::AddViz() {
    if (this->mEnabled && this->mRadius != 0.0 && this->mGrid()) {
        this->mGrid->AddCircle(&this->mLastPos, this->mRadius);
    }
}
  