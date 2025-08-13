#include "SimResources.h"


inline gpg::Rect2i PointToLocation(Wm3::Vector3f *pos, Wm3::Vector2i *size) {
    int x = (int) (pos->X() - (float) size->X() * 0.5);
    int z = (int) (pos->Z() - (float) size->Z() * 0.5);
    return {x, z, x + size->x, z + size->z}
}


// 0x00546170
bool Moho::ResourceDeposit::Intersects(Moho::CGeomSolid3 *solid, Moho::CHeightField *field) {
    Wm3::AxisAlignedBox3f box;
    /*
    ... TODO
    */
    return solid->Intersects(&box);
}

// 0x00545F10
void Moho::CSimResources::AddDeposit(Moho::EDepositType type, gpg::Rect2i *pos) {
    this->mLock->Lock();
    this->mDeposits.emplace_back(*pos, type);
    this->mLock->Unlock();
}

// 0x00545E80
void Moho::CSimResources::AddDepositPoint(Moho::EDepositType type, Wm3::Vector3f *pos, Wm3::Vector2i *size) {
    this->mLock->Lock();
    this->mDeposits.emplace_back(PointToLocation(pos, size), type);
    this->mLock->Unlock();
}

// 0x00545FC0

// 0x00546060
bool Moho::CSimResources::IsDepositAt(gpg::Rect2i *pos, Moho::EDepositType type) {
    this->mLock->Lock();
    bool found = false;
    for (auto i = this->mDeposits.begin(); i != this->mDeposits.end(); ++i) {
        if (i->mType == type
            && pos->x0 < i->mLocation.x1 && i->mLocation.x0 < pos->x1
            && pos->z0 < i->mLocation.z1 && i->mLocation.z0 < pos->z1
            && pos->x0 < pos->x1 && pos->z0 < pos->z1
            && i->mLocation.x0 < i->mLocation.x1 && i->mLocation.z0 < i->mLocation.z1
        ) {
            found = true;
            break; 
        }
    }
    this->mLock->Unlock();
    return found;
}

// 0x00545FD0
bool Moho::CSimResources::IsDepositAtPoint(Wm3::Vector3f *pos, Wm3::Vector2i *size, Moho::EDepositType type) {
    return this->IsDepositAt(PointToLocation(pos, size), type);
}

// 0x00546470
void Moho::CSimResources::DepositCollides(Moho::CGeomSolid3 *solid, Moho::CHeightField *field, gpg::fastvector<Moho::ResourceDeposit *> *dest, Moho::EDepositType type) {
    if (type != DEPOTYPE_None) {
        for (auto i = this->mDeposits.begin(); i != this->mDeposits.end(); ++i) {
            if (i->mType == type && i->Intersects(solid, field)) {
                dest->append(i);
            }
        }
    } else {
        for (auto i = this->mDeposits.begin(); i != this->mDeposits.end(); ++i) {
            if (i->Intersects(solid, field)) {
                dest->append(i);
            }
        }
    }
}

// 0x00546650
bool Moho::CSimResources::DepositIsInArea(Moho::EDepositType type, gpg::Rect2i *area) {
    this->mLock->Lock();
    bool found = false;
    for (auto i = this->mDeposits.begin(); i != this->mDeposits.end(); ++i) {
        if (i->mType == type
            && area->x0 <= i->mLocation.x0 && i->mLocation.x1 <= area->x1
            && area->z0 <= i->mLocation.z0 && i->mLocation.z1 <= area->z1
        ) {
            found = true;
            break;
        }
    }
    this->mLock->Unlock();
    return found;
}

// 0x005465C0
bool Moho::CSimResources::DepositIsInAreaPoint(Moho::EDepositType type, Wm3::Vector3f *pos, Wm3::Vector2i *size) {
    return this->DepositIsInArea(type, PointToLocation(pos, size));
}

// 0x00546760
bool Moho::CSimResources::FindClosestDespoit(Moho::GridPos *from, Moho::GridPos *outPos, float radius, Moho::EDepositType type) {
    float closestDist = 3.4028235e38;
    Moho::GridPos closestPos;
    for (auto i = this->mDeposits.begin(); i != this->mDeposits.end(); ++i) {
        if (i->mType == type) {
            Moho::GridPos curPos {
                i->mLocation.x0 + (i->mLocation.x1 - i->mLocation.x0) / 2,
                i->mLocation.z0 + (i->mLocation.z1 - i->mLocation.z0) / 2
            };
            float curDist = sqrt((from->X() - curPos.X())*(from->X() - curPos.X()) + (from->Z() - curPos.Z())*(from->Z() - curPos.Z()));
            if (curDist < closestDist && curDist < radius) {
                closestDist = curDist;
                closestPos = curPos;
            }
        }
    }
    if (closestDist == 3.4028235e38) {
        return false;
    } else {
        *outPos = closestPos;
        return true;
    }
}

bool Moho::CSimResources::AreaHasDeposit(Moho::EDepositType type, gpg::Rect2f *area) {
    this->mLock->Lock();
    bool found = false;
    for (auto i = this->mDeposits.begin(); i != this->mDeposits.end(); ++i) {
        if (i->mType == type) {
            gpg::Rect2i loc{0.0, 0.0, 0.0, 0.0};
            if (type == DEPOTYPE_Mass) {
                loc = {
                    (float) i->mLocation.x0 - 0.5,
                    (float) i->mLocation.z0 - 0.5,
                    (float) i->mLocation.x1 + 0.5,
                    (float) i->mLocation.z1 + 0.5
                };
            } else if (type == DEPOTYPE_Hydrocarbon) {
                loc = {
                    (float) i->mLocation.x0 - 1.5,
                    (float) i->mLocation.z0 - 1.5,
                    (float) i->mLocation.x1 + 1.5,
                    (float) i->mLocation.z1 + 1.5
                };
            }
            if (area->x0 < loc.x1 && loc.x0 < area->x1
                && area->z0 < loc.z1 && loc.z0 < area->z1
                && area->x0 < area->x1 && area->z0 < area->z1
                && loc.x0 < loc.x1 && loc.z0 < loc.z1
            ) {
                found = true;
                break;
            }
        }
    }
    this->mLock->Unlock();
    return found;
}
