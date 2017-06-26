#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <iostream>
using namespace std;

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE
class Button;

// Inherits from QWidget
class Calculator : public QWidget
{
    Q_OBJECT

public:
    // Constructs Calculator with its parent
    Calculator(QWidget *parent = 0);

// Slots of the Calculator
private slots:
    // Detects clicked digit
    void digitClicked();
    // Detects when plus or minus operator clicked
    void additiveOperatorClicked();
    // Detects when equal operator clicked
    void equalClicked();
    // Detects clear button
    void clear();

private:
    // Creates the button with text and slot
    Button *createButton(const QString &text, const char *member);
    void abortOperation();
    // Calculates the result with given operand, operator and previous result
    bool calculate(string rightOperand, const QString &pendingOperator);

    // Stores the last result
    string sumSoFar;
    // Check next button click operand or operator
    bool waitingForOperand;
    // Stores the clicked operator
    QString pendingAdditiveOperator;

    // Stores the display screen
    QLineEdit *display;

    // Number of digit in hex calculator
    enum { NumDigitButtons = 16 };
    // Stores all digits
    Button *digitButtons[NumDigitButtons];
};

#endif
