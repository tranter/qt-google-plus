#ifndef ABSTRACTTABFORM_H
#define ABSTRACTTABFORM_H

#include <QWidget>
#include <QTableWidgetItem>

#include "oauth2.h"
#include "plus_data_manager.h"

#include <QDebug>
#include <QStringListModel>

class AbstractTabForm : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractTabForm(QWidget * parent = 0);

    void setOAuth2(OAuth2 * newOAuth2);

    virtual void setPlusDataManager(PlusDataManager * newPlusDataManager);
    virtual void setLanguageListModel(QStringListModel * newLanguageListModel);

    static QString nextPageToken(QStringList & list);
    static QString prevPageToken(QStringList & list);

    static QTableWidgetItem * createItem(const QString & text);

signals:
    void errorOccured(const QString & );

protected:
    OAuth2 * m_pOauth2;
    PlusDataManager  * m_pPlusDataManager;
    QStringListModel * m_pLanguageListModel;
};

#endif // ABSTRACTTABFORM_H
