#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJSEngine>
#include <QJSValue>
#include <cmath>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<QPushButton *> buttons = findChildren<QPushButton *>();
    for (QPushButton *button : buttons) {
        connect(button, &QPushButton::clicked, this, [=]() {
            onButtonClicked(button->text());
        });
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonClicked(const QString &buttonText)
{
    QString text = ui->lineEdit->text();
    if (text == "0" && buttonText != "." && buttonText != "=")
        text = "";

    // 数字或小数点
    if (buttonText[0].isDigit() || buttonText == ".") {
        ui->lineEdit->setText(text + buttonText);
    }
    // 清除
    else if (buttonText == "C" || buttonText == "CE") {
        ui->lineEdit->setText("0");
    }
    // 退格
    else if (buttonText == "⌫" || buttonText == "⟲" || buttonText == "←") {
        text.chop(1);
        if (text.isEmpty()) text = "0";
        ui->lineEdit->setText(text);
    }
    // 运算符
    else if (buttonText == "+" || buttonText == "−" || buttonText == "*" || buttonText == "÷") {
        ui->lineEdit->setText(text + " " + buttonText + " ");
    }
    // 平方
    else if (buttonText.contains("^2")) {
        bool ok;
        double val = text.toDouble(&ok);
        if (ok)
            ui->lineEdit->setText(QString::number(val * val));
    }
    // 平方根
    else if (buttonText.contains("√")) {
        bool ok;
        double val = text.toDouble(&ok);
        if (ok)
            ui->lineEdit->setText(QString::number(std::sqrt(val)));
    }
    // 倒数
    else if (buttonText.contains("1/x")) {
        bool ok;
        double val = text.toDouble(&ok);
        if (ok && val != 0)
            ui->lineEdit->setText(QString::number(1 / val));
    }
    // 等号
    else if (buttonText == "=") {
        QString expression = ui->lineEdit->text();
        // 简单替换符号以便计算
        expression.replace("÷", "/");
        expression.replace("×", "*");
        expression.replace("−", "-");

        QJSEngine engine;
        QJSValue result = engine.evaluate(expression);
        if (!result.isError())
            ui->lineEdit->setText(result.toString());
    }
}

