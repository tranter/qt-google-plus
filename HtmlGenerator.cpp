#include "HtmlGenerator.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>

QString HtmlGenerator::templateHtml =
    "<html><body style=\"background-color: #efefef;\">"
    "<div style=\"margin: auto; width: 500px; padding: 15px;"
    "background-color: white;\">%1</div></body></html>";

HtmlGenerator::HtmlGenerator()
{
}

QString
HtmlGenerator::
loadingHtml()
{
    return "<html><body style=\""
            "background-image: url('qrc:/images/ajax-loader.gif');"
            "background-position: center;"
            "background-repeat: no-repeat;"
            "\"></body></html>";
}

QString
HtmlGenerator::
emptyInfoHtml()
{
    return  "<h4 style=\"text-align: center;\">This page is empty</h4>";
}

QString
HtmlGenerator::
createHtmlForPerson(const QVariant & json)
{
    QString content;
    QMap<QString, QVariant> map = json.toMap();

    QString imgUrl = map["image"].toMap()["url"].toString();
    int idx = imgUrl.indexOf("sz=");
    imgUrl.remove(idx, imgUrl.size());
    imgUrl += "sz=100";

    QString gPlusLinkProfile = map["url"].toString();
    if( ! gPlusLinkProfile.isEmpty() )
        gPlusLinkProfile = QString("<a href=\"%1\">link to google profile</a>").arg(gPlusLinkProfile);

    QString info = QString(
        "<strong>%1 %2</strong><br/>%3<br/><small>%4</small>"
    ).arg(
        map["name"].toMap()["givenName"].toString(),
        map["name"].toMap()["familyName"].toString(),
        map["gender"].toString(),
        gPlusLinkProfile
    );

    content += QString(
        "<p>"
            "<img src=\"%1\" alt=\"person image\" style=\"float: left; margin-right: 15px;\">"
            "<div style=\"height: 100px;\">%2</div>"
        "</p>"
    ).arg(imgUrl, info);


    QVariantList varList = map["urls"].toList();

    if( varList.size() )
    {
        QString urls("<p><strong>Urls:</strong><ul>");

        foreach(const QVariant & v, varList)
        {
            if( v.toMap()["type"].toString() == "json" )
                continue;

            urls += QString("<li><a href=\"%1\">%1</a></li>").arg(v.toMap()["value"].toString());
        }

        urls += "</ul></p>";
        content += urls;
    }

    varList = map["organizations"].toList();

    if( varList.size() )
    {
        QString organizations("<p><strong>Organizations:</strong><ul>");
        QString name, title, type;

        foreach(const QVariant & v, varList)
        {
            name  = v.toMap()["name"].toString();
            title = v.toMap()["title"].toString();
            type  = v.toMap()["type"].toString();

            organizations += "<li>";
            if( ! name.isEmpty())
                organizations += QString("<strong>%1</strong><br/>").arg(name);

            if( ! title.isEmpty() )
            {
                organizations += title;
                organizations += "<br/>";
            }

            if( ! type.isEmpty() )
                organizations += QString("<i>%1</i>").arg(type);
            organizations += "</li>";
        }

        organizations += "</ul></p>";
        content += organizations;
    }

    varList = map["placesLived"].toList();

    if( varList.size() )
    {
        QString placesLived("<p><strong>Places lived:</strong><ul>");

        foreach(const QVariant & v, varList)
            placesLived += "<li>" + v.toMap()["value"].toString() + "</li>";

        placesLived += "</ul></p>";
        content += placesLived;
    }

    return templateHtml.arg(content);
}

