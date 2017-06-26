#include <QtWidgets>

#include "button.h"

// Constructs button with text
Button::Button(const QString &text, QWidget *parent): QToolButton(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    // Sets the button text
    setText(text);
}
