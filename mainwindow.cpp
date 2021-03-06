#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"

MainWindow::MainWindow(QWidget * p) :
    QMainWindow(p),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionLogin, SIGNAL(triggered()), this, SLOT(startLogin()));

    m_pForm = new Form(this);
    setCentralWidget(m_pForm);

    connect(m_pForm, SIGNAL(userEmailChanged(QString)), this, SLOT(setUserEmail(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startLogin()
{
    m_pForm->startLogin(true);
}

void MainWindow::startLoginDefault()
{
    m_pForm->startLogin(false);
}

void MainWindow::
setUserEmail(const QString & email)
{
    ui->statusBar->showMessage( "Current google account: " + email );
    setWindowTitle( QString("Google+ API Client (%1)").arg(email) );
}
