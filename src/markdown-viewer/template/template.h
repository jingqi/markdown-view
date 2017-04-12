
#ifndef ___HEADFILE_90271329_BD3E_4099_A9D3_F30163F4259B_
#define ___HEADFILE_90271329_BD3E_4099_A9D3_F30163F4259B_

#include <QString>

#include "../markdown_viewer_config.h"


namespace organic
{

class Template
{
public:
    enum RenderOption
    {
        ScrollbarSynchronization = 0x00000001,
        MathSupport              = 0x00000002,
        CodeHighlighting         = 0x00000004,
        DiagramSupport           = 0x00000008,
        MathInlineSupport        = 0x00000010
    };
    Q_DECLARE_FLAGS(RenderOptions, RenderOption)

    virtual ~Template() {}

    const QString& code_highlighting_style() const
    {
        return _highlighting_style;
    }

    void set_code_highlighting_style(const QString &style)
    {
        _highlighting_style = style;
    }

#if !MARKDOWN_VIEWER_USE_QTWEBKIT
    const QString& theme_css_url() const
    {
        return _theme_css_url;
    }

    void set_theme_css_url(const QString &css_url)
    {
        _theme_css_url = css_url;
    }
#endif

    virtual QString render(const QString &body, RenderOptions options) const = 0;
    virtual QString export_as_html(const QString &header, const QString &body, RenderOptions options) const = 0;

private:
    QString _highlighting_style;
#if !MARKDOWN_VIEWER_USE_QTWEBKIT
    QString _theme_css_url;
#endif
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Template::RenderOptions)

}

#endif
