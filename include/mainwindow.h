#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "include/authentication.h"
#include "include/callsdisplay.h"
#include "include/connecttodb.h"
#include <QInputDialog>
#include <QMainWindow>
#include <QStringList>
#include <QThread>
#include <QTimer>
#include <QtGui>
Q_DECLARE_METATYPE(std::shared_ptr<Authentication>);
QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void getLogin(QString userName);
private slots:
    void on_authBtn_clicked();

    void on_autoRefresh_stateChanged(int arg1);

    void on_refreshQuery_clicked();

    void on_statusBox_currentIndexChanged(int index);

private:
    void createUiTable();

    Ui::MainWindow *ui;

    Authentication *newAuthWindow = new Authentication;;

    QTimer *checkNewAppeals;

    CallsDisplay *tableModel;

    QStringList statusAppealList;

    QThread *threadDownload;
};
#endif // MAINWINDOW_H
