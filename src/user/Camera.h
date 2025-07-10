
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
        _DWORD alignment;
        Moho::CGeomSolid3 solid1;
        Moho::CGeomSolid3 solid2;
        float lodScale;
        Moho::VMatrix4 viewport;
        int v160;
    };


    class RCamCamera : Moho::Broadcaster
    {
    };

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


    class CameraImpl : Moho::RCamCamera, Moho::CScriptEvent
    {
        std::string name;
        Moho::STIMap *stiMap;
        Moho::GeomCamera3 cam;
        float v187;
        _BYTE isOrtho;
        _BYTE isRotated;
        _BYTE revertRotation;
        _BYTE bytesd;
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
        _DWORD list1;
        _DWORD targetEntities;
        _DWORD v216;
        _DWORD v217;
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
        _BYTE canShake;
        _BYTE v256b;
        _BYTE v256c;
        _BYTE v256d;
        enum_CameraAccType accType;
        float updateFrame;
        float v259;
        _DWORD v260;
        gpg::fastvector_n<Moho::UserUnit, 80> soundEntitiesInFrustrum;
        gpg::fastvector_n<Moho::UserUnit, 80> allUnitsInFrustrum;
        gpg::fastvector_n<Moho::UserUnit, 80> armyUnitsInFrustum;
        float maxZoomMult;
        _DWORD v514;
    };

    struct RCamManager
    {
        std::vector<Moho::RCamCamera> cams;

        std::vector<Moho::RCamCamera> GetAllCameras() asm("0x007AAB60");
    };

    
    static Moho::RCamManager camManager asm("0x010C79D4");

    Moho::RCamManager *GetCamManager() asm("0x007AAC00");
    std::vector<Moho::RCamCamera> CAM_GetAllRCamCameras(std::vector<Moho::RCamCamera> *dest) asm("0x007AADE0");


};