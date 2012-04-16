#include "ActivitiesWidget.h"
#include "ui_ActivitiesWidget.h"

#include "HtmlGenerator.h"

#include <QDateTime>
#include <QTextEdit>

ActivitiesWidget::ActivitiesWidget(QWidget * p) :
    PlusWidget(p),
    ui(new Ui::ActivitiesWidget)
{
    ui->setupUi(this);

    m_activitiesHeaderLabels << "Title" << "Author" << "Published" << "Replies" << "Resharers" << "Plusoners";
    ui->activitiesTableWidget->setHorizontalHeaderLabels(m_activitiesHeaderLabels);

    ui->activitiesTableWidget->setColumnWidth(0, 150);
    ui->activitiesTableWidget->setColumnWidth(1, 150);
    ui->activitiesTableWidget->setColumnWidth(2, 130);
    ui->activitiesTableWidget->setColumnWidth(3, 55);
    ui->activitiesTableWidget->setColumnWidth(4, 75);
    ui->activitiesTableWidget->setColumnWidth(5, 75);

    createConnections();
}

ActivitiesWidget::~ActivitiesWidget()
{
    delete ui;
}

void
ActivitiesWidget::
createConnections()
{
    // activities
    connect(ui->activitiesTableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(showActivity(QModelIndex)));

    connect(ui->pageNavigatorWidget,    SIGNAL(prevPageToggled()), this, SLOT(prevPage()));
    connect(ui->pageNavigatorWidget,    SIGNAL(nextPageToggled()), this, SLOT(nextPage()));
    connect(ui->activitiesFindButton,   SIGNAL(clicked()), this, SLOT(findActivities()));
    connect(ui->activitiesFindLineEdit, SIGNAL(returnPressed()), this, SLOT(findActivities()));

    // comments
    connect(ui->activitiesShowCommentsButton, SIGNAL(clicked()), this, SLOT(showComments()));

    // by activity
    connect( ui->showPeopleByActivityButton, SIGNAL(clicked()), this, SLOT(findPeopleByActivity()));
}

QTableWidgetItem *
ActivitiesWidget::
createItem(const QString & text)
{
    QTableWidgetItem * item = new QTableWidgetItem( text );
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return item;
}

void
ActivitiesWidget::
setPlusDataManager(PlusDataManager * newPlusDataManager)
{
    PlusWidget::setPlusDataManager(newPlusDataManager);
    connect(m_pPlusDataManager, SIGNAL(newActivity(QVariant)), this, SLOT(showActivity(QVariant)));
}

void
ActivitiesWidget::
setLanguageListModel(QStringListModel * newLanguageListModel)
{
    PlusWidget::setLanguageListModel(newLanguageListModel);
    ui->activitiesLangComboBox->setModel(m_pLanguageListModel);
}

void
ActivitiesWidget::
findActivities()
{
    if( ui->activitiesFindLineEdit->text().isEmpty() )
    {
        emit errorOccured("activity find line edit is empty");
        return;
    }

    m_pagesTokens.clear();

    m_pPlusDataManager->findActivities(
        m_pOauth2->accessToken(), ui->activitiesFindLineEdit->text(),
        QString(), ui->activitiesMaxResSpinBox->value(),
        ui->activitiesLangComboBox->currentText(),
        ui->activitiesOrderingComboBox->currentIndex()
    );
}

void
ActivitiesWidget::
nextPage()
{
    m_pPlusDataManager->findActivities(
        m_pOauth2->accessToken(), ui->activitiesFindLineEdit->text(),
        nextPageToken(), ui->activitiesMaxResSpinBox->value(),
        ui->activitiesLangComboBox->currentText(),
        ui->activitiesOrderingComboBox->currentIndex()
    );
}

void
ActivitiesWidget::
prevPage()
{
    m_pPlusDataManager->findActivities(
        m_pOauth2->accessToken(), ui->activitiesFindLineEdit->text(),
        prevPageToken(), ui->activitiesMaxResSpinBox->value(),
        ui->activitiesLangComboBox->currentText(),
        ui->activitiesOrderingComboBox->currentIndex()
    );
}

void
ActivitiesWidget::
clearContents()
{
    ui->activitiesTableWidget->clear();
    ui->activitiesTableWidget->setHorizontalHeaderLabels( m_activitiesHeaderLabels );
    ui->activitiesWebView->setHtml( "" );
}

