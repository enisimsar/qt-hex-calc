#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE
class Button;

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = 0);

private slots:
    void digitClicked();
    void additiveOperatorClicked();
    void equalClicked();
    void clear();

private:
    Button *createButton(const QString &text, const char *member);
    void abortOperation();
    bool calculate(double rightOperand, const QString &pendingOperator);

    double sumInMemory;
    double sumSoFar;
    QString pendingAdditiveOperator;
    bool waitingForOperand;

    QLineEdit *display;

    enum { NumDigitButtons = 16 };
    Button *digitButtons[NumDigitButtons];
};

#endif
