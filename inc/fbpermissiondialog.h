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

#ifndef FB_PERMISSION_DIALOG_H_
#define FB_PERMISSION_DIALOG_H_

#include "fblogindialog.h"
#include "fbconnectglobal.h"

// FORWARD DECLARATIONS

class QFACEBOOKCONNECTSHARED_EXPORT FBPermissionDialog : public FBLoginDialog
{
    Q_OBJECT

private:
    QString iPermission;

private slots:    
    void redirectToLoginDelayed();

public:
    FBPermissionDialog(FBSession* aSession);
    FBPermissionDialog();

    void load();

    /**
     * The extended permission to request.
     *
     * See http://wiki.developers.facebook.com/index.php/Extended_permissions
     */
    inline void setPermissionToRequest( const QString& aPermission) { iPermission = aPermission; }

protected:
    void dialogDidSucceed(const QUrl& aUrl);
    void shouldLoadUrl ( const QUrl& aUrl );

private:
    void redirectToLogin();	
 };
#endif
