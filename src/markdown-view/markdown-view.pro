
TARGET = markdown-view
TEMPLATE = lib
VERSION = 1.0.0

include(../global.pri)

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

qtHaveModule(webkitwidgets) {
    QT += webkitwidgets
    DEFINES += WITH_QTWEBENGINE=0
} else: qtHaveModule(webenginewidgets) {
    QT += webenginewidgets webchannel
    DEFINES += WITH_QTWEBENGINE=1
} else {
    message(Neither QtWebEngine nor QtWebEngine found!)
}

DEFINES += BUILDING_MARKDOWN_PREVIEWER_SHARED_LIB

# 头文件
HEADERS += $$files(*.h, true)

# 源文件
SOURCES += $$files(*.cpp, true)

# ui 文件
FORMS += $$files(*.ui, true)

# 资源文件
RESOURCES += $$files(*.qrc, true)

# discount
INCLUDEPATH += \
    $$PWD/../../3rdparty/discount.git \
    $$PWD/../discount
LIBS += -L$$OUT_PWD/../discount$${OUT_TAIL} -ldiscount

# hoedown
INCLUDEPATH += $$PWD/../../3rdparty/hoedown.git
LIBS += -L$$OUT_PWD/../hoedown$${OUT_TAIL} -lhoedown
