#include "fbstreamdialog.h"
#include "fbsession.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// global

static const QString kStreamURL = "http://www.facebook.com/connect/prompt_feed.php";

///////////////////////////////////////////////////////////////////////////////////////////////////


FBStreamDialog::FBStreamDialog(FBSession* aSession) : FBDialog(aSession)
{}

void FBStreamDialog::load ()
{
    Dictionary getParams, postParams;
    getParams["display"] = "touch";

    postParams["api_key"] = iSession->apiKey();
    postParams["session_key"] = iSession->sessionKey();
    postParams["preview"] = "1";
    postParams["callback"] = "fbconnect:success";
    postParams["cancel"] = "fbconnect:cancel";
    postParams["attachment"] = iAttachment;
    postParams["action_links"] = iActionLinks;
    postParams["target_id"] = iTargetId;
    postParams["user_message_prompt"] = iUserMessagePrompt;

    loadURL(kStreamURL, QNetworkAccessManager::PostOperation, getParams, postParams );

}
