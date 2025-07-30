#include <vector>
#include <string>
#include "gpgcore/containers/fastvector.h"
#include "user/UserUnit.h"

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
    Moho::VTransform tranform;
    gpg::gal::Matrix projection;
    gpg::gal::Matrix view;
    gpg::gal::Matrix viewProjection;
    gpg::gal::Matrix inverseProjection;
    gpg::gal::Matrix inverseView;
    gpg::gal::Matrix inverseViewProjection;
    int alignment;
    Moho::CGeomSolid3 solid1;
    Moho::CGeomSolid3 solid2;
    float lodScale;
    Moho::VMatrix4 viewport;
    int v160;
};

// 0x00E3C544
class RCamCamera : public Moho::Broadcaster<Moho::SCameraTracking>
{
};

static Moho::RCamManager camManager; // 0x010C79D4

struct SCamShakeParams
{
    float v1;
    float v2;
    float v3;
    float v4;
    float v5;
    float v6;
    float minTime;
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
    std::string name;
    Moho::STIMap *stiMap;
    Moho::GeomCamera3 cam;
    float v187;
    bool isOrtho;
    bool isRotated;
    bool revertRotation;
    float farFOV;
    float farPitch;
    float v191;
    float heading;
    float headingZoom;
    float targetZoom;
    float nearZoom;
    float zoom;
    Wm3::Vector3f offset;
    Wm3::Vector2f pivot;
    float headingRate;
    float zoomRate;
    enum_CamTargetType targetType;
    Wm3::Vector3f targetLocation;
    Wm3::AxisAlignedBox3f targetBox;
    int list1;
    int targetEntities;
    int v216;
    int v217;
    float targetTimeLeft;
    bool targetTime;
    Moho::ECamTimeSource timeSource;
    Moho::SystemTimeSource *systemTimeSource;
    Moho::GameTimeSource *gameTimeSource;
    float lastFrameTime;
    bool enableEaseInOut;
    bool v224b;
    bool v224c;
    bool v224d;
    float v225;
    Wm3::Vector3f timedMoveOffset;
    float timedMoveZoom;
    float v230;
    float v231;
    float time;
    float timedMovePitch;
    float v234;
    Wm3::Vector3f v235;
    Wm3::Vector3f v238;
    float v241;
    float v242;
    float v243;
    float v244;
    float v245;
    float v246;
    Moho::SCamShakeParams camShakeParams;
    float totalTime;
    float v255;
    bool canShake;
    char v256b;
    char v256c;
    char v256d;
    enum_CameraAccType accType;
    float updateFrame;
    float v259;
    int v260;
    gpg::fastvector_n<Moho::UserUnit, 80> soundEntitiesInFrustrum;
    gpg::fastvector_n<Moho::UserUnit, 80> allUnitsInFrustrum;
    gpg::fastvector_n<Moho::UserUnit, 80> armyUnitsInFrustum;
    float maxZoomMult;
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
