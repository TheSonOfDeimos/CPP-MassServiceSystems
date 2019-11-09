#ifndef LIBMSS_GLOBAL_H
#define LIBMSS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBMSS_LIBRARY)
#  define LIBMSS_EXPORT Q_DECL_EXPORT
#else
#  define LIBMSS_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBMSS_GLOBAL_H
