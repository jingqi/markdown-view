
#include "discount_markdown_converter.h"

extern "C"
{
// For discount
#ifdef Q_OS_WIN
#   include <windows.h>
#endif
#include <mkdio.h>
}

#include "markdown_document.h"
#include "../template/html_template.h"

namespace organic
{

class DiscountMarkdownDocument : public MarkdownDocument
{
    MMIOT *_discount_document;

public:
    explicit DiscountMarkdownDocument(MMIOT *document)
        : _discount_document(document)
    {}

    ~DiscountMarkdownDocument()
    {
        ::mkd_cleanup(_discount_document);
    }

    MMIOT *document() const
    {
        return _discount_document;
    }
};


DiscountMarkdownConverter::DiscountMarkdownConverter()
{}

MarkdownDocument *DiscountMarkdownConverter::create_document(const QString &text, ConverterOptions options)
{
    MMIOT *doc = 0;

    if (text.length() > 0)
    {
        QString markdownText(text);

        // text has to always end with a line break,
        // otherwise characters are missing in HTML
        if (!markdownText.endsWith('\n'))
            markdownText.append('\n');

        unsigned long converterOptions = translate_converter_options(options);

        QByteArray utf8Data = markdownText.toUtf8();
        doc = ::mkd_string(utf8Data, utf8Data.length(), converterOptions);

        ::mkd_compile(doc, converterOptions);
    }

    return new DiscountMarkdownDocument(doc);
}

QString DiscountMarkdownConverter::render_as_html(MarkdownDocument *document)
{
    QString html;

    if (document) {
        DiscountMarkdownDocument *doc = dynamic_cast<DiscountMarkdownDocument*>(document);

        if (doc && doc->document())
        {
            char *out;
            ::mkd_document(doc->document(), &out);

            html = QString::fromUtf8(out);
        }
    }

    return html;
}

QString DiscountMarkdownConverter::render_as_table_of_contents(MarkdownDocument *document)
{
    QString toc;

    if (document)
    {
        DiscountMarkdownDocument *doc = dynamic_cast<DiscountMarkdownDocument*>(document);

        if (doc && doc->document())
        {
            // generate table of contents
            char *out;
            ::mkd_toc(doc->document(), &out);

            toc = QString::fromUtf8(out);
        }
    }

    return toc;
}

Template *DiscountMarkdownConverter::template_renderer() const
{
    static HtmlTemplate htmlTemplate;
    return &htmlTemplate;
}

MarkdownConverter::ConverterOptions DiscountMarkdownConverter::supported_options() const
{
    return MarkdownConverter::AutolinkOption |
           MarkdownConverter::NoStrikethroughOption |
           MarkdownConverter::NoAlphaListOption |
           MarkdownConverter::NoDefinitionListOption |
           MarkdownConverter::NoSmartypantsOption |
           MarkdownConverter::ExtraFootnoteOption |
           MarkdownConverter::NoSuperscriptOption;
}

unsigned long DiscountMarkdownConverter::translate_converter_options(ConverterOptions options) const
{
    // NOTE 语法高亮、流程图需要开启 MKD_FENCEDCODE
    unsigned long converterOptions = MKD_TOC | MKD_NOSTYLE | MKD_FENCEDCODE;

    // autolink
    if (options.testFlag(MarkdownConverter::AutolinkOption))
        converterOptions |= MKD_AUTOLINK;

    // strikethrough
    if (options.testFlag(MarkdownConverter::NoStrikethroughOption))
        converterOptions |= MKD_NOSTRIKETHROUGH;

    // alphabetic lists
    if (options.testFlag(MarkdownConverter::NoAlphaListOption))
        converterOptions |= MKD_NOALPHALIST;

    // definition lists
    if (options.testFlag(MarkdownConverter::NoDefinitionListOption))
        converterOptions |= MKD_NODLIST;

    // SmartyPants
    if (options.testFlag(MarkdownConverter::NoSmartypantsOption))
        converterOptions |= MKD_NOPANTS;

    // Footnotes
    if (options.testFlag(MarkdownConverter::ExtraFootnoteOption))
        converterOptions |= MKD_EXTRA_FOOTNOTE;

    // Superscript
    if (options.testFlag(MarkdownConverter::NoSuperscriptOption))
        converterOptions |= MKD_NOSUPERSCRIPT;

    return converterOptions;
}

}
