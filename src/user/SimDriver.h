
namespace Moho {

// 0x00E33464
class ISTIDriver
{
public:
    virtual ~ISTIDriver() = default; // 0x0073B0D0
    virtual void DisconnectClients() = 0;
    virtual void Shutdown() = 0;
};

// 0x00E3350C
class CSimDriver : public Moho::ISTIDriver
{
public:

    ~CSimDriver() override; // 0x0073B910
};

}
