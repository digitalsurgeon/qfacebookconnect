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

#ifndef FB_LOGIN_DIALOG_H_
#define FB_LOGIN_DIALOG_H_

#include "fbdialog.h"

#include <QNetworkReply>

// FORWARD DECLARATIONS
class FBRequest;

class QFACEBOOKCONNECTSHARED_EXPORT FBLoginDialog : public FBDialog
{
    Q_OBJECT

private:
    FBRequest* iGetSessionRequest;
	
private slots:
    /* slots for handling signals from FBRequest iGetSessionRequest */
    void requestDidLoad (const QVariant& aResult);

    void requestFailedWithFacebookError ( const FBError& aError );
    void requestFailedWithNetworkError( QNetworkReply::NetworkError code );

#if defined(Q_OS_SYMBIAN)
	/* This is a workaround a Qt bug which is very visible on symbian or any touch device without a keyboard.
	Virtual keyboard wont be shown for <input type="email">, so we replace type="email" with type="text" in this workaround..
	This will be fixed in Qt 4.7.0, more info on this: http://code.google.com/p/qfacebookconnect/issues/detail?id=9 */
	void loadFinished ( bool ok );
	/* End work around */
#endif

public:
    FBLoginDialog(FBSession* aSession);
    FBLoginDialog();

    void load();

protected:
    virtual void dialogWillDisappear();
    virtual void dialogDidSucceed(const QUrl& aUrl);

private:
    void connectToGetSession(const QString& aToken);
    void loadLoginPage();
	
 };
#endif
