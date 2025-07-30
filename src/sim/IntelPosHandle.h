#include "boost/shared_ptr.hpp"
#include "Wm3Vector3.h"
#include "sim/IntelGrid.h"

namespace Moho {

// 0x00E361C4
class CIntelPosHandle
{
    Wm3::Vector3f lastPos;
    unsigned int radius;
    int lastTickUpdated;
    bool isEnabled;
    boost::shared_ptr<Moho::CIntelGrid> grid;

    void UpdatePos(int curTick, Wm3::Vector3f *newPos); // 0x0076F1E0
    
    void Update(Wm3::Vector3f *pos); // 0x0076EFC0

    virtual void AddViz(Wm3::Vector3f *pos);
    virtual void SubViz(Wm3::Vector3f *pos);
};

}
