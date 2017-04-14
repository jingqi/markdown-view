
#ifndef ___HEADFILE_838F647F_D828_4605_AC67_69B4FEC55C43_
#define ___HEADFILE_838F647F_D828_4605_AC67_69B4FEC55C43_

#include "markdown_converter.h"


namespace organic
{

class DiscountMarkdownConverter : public MarkdownConverter
{
public:
    DiscountMarkdownConverter();

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
