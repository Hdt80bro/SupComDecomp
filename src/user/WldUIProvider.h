#include "core/ScriptObject.h"
#include <vector>

namespace Moho {

struct SOCellPos
{
    short x;
    short z;
};

// 0x00E47D20
class IWldUIProvider
{
public:
    virtual ~IWldUIProvider() = default; // 0x0086A360
    virtual void StartLoadingDialog() = 0;
    virtual void UpdateLoadingDialog(float delta) = 0;
    virtual void StopLoadingDialog() = 0;
    virtual void StartWaitingDialog() = 0;
    virtual void UpdateWaitingDialog(float delta) = 0;
    virtual void StopWaitingDialog() = 0;
    virtual void CreateGameInterface(bool isReplay) = 0;
    virtual void OnStart() = 0;
    virtual void DestroyGameInterface() = 0;
};

// 0x00E47D4C
class CLuaWldUIProvider :
    public Moho::IWldUIProvider,
    public Moho::CScriptObject
{
public:
    std::vector<Moho::SOCellPos> mPrefetchData;

    ~CLuaWldUIProvider() override = default; // 0x0086A5A0
    void StartLoadingDialog() override; // 0x0086A650
    void UpdateLoadingDialog(float delta) override; // 0x0086A660
    void StopLoadingDialog() override; // 0x0086A680
    void StartWaitingDialog() override; // 0x0086A690
    void UpdateWaitingDialog(float delta) override; // 0x0086A6A0
    void StopWaitingDialog() override; // 0x0086A6C0
    void CreateGameInterface(bool isReplay) override; // 0x0086A6E0
    void OnStart() override; // 0x0086A6D0
    void DestroyGameInterface() override; // 0x0086A8C0

};


}
