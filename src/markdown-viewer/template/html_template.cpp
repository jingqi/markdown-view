
#include <QFile>
#include <QRegularExpression>

#include "html_template.h"


namespace organic
{

HtmlTemplate::HtmlTemplate()
{
    QFile f(":/markdown-viewer/template.html");
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        _html_template = f.readAll();
}

HtmlTemplate::HtmlTemplate(const QString &templateString)
    : _html_template(templateString)
{}

QString HtmlTemplate::render(const QString &body, RenderOptions options) const
{
    // add scrollbar synchronization
    options |= Template::ScrollbarSynchronization;

    QString htmlBody(body);

    // Mermaid and highlighting.js don't work nicely together
    // So we need to replace the <code> section by a <div> section
    if (options.testFlag(Template::CodeHighlighting) && options.testFlag(Template::DiagramSupport))
        convert_diagram_code_section_to_div(htmlBody);

    return render_as_html(QString(), htmlBody, options);
}

QString HtmlTemplate::export_as_html(const QString &header, const QString &body, RenderOptions options) const
{
    // clear code highlighting option since it depends on the resource file
    options &= ~Template::CodeHighlighting;
    options &= ~Template::ScrollbarSynchronization;

    return render_as_html(header, body, options);
}

QString HtmlTemplate::render_as_html(const QString &header, const QString &body, Template::RenderOptions options) const
{
    if (_html_template.isEmpty())
        return body;

    QString htmlHeader = build_html_header(options);
    htmlHeader += header;

    return QString(_html_template)
        .replace(QLatin1String("<!--__HTML_HEADER__-->"), htmlHeader)
        .replace(QLatin1String("<!--__HTML_CONTENT__-->"), body);
}

QString HtmlTemplate::build_html_header(RenderOptions options) const
{
    QString header;

#if !MARKDOWN_VIEWER_USE_QTWEBKIT
    if (!theme_css_url().isEmpty())
        header += QString("<link rel=\"stylesheet\" href=\"%1\">").arg(theme_css_url());
#endif

    // add javascript for scrollbar synchronization
    if (options.testFlag(Template::ScrollbarSynchronization))
    {
#if MARKDOWN_VIEWER_USE_QTWEBKIT
        header += "<script type=\"text/javascript\">window.onscroll = function() { synchronizer.viewer_scrolled(); }; </script>\n";
#else
        header +=
                "<script type=\"text/javascript\" src=\"qrc:///qtwebchannel/qwebchannel.js\"></script>"
                "<script type=\"text/javascript\">window.onscroll = function() {"
                "    new QWebChannel(qt.webChannelTransport, function (channel) {"
                "        var sync = channel.objects.synchronizer;"
                "        sync.viewer_scrolled(document.body.scrollTop / document.body.scrollHeight);"
                "    });"
                "}; </script>";
#endif
    }

    // add MathJax.js script to HTML header
    if (options.testFlag(Template::MathSupport))
    {
        // Add MathJax support for inline LaTeX Math
        if (options.testFlag(Template::MathInlineSupport))
            header += "<script type=\"text/x-mathjax-config\">MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\\\(','\\\\)']]}});</script>";

#ifdef Q_OS_WIN
        // XXX 部分 Windows 下 MathJax 在 QtWebView 中显示字体过小
        header += "<style type=\"text/css\">span.MathJax{zoom:1.6;}</style>"; // For mathjax HTML-CSS renderer
        header += "<style type=\"text/css\">span.MathJax_SVG{zoom:1.6;}</style>"; // For mathjax SVG renderer
#endif

        // Use online mathjax, and HTML-CSS renderer
        // header += "<script type=\"text/javascript\" src=\"http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML\"></script>\n";

        // Use local mathjax, and SVG renderer
        header += "<script type=\"text/javascript\" src=\"qrc:/mathjax/resources/scripts/mathjax/MathJax.js?config=TeX-AMS-MML_SVG\"></script>\n";
    }

    // add Highlight.js script to HTML header
    if (options.testFlag(Template::CodeHighlighting))
    {
        header += QString("<link rel=\"stylesheet\" href=\"qrc:/highlight.js/resources/scripts/highlight.js/styles/%1.css\">\n").arg(code_highlighting_style());
        header += "<script src=\"qrc:/highlight.js/resources/scripts/highlight.js/highlight.pack.js\"></script>\n";
        header += "<script>hljs.initHighlightingOnLoad();</script>\n";
    }

    // add mermaid.js script to HTML header
    if (options.testFlag(Template::DiagramSupport))
    {
        header += "<link rel=\"stylesheet\" href=\"qrc:/mermaid/resources/scripts/mermaid/mermaid.css\">\n";
        header += "<script src=\"qrc:/mermaid/resources/scripts/mermaid/mermaid.full.min.js\"></script>\n";
    }

    return header;
}

void HtmlTemplate::convert_diagram_code_section_to_div(QString &body) const
{
    static const QRegularExpression rx(QStringLiteral("<pre><code class=\"mermaid\">(.*?)</code></pre>"),
                                       QRegularExpression::DotMatchesEverythingOption);
    body.replace(rx, QStringLiteral("<div class=\"mermaid\">\n\\1</div>"));
}

}
