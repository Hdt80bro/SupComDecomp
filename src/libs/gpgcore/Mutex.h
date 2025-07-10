namespace gpg {

    struct Mutex
    {
        LPCRITICAL_SECTION m_mutex;
        bool m_critical_section;
    };

};