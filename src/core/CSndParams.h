
namespace Moho {

    struct CSndParams
    {
        std::string cue1;
        std::string bank;
        CSndVar *lodCutoff;
        CSndVar *cue2;
        int v1;
        __int16 cueId;
        __int16 bankId;
        boost::shared_ptr<AudioEngine> v3;
    };    

};