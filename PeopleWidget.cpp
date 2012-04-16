#include "PeopleWidget.h"
#include "ui_PeopleWidget.h"

#include "HtmlGenerator.h"

#include "PeopleListModel.h"
#include "PeopleItemDelegate.h"
#include "ImageDownloader.h"

PeopleWidget::
PeopleWidget(QWidget * p) :
    PlusWidget(p),
    ui(new Ui::PeopleWidget)
{
    ui->setupUi(this);
    ui->listView->setItemDelegate( new PeopleItemDelegate(this) );

    m_pPeopleListModel = new PeopleListModel(this);
    ui->listView->setModel(m_pPeopleListModel);
    m_selectedPeopleRow = -1;

    m_pImageDownloader = new ImageDownloader(this);

    m_findPeopleBy = StandardFindPeople;
    m_selectedActivityRow = -1;
    m_selectedActivityCollection = "resharers";

    createConnections();
}

PeopleWidget::
~PeopleWidget()
{
    delete ui;
}

void
PeopleWidget::
createConnections()
{
    connect(ui->searchPeopleLineEdit, SIGNAL(returnPressed()), this, SLOT(searchPeople()));

    connect(ui->pageNavigatorWidget, SIGNAL(nextPageToggled()), this, SLOT(nextPage()));
    connect(ui->pageNavigatorWidget, SIGNAL(prevPageToggled()), this, SLOT(prevPage()));
    connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(searchPeople()));

    connect(ui->listView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(showPerson(const QModelIndex &)));
    connect(ui->showActivitiesButton, SIGNAL(clicked()), this, SLOT(showPersonActivities()));

    connect(m_pImageDownloader, SIGNAL(newPixmap(int,QPixmap)), m_pPeopleListModel, SLOT(setPixmap(int,QPixmap)));
}

void
PeopleWidget::
setPlusDataManager(PlusDataManager * newPlusDataManager)
{
    PlusWidget::setPlusDataManager(newPlusDataManager);

    connect(m_pPlusDataManager, SIGNAL(newPersonData(QVariant)), this, SLOT(showPerson(QVariant)));
    connect(m_pPlusDataManager, SIGNAL(peopleFeedReady(QString)), this, SLOT(setPeopleFeed(QString)));
}

void
PeopleWidget::
setLanguageListModel(QStringListModel * newLanguageListModel)
{
    PlusWidget::setLanguageListModel(newLanguageListModel);
    ui->langComboBox->setModel(m_pLanguageListModel);
}

void
PeopleWidget::
findPeopleByActivity(int currentActivityRow, const QString & currentCollection)
{
    m_findPeopleBy = FindPeopleByActivity;

    m_selectedActivityRow = currentActivityRow;
    m_selectedActivityCollection = currentCollection;

    m_pagesTokens.clear();
    ui->searchPeopleLineEdit->clear();

    m_pPlusDataManager->findPeopleByActivity(
        m_selectedActivityRow, m_pOauth2->accessToken(),
        QString(), m_selectedActivityCollection,
        ui->maxResSpinBox->value()
    );
}

void
PeopleWidget::
clearOnActivitiesChanged()
{
    if( m_findPeopleBy == FindPeopleByActivity )
        clearContents();
}

void
PeopleWidget::
searchPeople()
{
    QString person = ui->searchPeopleLineEdit->text();
    if( person.isEmpty() )
    {
        emit errorOccured(trUtf8("Person name is empty"));
        return;
    }

    m_findPeopleBy = StandardFindPeople;

    clearContents();

    m_pPlusDataManager->searchPeople(
        m_pOauth2->accessToken(), person,
        QString(), ui->maxResSpinBox->value(),
        ui->langComboBox->currentText()
    );
}

void
PeopleWidget::
clearContents()
{
    m_pagesTokens.clear();
    m_pPeopleListModel->clear();
    ui->pageNavigatorWidget->enableNextButton( false );
    ui->pageNavigatorWidget->enablePrevButton( false );

    ui->webView->setHtml("");
    m_selectedPeopleRow = -1;
}

void
PeopleWidget::
setPeopleFeed(const QString l_nextPageToken)
{
    m_pImageDownloader->stop();

    qDebug() << Q_FUNC_INFO;

    QVariantList varList = m_pPlusDataManager->getPeople();

    m_selectedPeopleRow = -1;

    QStringList items;
    QStringList imageUrls;

    foreach( const QVariant & var, varList )
    {
        items.append( var.toMap()["displayName"].toString() );
        imageUrls.append( var.toMap()["image"].toMap()["url"].toString() );
    }

    m_pPeopleListModel->setItems( items );

    int index = m_pagesTokens.indexOf(l_nextPageToken);
    if( index < 0 )
        m_pagesTokens << l_nextPageToken;

    ui->pageNavigatorWidget->enableNextButton( ! l_nextPageToken.isEmpty() );
    ui->pageNavigatorWidget->enablePrevButton( m_pagesTokens.size() > 1 );
    ui->pageNavigatorWidget->setPageNum( m_pagesTokens.size() );

    m_pImageDownloader->setUrls(imageUrls);
    m_pImageDownloader->start();
}



void
PeopleWidget::
nextPage()
{
    if( m_findPeopleBy == StandardFindPeople )
    {
        m_pPlusDataManager->searchPeople(
            m_pOauth2->accessToken(), ui->searchPeopleLineEdit->text(),
            nextPageToken(), ui->maxResSpinBox->value(),
            ui->langComboBox->currentText()
        );
        return;
    }

    m_pPlusDataManager->findPeopleByActivity(
        m_selectedActivityRow, m_pOauth2->accessToken(),
        nextPageToken(), m_selectedActivityCollection,
        ui->maxResSpinBox->value()
    );
}

void
PeopleWidget::
prevPage()
{
    if( m_findPeopleBy == StandardFindPeople )
    {
        m_pPlusDataManager->searchPeople(
            m_pOauth2->accessToken(), ui->searchPeopleLineEdit->text(),
            prevPageToken(), ui->maxResSpinBox->value(),
            ui->langComboBox->currentText()
        );
        return;
    }

    m_pPlusDataManager->findPeopleByActivity(
        m_selectedActivityRow, m_pOauth2->accessToken(),
        prevPageToken(), m_selectedActivityCollection,
        ui->maxResSpinBox->value()
    );
}

void
PeopleWidget::
showPerson(const QModelIndex & idx)
{
    m_selectedPeopleRow = idx.row();

    if( m_selectedPeopleRow < 0 )
    {
        emit errorOccured("You must select one person item");
        return;
    }

    ui->webView->setHtml( HtmlGenerator::loadingHtml() );
    m_pPlusDataManager->getPersonData(m_selectedPeopleRow, m_pOauth2->accessToken());
}

void
PeopleWidget::
showPerson(const QVariant & json)
{
    ui->webView->setHtml( HtmlGenerator::createHtmlForPerson(json) );
}

void
PeopleWidget::
showPersonActivities()
{
    if( m_selectedPeopleRow < 0 )
    {
        emit errorOccured("You must select one person item");
        return;
    }

    emit showPersonActivities(m_selectedPeopleRow);
}
