#include "WebViewDialog.h"
#include "ui_WebViewDialog.h"

WebViewDialog::WebViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebViewDialog)
{
    ui->setupUi(this);
}


WebViewDialog::~WebViewDialog()
{
    delete ui;
}


void
WebViewDialog::
setHtmlContent(const QString & html)
{
    ui->webView->setHtml(html);
}
