#ifdef CISO646
#include <ciso646>
#endif

#include "plus_data_manager.h"

#include <QJson/Parser>

#include <QNetworkReply>
#include <QApplication>

#include <QDebug>

PlusDataManager::PlusDataManager(QObject * p) :
    QObject(p),
    m_pNetworkAccessManager(new QNetworkAccessManager(this))
{
    connect(m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

void
PlusDataManager::
sendRequest(const QString & url, const QString & access_token,
            RequestType type, QIODevice * ioDev )
{
    QNetworkRequest request;
    QUrl percentEncUrl( QUrl::toPercentEncoding(url, ":/?=&", " ") );

    qDebug() << Q_FUNC_INFO;
    qDebug() << percentEncUrl;

    request.setUrl( percentEncUrl );
    request.setRawHeader("Authorization", QString("OAuth %1").arg(access_token).toLatin1());
    //request.setRawHeader("X-JavaScript-User-Agent", "Google APIs Explorer");

    QApplication::setOverrideCursor(Qt::WaitCursor);

    switch(type)
    {
    case GetRequest:    m_pNetworkAccessManager->get( request );            break;
    case PostRequest:   m_pNetworkAccessManager->post( request, ioDev );    break;
    case PutRequest:    m_pNetworkAccessManager->put( request, ioDev );     break;
    case DelRequest:    m_pNetworkAccessManager->deleteResource( request ); break;
    }

}

QVariant
PlusDataManager::
getValue(const QMap<QString, QVariant> & map, const QString & key, const QString & subKey)
{
    QVariant v = map[key];
    if( subKey.isEmpty() )
        return v;

    return v.toMap()[subKey];
}

QVariantList
PlusDataManager::
getListValue(const QMap<QString, QVariant> & map, const QString & key)
{
    QVariant v = map[key];
    if( v.isNull() )
        return QVariantList();
    return v.toMap().values();
}

void
PlusDataManager::
replyFinished(QNetworkReply * networkReply)
{
    qDebug() << __FUNCTION__;

    QApplication::restoreOverrideCursor();
    QString json = networkReply->readAll();

    qDebug() << "Reply = " << json;
    qDebug() << "URL = " << networkReply->url();
    QString strUrl = networkReply->url().toString();


    QJson::Parser parser;
    bool ok;
    // json is a QString containing the data to convert
#if QT_VERSION >= 0x050000
    // Qt5 code
    QVariant result = parser.parse (json.toUtf8(), &ok);
#else
    // Qt4 code
    QVariant result = parser.parse (json.toLatin1(), &ok);
#endif

    if( !ok )
    {
        emit errorOccured(QString("Cannot convert to QJson object: %1").arg(json));
        return;
    }

    if(result.toMap().contains("error"))
    {
        qDebug() << "ERROR occured:\n" << strUrl;
        emit errorOccured(result.toMap()["error"].toMap()["message"].toString());
        return;
    }

    if(strUrl.contains("userinfo"))
    {
        m_strUserEmail = result.toMap()["email"].toString();
        emit userEmailReady( m_strUserEmail );
        return;
    }

    QString kind = result.toMap()["kind"].toString();
    if( kind.isEmpty() )
    {
        emit errorOccured("kind informataion is empty");
        return;
    }
    if( ! kind.startsWith("plus#") )
    {
        emit errorOccured("Reply data isn't from Google+\nkind type: " + kind);
        return;
    }
    kind.remove(0, 5);

    if(kind == "peopleFeed")
    {
        QString nextPageToken = result.toMap()["nextPageToken"].toString();
        m_people = result.toMap()["items"].toList();

        emit peopleFeedReady( nextPageToken );
    }
    else if(kind == "person")
    {
        emit newPersonData( result );
    }
    else if(kind == "activityFeed")
    {
        QString nextPageToken = result.toMap()["nextPageToken"].toString();
        m_activities = result.toMap()["items"].toList();

        emit activitiesFeedReady( nextPageToken );
    }
    else if(kind == "activity")
    {
        emit newActivity( result );
    }

    else if(kind == "commentFeed")
    {
        QString nextPageToken = result.toMap()["nextPageToken"].toString();
        m_comments = result.toMap()["items"].toList();

        emit commentsFeedReady( nextPageToken );
    }
    else if(kind == "comment")
    {
        emit newComment( result );

    } else {
        emit errorOccured( QString("\"plus#%1\" kind type isn't supported").arg(kind) );
    }


}


void
PlusDataManager::
searchPeople(const QString & access_token, const QString & person,
             const QString & pageToken, int maxResult, const QString lang )
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << person << pageToken << maxResult << lang;

    if( person.isEmpty() )
    {
        emit errorOccured("Person query is empty");
        return;
    }

    QString url = QString("https://www.googleapis.com/plus/v1/people?query=%1")
            .arg( person );

    if( ! pageToken.isEmpty() )
        url += "&pageToken=" + pageToken;
    if( maxResult > 0 )
        url += "&maxResults=" + QString::number(maxResult);
    if( ! lang.isEmpty() )
        url += "&language=" + lang;

    sendRequest(url, access_token, GetRequest);
}


void
PlusDataManager::
getPersonData(int listId, const QString& access_token)
{
    qDebug() << Q_FUNC_INFO;
    if( listId < 0 || m_people.size() <= listId  )
    {
        emit errorOccured("wrong list id in getting person data");
        return;
    }

    QString userId = m_people[listId].toMap()["id"].toString();
    sendRequest("https://www.googleapis.com/plus/v1/people/" + userId, access_token, GetRequest);
}

void
PlusDataManager::
getActivities(
    int listId, const QString & access_token,
    const QString & pageToken, int maxResult )
{
    qDebug() << Q_FUNC_INFO;
    if( listId < 0 or m_people.size() <= listId  )
    {
        emit errorOccured("wrong list id: " + QString::number(listId));
        return;
    }

    QString userId = m_people[listId].toMap()["id"].toString();
    QString url = QString("https://www.googleapis.com/plus/v1/people/%1/activities/%2?")
            .arg(userId, "public");

    if( ! pageToken.isEmpty() )
        url += "&pageToken=" + pageToken;
    if( maxResult > 0 )
        url += "&maxResults=" + QString::number(maxResult);

    sendRequest(url, access_token, GetRequest);
}

void
PlusDataManager::
getActivity(int listId, const QString & access_token)
{
    qDebug() << Q_FUNC_INFO;
    if( listId < 0 or m_activities.size() <= listId  )
    {
        emit errorOccured("wrong list id in getting activity id");
        return;
    }

    QString activityId = m_activities[listId].toMap()["id"].toString();
    if( activityId.isEmpty() )
    {
        emit errorOccured("An activityId is empty");
        return;
    }

    QString url = "https://www.googleapis.com/plus/v1/activities/" + activityId;
    sendRequest(url, access_token, GetRequest);
}

void
PlusDataManager::
findActivities(
    const QString & access_token, const QString & query,
    const QString & pageToken, int maxResult, const QString & lang,
    int type
) {
    if( query.isEmpty() )
    {
        emit errorOccured("Activity query is empty");
        return;
    }

    QString url = QString("https://www.googleapis.com/plus/v1/activities?query=%1").arg( query );

    if( ! pageToken.isEmpty() )
        url += "&pageToken=" + pageToken;
    if( maxResult > 0 )
        url += "&maxResults=" + QString::number(maxResult);
    if( ! lang.isEmpty() )
        url += "&language=" + lang;

    url += "&orderBy=";
    url += (type == ActivitiesByRecent ? "recent" : "best");

    sendRequest(url, access_token, GetRequest);
}


void
PlusDataManager::
getComments(
    int activityListId, const QString & access_token,
    const QString & pageToken, int maxResult
)
{
    qDebug() << Q_FUNC_INFO;
    if( activityListId < 0 or m_activities.size() <= activityListId  )
    {
        emit errorOccured("wrong activity id: " + QString::number(activityListId));
        return;
    }

    QString acId = m_activities[activityListId].toMap()["id"].toString();
    QString url = QString("https://www.googleapis.com/plus/v1/activities/%1/comments?").arg(acId);

    if( ! pageToken.isEmpty() )
        url += "&pageToken=" + pageToken;
    if( maxResult > 0 )
        url += "&maxResults=" + QString::number(maxResult);

    sendRequest( url, access_token, GetRequest);
}

void
PlusDataManager::
getComment(
    int commentListId, const QString & access_token
)
{
    qDebug() << Q_FUNC_INFO;
    if( commentListId < 0 or m_comments.size() <= commentListId  )
    {
        emit errorOccured("wrong comment id: " + QString::number(commentListId));
        return;
    }

    QString commId = m_comments[commentListId].toMap()["id"].toString();
    sendRequest( "https://www.googleapis.com/plus/v1/comments/" + commId, access_token );
}

void
PlusDataManager::
findPeopleByActivity(
    int activityListId, const QString & access_token,
    const QString & pageToken, const QString & collection,
    int maxResult
) {
    qDebug() << Q_FUNC_INFO;

    if( activityListId < 0 or m_activities.size() <= activityListId  )
    {
        emit errorOccured("wrong activity list id: " + QString::number(activityListId));
        return;
    }

    QString activityId = m_activities[activityListId].toMap()["id"].toString();
    if( activityId.isEmpty() )
    {
        emit errorOccured("An activityId is empty");
        return;
    }

    if( collection != "resharers" and collection != "plusoners" )
    {
        emit errorOccured("wrong collection: " + collection + "; required: plusoners or resharers");
        return;
    }

    QString url = QString("https://www.googleapis.com/plus/v1/activities/%1/people/%2?").arg( activityId, collection );

    if( ! pageToken.isEmpty() )
        url += "&pageToken=" + pageToken;
    if( maxResult > 0 )
        url += "&maxResults=" + QString::number(maxResult);

    sendRequest(url, access_token, GetRequest);
}

void PlusDataManager::
getUserEmail(const QString& access_token)
{
    sendRequest("https://www.googleapis.com/oauth2/v1/userinfo", access_token );
}
