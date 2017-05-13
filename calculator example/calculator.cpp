#include <QtWidgets>

#include <cmath>

#include "button.h"
#include "calculator.h"

Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
{
    sumInMemory = 0.0;
    sumSoFar = 0.0;
    waitingForOperand = true;

    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);

    for (int i = 0; i < NumDigitButtons; ++i) {
        digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));
    }

    Button *plusButton = createButton(tr("+"), SLOT(additiveOperatorClicked()));
    Button *minusButton = createButton(tr("-"), SLOT(additiveOperatorClicked()));
    Button *equalButton = createButton(tr("="), SLOT(equalClicked()));
    Button *clearButton = createButton(tr("Clear"), SLOT(clear()));

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
    int digitValue = clickedButton->text().toInt();
    if (display->text() == "0" && digitValue == 0.0)
        return;

    if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
    }
    display->setText(display->text() + QString::number(digitValue));
}

void Calculator::additiveOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();

    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        display->setText(QString::number(sumSoFar));
    } else {
        sumSoFar = operand;
    }

    pendingAdditiveOperator = clickedOperator;
    waitingForOperand = true;
}

void Calculator::equalClicked()
{
    double operand = display->text().toDouble();

    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        pendingAdditiveOperator.clear();
    } else {
        sumSoFar = operand;
    }

    display->setText(QString::number(sumSoFar));
    sumSoFar = 0.0;
    waitingForOperand = true;
}

void Calculator::clear()
{
    /*if (waitingForOperand)
        return;*/

    sumSoFar = 0.0;
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
    display->setText(tr("####"));
}

bool Calculator::calculate(double rightOperand, const QString &pendingOperator)
{
    if (pendingOperator == tr("+")) {
        sumSoFar += rightOperand;
    } else if (pendingOperator == tr("-")) {
        sumSoFar -= rightOperand;
    }
    return true;
}
