
#ifndef ___HEADFILE_A14F7347_4102_4E5F_A6FF_1F53A227C480_
#define ___HEADFILE_A14F7347_4102_4E5F_A6FF_1F53A227C480_

#include "markdown_viewer_config.h"

#if WITH_QTWEBENGINE
#   include <QWebEngineView>
#else
#   include <QWebView>
#endif

#include "markdown_viewer_options.h"

namespace organic
{

class HtmlPreviewGenerator;

#if WITH_QTWEBENGINE
class MDVIEWER_API MarkdownViewer : public QWebEngineView
#else
class MDVIEWER_API MarkdownViewer : public QWebView
#endif
{
    Q_OBJECT

    QAction *_action_reload = NULL,
        *_action_zoom_in = NULL, *_action_zoom_out = NULL, *_action_reset_zoom = NULL;
    QMenu *_menu_popup = NULL;

    QUrl _base_url;
    HtmlPreviewGenerator *_preview_generator = NULL;

public:
    MarkdownViewer(QWidget *parent = NULL);
    ~MarkdownViewer();

    void set_options(MarkdownViewerOptions *options);
    void set_code_highlighting_style(const QString& style_name);
    void set_theme_css_url(const QString& css_url);
    void set_base_url(const QUrl& base_url);
    void set_markdown_content(const QString& markdown);
    void scroll_to_anchor(const QUrl& url);

protected:
    virtual QString post_process_html(const QString& html);

private:
    void init_actions();
    void init_popup_menu();

    virtual void contextMenuEvent(QContextMenuEvent *e) override;

signals:
    void html_ready();

private slots:
    void html_result_ready(const QString& html);
    void zoom_in();
    void zoom_out();
    void reset_zoom();
};

}

#endif
