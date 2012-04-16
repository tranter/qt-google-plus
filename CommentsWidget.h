#ifndef COMMENTSWIDGET_H
#define COMMENTSWIDGET_H

#include "PlusWidget.h"

class CommentsWidget : public PlusWidget
{
    Q_OBJECT
public:
    explicit CommentsWidget(QWidget *parent = 0);
    
    void setSelectedActivityRow(int newSelectedActivityRow) {
        m_selectedActivityRow = newSelectedActivityRow;
    }
    void setPlusDataManager(PlusDataManager * newPlusDataManager);

    void showComments();

private slots:
    void setCommentsFeed(const QString nextPageToken);

private:
    void nextPage();
    void prevPage();
    void clearContents();
    
private:
    int m_selectedActivityRow;
    class QWebView * m_pWebView;
};

#endif // COMMENTSWIDGET_H
