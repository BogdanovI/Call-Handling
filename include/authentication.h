#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql>

namespace Ui
{
class Authentication;
}

class Authentication : public QDialog
{
    Q_OBJECT

public:
    explicit Authentication(QWidget *parent = nullptr);
    ~Authentication();
signals:
    void setLogin(QString userName);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::Authentication *ui;
    void authState(int status, QString userName);
    enum status
    {
        OK = 0,
        FAIL_LOGIN = 1,
        FAIL_PASSWORD = 2
    };
    std::unique_ptr<QSettings> rememberLP;
};

#endif // AUTHENTICATION_H
