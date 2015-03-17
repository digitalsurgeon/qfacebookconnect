#include "fblogindialog.h"
#include "fbrequest.h"
#include "fbsession.h"
#include "fberror.h"

#if defined(Q_OS_SYMBIAN)
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QWebElementCollection>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// global

static const QString kLoginURL = "http://www.facebook.com/login.php";

///////////////////////////////////////////////////////////////////////////////////////////////////
void FBLoginDialog::connectToGetSession(const QString& aToken)
{
    iGetSessionRequest = FBRequest::requestWithSession(iSession);

    connect(iGetSessionRequest, SIGNAL(requestDidLoad(QVariant)), this, SLOT(requestDidLoad(QVariant)));
    connect(iGetSessionRequest, SIGNAL(requestFailedWithNetworkError(QNetworkReply::NetworkError)), this, SLOT(requestFailedWithNetworkError(QNetworkReply::NetworkError)));
    connect(iGetSessionRequest, SIGNAL(requestFailedWithFacebookError(FBError)), this, SLOT(requestFailedWithFacebookError(FBError)));

    Dictionary params;
    params["auth_token"] = aToken;

    if (iSession->apiSecret().length())
    {
        params["generate_session_secret"]="1";
    }

    if (iSession->getSessionProxy().length())
    {
        iGetSessionRequest->post(iSession->getSessionProxy(),params);
    }
    else
    {
        iGetSessionRequest->call("facebook.auth.getSession", params);
    }
}

void FBLoginDialog::loadLoginPage()
{
#if defined(Q_OS_SYMBIAN)
	/* This is a workaround a Qt bug which is very visible on symbian or any touch device without a keyboard.
	Virtual keyboard wont be shown for <input type="email">, so we replace type="email" with type="text" in this workaround..
	This will be fixed in Qt 4.7.0, more info on this: http://code.google.com/p/qfacebookconnect/issues/detail?id=9 */
	connect ( iWebView.page(), SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
	/* end workaround */
#endif
	
    Dictionary getParams, postParams;

    getParams["fbconnect"] = "1";
    getParams["connect_display"] = "touch";
    getParams["api_key"] = iSession->apiKey();
    getParams["next"] = "fbconnect://success";

    loadURL(kLoginURL,QNetworkAccessManager::GetOperation, getParams, postParams);

}
///////////////////////////////////////////////////////////////////////////////////////////////////

FBLoginDialog::FBLoginDialog() : FBDialog () {}

FBLoginDialog::FBLoginDialog(FBSession* aSession) : FBDialog(aSession) {}

///////////////////////////////////////////////////////////////////////////////////////////////////
// FBDialog

void FBLoginDialog::load()
{
    loadLoginPage();
}

void FBLoginDialog::dialogWillDisappear()
{
  iGetSessionRequest->cancel();

  if (!iSession->isConnected())
  {
      iSession->cancelLogin();
  }
}

void FBLoginDialog::dialogDidSucceed(const QUrl& aUrl)
{
    const QString authToken = "auth_token=";
    const QString url ( aUrl.toString() );

    int start = url.indexOf(authToken);
    if (start != -1)
    {
        QString token;
        int end = url.indexOf("&", start);
        int delta = start + authToken.size() + 1;
        if (end != -1)
        {
            token = url.mid(delta, end - delta );
        }
        else
        {
            // -1 because delta assumes & character which is in this case missing
            token = url.right(delta - 1);
        }

        connectToGetSession(token);
    }
}

#if defined(Q_OS_SYMBIAN)
/* This is a workaround a Qt bug which is very visible on symbian or any touch device without a keyboard.
Virtual keyboard wont be shown for <input type="email">, so we replace type="email" with type="text" in this workaround..
This will be fixed in Qt 4.7.0, more info on this: http://code.google.com/p/qfacebookconnect/issues/detail?id=9 */	
void FBLoginDialog::loadFinished ( bool ok )
{
    if (ok)
    {
        QWebPage* page = static_cast<QWebPage*>(sender());
        QWebFrame* frame = page->mainFrame();
        QWebElementCollection	collection = frame->findAllElements("input");

        for ( int i = 0 ; i < collection.count(); i ++)
        {
        QWebElement element = collection[i];

            if (element.attribute("type", "").compare("email")==0)
            {
                element.removeAttribute("type");
                element.setAttribute("type","text");
                break;
            }
        }
    }
}
/* end work around */
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
// slots for signals from FBRequest

void FBLoginDialog::requestDidLoad(const QVariant& aResult)
{
    bool conversionError = false;
    QVariantHash object = aResult.toHash();
    QVariant ui = object.value("uid");
    FBUID uid = object.value("uid").toULongLong(&conversionError);
    QString sessionKey = object.value("session_key").toString();
    QString sessionSecret = object.value("secret").toString();

    QVariant ex = object.value("expires");
    uint expires = object.value("expires").toUInt(&conversionError);
    QDateTime expiration; expiration.setTime_t( expires );

    iSession->beginSession(uid,sessionKey, sessionSecret,expiration);
    iSession->resume();

    dismissWithSuccess(true, true);
}

void FBLoginDialog::requestFailedWithFacebookError (const FBError& aCode )
{
    dismissWithError(aCode, true);
}

void FBLoginDialog::requestFailedWithNetworkError( QNetworkReply::NetworkError aCode )
{
    dismissWithError(aCode, true);
}
