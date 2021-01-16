#include "include/authentication.h"
#include "ui_authentication.h"

Authentication::Authentication(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Authentication)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    Authentication::setWindowIcon(QIcon(":img/auth.png"));
    rememberLP = std::make_unique<QSettings>(QDir::currentPath() + "/config.ini", QSettings::IniFormat);
    ui->logEdit->setText(rememberLP->value("login").toString());
    ui->pwdEdit->setText(rememberLP->value("password").toString());
    if (rememberLP->value("remember") == "true")
        ui->rememberUser->setCheckState(Qt::Checked);
    else
        ui->rememberUser->setCheckState(Qt::Unchecked);
}

Authentication::~Authentication()
{
    delete ui;
}

void Authentication::on_buttonBox_accepted()
{
    QSqlQuery userVerification("SELECT login, password, user_name FROM users");
    QString tempLogin, tempPassword, tempUserName;

    if (ui->rememberUser->checkState() == Qt::Checked)
    {
        rememberLP->setValue("login", ui->logEdit->text());
        rememberLP->setValue("password", ui->pwdEdit->text());
        rememberLP->setValue("remember", "true");
        rememberLP->sync();
    }
    else if (ui->rememberUser->checkState() == Qt::Unchecked)
    {
        rememberLP->remove("login");
        rememberLP->remove("password");
        rememberLP->remove("remember");
        rememberLP->sync();
    }

    int authStatus = FAIL_LOGIN;
    while (userVerification.next())
    {
        tempLogin = userVerification.value(0).toString();
        tempPassword = userVerification.value(1).toString();
        tempUserName = userVerification.value(2).toString();
        if (tempLogin == ui->logEdit->text() && tempPassword == ui->pwdEdit->text())
        {
            authStatus = OK;
            emit setLogin(tempUserName);
            break;
        }
        else if (tempLogin != ui->logEdit->text())
        {
            authStatus = FAIL_LOGIN;
        }
        else if (tempLogin == ui->logEdit->text() && tempPassword != ui->pwdEdit->text())
        {
            authStatus = FAIL_PASSWORD;
        }
    }
    authState(authStatus, tempUserName);
}

void Authentication::authState(int status, QString userName)
{
    switch (status)
    {
    case OK:
        QMessageBox::information(this, "Успешно", "Вы вошли как " + userName);
        break;
    case FAIL_LOGIN:
        QMessageBox::warning(this, "Ошибка", "Вы ввели неправильный логин, попробуйте снова");
        break;
    case FAIL_PASSWORD:
        QMessageBox::warning(this, "Ошибка", "Вы ввели неправильный пароль,  попробуйте снова");
    }
}
