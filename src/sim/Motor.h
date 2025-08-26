#include "sim/Entity.h"
#include "core/ScriptObject.h"

namespace Moho {

// 0x00E29180
class Motor
{
public:
    virtual void Func1() {} // 0x00694AB0
    virtual void Update(Moho::Entity *ent) = 0;
    virtual ~Motor() = default; // 0x00694AD0
};

class MotorFallDown :
    public Moho::Motor,        // 0x00E29190
    public Moho::CScriptObject // 0x00E291A0
{
public:
    float mDirection;
    float mAngle;
    float mDepth;
    bool mFlag;

    void Update(Moho::Entity *ent) override; // 0x00695180
    ~MotorFallDown() override = default; // 0x00694D70
};

class MotorSinkAway :
    public Moho::Motor,        // 0x00E29264
    public Moho::CScriptObject // 0x00E29274
{
public:
    float mDY;
    
    void Update(Moho::Entity *ent) override; // 0x00696940
    ~MotorSinkAway() override = default; // 0x00696580
};

}
