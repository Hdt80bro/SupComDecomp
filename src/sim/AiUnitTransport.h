#include "core/TDatListItem.h"
#include "core/WeakPtr.h"
#include "Wm3Vector3.h"
#include <vector>

namespace Moho {

struct SAiReservedTransportBone
{
    int v1;
    int v2;
    Moho::WeakPtr<Moho::Unit> mUnit;
    std::vector<int> mBones;
};

struct SAttachPoint
{
    int mIndex;
    Wm3::Vector3f mLocalPos;
    float mDistSq;
};

struct STransportPickUpInfo
{
    Moho::SCoordsVec2 v0;
    Wm3::Quaternionf mOri;
    Wm3::Vector3f mPos;
    int v1;
    Moho::EntitySetTemplate<Moho::Unit> mUnits;
    bool mHasSpace;
    int v67;
};

// 0x00E1F0AC
class IAiTransport : public Moho::TDatListItem<Moho::IAiTransport>
{
public:
    virtual ~IAiTransport() = default; // 0x005E3C70
    virtual bool TransportIsAirStagingPlatform() const = 0;
    virtual bool TransportIsTeleporter() const = 0;
    virtual Moho::EntitySetTemplate<Moho::Unit> TransportGetLoadedUnits(bool) const = 0;
    virtual void TransportAddPickupUnits(const Moho::EntitySetTemplate<Moho::Unit> &, Moho::SCoordsVec2) = 0;
    virtual void TransportRemovePickupUnit(Moho::Unit *, bool) = 0;
    virtual void TransportRemoveUnitReservation(Moho::Unit *) = 0;
    virtual unsigned int TransportGetPickupUnitCount() const = 0;
    virtual Moho::EntitySetTemplate<Moho::Unit> TransportGetPickupUnits() = 0;
    virtual bool TransportCanCarryUnit(Moho::Unit *) const = 0;
    virtual bool TransportHasSpaceFor(const Moho::RUnitBlueprint *) = 0;
    virtual bool TransportAssignSlot(Moho::Unit *, int) = 0;
    virtual bool TransportAttachUnit(Moho::Unit *) = 0;
    virtual bool TransportDetachUnit(Moho::Unit *) = 0;
    virtual Moho::EntitySetTemplate<Moho::Unit> TransportDetachAllUnits(bool) = 0;
    virtual bool TransportIsUnitAssignedForPickup(Moho::Unit *) const = 0;
    virtual Moho::SOCellPos TransportGetPickupUnitPos(Moho::Unit *) const = 0;
    virtual void TransportAtPickupPosition() = 0;
    virtual bool TransportIsReadyForUnit(Moho::Unit *) const = 0;
    virtual int TransportGetAttachBone(Moho::Unit *) const = 0;
    virtual Moho::SOCellPos TransportGetAttachPosition(Moho::Unit *) const = 0;
    virtual Wm3::Vector3f TransportGetAttachBonePosition(Moho::Unit *) const = 0;
    virtual Moho::VTransform TransportGetAttachBoneTransform(Moho::Unit *) const = 0;
    virtual Wm3::Vector3f TransportGetAttachFacing(Moho::Unit *) const = 0;
    virtual Wm3::Vector3f TransportGetPickupFacing() const = 0;
    virtual void TransportAddToStorage(Moho::Unit *) = 0;
    virtual void TransportRemoveFromStorage(Moho::Unit *, Moho::VTransform &) = 0;
    virtual Moho::EntitySetTemplate<Moho::Unit> TransportGetStoredUnits() const = 0;
    virtual bool TransportIsStoredUnit(Moho::Unit *) const = 0;
    virtual bool TransportHasAvailableStorage() const = 0;
    virtual int TransportReserveStorage(Moho::Unit *, Wm3::Vector3f &, Wm3::Vector3f &, float &) = 0;
    virtual void TransportClearReservation(Moho::Unit *) = 0;
    virtual void TransportResetReservation() = 0;
    virtual void TransportUnreserveUnattachedSpots() = 0;
    virtual void TransportRemoveFromWaitingList(Moho::Unit *) = 0;
    virtual Moho::EntitySetTemplate<Moho::Unit> TransportGetUnitsWaitingForPickup() const = 0;
    virtual Moho::IFormationInstance *TransportGetWaitingFormation() const = 0;
    virtual void TransportGenerateWaitingFormationForUnits(const Moho::EntitySetTemplate<Moho::Unit> &) = 0;
    virtual void TransportClearWaitingFormation() = 0;
    virtual void TranspotSetTeleportDest(Moho::Unit *) = 0;
    virtual Wm3::Vector3f TransportGetTeleportDest() const = 0;
    virtual Moho::Unit *TransportGetTeleportBeacon() const = 0;
    virtual bool TransportIsTeleportBeaconReady() const = 0;
};

// 0x00E1F3CC
class CAiTransportImpl : public Moho::IAiTransport
{
public:
    Moho::Unit *mUnit;
    Moho::WeakPtr<Moho::Unit> mTransportUnit;
    bool mStagingPlatform;
    bool mTeleportation;
    bool v6c;
    bool v6d;
    int mAttachpoints;
    int mNextGeneric;
    int v9;
    int mGenericOverflow;
    int v11;
    Moho::EntitySetTemplate<Moho::Unit> v12;
    Moho::EntitySetTemplate<Moho::Unit> v22;
    Moho::EntitySetTemplate<Moho::Unit> v32;
    std::vector<Moho::SAiReservedTransportBone> v42;
    Moho::STransportPickUpInfo res;
    Moho::CAiFormationInstance *mFormationInstance;
    Wm3::Vector3f v68;
    std::vector<Moho::SAttachPoint> mGenericAttachPoints;
    std::vector<Moho::SAttachPoint> mClass1AttachPoints;
    std::vector<Moho::SAttachPoint> mClass2AttachPoints;
    std::vector<Moho::SAttachPoint> mClass3AttachPoints;
    std::vector<Moho::SAttachPoint> mClass4AttachPoints;
    std::vector<Moho::SAttachPoint> mClassSAttachPoints;
    std::vector<Moho::SAttachPoint> mLaunchPoints;
    int v99;

