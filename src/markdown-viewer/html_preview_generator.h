
#ifndef ___HEADFILE_4D36612C_DDFB_43E6_A313_63FDCE1DAD37_
#define ___HEADFILE_4D36612C_DDFB_43E6_A313_63FDCE1DAD37_

#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>

#include "markdown_document.h"
#include "converter/markdown_converter.h"
#include "template/template.h"
#include "markdown_viewer_options.h"

namespace organic
{

class HtmlPreviewGenerator : public QThread
{
    Q_OBJECT

    MarkdownViewerOptions *_options = NULL;
    MarkdownDocument *_document = NULL;
    MarkdownConverter *_converter = NULL;
    QQueue<QString> _tasks;
    QMutex _tasks_mutex;
    QWaitCondition _buffer_not_empty;

public:
    explicit HtmlPreviewGenerator(QObject *parent = 0);

    void set_options(MarkdownViewerOptions *options);
    bool is_supported(MarkdownConverter::ConverterOption option) const;

public slots:
    void markdown_text_changed(const QString &text);
    QString export_html(const QString &styleSheet, const QString &highlightingScript);

    void set_math_support_enabled(bool enabled);
    void set_diagram_support_enabled(bool enabled);
    void set_code_highlighting_enabled(bool enabled);
    void set_code_highlighting_style(const QString &style);
#if WITH_QTWEBENGINE
    void set_theme_css_url(const QString &css_url);
#endif

    void markdown_converter_changed();

signals:
    void html_result_ready(const QString &html);
    void toc_result_ready(const QString &toc);

protected:
    virtual void run() override;

private:
    void generate_html_from_markdown();
    void generate_table_of_contents();
    MarkdownConverter::ConverterOptions converter_options() const;
    Template::RenderOptions render_options() const;
    int calculate_delay(const QString &text);
};

}

#endif
