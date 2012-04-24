#ifndef PLUS_DATA_MANAGER_H
#define PLUS_DATA_MANAGER_H

#include <QObject>
#include <QUrl>
#include <QStringList>
#include <QVariantList>

class PlusDataManager : public QObject
{
    Q_OBJECT
public:
    explicit PlusDataManager(QObject *parent = 0);

    enum ActivitiesOrdering { ActivitiesByBest = 0, ActivitiesByRecent };

    QVariantList getPeople()        const { return m_people;        }
    QVariantList getActivities()    const { return m_activities;    }
    QVariantList getComments()      const { return m_comments;      }
    QVariantList getActivityPeople()const { return m_activityPeople;}

    QString userEmail() const { return m_strUserEmail; }

signals:
    void errorOccured(const QString &);

    void peopleFeedReady(           const QString & pageToken );
    void activitiesFeedReady(       const QString & pageToken );
    void commentsFeedReady(         const QString & pageToken );
    void peopleFeedByActivityReady( const QString & pageToken );

    void newPersonData( const QVariant & jsonPerson  );
    void newActivity(   const QVariant & jsonActivity);
    void newComment(    const QVariant & jsonComment );

    void userEmailReady( const QString & userEmail );

public slots:
    // people
    void searchPeople(
        const QString & access_token, const QString & person,
        const QString & pageToken = QString(), int maxResult = 10, const QString lang = QString()
    );
    void getPersonData(int listId, const QString & access_token);

    // activities
    void getActivities(
        int listId, const QString & access_token,
        const QString & pageToken = QString(), int maxResult = 10
    );
    void findActivities(
        const QString & access_token, const QString & query,
        const QString & pageToken = QString(), int maxResult = 10,
        const QString & lang = QString(), int type = ActivitiesByRecent
    );
    void getActivity(int listId, const QString & access_token);

    // comments
    void getComments(
        int activityListId, const QString & access_token,
        const QString & pageToken = QString(), int maxResult = 10
    );
    void getComment(
        int commentListId, const QString & access_token
    );

    // people by activity
    void findPeopleByActivity(
        int activityListId, const QString & access_token,
        const QString & pageToken = QString(),
        const QString & collection = "resharers",
        int maxResult = 10
    );

    void getUserEmail(const QString & access_token);

private slots:
    void replyFinished(class QNetworkReply*);

private:
    enum RequestType { GetRequest, PostRequest, PutRequest, DelRequest };

    void sendRequest(
        const QString & url, const QString & access_token = QString(),
        RequestType type = GetRequest, QIODevice * ioDev = 0 );

    static QVariant getValue(const QMap<QString, QVariant> & map, const QString & key, const QString & subKey = QString());
    static QVariantList getListValue(const QMap<QString, QVariant> & map, const QString & key);

private:
    class QNetworkAccessManager * m_pNetworkAccessManager;

    QVariantList m_people;
    QVariantList m_activities;
    QVariantList m_comments;
    QVariantList m_activityPeople;

    QString m_strUserEmail;
};

#endif // PLUS_DATA_MANAGER_H
