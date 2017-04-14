
#ifndef ___HEADFILE_F8A3B755_8656_4D02_A611_CF0B1553E2EB_
#define ___HEADFILE_F8A3B755_8656_4D02_A611_CF0B1553E2EB_

namespace mdview
{

class MarkdownViewOptions
{
public:
    bool enable_math_support = true;
    bool enable_math_inline_support = true;
    bool enable_diagram_support = true;
    bool enable_code_highlight = true;

    bool enable_autolink = true;
    bool enable_strikethrough = true;
    bool enable_alphabetic_lists = true;
    bool enable_definition_lists = true;
    bool enable_smarty_pants = true;
    bool enable_footnotes = true;
    bool enable_superscript = true;
};

}

#endif
