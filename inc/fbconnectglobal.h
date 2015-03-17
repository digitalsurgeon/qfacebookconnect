/*
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef FBCONNECTGLOBAL_H
#define FBCONNECTGLOBAL_H

#include <QtGlobal>
#include <QHash>
#include <QString>

#if defined(QFACEBOOKCONNECT_LIBRARY)
#  define QFACEBOOKCONNECTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QFACEBOOKCONNECTSHARED_EXPORT Q_DECL_IMPORT
#endif

#ifdef QT_NO_OPENSSL
#error QFacebookConnect requires Qt compiled with SSL support.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

typedef unsigned long long FBUID;
typedef unsigned long long FBID;

typedef QHash<QString,QString> Dictionary;
typedef QHashIterator<QString, QString> DictionaryIterator;



#endif // FBCONNECTGLOBAL_H
