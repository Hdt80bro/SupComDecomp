
namespace Moho {

    struct CMersenneTwister
    {
        unsigned int state[624];
        int k;

        void Seed();
        void ShuffleState();
    };
    
    struct CRandomStream
    {
        Moho::CMersenneTwister twister;
        float marsagliaPair;
        bool hasMarsagliaPair;
    };

};