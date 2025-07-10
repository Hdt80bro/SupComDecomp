
namespace Moho {

    struct CAiFormationDBImpl
    {
        Moho::Sim *sim;
        gpg::fastvector_n<Moho::CAiFormationInstance, 10> formInstances;
    };
    

};