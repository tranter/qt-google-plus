#ifndef PLUSWIDGET_H
#define PLUSWIDGET_H

#include <QWidget>
#include <QTableWidgetItem>

#include "oauth2.h"
#include "plus_data_manager.h"

#include <QDebug>
#include <QStringListModel>

class PlusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlusWidget(QWidget * parent = 0);

    void setOAuth2(OAuth2 * newOAuth2);

    virtual void setPlusDataManager(PlusDataManager * newPlusDataManager);
    virtual void setLanguageListModel(QStringListModel * newLanguageListModel);

    QString nextPageToken() const;
    QString prevPageToken();

signals:
    void pageNumChanged(int);
    void thereIsNextPage(bool);
    void thereIsPrevPage(bool);

public slots:
    virtual void nextPage() = 0;
    virtual void prevPage() = 0;
    virtual void clearContents() = 0;

signals:
    void errorOccured(const QString & );

protected:
    OAuth2 * m_pOauth2;
    PlusDataManager  * m_pPlusDataManager;
    QStringListModel * m_pLanguageListModel;

    QStringList m_pagesTokens;
};

#endif // PLUSWIDGET_H
