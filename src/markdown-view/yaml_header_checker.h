
#ifndef ___HEADFILE_9B489DD6_E32E_49AD_A9B6_AF6FF2D34212_
#define ___HEADFILE_9B489DD6_E32E_49AD_A9B6_AF6FF2D34212_

#include <QString>

namespace mdview
{

class YamlHeaderChecker
{
    QString _header;
    QString _body;

public:
    YamlHeaderChecker(const QString &text);

    bool has_header() const;
    QString header() const;
    QString body() const;

    int body_offset() const;
};

}

#endif
