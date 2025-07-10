
inline gpg::Rect2i PointToLocation(Wm3::Vector3f *pos, Wm3::Vector2i *size) {
    int x = (int) (pos->x - (float) size->x * 0.5);
    int z = (int) (pos->z - (float) size->z * 0.5);
    return {x, z, x + size->x, z + size->z}
}


bool Moho::ResourceDeposit::Intersects(Moho::CGeomSolid3 *solid, Moho::CHeightField *field) {
    Wm3::AxisAlignedBox3f box;
    /*
    ...
    */
    return solid->Intersects(&box);
}


void Moho::CSimResources::AddDeposit(Moho::EDepositType type, gpg::Rect2i *pos) {
    this->lock->Lock();
    this->deposits->emplace_back{*pos, type};
    this->lock->Unlock();
}

void Moho::CSimResources::AddDepositPoint(Moho::EDepositType type, Wm3::Vector3f *pos, Wm3::Vector2i *size) {
    this->lock->Lock();
    this->deposits->emplace_back{PointToLocation(pos, size), type};
    this->lock->Unlock();
}

bool Moho::CSimResources::IsDepositAt(gpg::Rect2i *pos, Moho::EDepositType type) {
    this->lock->Lock();
    bool found = false;
    for (Moho::ResourceDeposit *i = this->resources.start; i != this->resources.end; ++i) {
        if (i->type == type
            && pos->x0 < i->location.x1 && i->location.x0 < pos->x1
            && pos->z0 < i->location.z1 && i->location.z0 < pos->z1
            && pos->x0 < pos->x1 && pos->z0 < pos->z1
            && i->location.x0 < i->location.x1 && i->location.z0 < i->location.z1
        ) {
            found = true;
            break; 
        }
    }
    this->lock->Unlock();
    return found;
}

bool Moho::CSimResources::IsDepositAtPoint(Wm3::Vector3f *pos, Wm3::Vector2i *size, Moho::EDepositType type) {
    return this->IsDepositAt(PointToLocation(pos, size), type);
}

void Moho::CSimResources::DepositCollides(Moho::CGeomSolid3 *solid, Moho::CHeightField *field, gpg::fastvector<Moho::ResourceDeposit*> *dest, Moho::EDepositType type) {
    if (type != DEPOTYPE_None) {
        for (Moho::ResourceDeposit *i = this->deposits.start; i != this->deposits.end; ++i) {
            if (i->type == type && i->Intersects(solid, field)) {
                dest->append(i);
            }
        }
    } else {
        for (Moho::ResourceDeposit *i = this->deposits.start; i != this->deposits.end; ++i) {
            if (i->Intersects(solid, field)) {
                dest->append(i);
            }
        }
    }
}

bool Moho::CSimResources::DepositIsInArea(Moho::EDepositType type, gpg::Rect2i *area) {
    this->lock->Lock();
    bool found = false;
    for (Moho::ResourceDeposit *i = this->resources.start; i != this->resources.end; ++i) {
        if (i->type == type
            && area->x0 <= i->location.x0 && i->location.x1 <= area->x1
            && area->z0 <= i->location.z0 && i->location.z1 <= area->z1
        ) {
            found = true;
            break;
        }
    }
    this->lock->Unlock();
    return found;
}

bool Moho::CSimResources::DepositIsInAreaPoint(Moho::EDepositType type, Wm3::Vector3f *pos, Wm3::Vector2i *size) {
    return this->DepositIsInArea(type, PointToLocation(pos, size));
}

bool Moho::CSimResources::FindClosestDespoit(Moho::GridPos *from, Moho::GridPos *outPos, float radius, Moho::EDepositType type) {
    float closestDist = 3.4028235e38;
    Moho::GridPos closestPos;
    for (Moho::ResourceDeposit *i = this->deposits.start; i != this->deposits.end; ++i) {
        if (i->type == type) {
            Moho::GridPos curPos {
                i->location.x0 + (i->location.x1 - i->location.x0) / 2,
                i->location.z0 + (i->location.z1 - i->location.z0) / 2
            };
            float curDist = sqrt((from->x - curPos.x)*(from->x - curPos.x) + (from->z - curPos.z)*(from->z - curPos.z));
            if (curDist < closestDist && curDist < radius) {
                closestDist = curDist;
                closestPos = curPos;
            }
        }
    }
    if (closestDist == 3.4028235e38) {
        return 0;
    } else {
        *outPos = closestPos;
        return 1;
    }
}

bool Moho::CSimResources::AreaHasDeposit(Moho::EDepositType type, gpg::Rect2f *area) {
    this->lock->Lock();
    bool found = false;
    for (Moho::ResourceDeposit *i = this->deposits.start; i != this->deposits.end; ++i) {
        if (i->depoType == type) {
            gpg::Rect2i loc {0.0, 0.0, 0.0, 0.0};
            if (type == DEPOTYPE_Mass) {
                loc = {
                    (float) i->location.x0 - 0.5,
                    (float) i->location.z0 - 0.5,
                    (float) i->location.x1 + 0.5,
                    (float) i->location.z1 + 0.5
                }
            } else if (type == DEPOTYPE_Hydrocarbon) {
                loc = {
                    (float) i->location.x0 - 1.5,
                    (float) i->location.z0 - 1.5,
                    (float) i->location.x1 + 1.5,
                    (float) i->location.z1 + 1.5
                }
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
    this->lock->Unlock();
    return found;
}
