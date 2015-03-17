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

#include "fbsession.h"
#include "fbrequest.h"

#include <QTimer>
///////////////////////////////////////////////////////////////////////////////////////////////////
// global

static const QString kAPIRestURL = "http://api.facebook.com/restserver.php";
static const QString kAPIRestSecureURL = "https://api.facebook.com/restserver.php";

static const int kMaxBurstRequests = 3;
static const int kBurstDuration = 2;

static FBSession* sharedSession = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Static class functions
FBSession* FBSession::session()
{
    return sharedSession;
}

void FBSession::setSession(FBSession* aSession)
{
    sharedSession = aSession;
}


FBSession* FBSession::sessionForApplication ( const QString& aAppKey, const QString& aAppSecret, const QString& aSessionProxy)
{
    FBSession* session = new FBSession ( aAppKey, aAppSecret, aSessionProxy );
    return session;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// instance public functions
FBSession::FBSession( const QString& aAppKey, const QString& aAppSecret, const QString& aGetSessionProxy ) :
        iApiKey (aAppKey),
        iApiSecret ( aAppSecret ),
        iGetSessionProxy ( aGetSessionProxy ),
        iUid(0),
        iRequestBurstCount(0),
        iSettings ("malang", "qfbconnect")
{
    if (!sharedSession)
    {
        sharedSession = this;
    }
}

FBSession::~FBSession()
{
}

const QString& FBSession::apiURL() const
{
    return kAPIRestURL;
}

const QString& FBSession::apiSecureURL() const
{
    return kAPIRestSecureURL;
}

bool FBSession::isConnected() const
{
    return iSessionKey.length() > 0 ;
}


void FBSession::beginSession ( FBUID aUid, const QString& aSessionKey, const QString& aSessionSecret, const QDateTime& aExpires )
{
    iUid = aUid;
    iSessionKey = aSessionKey;
    iSessionSecret = aSessionSecret;


    iExpirationDate = aExpires;

    save();
}

bool FBSession::resume()
{
    FBUID uid = iSettings.value("FBUserId", 0 ).toULongLong();
    if (uid)
    {
        QDateTime currentDateTime = QDateTime::currentDateTime();

        QDateTime expirationDate;

        if (iSettings.contains("FBSessionExpires"))
            expirationDate = iSettings.value( "FBSessionExpires").toDateTime();

        if ((expirationDate.isValid() && expirationDate > currentDateTime) || expirationDate.toTime_t() == 0 )
        {
            iUid = uid;
            iSessionKey = iSettings.value("FBSessionKey").toString();
            iSessionSecret = iSettings.value("FBSessionSecret").toString();

            iExpirationDate = expirationDate;

            emit sessionDidLogin( iUid );

            return true;

        }
    }
    return false;
}

FBUID FBSession::uid() const
{
    return iSettings.value("FBUserId", 0).toULongLong();
}

void FBSession::cancelLogin() {
    if (!isConnected()) {
        emit sessionDidNotLogin();
    }
}

void FBSession::logout() {
    if (iSessionKey.length())
    {
        emit sessionWillLogout(iUid);


        // no need to remove cookies, they will be removed once the app quits.
        // point to think about, what if user wants to log in again ??
        // hmm..
        // TODO: ^^

        iExpirationDate = QDateTime();
        iSessionKey.clear();
        iSessionSecret.clear();
        iUid = 0;

        unsave();

        emit sessionDidLogout();
    }
    else
    {
        unsave();
    }
}

void FBSession::send (FBRequest* aRequest) {
    performRequest (aRequest, true);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// instance private functions
void FBSession::save()
{
    if (iUid){
        iSettings.setValue("FBUserId", iUid);
    } else {
        iSettings.remove("FBUserId");
    }

    if (iSessionKey.length()) {
        iSettings.setValue("FBSessionKey", iSessionKey);
    } else {
        iSettings.remove("FBSessionKey");
    }

    if (iSessionSecret.length()) {
        iSettings.setValue("FBSessionSecret", iSessionSecret);
    } else {
        iSettings.remove("FBSessionSecret");
    }

    if (iExpirationDate.isValid()) {
        iSettings.setValue("FBSessionExpires", iExpirationDate);
    } else {
        iSettings.remove("FBSessionExpires");
    }

    iSettings.sync();
}

void FBSession::unsave()
{
    iSettings.remove("FBUserId");
    iSettings.remove("FBSessionKey");
    iSettings.remove("FBSessionSecret");
    iSettings.remove("FBSessionExpires");

    iSettings.sync();
}

void FBSession::startFlushTimer()
{
	int t = kBurstDuration;
    QTimer::singleShot( t, this, SLOT(requestTimerReady()));
}

void FBSession::enqueueRequest(FBRequest* aRequest)
{
    iRequestQueue.append(aRequest);
    startFlushTimer();
}

bool FBSession::performRequest(FBRequest* aRequest, bool aEnqueue) {
    // Stagger requests that happen in short bursts to prevent the server from rejecting
    // them for making too many requests in a short time

    int seconds = iLastRequestTime.secsTo( QDateTime::currentDateTime() );
	bool burst = seconds && (seconds < kBurstDuration);

	if (burst && (iRequestBurstCount > kMaxBurstRequests))
    {
        if (aEnqueue)
        {
            enqueueRequest(aRequest);
        }
        return false;
    }
    else
    {
        aRequest->connect();
		if (burst) {
			iRequestBurstCount++;
		} else {
            iRequestBurstCount = 1;
            iLastRequestTime = aRequest->timeStamp();
        }
    }
    return true;
}

void FBSession::flushRequestQueue()
{
    while ( iRequestQueue.count() ) {
      FBRequest* request = iRequestQueue.at(0);
      if (performRequest(request, false)) {
          iRequestQueue.removeAt(0);
      } else {
        startFlushTimer();
        break;
      }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// instance provate slots
void FBSession::requestTimerReady()
{
    flushRequestQueue();
}
