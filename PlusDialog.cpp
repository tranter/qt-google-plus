#include "PlusDialog.h"
#include "ui_PlusDialog.h"

#include "PlusWidget.h"

PlusDialog::PlusDialog(QWidget * p) :
    QDialog(p),
    ui(new Ui::PlusDialog),
    m_pPlusWidget(0)
{
    ui->setupUi(this);
}


PlusDialog::~PlusDialog()
{
    delete ui;
}

void
PlusDialog::
setPlusWidget(PlusWidget * plusWidget)
{
    m_pPlusWidget = plusWidget;
    ui->layout->addWidget(plusWidget);
    connect(ui->pageNavigatorWidget, SIGNAL(nextPageToggled()), plusWidget, SLOT(nextPage()));
    connect(ui->pageNavigatorWidget, SIGNAL(prevPageToggled()), plusWidget, SLOT(prevPage()));
    connect(plusWidget, SIGNAL(pageNumChanged(int)),   ui->pageNavigatorWidget, SLOT(setPageNum(int)));
    connect(plusWidget, SIGNAL(thereIsNextPage(bool)), ui->pageNavigatorWidget, SLOT(enableNextButton(bool)));
    connect(plusWidget, SIGNAL(thereIsPrevPage(bool)), ui->pageNavigatorWidget, SLOT(enablePrevButton(bool)));
}

void
PlusDialog::
closeEvent(QCloseEvent * e)
{
    m_pPlusWidget->clearContents();
    QDialog::closeEvent(e);
}