QString
HtmlGenerator::
createContentForActivity(const QVariant & jsonContent)
{
    static unsigned uniqeId = 0;

    QMap<QString, QVariant> json = jsonContent.toMap();
    QMap<QString, QVariant> attachmentMap;

    QString published = json["published"].toDateTime().toString("yyyy-MM-dd hh:mm");
    QString person = json["actor"].toMap()["displayName"].toString();
    QString personImage = json["actor"].toMap()["image"].toMap()["url"].toString();

    QMap<QString, QVariant> object = json["object"].toMap();
    QString objectContent = object["content"].toString();

    QVariantList attachmentsList = object["attachments"].toList();

    QString attachmentDisplayName;
    QString attachmentContent;
    QString attachmentType;

    QString content = QString(
    "<p>"
        "<img src=\"%1\" alt=\"person image\" style=\"float: left; margin-right: 15px;\">"
        "<b>%2</b><br/>"
        "<i>Published: %3</i>"
    "</p>"
    ).arg( personImage, person, published );

    content += QString("<p>%1</p>").arg(objectContent);

    content += "<p>";

    int i(0);
    foreach(const QVariant & attachment, attachmentsList)
    {
        attachmentMap = attachment.toMap();
        attachmentDisplayName = attachmentMap["displayName"].toString();
        attachmentContent     = attachmentMap["content"].toString();
        attachmentType        = attachmentMap["objectType"].toString();

        if( attachmentType == "photo" )
        {
            QString img = attachmentMap["fullImage"].toMap()["url"].toString();

            attachmentContent += QString(
                "<p style=\"text-align: center;\" ><img width=\"400\" src=\"%1\"/></p>"
            ).arg(img);

        } else if( attachmentType == "video" ) {

            QString url = attachmentMap["embed"].toMap()["url"].toString();
            QString img = attachmentMap["image"].toMap()["url"].toString();

            QString embedId = "embed_id_" + QString::number(++uniqeId) + "_" + QString::number(++i);

            QString js = QString(
                "this.style.display = 'none'; "
                "document.getElementById('%1').style.display = 'block'; "
                "document.getElementById('%1').src = '%2'; "
            ).arg(embedId, url);

            attachmentContent += QString(
                "<p style=\"text-align: center;\" >"
                    "<img width=\"400\" height=\"300\" src=\"%1\" onclick=\"%2\" style=\"cursor:pointer;\"/>"
                    "<embed id=\"%3\" width=\"400\" height=\"300\" src=\"%4\" "
                        "style=\"display: none; margin: auto; border: 1px solid black;\"/>"
                "</p>"
            ).arg(img, js, embedId, url);

        }

        content += QString("<b>%1</b><br/>%2").arg(attachmentDisplayName, attachmentContent);
    }

    content += "</p>";
    return content;
}

QString
HtmlGenerator::
createHtmlForActivity(const QVariant & json)
{
    return templateHtml.arg( createContentForActivity(json) );
}

QString
HtmlGenerator::
createHtmlForActivities(const QVariantList & jsonObjects)
{
    QString content;
    int size = jsonObjects.size();
    for(int i(0); i < size; ++i)
    {
        content += "<div>";
        content += createContentForActivity(jsonObjects[i]);
        content += "</div>";
    }

    return templateHtml.arg(content.isEmpty() ? emptyInfoHtml() : content);
}

QString
HtmlGenerator::
createHtmlForComment(const QVariant & jsonContent)
{
    QMap<QString, QVariant> json = jsonContent.toMap();

    QString published = json["published"].toDateTime().toString("yyyy-MM-dd hh:mm");
    QString person = json["actor"].toMap()["displayName"].toString();
    QString personImage = json["actor"].toMap()["image"].toMap()["url"].toString();

    QString header = QString(
    "<p>"
        "<img src=\"%1\" alt=\"person image\" style=\"float: left; margin-right: 15px;\">"
        "<b>%2</b><br/>"
        "<i>Published: %3</i>"
    "</p>"
    ).arg( personImage, person, published );

    QString comment = QString("<p>%1</p>").arg( json["object"].toMap()["content"].toString() );

    return QString("<div style=\"margin: 15px;\">%1%2</div>").arg(header, comment);
}

QString
HtmlGenerator::
createHtmlForComments(const QVariantList & jsonObjects)
{
    QString content;
    foreach(const QVariant & json, jsonObjects)
        content += createHtmlForComment(json);
    return templateHtml.arg(content.isEmpty() ? emptyInfoHtml() : content);
}
