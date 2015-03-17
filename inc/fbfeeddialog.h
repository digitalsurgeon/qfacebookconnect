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

#ifndef FB_FEED_DIALOG_H_
#define FB_FEED_DIALOG_H_

#include "fbdialog.h"

// FORWARD DECLARATIONS

class QFACEBOOKCONNECTSHARED_EXPORT FBFeedDialog : public FBDialog
{
    Q_OBJECT

private:

    long long iTemplateBundleId;
    QString iTemplateData;
    QString iBodyGeneral;
    QString iUserMessagePrompt;

public:
    FBFeedDialog(FBSession* aSession);
    FBFeedDialog();

    void load();

    /**
     * The id for a previously registered template bundle.
     */
    long long templateBundleId() const { return iTemplateBundleId; }
    void setTemplateBundleId( long long a) { iTemplateBundleId = a; }

    /**
     * A JSON string containing template data.
     *
     * See http://wiki.developers.facebook.com/index.php/Template_Data
     */
    const QString& templateData() const { return iTemplateData; }
    void setTemplateData( const QString& a ) { iTemplateData = a; }

    /**
     * Additional markup for a short story.
     */
    const QString& bodyGeneral() const { return iBodyGeneral; }
    void setBodyGeneral( const QString& a) { iBodyGeneral = a; }

    /**
     * Text you provide the user as a prompt to specify a userMessage. This appears
     * above the box where the user enters a custom message.
     * For example, "What's on your mind?"
     */
    const QString& userMessagePrompt() { return iUserMessagePrompt;}
    void setUserMessagePrompt( const QString& a) { iUserMessagePrompt = a; }


private:

    QString generateFeedInfo() const;

 };
#endif
