#include "AbstractTabForm.h"

AbstractTabForm::
AbstractTabForm(QWidget * p) :
    QWidget(p)
{}

void
AbstractTabForm::
setOAuth2(OAuth2 * newOAuth2)
{
    m_pOauth2 = newOAuth2;
}


void
AbstractTabForm::
setPlusDataManager(PlusDataManager * newPlusDataManager)
{
    m_pPlusDataManager = newPlusDataManager;
}

void
AbstractTabForm::
setLanguageListModel(QStringListModel * newLanguageListModel)
{
    m_pLanguageListModel = newLanguageListModel;
}

QString
AbstractTabForm::
nextPageToken(QStringList & list)
{
    return list.size() ? list.last() : QString();
}

QString
AbstractTabForm::
prevPageToken(QStringList & list)
{
    list.removeLast();
    list.removeLast();

    return list.size() ? list.last() : QString();
}

QTableWidgetItem *
AbstractTabForm::
createItem(const QString & text)
{
    QTableWidgetItem * item = new QTableWidgetItem( text );
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return item;
}
