#include "PageNavigatorWidget.h"
#include "ui_PageNavigatorWidget.h"

PageNavigatorWidget::
PageNavigatorWidget(QWidget * p) :
    QWidget(p),
    ui(new Ui::PageNavigatorWidget)
{
    ui->setupUi(this);
    connect(ui->nextToolButton, SIGNAL(clicked()), this, SIGNAL(nextPageToggled()));
    connect(ui->prevToolButton, SIGNAL(clicked()), this, SIGNAL(prevPageToggled()));
}

PageNavigatorWidget::
~PageNavigatorWidget()
{
    delete ui;
}

void
PageNavigatorWidget::
setPageNum(int num)
{
    setPageNum( QString::number(num) );
}

void
PageNavigatorWidget::
setPageNum(const QString & num)
{
    ui->labelPageNum->setText(num);
}


void
PageNavigatorWidget::
enableNextButton(bool value)
{
    ui->nextToolButton->setEnabled(value);
}

void
PageNavigatorWidget::
enablePrevButton(bool value)
{
    ui->prevToolButton->setEnabled(value);
}
