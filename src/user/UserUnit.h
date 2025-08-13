#include "user/UserEntity.h"
#include "core/IUnit.h"
#include "core/ScriptObject.h"

namespace Moho {

class UserUnit :
    public Moho::UserEntity,   // 0x00E4D93C
    public Moho::IUnit,        // 0x00E4D9AC
    public Moho::CScriptObject // 0x00E4DA08
{
public:
    Moho::SSTIUnitConstantData mConstDat;
    int gap;
    Moho::SSTIUnitVariableData mVarDat;
    int v155;
    int mCreator;
    int v157;
    struct_UserUnitManager *mManager;
    struct_UserUnitManager *mFactoryManager;
    int v160;
    int mSelectionSets;
    int mSelectionSetCount;
    bool v163a;
    bool mIsEngineer;
    bool mIsFactory;
    bool v163d;
    int v164;
    int v165;

};

}
