
#include <assert.h>

#include <QFile>

#include "markdown_viewer_config.h"
#include "converter/hoedown_markdown_converter.h"
#include "converter/discount_markdown_converter.h"

#include "yaml_header_checker.h"
#include "html_preview_generator.h"


namespace organic
{

HtmlPreviewGenerator::HtmlPreviewGenerator(QObject *parent)
    : QThread(parent)
{}

void HtmlPreviewGenerator::set_options(MarkdownViewerOptions *options)
{
    assert(NULL != options);
    _options = options;
    markdown_converter_changed();
}

bool HtmlPreviewGenerator::is_supported(MarkdownConverter::ConverterOption option) const
{
    return _converter->supported_options().testFlag(option);
}

void HtmlPreviewGenerator::markdown_text_changed(const QString &text)
{
    // cut YAML header
    YamlHeaderChecker checker(text);
    QString actualText = checker.has_header() ? checker.body() : text;

    // enqueue task to parse the markdown text and generate a new HTML document
    QMutexLocker locker(&_tasks_mutex);
    _tasks.enqueue(actualText);
    _buffer_not_empty.wakeOne();
}

QString HtmlPreviewGenerator::export_html(const QString &styleSheet, const QString &highlightingScript)
{
    if (!_document)
        return QString();

    QString header;
    if (!styleSheet.isEmpty())
        header += QString("\n<style>%1</style>").arg(styleSheet);

    if (!highlightingScript.isEmpty())
    {
        // FIXME: doesn't really belong here
        QString highlightStyle;
        QFile f(QString(":/scripts/highlight.js/styles/%1.css").arg(
                _converter->template_renderer()->code_highlighting_style()));
        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
            highlightStyle = f.readAll();

        header += QString("\n<style>%1</style>").arg(highlightStyle);
        header += QString("\n<script>%1</script>").arg(highlightingScript);
        header += "\n<script>hljs.initHighlightingOnLoad();</script>";
    }

    return _converter->template_renderer()->
            export_as_html(header, _converter->render_as_html(_document), render_options());
}

void HtmlPreviewGenerator::set_math_support_enabled(bool enabled)
{
    _options->enable_math_support = enabled;

    // regenerate a HTML document
    generate_html_from_markdown();
}

void HtmlPreviewGenerator::set_diagram_support_enabled(bool enabled)
{
    _options->enable_diagram_support = enabled;

    // regenerate a HTML document
    generate_html_from_markdown();
}

void HtmlPreviewGenerator::set_code_highlighting_enabled(bool enabled)
{
    _options->enable_code_highlight = enabled;

    // regenerate a HTML document
    generate_html_from_markdown();
}

void HtmlPreviewGenerator::set_code_highlighting_style(const QString &style)
{
    _converter->template_renderer()->set_code_highlighting_style(style);

    // regenerate a HTML document
    generate_html_from_markdown();
}

#if WITH_QTWEBENGINE
void HtmlPreviewGenerator::set_theme_css_url(const QString &css_url)
{
    _converter->template_renderer()->set_theme_css_url(css_url);
    generate_html_from_markdown();
}
#endif

void HtmlPreviewGenerator::markdown_converter_changed()
{
    QString style;

    if (_converter)
    {
        style = _converter->template_renderer()->code_highlighting_style();
        delete _converter;
    }

#if USE_HOEDOWN_CONVERTER
    _converter = new HoedownMarkdownConverter;
    _converter->template_renderer()->set_code_highlighting_style(style);
#else
    _converter = new DiscountMarkdownConverter;
    _converter->template_renderer()->set_code_highlighting_style(style);
#endif
}

void HtmlPreviewGenerator::run()
{
    forever {
        QString text;

        {
            // wait for new task
            QMutexLocker locker(&_tasks_mutex);
            while (_tasks.count() == 0)
                _buffer_not_empty.wait(&_tasks_mutex);

            // get last task from queue and skip all previous tasks
            while (!_tasks.isEmpty())
                text = _tasks.dequeue();
        }

        // end processing?
        if (text.isNull())
            return;

        // delay processing to see if more tasks are coming
        // (e.g. because the user is typing fast)
        this->msleep(calculate_delay(text));

        // no more new tasks?
        if (_tasks.isEmpty() && NULL != _converter)
        {
            // delete previous markdown document
            delete _document;

            // generate HTML from markdown
            _document = _converter->create_document(text, converter_options());
            generate_html_from_markdown();

            // generate table of contents
            generate_table_of_contents();
        }
    }
}

void HtmlPreviewGenerator::generate_html_from_markdown()
{
    if (!_document)
        return;

    QString html = _converter->template_renderer()->render(_converter->render_as_html(_document), render_options());
    emit html_result_ready(html);
}

void HtmlPreviewGenerator::generate_table_of_contents()
{
    if (!_document)
        return;

    QString toc = _converter->render_as_table_of_contents(_document);
    QString styledToc = QString("<html><head>\n<style type=\"text/css\">ul { list-style-type: none; padding: 0; margin-left: 1em; } a { text-decoration: none; }</style>\n</head><body>%1</body></html>").arg(toc);
    emit toc_result_ready(styledToc);
}

MarkdownConverter::ConverterOptions HtmlPreviewGenerator::converter_options() const
{
    MarkdownConverter::ConverterOptions parserOptionFlags(
        MarkdownConverter::TableOfContentsOption | MarkdownConverter::NoStyleOption);

    // autolink
    if (_options->enable_autolink)
        parserOptionFlags |= MarkdownConverter::AutolinkOption;

    // strikethrough
    if (!_options->enable_strikethrough)
        parserOptionFlags |= MarkdownConverter::NoStrikethroughOption;

    // alphabetic lists
    if (!_options->enable_alphabetic_lists)
        parserOptionFlags |= MarkdownConverter::NoAlphaListOption;

    // definition lists
    if (!_options->enable_definition_lists)
        parserOptionFlags |= MarkdownConverter::NoDefinitionListOption;

    // SmartyPants
    if (!_options->enable_smarty_pants)
        parserOptionFlags |= MarkdownConverter::NoSmartypantsOption;

    // Footnotes
    if (_options->enable_footnotes)
        parserOptionFlags |= MarkdownConverter::ExtraFootnoteOption;

    // Superscript
    if (!_options->enable_superscript)
        parserOptionFlags |= MarkdownConverter::NoSuperscriptOption;

    return parserOptionFlags;
}

Template::RenderOptions HtmlPreviewGenerator::render_options() const
{
    Template::RenderOptions renderOptionFlags;

    // math support
    if (_options->enable_math_support)
        renderOptionFlags |= Template::MathSupport;

    // inline math support
    if (_options->enable_math_inline_support)
        renderOptionFlags |= Template::MathInlineSupport;

    // diagram support
    if (_options->enable_diagram_support)
        renderOptionFlags |= Template::DiagramSupport;

    // code highlighting
    if (_options->enable_code_highlight)
        renderOptionFlags |= Template::CodeHighlighting;

    return renderOptionFlags;
}

int HtmlPreviewGenerator::calculate_delay(const QString &text)
{
    const int MINIMUM_DELAY = 200;
    const int MAXIMUM_DELAY = 2000;

    // calculate the processing delay based on amount of characters in the
    // markdown text. The delay is restricted to the interval [50, 2000] milliseconds;
    int delay = qMin(qMax(text.size() / 100, MINIMUM_DELAY), MAXIMUM_DELAY);

    return delay;
}

}
