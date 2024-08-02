#include "ToggleButton.h"

ToggleButton::ToggleButton(QWidget *parent)
    : QWidget(parent),
    toggleButton(new QPushButton(this)),
    toggleLabel(new QLabel(this)),
    layout(new QHBoxLayout(this))
{
    QFont labelFont;
    labelFont.setPointSize(20);
    labelFont.setFamily("Arial");
    labelFont.setBold(true);

    toggleLabel->setFont(labelFont);
    toggleLabel->setStyleSheet("background-color: transparent; color: white;");

    toggleButton->setCheckable(true);
    toggleButton->setStyleSheet("QPushButton { background-color: rgba(40,44,49,255); "
                              "color: white; "
                              "border: none; "
                              "border-radius: 5px; "
                              "padding: 5px 15px; } "
                              "QPushButton:checked { background-color: rgba(58,94,171,255); }");

    layout->addWidget(toggleLabel);
    layout->addWidget(toggleButton);
    layout->setAlignment(Qt::AlignLeft);

    setLayout(layout);

    connect(toggleButton, &QPushButton::toggled, this, &ToggleButton::handleToggle);
}

void ToggleButton::setLabelText(const QString &text)
{
    toggleLabel->setText(text);
}

void ToggleButton::setToggleState(bool state)
{
    toggleButton->setChecked(state);
}

bool ToggleButton::isToggled() const
{
    return toggleButton->isChecked();
}

void ToggleButton::handleToggle()
{
    emit toggled(toggleButton->isChecked());
}
