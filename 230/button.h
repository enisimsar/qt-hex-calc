// This header file includes our button class which inherits from QToolButton.
#ifndef BUTTON_H
#define BUTTON_H

#include <QToolButton>
// Inherits from QToolButton
class Button : public QToolButton
{
    Q_OBJECT

public:
    // Constructs Button with button text and its parent
    explicit Button(const QString &text, QWidget *parent = 0);
};

#endif
