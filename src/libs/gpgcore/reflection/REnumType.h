
namespace gpg {

    struct REnumType : RType
    {
        const char *prefix;
        std::vector<struct_Enumvalue> enumnames;
    };

};