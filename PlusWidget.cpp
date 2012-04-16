#include "PlusWidget.h"

PlusWidget::
PlusWidget(QWidget * p) :
    QWidget(p)
{}

void
PlusWidget::
setOAuth2(OAuth2 * newOAuth2)
{
    m_pOauth2 = newOAuth2;
}


void
PlusWidget::
setPlusDataManager(PlusDataManager * newPlusDataManager)
{
    m_pPlusDataManager = newPlusDataManager;
}

void
PlusWidget::
setLanguageListModel(QStringListModel * newLanguageListModel)
{
    m_pLanguageListModel = newLanguageListModel;
}

QString
PlusWidget::
nextPageToken() const
{
    return m_pagesTokens.size() ? m_pagesTokens.last() : QString();
}

QString
PlusWidget::
prevPageToken()
{
    m_pagesTokens.removeLast();
    m_pagesTokens.removeLast();

    return m_pagesTokens.size() ? m_pagesTokens.last() : QString();
}
