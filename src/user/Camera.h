#include "user/UserUnit.h"
#include "gpgcore/containers/fastvector.h"
#include <string>
#include <vector>

enum enum_CamTargetType
{
    CAMTARTYPE_Location = 0x0,
    CAMTARTYPE_Box = 0x1,
    CAMTARTYPE_Entity = 0x2,
    CAMTARTYPE_NoseCam = 0x3,
    CAMTARTYPE_Hermite = 0x4,
};

enum enum_CameraAccType
{
    CAMACCTYPE_Linear = 0x0,
    CAMACCTYPE_FastInSlowOut = 0x1,
    CAMACCTYPE_SlowInOut = 0x2,
};

namespace Moho {

struct GeomCamera3
{
    Moho::VTransform mTranform;
    gpg::gal::Matrix mProjection;
    gpg::gal::Matrix mView;
    gpg::gal::Matrix mViewProjection;
    gpg::gal::Matrix mInverseProjection;
    gpg::gal::Matrix mInverseView;
    gpg::gal::Matrix mInverseViewProjection;
    int alignment;
    Moho::CGeomSolid3 mSolid1;
    Moho::CGeomSolid3 mSolid2;
    float mLODScale;
    Moho::VMatrix4 mViewport;
    int v160;
};

// 0x00E3C544
class RCamCamera : public Moho::Broadcaster<Moho::SCameraTracking>
{
};

static Moho::RCamManager sCamManager; // 0x010C79D4

struct SCamShakeParams
{
    float v1;
    float v2;
    float v3;
    float v4;
    float v5;
    float v6;
    float mMinTime;
};

enum ECamTimeSource
{
    TIMESOURCE_System = 0x0,
    TIMESOURCE_Game = 0x1,
};



class CameraImpl :
    public Moho::RCamCamera, // 0x00E3C474
    public Moho::CScriptEvent // 0x00E3C528 0x00E3C530
{
public:
    std::string mName;
    Moho::STIMap *mMap;
    Moho::GeomCamera3 mCam;
    float v187;
    bool mIsOrtho;
    bool mIsRotated;
    bool mRevertRotation;
    float mFarFOV;
    float mFarPitch;
    float v191;
    float mHeading;
    float mHeadingZoom;
    float mTargetZoom;
    float mNearZoom;
    float mZoom;
    Wm3::Vector3f mOffset;
    Wm3::Vector2f mPivot;
    float mHeadingRate;
    float mZoomRate;
    enum_CamTargetType mTargetType;
    Wm3::Vector3f mTargetLocation;
    Wm3::AxisAlignedBox3f mTargetBox;
    int list1;
    int mTargetEntities;
    int v216;
    int v217;
    float mTargetTimeLeft;
    bool mTargetTime;
    Moho::ECamTimeSource mTimeSource;
    Moho::SystemTimeSource *mSystemTimeSource;
    Moho::GameTimeSource *mGameTimeSource;
    float mLastFrameTime;
    bool mEnableEaseInOut;
    bool v224b;
    bool v224c;
    bool v224d;
    float v225;
    Wm3::Vector3f mTimedMoveOffset;
    float mTimedMoveZoom;
    float v230;
    float v231;
    float mTime;
    float mTimedMovePitch;
    float v234;
    Wm3::Vector3f v235;
    Wm3::Vector3f v238;
    float v241;
    float v242;
    float v243;
    float v244;
    float v245;
    float v246;
    Moho::SCamShakeParams mCamShakeParams;
    float mTotalTime;
    float v255;
    bool mCanShake;
    char v256b;
    char v256c;
    char v256d;
    enum_CameraAccType mAccType;
    float mUpdateFrame;
    float v259;
    int v260;
    gpg::fastvector_n<Moho::UserUnit, 80> mSoundEntitiesInFrustrum;
    gpg::fastvector_n<Moho::UserUnit, 80> mAllUnitsInFrustrum;
    gpg::fastvector_n<Moho::UserUnit, 80> mArmyUnitsInFrustum;
    float mMaxZoomMult;
    int v514;
};

struct RCamManager
{
    std::vector<Moho::RCamCamera> cams;

    std::vector<Moho::RCamCamera> GetAllCameras() asm("0x007AAB60");
};

Moho::RCamManager *GetCamManager(); // 0x007AAC00
std::vector<Moho::RCamCamera> CAM_GetAllRCamCameras(std::vector<Moho::RCamCamera> *dest); // 0x007AADE0

}
