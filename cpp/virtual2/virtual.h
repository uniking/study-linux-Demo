//---------------virtual.h
class human
{
public:
    virtual void echo();
    void echo2();
    void echo3();
    virtual void humanecho();
    virtual ~human();
};

class woman:public human
{
public:
    virtual void echo();
    void echo2();

    void womanecho();
    virtual void womanecho2();

    virtual ~woman();
};

