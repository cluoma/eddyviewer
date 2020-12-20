//
// Created by colin on 2020-12-16.
//

#ifndef EDDYVIEWER_CAMERASHOT_H
#define EDDYVIEWER_CAMERASHOT_H

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

namespace dbo = Wt::Dbo;

class CameraShot
{
public:
    std::string filename;
    Wt::WDateTime serverdate;
    int64_t serverdateint;
    Wt::WDateTime createddate;
    int64_t createddateint;

    template<class Action>
    void persist(Action& a)
    {
        dbo::field(a, filename, "filename");
        dbo::field(a, serverdate, "serverdate");
        dbo::field(a, serverdateint, "serverdateint");
        dbo::field(a, createddate, "createddate");
        dbo::field(a, createddateint, "createddateint");
    }
};

namespace Wt
{
    namespace Dbo
    {
        template<>
        struct dbo_traits<CameraShot> : public dbo_default_traits
        {
            static const char *versionField()
            {
                return 0;
            }
        };
    }
}


#endif //EDDYVIEWER_CAMERASHOT_H
