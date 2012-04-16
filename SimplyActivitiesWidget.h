#ifndef SIMPLYACTIVITIESWIDGET_H
#define SIMPLYACTIVITIESWIDGET_H

#include "PlusWidget.h"

class SimplyActivitiesWidget : public PlusWidget
{
    Q_OBJECT
public:
    explicit SimplyActivitiesWidget(QWidget * parent = 0);
    
    void setSelectedPeopleRow(int newRow) { m_selectedPeopleRow = newRow; }

    void showActivities();
    void nextPage();
    void prevPage();
    void clearContents();

public slots:
    virtual void setActivitiesFeed( const QString nextPageToken );

private:
    int m_selectedPeopleRow;
    class QWebView * m_pWebView;
};

#endif // SIMPLYACTIVITIESWIDGET_H
