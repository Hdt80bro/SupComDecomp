#include "core/blueprints/Blueprint.h"
#include <string>
#include <vector>

namespace Moho {

struct RMeshBlueprintLOD
{
    std::string mMeshName;
    std::string mAlbedoName;
    std::string mSpecularName;
    std::string mLookupName;
    std::string mSecondaryName;
    std::string mShaderName;
    std::string mStr;
    float mLODCutoff;
    bool mScrolling;
    bool mOcclude;
    bool mSilhouette;
};


class RMeshBlueprint : public Moho::RBlueprint
{
public:
    std::vector<Moho::RMeshBlueprintLOD> mLODs;
    float mIconFadeInZoom;
    float mSortOrder;
    float mUniformScale;
    bool mStraddleWater;
};

}
