#ifndef PAGENAVIGATORWIDGET_H
#define PAGENAVIGATORWIDGET_H

#include <QWidget>

namespace Ui {
class PageNavigatorWidget;
}

class PageNavigatorWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PageNavigatorWidget(QWidget *parent = 0);
    ~PageNavigatorWidget();

signals:
    void nextPageToggled();
    void prevPageToggled();

public slots:
    void setPageNum(int);
    void setPageNum(const QString &);
    void enableNextButton(bool);
    void enablePrevButton(bool);
    
private:
    Ui::PageNavigatorWidget * ui;
};

#endif // PAGENAVIGATORWIDGET_H
