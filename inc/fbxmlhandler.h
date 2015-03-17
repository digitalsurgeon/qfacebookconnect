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

#ifndef FB_XML_HANDLER_H
#define FB_XML_HANDLER_H

#include <QObject>
#include <QString>
#include <QXmlDefaultHandler>
#include <QVariantList>

#include "fbconnectglobal.h"

// FORWARD DECLARATIONS
class FBRequest;

///////////////////////////////////////////////////////////////////////////////////////////////////
class QFACEBOOKCONNECTSHARED_EXPORT FBXMLHandler : public QObject, public QXmlDefaultHandler
{
    Q_OBJECT

private:
    QVariantList        iStack;
    QList<QString>      iNameStack;
    QVariant            iRootObject;
    QString             iRootName;
    QString             iChars;

    QString         iParseErrorMessage;
    bool            iError;

public: /* class functions */

    FBXMLHandler();
    ~FBXMLHandler();

    inline const QString& rootName() const { return iRootName; }
    inline QVariant rootObject() const { return iRootObject; }

    inline bool parseError() const { return iError; }

private:
    /* methods from QXmlDefaultHandler */
    bool startElement( const QString & namespaceURI,
                       const QString & localName,
                       const QString & qName,
                       const QXmlAttributes & atts);
    bool characters(const QString& text);
    bool endElement( const QString & namespaceURI,
                     const QString & localName,
                     const QString & qName );

    /* methods from QXmlErrorHandler */
    bool error(const QXmlParseException& exception);
    bool fatalError(const QXmlParseException& exception);

    /* private functions*/
    const QString& topName() const;
    void flushCharacters();
    QVariant topObject(bool aCreate);

    QVariant topContainer();


    void initWhiteSpaceHash();
    QHash<QChar, bool> iWhiteSpaceAndNewLineCharSet;

};

#endif // FB_XML_HANDLER_H
