#ifndef SECURITYCONTROLS_H
#define SECURITYCONTROLS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

class ToggleButton;

class SecurityControls : public QWidget
{
    Q_OBJECT

public:
    explicit SecurityControls(QWidget *parent = nullptr);

private slots:
    void checkFirewallState();
    void handleFirewallToggle(bool enabled);

private:
    void displaySecurityDetails();

    QVBoxLayout *optionPanelLayout;
    ToggleButton *firewallToggle;
    QTimer *firewallCheckTimer;
    bool lastKnownFirewallState;
};

#endif // SECURITYCONTROLS_H
