#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tableModel = new CallsDisplay;
    threadDownload = new QThread(this);
    connect(this, SIGNAL(destroyed()), threadDownload, SLOT(quit()));
    MainWindow::setFixedSize(1024, 620);
    MainWindow::setWindowIcon(QIcon(":img/logo.png"));
    ui->refreshQuery->setIcon(QIcon(":img/refresh.png"));
    ui->statusbar->showMessage(tableModel->checkDataBaseConnection());
    createUiTable();
    checkNewAppeals = new QTimer(this);
    checkNewAppeals->setInterval(60000);
    connect(checkNewAppeals, SIGNAL(timeout()), tableModel, SLOT(slotForReloadAppels()));
    checkNewAppeals->start();
    ui->mainTable->setDisabled(true);
    ui->statusBox->addItems(statusAppealList << "В работе"
                                             << "Выполнено");
}

MainWindow::~MainWindow()
{
    delete newAuthWindow;
    delete tableModel;
    delete ui;
}

void MainWindow::getLogin(QString userName)
{
    tableModel->who.clear();
    tableModel->who.append(userName);
    ui->who->setText(userName);
    if (ui->statusBox->currentIndex() == 0)
    {
        ui->mainTable->setDisabled(false);
    }
}

void MainWindow::createUiTable()
{
    ui->mainTable->setModel(tableModel);
    ui->mainTable->verticalHeader()->setVisible(false);
    ui->mainTable->setColumnWidth(0, 45);
    ui->mainTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->mainTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->mainTable->setColumnWidth(3, 200);
    ui->mainTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->mainTable->setColumnWidth(5, 150);
    ui->mainTable->setColumnWidth(6, 75);
    ui->mainTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->mainTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->mainTable->showGrid();
    tableModel->moveToThread(threadDownload);
    threadDownload->start();
}

void MainWindow::on_authBtn_clicked()
{
    connect(newAuthWindow, SIGNAL(setLogin(QString)), this, SLOT(getLogin(QString)));
    newAuthWindow->show();    
}

void MainWindow::on_autoRefresh_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        ui->statusbar->showMessage("Авто обовленеия включены");
        checkNewAppeals->start();
    }
    else
    {
        ui->statusbar->showMessage("Авто обовленеия выключены");
        checkNewAppeals->stop();
    }
}

void MainWindow::on_refreshQuery_clicked()
{
    tableModel->downloadAppels(ui->statusBox->currentIndex());
}

void MainWindow::on_statusBox_currentIndexChanged(int index)
{
    tableModel->downloadAppels(index);
    if (index == 1)
    {
        ui->mainTable->setDisabled(true);
    }
    else if (ui->who->text() != "Гость")
    {
        ui->mainTable->setDisabled(false);
    }
}
