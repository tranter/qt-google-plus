#ifndef PLUSDIALOG_H
#define PLUSDIALOG_H

#include <QDialog>

namespace Ui {
class PlusDialog;
}

class PlusWidget;

class PlusDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PlusDialog(QWidget * parent = 0);
    ~PlusDialog();

    void setPlusWidget(PlusWidget * plusWidget);

private:
    void closeEvent(QCloseEvent *);

private:
    Ui::PlusDialog * ui;
    PlusWidget * m_pPlusWidget;
};

#endif // PLUSDIALOG_H
