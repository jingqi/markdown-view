
TARGET = markdown-viewer
TEMPLATE = lib
VERSION = 1.0.0

include(../../global.pri)

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

# FIXME 从 Qt5.6 开始， QWebKit 不再被默认支持，改为使用 QtWebEngine
#       但是 Windows 下到 Qt5.7 为止，Mingw 尚不支持 QtWebEngine
lessThan(QT_VERSION, 5.6) {
    QT += webkitwidgets
} else {
    win32 {
        message(QtWebEngine not supported yet for mingw in Windows!)
    } else {
        QT += webenginewidgets webchannel
    }
}

DEFINES += BUILDING_MARKDOWN_VIEWER_DLL

# 头文件
HEADERS += $$files(*.h, true)

# 源文件
SOURCES += $$files(*.cpp, true)

# ui 文件
FORMS += $$files(*.ui, true)

# 资源文件
RESOURCES += $$files(*.qrc, true)

# nut
INCLUDEPATH += $$PWD/../../../3rdparty/nut.git/src
LIBS += -L$$OUT_PWD/../../../3rdparty/nut.git/proj/qtcreator/pro/nut$${OUT_TAIL}
win32: LIBS += -lnut1
else: LIBS += -lnut

# discount
INCLUDEPATH += \
    $$PWD/../../../3rdparty/discount.git \
    $$PWD/../discount
LIBS += -L$$OUT_PWD/../discount$${OUT_TAIL} -ldiscount

# hoedown
INCLUDEPATH += $$PWD/../../../3rdparty/hoedown.git
LIBS += -L$$OUT_PWD/../hoedown$${OUT_TAIL} -lhoedown

# 修复链接
mac {
    QMAKE_POST_LINK += install_name_tool -change libdiscount.1.dylib \
        @rpath/libdiscount.1.dylib \
        $$OUT_PWD/lib$${TARGET}.dylib ;
    QMAKE_POST_LINK += install_name_tool -change libhoedown.1.dylib \
        @rpath/libhoedown.1.dylib \
        $$OUT_PWD/lib$${TARGET}.dylib ;
}
