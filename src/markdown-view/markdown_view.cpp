
#include <assert.h>

#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>

#include "markdown_view.h"

#if WITH_QTWEBENGINE
#   include <QWebEngineSettings>
#else
#   include <QWebFrame>
#endif

#include "html_preview_generator.h"


namespace organic
{

MarkdownView::MarkdownView(QWidget *parent)
#if WITH_QTWEBENGINE
    :QWebEngineView(parent),
#else
    : QWebView(parent),
#endif
     _preview_generator(new HtmlPreviewGenerator)
{
#if WITH_QTWEBENGINE
    settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
#else
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
#endif

    init_actions();
    init_popup_menu();

    _base_url = QUrl::fromLocalFile(qApp->applicationDirPath());

    // Setup preview
    connect(_preview_generator, SIGNAL(html_result_ready(QString)),
            this, SLOT(html_result_ready(QString)));
    _preview_generator->start();
}

MarkdownView::~MarkdownView()
{
    _preview_generator->markdown_text_changed(QString());
    _preview_generator->wait();
    delete _preview_generator;
    _preview_generator = NULL;
}

void MarkdownView::init_actions()
{
    assert(NULL == _action_reload);
    _action_reload = new QAction(QIcon(":/markdown-view/refresh"), tr("重新加载(&R)"), this);
    _action_reload->setStatusTip(tr("重新加载页面"));
    _action_reload->setToolTip(tr("重新加载页面"));
    _action_reload->setShortcut(QKeySequence::Refresh);
    _action_reload->setPriority(QAction::LowPriority);
    connect(_action_reload, SIGNAL(triggered()),
           this, SLOT(reload()));

    assert(NULL == _action_zoom_in);
    _action_zoom_in = new QAction(QIcon(":/markdown-view/zoom-in"), tr("放大(&I)"), this);
    _action_zoom_in->setStatusTip(tr("放大"));
    _action_zoom_in->setToolTip(tr("放大"));
    _action_zoom_in->setShortcut(QKeySequence::ZoomIn);
    _action_zoom_in->setPriority(QAction::LowPriority);
    connect(_action_zoom_in, SIGNAL(triggered()),
            this, SLOT(zoom_in()));

    assert(NULL == _action_zoom_out);
    _action_zoom_out = new QAction(QIcon(":/markdown-view/zoom-out"), tr("缩小(&O)"), this);
    _action_zoom_out->setStatusTip(tr("缩小"));
    _action_zoom_out->setToolTip(tr("缩小"));
    _action_zoom_out->setShortcut(QKeySequence::ZoomOut);
    _action_zoom_out->setPriority(QAction::LowPriority);
    connect(_action_zoom_out, SIGNAL(triggered()),
            this, SLOT(zoom_out()));

    assert(NULL == _action_reset_zoom);
    _action_reset_zoom = new QAction(QIcon(":/markdown-view/reset-zoom"), tr("重置缩放(&Z)"), this);
    _action_reset_zoom->setStatusTip(tr("重置缩放"));
    _action_reset_zoom->setToolTip(tr("重置缩放"));
    _action_reset_zoom->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    _action_reset_zoom->setPriority(QAction::LowPriority);
    connect(_action_reset_zoom, SIGNAL(triggered()),
            this, SLOT(reset_zoom()));
}

void MarkdownView::init_popup_menu()
{
    assert(NULL == _menu_popup);
    _menu_popup = new QMenu(this);

    // _menu_popup->addAction(_action_reload);
    // _menu_popup->addSeparator();
    _menu_popup->addAction(_action_zoom_in);
    _menu_popup->addAction(_action_zoom_out);
    _menu_popup->addAction(_action_reset_zoom);
}

void MarkdownView::set_options(MarkdownViewOptions *options)
{
    _preview_generator->set_options(options);
}

void MarkdownView::set_code_highlighting_style(const QString &style_name)
{
    _preview_generator->set_code_highlighting_style(style_name);
}

void MarkdownView::set_theme_css_url(const QString &css_url)
{
#if WITH_QTWEBENGINE
    _preview_generator->set_theme_css_url(css_url);
#else
    page()->settings()->setUserStyleSheetUrl(QUrl(css_url));
#endif
}

void MarkdownView::set_base_url(const QUrl &base_url)
{
    _base_url = base_url;
}

void MarkdownView::set_markdown_content(const QString &markdown)
{
    _preview_generator->markdown_text_changed(markdown);
}

void MarkdownView::html_result_ready(const QString &html)
{
#if !defined(NDEBUG) && 0
    qDebug() << html << endl;
#endif
    const QString post = post_process_html(html);
    setHtml(post, _base_url);
    emit html_ready();
}

QString MarkdownView::post_process_html(const QString &html)
{
    return html;
}

void MarkdownView::scroll_to_anchor(const QUrl &url)
{
    QString anchor = url.toString().remove("#");
#if !WITH_QTWEBENGINE
    page()->mainFrame()->scrollToAnchor(anchor);
#endif
}

void MarkdownView::contextMenuEvent(QContextMenuEvent *e)
{
    assert(NULL != e);
    if (NULL == _menu_popup)
        return;
    _menu_popup->exec(e->globalPos());
}

void MarkdownView::zoom_in()
{
    setZoomFactor(zoomFactor() * 1.1);
}

void MarkdownView::zoom_out()
{
    setZoomFactor(zoomFactor() * 0.9);
}

void MarkdownView::reset_zoom()
{
    setZoomFactor(1);
}

}
