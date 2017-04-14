
#ifndef ___HEADFILE_40770E1D_1870_4A90_AFF6_5455C589C401_
#define ___HEADFILE_40770E1D_1870_4A90_AFF6_5455C589C401_

#include <QtGlobal>

#if defined(BUILDING_MARKDOWN_VIEW_SHARED_LIB)
#   define MDVIEW_API Q_DECL_EXPORT
#else
#   define MDVIEW_API Q_DECL_IMPORT
#endif

// 使用 discount 还是 hoedown 来作为 markdown 转 html 的引擎
#define USE_HOEDOWN_CONVERTER 0

#endif