    ~CAiTransportImpl() override; // 0x005E8280
    bool TransportIsAirStagingPlatform() const override; // 0x005E60F0
    bool TransportIsTeleporter() const override; // 0x005E6100
    Moho::EntitySetTemplate<Moho::Unit> TransportGetLoadedUnits(bool) const override; // 0x005E6110
    void TransportAddPickupUnits(const Moho::EntitySetTemplate<Moho::Unit> &, Moho::SCoordsVec2) override; // 0x005E6260
    void TransportRemovePickupUnit(Moho::Unit *, bool) override; // 0x005E64A0
    void TransportRemoveUnitReservation(Moho::Unit *) override; // 0x005E64D0
    unsigned int TransportGetPickupUnitCount() const override; // 0x005E65A0
    Moho::EntitySetTemplate<Moho::Unit> TransportGetPickupUnits() override; // 0x005E65F0
    bool TransportCanCarryUnit(Moho::Unit *) const override; // 0x005E6870
    bool TransportHasSpaceFor(const Moho::RUnitBlueprint *) override; // 0x005E6C70
    bool TransportAssignSlot(Moho::Unit *, int) override; // 0x005E6E30
    bool TransportAttachUnit(Moho::Unit *) override; // 0x005E7100
    bool TransportDetachUnit(Moho::Unit *) override; // 0x005E7170
    Moho::EntitySetTemplate<Moho::Unit> TransportDetachAllUnits(bool) override; // 0x005E73E0
    bool TransportIsUnitAssignedForPickup(Moho::Unit *) const override; // 0x005E6690
    Moho::SOCellPos TransportGetPickupUnitPos(Moho::Unit *) const override; // 0x005E66B0
    void TransportAtPickupPosition() override; // 0x005E77B0
    bool TransportIsReadyForUnit(Moho::Unit *) const override; // 0x005E77C0
    int TransportGetAttachBone(Moho::Unit *) const override; // 0x005E7930
    Moho::SOCellPos TransportGetAttachPosition(Moho::Unit *) const override; // 0x005E77F0
    Wm3::Vector3f TransportGetAttachBonePosition(Moho::Unit *) const override; // 0x005E7950
    Moho::VTransform TransportGetAttachBoneTransform(Moho::Unit *) const override; // 0x005E7A60
    Wm3::Vector3f TransportGetAttachFacing(Moho::Unit *) const override; // 0x005E7AD0
    Wm3::Vector3f TransportGetPickupFacing() const override; // 0x005E7BB0
    void TransportAddToStorage(Moho::Unit *) override; // 0x005E7BE0
    void TransportRemoveFromStorage(Moho::Unit *, Moho::VTransform &) override; // 0x005E7CF0
    Moho::EntitySetTemplate<Moho::Unit> TransportGetStoredUnits() const override; // 0x005E7E60
    bool TransportIsStoredUnit(Moho::Unit *) const override; // 0x005E8050
    bool TransportHasAvailableStorage() const override; // 0x005E7E80
    int TransportReserveStorage(Moho::Unit *, Wm3::Vector3f &, Wm3::Vector3f &, float &) override; // 0x005E7EC0
    void TransportClearReservation(Moho::Unit *) override; // 0x005E8020
    void TransportResetReservation() override; // 0x005E8040
    void TransportUnreserveUnattachedSpots() override; // 0x005E6530
    void TransportRemoveFromWaitingList(Moho::Unit *) override; // 0x005E5F10
    Moho::EntitySetTemplate<Moho::Unit> TransportGetUnitsWaitingForPickup() const override; // 0x005E5EF0
    Moho::IFormationInstance *TransportGetWaitingFormation() const override; // 0x005E5F30
    void TransportGenerateWaitingFormationForUnits(const Moho::EntitySetTemplate<Moho::Unit> &) override; // 0x005E5F40
    void TransportClearWaitingFormation() override; // 0x005E60A0
    void TranspotSetTeleportDest(Moho::Unit *) override; // 0x005E8080
    Wm3::Vector3f TransportGetTeleportDest() const override; // 0x005E8120
    Moho::Unit *TransportGetTeleportBeacon() const override; // 0x005E81C0
    bool TransportIsTeleportBeaconReady() const override; // 0x005E81D0
};

}
