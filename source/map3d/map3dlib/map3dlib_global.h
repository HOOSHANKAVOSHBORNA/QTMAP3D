#ifndef MAP3DLIB_GLOBAL_H
#define MAP3DLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MAP3DLIB_LIBRARY)
#  define MAP3DLIB_EXPORT Q_DECL_EXPORT
#else
#  define MAP3DLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // MAP3DLIB_GLOBAL_H
