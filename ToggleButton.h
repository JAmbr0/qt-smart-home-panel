#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

class ToggleButton : public QWidget
{
    Q_OBJECT

public:
    explicit ToggleButton(QWidget *parent = nullptr);

    void setLabelText(const QString &text);
    void setToggleState(bool state);
    bool isToggled() const;

signals:
    void toggled(bool state);

private slots:
    void handleToggle();

private:
    QPushButton *toggleButton;
    QLabel *toggleLabel;
    QHBoxLayout *layout;
};

#endif // TOGGLEBUTTON_H
