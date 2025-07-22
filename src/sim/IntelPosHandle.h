
namespace Moho {

    class CIntelPosHandle
{
    Wm3::Vector3f lastPos;
    unsigned int radius;
    int lastTickUpdated;
    bool isEnabled;
    boost::shared_ptr<CIntelGrid> grid;

    void UpdatePos(int curTick, Wm3::Vector3f *newPos); // 0x0076F1E0
    
    void Update(Wm3::Vector3f *pos); // 0x0076EFC0

    virtual void AddViz(Wm3::Vector3f *pos);
    virtual void SubViz(Wm3::Vector3f *pos);
};

}
