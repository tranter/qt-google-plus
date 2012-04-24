#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "oauth2.h"
#include "plus_data_manager.h"

#include <QModelIndex>

namespace Ui {
    class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

    void startLogin(bool bForce); //Show login dialog even if there is gotten already access_token.

signals:
    void userEmailChanged(const QString & userEmail);

private:
    void setupActivitiesWidget();
    void setupSimplyActivitiesWidget();
    void setupPeopleWidget();
    void setupCommentsWidget();

private slots:
    void loginDone();
    void errorOccured(const QString & );

    void showPeopleTabAsCurrent();
    void reorganizeLanguageItems();

    void setActivitiesIntoTab()     { redirectTo = RedirectActivitiesToTab; }
    void setActivitiesIntoDialog()  { redirectTo = RedirectActivitiesToDialog; }

    void setActivitiesFeed( const QString & pageToken );
    void showPersonActivities(int selectedPeopleRow);
    void showActivitiesComments(int selectedActivityRow);

private:
    Ui::Form * ui;
    OAuth2 * m_pOauth2;

    PlusDataManager m_plusDataManager;

    class QStringListModel * m_pLanguageListModel;

    enum ActivitiesRedirect {
        RedirectActivitiesToTab,
        RedirectActivitiesToDialog
    };

    ActivitiesRedirect redirectTo;

    class SimplyActivitiesWidget * m_pSimplyActivitiesWidget;
    class PlusDialog * m_pSimplyActivitiesDialog;

    class CommentsWidget * m_pCommentsWidget;
    class PlusDialog * m_pCommentsDialog;
};

#endif // FORM_H
