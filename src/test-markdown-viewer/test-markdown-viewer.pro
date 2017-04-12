
TARGET = test-markdown-viewer
TEMPLATE = app

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

# 头文件
HEADERS += $$files(*.h*, true)

# 源文件
SOURCES += $$files(*.c*, true)

# ui 文件
FORMS += $$files(*.ui, true)

# 资源文件
RESOURCES += $$files(*.qrc, true)

# 其他文件
OTHER_FILES += test.md

# markdown-viewer
INCLUDEPATH += $$PWD/..
LIBS += -L$$OUT_PWD/../markdown-viewer$${OUT_TAIL}
win32: LIBS += -lmarkdown-viewer1
else: LIBS += -lmarkdown-viewer

# nut
INCLUDEPATH += $$PWD/../../../3rdparty/nut.git/src
LIBS += -L$$OUT_PWD/../../../3rdparty/nut.git/proj/qtcreator/pro/nut$${OUT_TAIL}
win32: LIBS += -lnut1
else: LIBS += -lnut

# 资源打包
mac {
    libs.path = Contents/Frameworks
    libs.files = \
        $$OUT_PWD/../../../3rdparty/nut.git/proj/qtcreator/pro/nut/libnut.1.dylib \
        $$OUT_PWD/../discount/libdiscount.1.dylib \
        $$OUT_PWD/../hoedown/libhoedown.1.dylib \
        $$OUT_PWD/../markdown-viewer/libmarkdown-viewer.1.dylib
    QMAKE_BUNDLE_DATA += libs
} else: win32 {
    # 拷贝资源文件
    SRC = $$OUT_PWD/../../../3rdparty/nut.git/proj/qtcreator/pro/nut/$${DEBUG_MODE}/nut1.dll
    POST_TARGETDEPS += $${SRC}
    SRC ~= s,/,\\,g
    DST = $$OUT_PWD/$${DEBUG_MODE}
    DST ~= s,/,\\,g
    QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /i $${SRC} $${DST}$$escape_expand(\n\t))

    SRC = $$OUT_PWD/../discount/$${DEBUG_MODE}/discount.dll
    POST_TARGETDEPS += $${SRC}
    SRC ~= s,/,\\,
    QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /i $${SRC} $${DST}$$escape_expand(\n\t))

    SRC = $$OUT_PWD/../hoedown/$${DEBUG_MODE}/hoedown.dll
    POST_TARGETDEPS += $${SRC}
    SRC ~= s,/,\\,
    QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /i $${SRC} $${DST}$$escape_expand(\n\t))

    SRC = $$OUT_PWD/../markdown-viewer/$${DEBUG_MODE}/markdown-viewer1.dll
    POST_TARGETDEPS += $${SRC}
    SRC ~= s,/,\\,
    QMAKE_POST_LINK += $$quote(cmd /c xcopy /y /i $${SRC} $${DST}$$escape_expand(\n\t))
} else: unix {
    SRC = $$OUT_PWD/../../../3rdparty/nut.git/proj/qtcreator/pro/nut/libnut.1.so
    POST_TARGETDEPS += $${SRC}
    QMAKE_POST_LINK += cp -Lf $${SRC} $$OUT_PWD/ ;

    SRC = $$OUT_PWD/../discount/libdiscount.1.so
    POST_TARGETDEPS += $${SRC}
    QMAKE_POST_LINK += cp -Lf $${SRC} $$OUT_PWD/ ;

    SRC = $$OUT_PWD/../hoedown/libhoedown.1.so
    POST_TARGETDEPS += $${SRC}
    QMAKE_POST_LINK += cp -Lf $${SRC} $$OUT_PWD/ ;

    SRC = $$OUT_PWD/../markdown-viewer/libmarkdown-viewer.1.so
    POST_TARGETDEPS += $${SRC}
    QMAKE_POST_LINK += cp -Lf $${SRC} $$OUT_PWD/ ;
}
