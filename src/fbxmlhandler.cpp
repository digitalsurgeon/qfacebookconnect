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

#include "fbxmlhandler.h"

#include <QVariantList>
///////////////////////////////////////////////////////////////////////////////////////////////////
// instance public functions

FBXMLHandler::FBXMLHandler() : iError(false)
{
    initWhiteSpaceHash();
}

FBXMLHandler::~FBXMLHandler()
{
    iWhiteSpaceAndNewLineCharSet.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// instance private functions

void FBXMLHandler::initWhiteSpaceHash()
{
    iWhiteSpaceAndNewLineCharSet.insert(QChar(0x20), true);
    iWhiteSpaceAndNewLineCharSet.insert(QChar(0x9), true);
    iWhiteSpaceAndNewLineCharSet.insert(QChar(0xA), true);
    iWhiteSpaceAndNewLineCharSet.insert(QChar(0xB), true);
    iWhiteSpaceAndNewLineCharSet.insert(QChar(0xC), true);
    iWhiteSpaceAndNewLineCharSet.insert(QChar(0xD), true);
    iWhiteSpaceAndNewLineCharSet.insert(QChar(0x85), true);
}

/* methods from QXmlErrorHandler */
bool FBXMLHandler::error(const QXmlParseException& exception)
{
    iParseErrorMessage = exception.message();
    iError = true;
    return false;
}

bool FBXMLHandler::fatalError(const QXmlParseException& exception)
{
    iParseErrorMessage = exception.message();
    iError = true;
    return false;
}

/* methods from QXmlDefaultHandler */
bool FBXMLHandler::startElement( const QString & /*namespaceURI*/,
                                 const QString & localName,
                                 const QString & /*qName*/,
                                 const QXmlAttributes & atts)
{
    flushCharacters();

    QVariant item ;

    if  (atts.value("list").compare("true")==0)
    {
        item = QVariantList();
    }

    iStack.append(item);
    iNameStack.append(localName);

    return true;
}

bool FBXMLHandler::characters(const QString& aText)
{
    iChars.append(aText);
    return true;
}

bool FBXMLHandler::endElement( const QString & /*namespaceURI*/,
                               const QString & /*localName*/,
                               const QString & /*qName*/ )
{
    flushCharacters();

    QVariant c = iStack [iStack.count() - 1] ;
    QString name = topName();

    iStack.removeLast();
    iNameStack.removeLast();

    if (!iStack.count())
    {
        iRootObject = c;
        iRootName = name;
    }
    else
    {
        QVariant tC = iStack[iStack.count() - 1] ;
        if (tC.isNull())
        {
            tC = QVariantHash();
            iStack.replace(iStack.count() - 1, tC);
        }

        if (tC.type() == QVariant::List)
        {
            QVariantList list = tC.toList();
            list.append( c.toHash() );

            iStack.replace( iStack.count() - 1 , list);

        }
        else if (tC.type() == QVariant::Hash)
        {
            QVariantHash hash = tC.toHash();
            if (c.isNull())
            {
                c  = QString("");
            }
            hash.insert( name, c );

            iStack.replace( iStack.count() - 1 , hash);
        }
    }

    return true;
}


/* */
const QString& FBXMLHandler::topName() const
{
    return iNameStack.last();
}

QVariant FBXMLHandler::topObject(bool /*aCreate*/)
{
    QVariant item ;
    {
        iStack.replace(iStack.count() - 1, item);
    }
    return item;
}

QVariant FBXMLHandler::topContainer()
{
    if (iStack.count() < 2)
    {
        return QVariant();
    }
    else
    {

        QVariant item = iStack[iStack.count() - 2 ];
        {
            iStack.replace( iStack.count() - 2 , item);
        }

        return item;
    }
}

void FBXMLHandler::flushCharacters()
{
    for ( int i = 0; i < iChars.length(); i ++)
    {
        QChar uniChar = iChars.at(i);

        if (!iWhiteSpaceAndNewLineCharSet.contains(uniChar))
        {

            QVariant container;
            if (iStack.count() >= 2)
            {
                container = iStack[iStack.count() - 2];
                if (container.isNull())
                {
                    container = QVariantHash();
                    iStack.replace( iStack.count() - 2 , container);
                }
            }

            if (container.type() == QVariant::List)
            {                
                QVariantHash object;
                object.insert( topName(), iChars );
                iStack.replace (iStack.count() - 1, object);
            } else {
                QVariant object(iChars);
                iStack.replace (iStack.count() - 1, object);
            }
            break;
        }

    }

    iChars.clear();
}
