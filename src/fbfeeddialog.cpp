#include "fbfeeddialog.h"
#include "fbsession.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// global

static const QString kFeedURL = "http://www.facebook.com/connect/prompt_feed.php";

///////////////////////////////////////////////////////////////////////////////////////////////////


FBFeedDialog::FBFeedDialog(FBSession* aSession) : FBDialog(aSession)
{}

void FBFeedDialog::load ()
{
    Dictionary getParams, postParams;
    getParams["display"] = "touch";

    postParams["api_key"] = iSession->apiKey();
    postParams["session_key"] = iSession->sessionKey();
    postParams["preview"] = "1";
    postParams["callback"] = "fbconnect:success";
    postParams["cancel"] = "fbconnect:cancel";
    postParams["feed_info"] = generateFeedInfo();
    postParams["feed_target_type"] = "self_feed";
    postParams["user_message_prompt"] = iUserMessagePrompt;

    loadURL(kFeedURL, QNetworkAccessManager::PostOperation, getParams, postParams );

}


QString FBFeedDialog::generateFeedInfo() const
{
    QStringList pairs;

    if (iTemplateBundleId) {
        pairs.append("\"template_id\":" + QString::number( iTemplateBundleId, 10 ));
    }
    if (iTemplateData.length()) {
        pairs.append("\"template_data\":" + iTemplateData);
    }
    if (iBodyGeneral.length()) {
        pairs.append("\"body_general\":" + iBodyGeneral);
    }

    return "{" + pairs.join(",") + "}";
}
