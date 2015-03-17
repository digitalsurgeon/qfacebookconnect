#include "fbpermissiondialog.h"
#include "fbsession.h"

#include <QTimer>
///////////////////////////////////////////////////////////////////////////////////////////////////
// global

static const QString kPermissionURL = "http://www.facebook.com/connect/prompt_permission.php";

///////////////////////////////////////////////////////////////////////////////////////////////////


FBPermissionDialog::FBPermissionDialog() : FBLoginDialog () {}
FBPermissionDialog::FBPermissionDialog(FBSession* aSession) : FBLoginDialog(aSession) {}

void FBPermissionDialog::load ()
{
    Dictionary params, postParams;
    params["display"] = "touch";
    params["api_key"] = iSession->apiKey();
    params["session_key"] = iSession->sessionKey();
    params["ext_perm"] = iPermission;
    params["next"] = "fbconnect:success";
    params["cancel"] = "fbconnect:cancel";

    loadURL(kPermissionURL, QNetworkAccessManager::GetOperation, params, postParams );
}


void FBPermissionDialog::dialogDidSucceed(const QUrl& aUrl)
{
     if (iPermission.compare( "offline_access") == 0)
     {
         FBDialog::dialogDidSucceed(aUrl);
     }
     else
     {
         dismissWithSuccess(true, true);
     }
}

void FBPermissionDialog::shouldLoadUrl ( const QUrl& aUrl )
{
      if (iPermission.compare( "offline_access") == 0)
      {
          if (aUrl.scheme().compare("fbconnect")==0)
          {
              const QString encodedPath ( aUrl.encodedPath());
              if (encodedPath.compare("success")==0)
              {
                  redirectToLogin();
                  iIgnorePageLoadCompleteEvent = true;
                  return;
              }
          }
      }

      FBDialog::shouldLoadUrl(aUrl);
  }

void FBPermissionDialog::redirectToLogin()
{
    QTimer::singleShot(10,this, SLOT(redirectToLoginDelayed()) );
}

void FBPermissionDialog::redirectToLoginDelayed()
{
    FBDialog::load();
}
