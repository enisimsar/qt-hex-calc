#include <QtWidgets>
#include <cmath>
#include <iostream>
#include "button.h"
#include "calculator.h"
// For hex conversion
#include <sstream>
#include <string>

using namespace std;

// Constructs calculator
Calculator::Calculator(QWidget *parent) : QWidget(parent) {
  // Initial values of result
  sumSoFar = "0";
  waitingForOperand = true;

  // Creates a display screen with Initial value zero
  display = new QLineEdit("0");
  display->setReadOnly(true);
  display->setAlignment(Qt::AlignRight);

  // Creates all digit buttons and add into digitButtons array.
  for (int i = 0; i < NumDigitButtons; ++i) {
    digitButtons[i] = createButton(
        QString::number(i, NumDigitButtons).toUpper(), SLOT(digitClicked()));
  }

  // Creates operators buttons and clear button
  Button *plusButton = createButton(tr("+"), SLOT(additiveOperatorClicked()));
  Button *minusButton = createButton(tr("-"), SLOT(additiveOperatorClicked()));
  Button *equalButton = createButton(tr("="), SLOT(equalClicked()));
  Button *clearButton = createButton(tr("CLR"), SLOT(clear()));

  // Creates first row of the calculator
  QHBoxLayout *firstRow = new QHBoxLayout;
  firstRow->addWidget(plusButton);
  firstRow->addWidget(minusButton);
  firstRow->addWidget(equalButton);
  firstRow->addWidget(clearButton);

  // Creates second row of the calculator
  QHBoxLayout *secondRow = new QHBoxLayout;
  secondRow->addWidget(digitButtons[0]);
  secondRow->addWidget(digitButtons[1]);
  secondRow->addWidget(digitButtons[2]);
  secondRow->addWidget(digitButtons[3]);

  // Creates third row of the calculator
  QHBoxLayout *thirdRow = new QHBoxLayout;
  thirdRow->addWidget(digitButtons[4]);
  thirdRow->addWidget(digitButtons[5]);
  thirdRow->addWidget(digitButtons[6]);
  thirdRow->addWidget(digitButtons[7]);

  // Creates fourth row of the calculator
  QHBoxLayout *fourthRow = new QHBoxLayout;
  fourthRow->addWidget(digitButtons[8]);
  fourthRow->addWidget(digitButtons[9]);
  fourthRow->addWidget(digitButtons[10]);
  fourthRow->addWidget(digitButtons[11]);

  // Creates fifth row of the calculator
  QHBoxLayout *fifthRow = new QHBoxLayout;
  fifthRow->addWidget(digitButtons[12]);
  fifthRow->addWidget(digitButtons[13]);
  fifthRow->addWidget(digitButtons[14]);
  fifthRow->addWidget(digitButtons[15]);

  // Adds display screen and all rows to main layout
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(display);
  mainLayout->addLayout(firstRow);
  mainLayout->addLayout(secondRow);
  mainLayout->addLayout(thirdRow);
  mainLayout->addLayout(fourthRow);
  mainLayout->addLayout(fifthRow);

  // Sets the main layout and window title
  setLayout(mainLayout);
  setWindowTitle(tr("Calculator"));
}

// Triggered when a digit button clicked
void Calculator::digitClicked() {
  // Takes clicked button
  Button *clickedButton = qobject_cast<Button *>(sender());
  // Takes the digit value and stores into a string
  string digitValue = clickedButton->text().toUtf8().constData();
  // If given digit is zero display retains with zero
  if (display->text() == "0" && digitValue == "0")
    return;

  // If the previous click is operator, clears display
  if (waitingForOperand) {
    display->clear();
    waitingForOperand = false;
  }
  // Adds new digit to right of the display screen
  display->setText(display->text() + QString::fromStdString(digitValue));
}

// Triggered when a operator button clicked
void Calculator::additiveOperatorClicked() {
  // Takes clicked button
  Button *clickedButton = qobject_cast<Button *>(sender());
  // Takes clicked operator as QString
  QString clickedOperator = clickedButton->text();

  // Converts QString to string
  string operand = display->text().toUtf8().constData();

  // Checks the operator
  if (!pendingAdditiveOperator.isEmpty()) {
    // Checks calculation result
    if (!calculate(operand, pendingAdditiveOperator)) {
      abortOperation();
      return;
    }
    // Changes the display screen with new result
    display->setText(QString::fromStdString(sumSoFar));
  } else {
    // If operator is not entered, operand string stores into result
    sumSoFar = operand;
  }

  // Puts the input operator to pendingAdditiveOperator and waits operand
  pendingAdditiveOperator = clickedOperator;
  waitingForOperand = true;
}

// Triggered when equal button clicked
void Calculator::equalClicked() {
  // Takes operand to display
  string operand = display->text().toUtf8().constData();

  // Checks the operator
  if (!pendingAdditiveOperator.isEmpty()) {
    if (!calculate(operand, pendingAdditiveOperator)) {
      abortOperation();
      return;
    }
    // Clears the previous operator
    pendingAdditiveOperator.clear();
  } else {
    // If operator is not entered, operand string stores into result
    sumSoFar = operand;
  }

  // Changes display screen with last result
  display->setText(QString::fromStdString(sumSoFar));
  // Clears last result
  sumSoFar = "0";
  waitingForOperand = true;
}

// Triggered when CLR button clicked
void Calculator::clear() {
  /*FOR CLEAR
  if (waitingForOperand)
      return;*/

  /*FOR CLEAR ALL*/
  // Clears last result
  sumSoFar = "0";
  // Clears the previous operator
  pendingAdditiveOperator.clear();

  // Clears display screen
  display->setText("0");
  waitingForOperand = true;
}

// Creates the button with text and SLOT
Button *Calculator::createButton(const QString &text, const char *member) {
  // Creates a button
  Button *button = new Button(text);
  // Connect Signal of the button and Slot of the button
  connect(button, SIGNAL(clicked()), this, member);
  // Returns this button
  return button;
}

// Runs in error
void Calculator::abortOperation() {
  // Clears all values
  clear();
  // Displays "ERROR" in the display screen
  display->setText(tr("ERROR"));
}

// Calculates the result of last operand and previous operand with given
// operator
bool Calculator::calculate(string rightOperand,
                           const QString &pendingOperator) {
  // For big numbers
  long long operandLong1, operandLong2, result;
  // Converts all hex numbers to decimal numbers
  stringstream ss;
  ss << hex << sumSoFar << endl;
  ss >> operandLong1;
  ss << hex << rightOperand << endl;
  ss >> operandLong2;

  // Calculates the result according to given operator
  if (pendingOperator == tr("+")) {
    result = operandLong1 + operandLong2;
  } else {
    result = operandLong1 - operandLong2;
  }

  // If result is negative, stores minus sign
  string sign = "";
  if (result < 0) {
    sign = "-";
  }

  // Converts decimal number to hex numbers and adds sign string to front
  ss << sign << hex << uppercase << abs(result) << endl;
  ss >> sumSoFar;
  return true;
}
