#include <QtWidgets>

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include "button.h"
#include "calculator.h"

using namespace std;

Calculator::Calculator(QWidget *parent): QWidget(parent)
{
    sumInMemory = "0";
    sumSoFar = "0";
    waitingForOperand = true;

    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);

    for (int i = 0; i < NumDigitButtons; ++i) {
        digitButtons[i] = createButton(QString::number(static_cast<unsigned char>(i), 16).toUpper(), SLOT(digitClicked()));
    }

    Button *plusButton = createButton(tr("+"), SLOT(additiveOperatorClicked()));
    Button *minusButton = createButton(tr("-"), SLOT(additiveOperatorClicked()));
    Button *equalButton = createButton(tr("="), SLOT(equalClicked()));
    Button *clearButton = createButton(tr("CLR"), SLOT(clear()));

    QHBoxLayout *firstRow = new QHBoxLayout;
    firstRow->addWidget(plusButton);
    firstRow->addWidget(minusButton);
    firstRow->addWidget(equalButton);
    firstRow->addWidget(clearButton);

    QHBoxLayout *secondRow = new QHBoxLayout;
    secondRow->addWidget(digitButtons[0]);
    secondRow->addWidget(digitButtons[1]);
    secondRow->addWidget(digitButtons[2]);
    secondRow->addWidget(digitButtons[3]);

    QHBoxLayout *thirdRow = new QHBoxLayout;
    thirdRow->addWidget(digitButtons[4]);
    thirdRow->addWidget(digitButtons[5]);
    thirdRow->addWidget(digitButtons[6]);
    thirdRow->addWidget(digitButtons[7]);

    QHBoxLayout *fourthRow = new QHBoxLayout;
    fourthRow->addWidget(digitButtons[8]);
    fourthRow->addWidget(digitButtons[9]);
    fourthRow->addWidget(digitButtons[10]);
    fourthRow->addWidget(digitButtons[11]);

    QHBoxLayout *fifthRow = new QHBoxLayout;
    fifthRow->addWidget(digitButtons[12]);
    fifthRow->addWidget(digitButtons[13]);
    fifthRow->addWidget(digitButtons[14]);
    fifthRow->addWidget(digitButtons[15]);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(display);
    mainLayout->addLayout(firstRow);
    mainLayout->addLayout(secondRow);
    mainLayout->addLayout(thirdRow);
    mainLayout->addLayout(fourthRow);
    mainLayout->addLayout(fifthRow);

    setLayout(mainLayout);
    setWindowTitle(tr("Calculator"));
}

void Calculator::digitClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    string digitValue = clickedButton->text().toUtf8().constData();
    if (display->text() == "0" && digitValue == "0")
        return;

    if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
    }
    display->setText(display->text() + QString::fromStdString(digitValue));
}

void Calculator::additiveOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();

    string operand = display->text().toUtf8().constData();

    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        display->setText(QString::fromStdString(sumSoFar));
    } else {
        sumSoFar = operand;
    }

    pendingAdditiveOperator = clickedOperator;
    waitingForOperand = true;
}

void Calculator::equalClicked()
{
    string operand = display->text().toUtf8().constData();

    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        pendingAdditiveOperator.clear();
    } else {
        sumSoFar = operand;
    }

    display->setText(QString::fromStdString(sumSoFar));
    sumSoFar = "0";
    waitingForOperand = true;
}

void Calculator::clear()
{
    /*FOR CLEAR
    if (waitingForOperand)
        return;*/

    /*FOR CLEAR ALL*/
    sumSoFar = "0";
    pendingAdditiveOperator.clear();

    display->setText("0");
    waitingForOperand = true;
}

Button *Calculator::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void Calculator::abortOperation()
{
    clear();
    display->setText(tr("ERROR"));
}

bool Calculator::calculate(string rightOperand, const QString &pendingOperator)
{
    long operandLong1;
    long operandLong2;
    long result;
    stringstream ss;
    ss << hex << sumSoFar;
    ss >> operandLong1;
    operandLong1 = static_cast<long>(operandLong1);
    stringstream sss;
    sss << hex << rightOperand;
    sss >> operandLong2;
    operandLong2 = static_cast<long>(operandLong2);

    if (pendingOperator == tr("+")) {
        result = operandLong1 + operandLong2;
    } else {
        result = operandLong1 - operandLong2;
    }

    stringstream ssss;
    ssss << hex << result;
    sumSoFar =  ssss.str();
    transform(sumSoFar.begin(), sumSoFar.end(), sumSoFar.begin(),(int (*)(int))toupper);
    return true;
}
