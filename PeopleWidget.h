#ifndef PEOPLEWIDGET_H
#define PEOPLEWIDGET_H

#include "PlusWidget.h"

namespace Ui {
class PeopleWidget;
}

class PeopleWidget : public PlusWidget
{
    Q_OBJECT
    
public:
    explicit PeopleWidget(QWidget *parent = 0);
    ~PeopleWidget();

    void setPlusDataManager(PlusDataManager * newPlusDataManager);
    void setLanguageListModel(QStringListModel * newLanguageListModel);
    
signals:
    void showPersonActivities(int selectedPeopleRow);

public slots:
    void findPeopleByActivity(int currentActivityRow, const QString & currentCollection);
    void clearOnActivitiesChanged();

private slots:
    void setPeopleFeed( const QString nextPageToken );

    void searchPeople();
    void nextPage();
    void prevPage();

    void showPerson(const QModelIndex &);
    void showPerson(const QVariant &);

    void showPersonActivities();

private:
    void createConnections();
    void clearContents();

    enum FindPeopleBy {
        StandardFindPeople,
        FindPeopleByActivity
    };

private:
    Ui::PeopleWidget * ui;

    FindPeopleBy m_findPeopleBy;

    int m_selectedActivityRow;
    QString m_selectedActivityCollection;

    class PeopleListModel * m_pPeopleListModel;
    int m_selectedPeopleRow;
    class ImageDownloader * m_pImageDownloader;
};

#endif // PEOPLEWIDGET_H
