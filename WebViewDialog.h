#ifndef WEBVIEWDIALOG_H
#define WEBVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class WebViewDialog;
}

class WebViewDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit WebViewDialog(QWidget *parent = 0);

    ~WebViewDialog();

    void setHtmlContent(const QString & html);
    
private:
    Ui::WebViewDialog *ui;
};

#endif // WEBVIEWDIALOG_H
