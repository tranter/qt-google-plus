#include "CommentsWidget.h"
#include "HtmlGenerator.h"

#include <QWebView>
#include <QVBoxLayout>

CommentsWidget::CommentsWidget(QWidget * p) :
    PlusWidget(p)
{
    m_selectedActivityRow = -1;

    m_pWebView = new QWebView(this);

    setLayout(new QVBoxLayout(this));
    layout()->addWidget(m_pWebView);
}

void
CommentsWidget::
setPlusDataManager(PlusDataManager * newPlusDataManager)
{
    PlusWidget::setPlusDataManager(newPlusDataManager);
    connect(m_pPlusDataManager, SIGNAL(commentsFeedReady(QString)), this, SLOT(setCommentsFeed(QString)));
}

void
CommentsWidget::
showComments()
{
    clearContents();
    m_pWebView->setHtml( HtmlGenerator::loadingHtml() );
    m_pPlusDataManager->getComments( m_selectedActivityRow, m_pOauth2->accessToken() );
}

void
CommentsWidget::
nextPage()
{
    m_pWebView->setHtml( HtmlGenerator::loadingHtml() );
    m_pPlusDataManager->getComments(
        m_selectedActivityRow, m_pOauth2->accessToken(), nextPageToken()
    );
}

void
CommentsWidget::
prevPage()
{
    m_pWebView->setHtml( HtmlGenerator::loadingHtml() );
    m_pPlusDataManager->getComments(
        m_selectedActivityRow, m_pOauth2->accessToken(), prevPageToken()
    );
}

void
CommentsWidget::
clearContents()
{
    m_pagesTokens.clear();
    m_pWebView->setHtml( "" );
}

void
CommentsWidget::
setCommentsFeed(const QString l_nextPageToken)
{
    int index = m_pagesTokens.indexOf(l_nextPageToken);
    if( index < 0 )
    {
        m_pagesTokens << l_nextPageToken;
        emit pageNumChanged(m_pagesTokens.size());
    }

    emit thereIsNextPage( ! l_nextPageToken.isEmpty() );
    emit thereIsPrevPage( m_pagesTokens.size() > 1 );

    m_pWebView->setHtml( HtmlGenerator::createHtmlForComments(m_pPlusDataManager->getComments()) );
}
