#include "form.h"
#include "ui_form.h"

#include <QDebug>
#include <QMessageBox>

#include <QSettings>
#include <QStringListModel>
#include <QWebSettings>

#include "PeopleWidget.h"
#include "ActivitiesWidget.h"
#include "SimplyActivitiesWidget.h"
#include "CommentsWidget.h"
#include "PlusDialog.h"

Form::Form(QWidget * p) :
    QWidget(p),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    //Enabling video in the WebView.
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);

    m_pOauth2 = new OAuth2(this);
    connect(m_pOauth2, SIGNAL(loginDone()), this, SLOT(loginDone()));

    connect(& m_plusDataManager, SIGNAL(errorOccured(QString)),
            this, SLOT(errorOccured(QString)));
    connect(& m_plusDataManager, SIGNAL(activitiesFeedReady(QString)),
            this, SLOT(setActivitiesFeed(QString)));

    m_pLanguageListModel = new QStringListModel(this);
    connect( m_pLanguageListModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
             this, SLOT(reorganizeLanguageItems()) );

    QSettings settings("ICS", "Google API Plus Client");
    QStringList lList; lList << "en-GB" << "en-US" << "de" << "pl" << "ru" << "uk";
    m_pLanguageListModel->setStringList( settings.value("languages", lList).toStringList() );

    setupActivitiesWidget();
    setupSimplyActivitiesWidget();
    setupPeopleWidget();
    setupCommentsWidget();
}

Form::~Form()
{
    QSettings settings("ICS", "Google API Plus Client");
    settings.setValue("languages", m_pLanguageListModel->stringList());

    delete ui;
}

void Form::startLogin(bool bForce)
{
    //Now we allow to start logging in when m_oauth2.isAuthorized(). User can log in using another Google account.
    //m_oauth2.startLogin(this, bForce); //this is a parent widget for a login dialog.
    m_pOauth2->startLogin(bForce); //this is a parent widget for a login dialog.
}

void
Form::
setupActivitiesWidget()
{
    redirectTo = RedirectActivitiesToTab;

    connect(ui->activitiesWidget, SIGNAL(errorOccured(QString)),
            this, SLOT(errorOccured(QString)));

    connect(ui->activitiesWidget, SIGNAL(findingPeopleByActivity(int,QString)),
            this, SLOT(showPeopleTabAsCurrent()));

    connect(ui->activitiesWidget, SIGNAL(findingPeopleByActivity(int,QString)),
            ui->peopleWidget, SLOT(findPeopleByActivity(int,QString)));

    connect(ui->activitiesWidget, SIGNAL(activitiesChanged()),
            ui->peopleWidget, SLOT(clearOnActivitiesChanged()));

    connect(ui->activitiesWidget, SIGNAL(showActivitiesComments(int)),
            this, SLOT(showActivitiesComments(int)));

    ui->activitiesWidget->setOAuth2(m_pOauth2);
    ui->activitiesWidget->setPlusDataManager( & m_plusDataManager );
    ui->activitiesWidget->setLanguageListModel(m_pLanguageListModel);
}

void
Form::
setupSimplyActivitiesWidget()
{
    m_pSimplyActivitiesWidget = new SimplyActivitiesWidget(this);
    m_pSimplyActivitiesWidget->setOAuth2(m_pOauth2);
    m_pSimplyActivitiesWidget->setPlusDataManager( & m_plusDataManager );

    m_pSimplyActivitiesDialog = new PlusDialog(this);
    m_pSimplyActivitiesDialog->setPlusWidget(m_pSimplyActivitiesWidget);
}

void
Form::
setupPeopleWidget()
{
    connect(ui->peopleWidget, SIGNAL(errorOccured(QString)),
            this, SLOT(errorOccured(QString)));

    connect(ui->peopleWidget, SIGNAL(showPersonActivities(int)),
            this, SLOT(showPersonActivities(int)));

    ui->peopleWidget->setOAuth2(m_pOauth2);
    ui->peopleWidget->setPlusDataManager( & m_plusDataManager );
    ui->peopleWidget->setLanguageListModel(m_pLanguageListModel);
}

void
Form::
setupCommentsWidget()
{
    m_pCommentsWidget = new CommentsWidget(this);
    m_pCommentsWidget->setOAuth2(m_pOauth2);
    m_pCommentsWidget->setPlusDataManager( & m_plusDataManager );

    m_pCommentsDialog = new PlusDialog(this);
    m_pCommentsDialog->setPlusWidget(m_pCommentsWidget);
}

void Form::loginDone()
{

}

void Form::errorOccured(const QString& error)
{
    if(error == "Invalid Credentials")
    {
        m_pOauth2->startLogin(true);
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), error);
    }
}

void
Form::
showPeopleTabAsCurrent()
{
    ui->tabWidget->setCurrentIndex(0);
}

void
Form::
reorganizeLanguageItems()
{
    qDebug() << Q_FUNC_INFO;

    // ComboBox::insertPolicy == InsertAtTop
    int max_stored_langs = 10;
    int toRemove = m_pLanguageListModel->rowCount() - max_stored_langs;

    if( 0 < toRemove )
        m_pLanguageListModel->removeRows(max_stored_langs-1, toRemove);
}

void
Form::
setActivitiesFeed( const QString & pageToken )
{
    if( redirectTo == RedirectActivitiesToTab )
        ui->activitiesWidget->setActivitiesFeed( pageToken );
    else
        m_pSimplyActivitiesWidget->setActivitiesFeed( pageToken );
}

void
Form::
showPersonActivities(int selectedPeopleRow)
{
    redirectTo = RedirectActivitiesToDialog;
    m_pSimplyActivitiesWidget->setSelectedPeopleRow(selectedPeopleRow);
    m_pSimplyActivitiesWidget->showActivities();
    m_pSimplyActivitiesDialog->exec();
    redirectTo = RedirectActivitiesToTab;
}

void
Form::
showActivitiesComments(int selectedActivityRow)
{
    m_pCommentsWidget->setSelectedActivityRow(selectedActivityRow);
    m_pCommentsWidget->showComments();
    m_pCommentsDialog->exec();
}
