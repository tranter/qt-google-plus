#ifndef ACTIVITIESWIDGET_H
#define ACTIVITIESWIDGET_H

#include "PlusWidget.h"

namespace Ui {
class ActivitiesWidget;
}

class ActivitiesWidget : public PlusWidget
{
    Q_OBJECT
    
public:
    explicit ActivitiesWidget(QWidget *parent = 0);
    ~ActivitiesWidget();

    void setPlusDataManager(PlusDataManager * newPlusDataManager);
    void setLanguageListModel(QStringListModel * newLanguageListModel);

public slots:
    void setActivitiesFeed( const QString nextPageToken );

signals:
    void findingPeopleByActivity(int currentActivityRow, const QString & currentCollection);
    void collectionActivityChanged(const QString &);
    void showActivitiesComments(int);
    void activitiesChanged();

private slots:
    void findActivities();
    void showActivity(const QModelIndex &);
    void showActivity(const QVariant & json);

    void showComments();

    void findPeopleByActivity();

private:
    void createConnections();
    static QTableWidgetItem * createItem(const QString & text);
    void nextPage();
    void prevPage();
    void clearContents();
    
private:
    Ui::ActivitiesWidget * ui;

    QStringList m_activitiesHeaderLabels;
};

#endif // ACTIVITIESWIDGET_H
