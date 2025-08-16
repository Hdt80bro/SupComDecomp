#include "core/IntelGrid.h"
#include "Wm3Vector3.h"
#include "boost/shared_ptr.hpp"

namespace Moho {

// 0x00E361C4
class CIntelPosHandle
{
public:
    Wm3::Vector3f mLastPos;
    unsigned int mRadius;
    int mLastTickUpdated;
    bool mEnabled;
    boost::shared_ptr<Moho::CIntelGrid> mGrid;

    virtual void AddViz();
    virtual void SubViz();

    void UpdatePos(int curTick, Wm3::Vector3f *newPos); // 0x0076F1E0    
    void Update(Wm3::Vector3f *pos); // 0x0076EFC0
};

}