void
ActivitiesWidget::
setActivitiesFeed( const QString l_nextPageToken )
{
    qDebug() << Q_FUNC_INFO;

    QMap<QString, QVariant> map;
    QVariantList varList( m_pPlusDataManager->getActivities() );

    ui->activitiesTableWidget->setHorizontalHeaderLabels( m_activitiesHeaderLabels );
    ui->activitiesTableWidget->setRowCount( varList.size() );

    int i(0);
    QVariant countItems(0);

    QTextEdit htmlEncoder;

    foreach(const QVariant & var, varList)
    {
        map = var.toMap();

        QString title = map["title"].toString();


        QMap<QString, QVariant> object = map["object"].toMap();
        QVariantList attachments;

        if( title.isEmpty() )
        {
            title = object["content"].toString().left(30);
            if( ! title.isEmpty() )
                title += "...";
            else
            {
                attachments = object["attachments"].toList();
                if( attachments.size() )
                {
                    title = attachments.first().toMap()["displayName"].toString();
                    if( title.isEmpty() )
                    {
                        title = attachments.first().toMap()["content"].toString().left(30);
                        if( ! title.isEmpty() )
                            title += "...";
                    }
                }
            }
        }

        if( title.isEmpty() )
            title = "Without title";
        else {
            htmlEncoder.setHtml(title);
            title = htmlEncoder.toPlainText();
        }

        ui->activitiesTableWidget->setItem(i, 0, createItem( title ) );
        ui->activitiesTableWidget->setItem(i, 1, createItem( map["actor"].toMap()["displayName"].toString() ) );
        ui->activitiesTableWidget->setItem(i, 2, createItem( map["published"].toDateTime().toString("yyyy-MM-dd hh:mm") ) );

        countItems = object["replies"].toMap()["totalItems"];
        ui->activitiesTableWidget->setItem(i, 3, createItem( countItems.toInt() > 1 ? countItems.toString() : QString("none") ) );

        countItems = object["plusoners"].toMap()["totalItems"];
        ui->activitiesTableWidget->setItem(i, 5, createItem( countItems.toInt() > 1 ? countItems.toString() : QString("none") ) );

        countItems = object["resharers"].toMap()["totalItems"];
        ui->activitiesTableWidget->setItem(i, 4, createItem( countItems.toInt() > 1 ? countItems.toString() : QString("none") ) );

        ++i;
    }
    int index = m_pagesTokens.indexOf(l_nextPageToken);
    if( index < 0 )
        m_pagesTokens << l_nextPageToken;

    ui->pageNavigatorWidget->enableNextButton( ! l_nextPageToken.isEmpty() );
    ui->pageNavigatorWidget->enablePrevButton( m_pagesTokens.size() > 1 );
    ui->pageNavigatorWidget->setPageNum( m_pagesTokens.size() );

    emit activitiesChanged();
}

void
ActivitiesWidget::
showActivity(const QModelIndex & idx)
{
    int cRow = idx.row();
    if( cRow < 0 )
    {
        emit errorOccured("Wrong item id, please select activity");
        return;
    }

    ui->activitiesWebView->setHtml( HtmlGenerator::loadingHtml() );
    m_pPlusDataManager->getActivity( cRow, m_pOauth2->accessToken() );
}

void
ActivitiesWidget::
showActivity(const QVariant & json)
{
    ui->activitiesWebView->setHtml( HtmlGenerator::createHtmlForActivity(json) );
}


void
ActivitiesWidget::
showComments()
{
    int cRow = ui->activitiesTableWidget->currentRow();
    if( cRow < 0 )
    {
        emit errorOccured("You must select one activity item");
        return;
    }

    QString repliesCount = ui->activitiesTableWidget->item(cRow, 3)->text();
    if( repliesCount == "none" )
    {
        emit errorOccured("There is now replies for selected activity");
        return;
    }

    emit showActivitiesComments(cRow);
}

void
ActivitiesWidget::
findPeopleByActivity()
{
    int cRow = ui->activitiesTableWidget->currentRow();
    if( cRow < 0 )
    {
        emit errorOccured("You must select one activity item");
        return;
    }

    int currentColumnCollection = ui->collectionedActivityComboBox->currentIndex() + 4;

    QString itemText = ui->activitiesTableWidget->item(cRow, currentColumnCollection)->text();
    if( itemText == "none" )
    {
        emit errorOccured( QString("There is no %1 items").arg( ui->collectionedActivityComboBox->currentText() ) );
        return;
    }

    emit findingPeopleByActivity(cRow, ui->collectionedActivityComboBox->currentText());
}

