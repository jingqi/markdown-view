
#include "hoedown_markdown_converter.h"

extern "C"
{
// For hoedown
#include <src/html.h>
#include <src/markdown.h>
}

#include "markdown_document.h"
#include "../template/html_template.h"

namespace organic
{

class HoedownMarkdownDocument : public MarkdownDocument
{
public:
    explicit HoedownMarkdownDocument(hoedown_buffer *document, unsigned long options)
        : _hoedown_document(document), _converter_options(options)
    {}

    ~HoedownMarkdownDocument()
    {
        ::hoedown_buffer_free(_hoedown_document);
    }

    hoedown_buffer *document() const
    {
        return _hoedown_document;
    }

    unsigned long options() const
    {
        return _converter_options;
    }

private:
    hoedown_buffer *_hoedown_document;
    unsigned long _converter_options;
};


HoedownMarkdownConverter::HoedownMarkdownConverter()
{}

MarkdownDocument *HoedownMarkdownConverter::create_document(const QString &text, ConverterOptions options)
{
    hoedown_buffer *doc = 0;

    if (text.length() > 0)
    {
        QString markdownText(text);

        QByteArray utf8Data = markdownText.toUtf8();
        doc = ::hoedown_buffer_new(utf8Data.length());
        ::hoedown_buffer_puts(doc, utf8Data.data());
    }

    return new HoedownMarkdownDocument(doc, translate_converter_options(options));
}

QString HoedownMarkdownConverter::render_as_html(MarkdownDocument *document)
{
    QString html;

    if (document)
    {
        HoedownMarkdownDocument *doc = dynamic_cast<HoedownMarkdownDocument*>(document);

        if (doc && doc->document())
        {
            hoedown_buffer *in = doc->document();
            hoedown_buffer *out = ::hoedown_buffer_new(64);

            hoedown_renderer *renderer = ::hoedown_html_renderer_new(0, 16);
            hoedown_markdown *markdown = ::hoedown_markdown_new(doc->options(), 16, renderer);

            ::hoedown_markdown_render(out, in->data, in->size, markdown);

            ::hoedown_markdown_free(markdown);
            ::hoedown_html_renderer_free(renderer);

            html = QString::fromUtf8(::hoedown_buffer_cstr(out));

            ::hoedown_buffer_free(out);
        }
    }

    return html;
}

QString HoedownMarkdownConverter::render_as_table_of_contents(MarkdownDocument *document)
{
    QString toc;

    if (document)
    {
        HoedownMarkdownDocument *doc = dynamic_cast<HoedownMarkdownDocument*>(document);

        if (doc->document())
        {
            hoedown_buffer *in = doc->document();
            hoedown_buffer *out = ::hoedown_buffer_new(64);

            hoedown_renderer *renderer = ::hoedown_html_toc_renderer_new(16);
            hoedown_markdown *markdown = ::hoedown_markdown_new(doc->options(), 16, renderer);

            ::hoedown_markdown_render(out, in->data, in->size, markdown);

            ::hoedown_markdown_free(markdown);
            ::hoedown_html_renderer_free(renderer);

            toc = QString::fromUtf8(::hoedown_buffer_cstr(out));

            ::hoedown_buffer_free(out);
        }
    }

    return toc;
}

Template *HoedownMarkdownConverter::template_renderer() const
{
    static HtmlTemplate html_template;
    return &html_template;
}

MarkdownConverter::ConverterOptions HoedownMarkdownConverter::supported_options() const
{
    return MarkdownConverter::AutolinkOption |
           MarkdownConverter::NoStrikethroughOption |
           MarkdownConverter::ExtraFootnoteOption |
           MarkdownConverter::NoSuperscriptOption;
}

unsigned long HoedownMarkdownConverter::translate_converter_options(ConverterOptions options) const
{
    unsigned long converterOptions = HOEDOWN_EXT_FENCED_CODE | HOEDOWN_EXT_TABLES;

    // autolink
    if (options.testFlag(MarkdownConverter::AutolinkOption))
        converterOptions |= HOEDOWN_EXT_AUTOLINK;

    // strikethrough
    if (!options.testFlag(MarkdownConverter::NoStrikethroughOption))
        converterOptions |= HOEDOWN_EXT_STRIKETHROUGH;

//    // alphabetic lists
//    if (!options->isAlphabeticListsEnabled())
//        converterOptions |= MKD_NOALPHALIST;

//    // definition lists
//    if (!options->isDefinitionListsEnabled())
//        converterOptions |= MKD_NODLIST;

//    // SmartyPants
//    if (!options->isSmartyPantsEnabled())
//        converterOptions |= MKD_NOPANTS;

    // Footnotes
    if (options.testFlag(MarkdownConverter::ExtraFootnoteOption))
        converterOptions |= HOEDOWN_EXT_FOOTNOTES;

    // Superscript
    if (!options.testFlag(MarkdownConverter::NoSuperscriptOption))
        converterOptions |= HOEDOWN_EXT_SUPERSCRIPT;

    return converterOptions;
}

}
