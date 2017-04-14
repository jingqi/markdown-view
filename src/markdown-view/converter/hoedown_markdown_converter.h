
#ifndef ___HEADFILE_D09B2763_9B7A_40F5_A02F_897F6D3B251B_
#define ___HEADFILE_D09B2763_9B7A_40F5_A02F_897F6D3B251B_

#include <QString>

#include "markdown_converter.h"

namespace mdview
{

class HoedownMarkdownConverter : public MarkdownConverter
{
public:
    HoedownMarkdownConverter();

    virtual MarkdownDocument *create_document(const QString &text, ConverterOptions options) override;
    virtual QString render_as_html(MarkdownDocument *document) override;
    virtual QString render_as_table_of_contents(MarkdownDocument *document) override;

    virtual Template *template_renderer() const override;

    virtual ConverterOptions supported_options() const override;

private:
    unsigned long translate_converter_options(ConverterOptions options) const;
};

}

#endif
