
#include <QApplication>
#include <QFile>
#include <QIODevice>

#include <markdown-viewer/markdown_viewer.h>

using namespace organic;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QFile f(":/test.md");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QString all = f.readAll();

    MarkdownViewer w;
    MarkdownViewerOptions options;
    w.set_options(&options);
    w.set_code_highlighting_style("solarized-light");
    w.set_theme_css_url("qrc:/markdown-viewer/css/solarized-light.css");
    w.set_markdown_content(all);
    w.show();

    return a.exec();
}
