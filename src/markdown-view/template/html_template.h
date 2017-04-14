
#ifndef ___HEADFILE_907432E7_C9B7_47D1_ADED_57C5F4E6A373_
#define ___HEADFILE_907432E7_C9B7_47D1_ADED_57C5F4E6A373_

#include "template.h"


namespace organic
{

class HtmlTemplate : public Template
{
    QString _html_template;

public:
    HtmlTemplate();
	explicit HtmlTemplate(const QString &templateString);

    virtual QString render(const QString &body, RenderOptions options) const override;
    virtual QString export_as_html(const QString &header, const QString &body, RenderOptions options) const override;

private:
    QString render_as_html(const QString &header, const QString &body, RenderOptions options) const;
    QString build_html_header(RenderOptions options) const;
    void convert_diagram_code_section_to_div(QString &body) const;
};

}

#endif
