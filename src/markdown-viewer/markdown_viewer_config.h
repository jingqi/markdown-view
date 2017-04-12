
#ifndef ___HEADFILE_40770E1D_1870_4A90_AFF6_5455C589C401_
#define ___HEADFILE_40770E1D_1870_4A90_AFF6_5455C589C401_

#include <QtGlobal>

#include <nut/platform/platform.h>

#if defined(BUILDING_MARKDOWN_VIEWER_DLL)
#   define MDVIEWER_API DLL_EXPORT
#else
#   define MDVIEWER_API DLL_IMPORT
#endif

/**
 * FIXME Qt5.6 开始，不再默认支持 QWebKit，转而支持 QWebEngine
 *       但是 Windows 下 Mingw 暂不支持 QWebEngine(至少目前 Qt5.7 依然如此)
 */
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0)
#   define MARKDOWN_VIEWER_USE_QTWEBKIT 0
#else
#   define MARKDOWN_VIEWER_USE_QTWEBKIT 1
#endif

// 使用 discount 还是 hoedown 来作为 markdown 转 html 的引擎
#define USE_HOEDOWN_CONVERTER 0

#endif
