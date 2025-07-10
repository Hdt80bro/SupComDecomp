
namespace Moho {

    class CIntelPosHandle
    {
        Wm3::Vector3f lastPos;
        unsigned int radius;
        int lastTickUpdated;
        bool isEnabled;
        boost::shared_ptr<CIntelGrid> grid;

        void __usercall UpdatePos(
            Moho::CIntelPosHandle *this __EDX,
            int curTick __EAX,
            Wm3::Vector3f *newPos __ECX) asm("0x0076F1E0");
        
        void __usercall Update(Moho::CIntelPosHandle *this __ESI, Wm3::Vector3f *pos __EBX) asm("0x0076EFC0");

        virtual void __thiscall AddViz(Wm3::Vector3f *pos);
        virtual void __thiscall SubViz(Wm3::Vector3f *pos);
    };

};