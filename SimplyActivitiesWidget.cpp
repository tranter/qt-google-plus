#include "SimplyActivitiesWidget.h"
#include "HtmlGenerator.h"

#include <QVBoxLayout>
#include <QWebView>

SimplyActivitiesWidget::
SimplyActivitiesWidget(QWidget * p) :
    PlusWidget(p)
{
    m_selectedPeopleRow = -1;

    m_pWebView = new QWebView(this);

    setLayout(new QVBoxLayout(this));
    layout()->addWidget(m_pWebView);
}

void
SimplyActivitiesWidget::
setActivitiesFeed( const QString l_nextPageToken )
{
    int index = m_pagesTokens.indexOf(l_nextPageToken);
    if( index < 0 )
    {
        m_pagesTokens << l_nextPageToken;
        emit pageNumChanged(m_pagesTokens.size());
    }

    emit thereIsNextPage( ! l_nextPageToken.isEmpty() );
    emit thereIsPrevPage( m_pagesTokens.size() > 1 );

    QString html = HtmlGenerator::createHtmlForActivities(m_pPlusDataManager->getActivities());
    m_pWebView->setHtml( html );
}

void
SimplyActivitiesWidget::
showActivities()
{
    clearContents();
    m_pWebView->setHtml( HtmlGenerator::loadingHtml() );
    m_pPlusDataManager->getActivities(
        m_selectedPeopleRow, m_pOauth2->accessToken()
    );
}

void
SimplyActivitiesWidget::
nextPage()
{
    m_pWebView->setHtml( HtmlGenerator::loadingHtml() );
    m_pPlusDataManager->getActivities(
        m_selectedPeopleRow, m_pOauth2->accessToken(),
        nextPageToken()
    );
}

void
SimplyActivitiesWidget::
prevPage()
{
    m_pWebView->setHtml( HtmlGenerator::loadingHtml() );
    m_pPlusDataManager->getActivities(
        m_selectedPeopleRow, m_pOauth2->accessToken(),
        prevPageToken()
    );
}

void
SimplyActivitiesWidget::
clearContents()
{
    m_pagesTokens.clear();
    m_pWebView->setHtml( "" );
}
