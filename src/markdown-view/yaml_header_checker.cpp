
#include <QRegularExpression>

#include "yaml_header_checker.h"

namespace mdview
{

YamlHeaderChecker::YamlHeaderChecker(const QString &text)
{
    QRegularExpression rx(R"(^---\s*\n(.*?\n)?(---|\.\.\.)\s*(\n|$))",
                          QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match = rx.match(text);
    if (match.hasMatch())
    {
        _header = match.captured(0);
        _body = text.mid(_header.length());
    }
    else
    {
        _body = text;
    }
}

bool YamlHeaderChecker::has_header() const
{
    return !_header.isEmpty();
}

QString YamlHeaderChecker::header() const
{
    return _header;
}

QString YamlHeaderChecker::body() const
{
    return _body;
}

int YamlHeaderChecker::body_offset() const
{
    return _header.length();
}

}
