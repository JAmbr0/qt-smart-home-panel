#include "SecurityControls.h"
#include "ToggleButton.h"
#include <QStringList>
#include <QProcess>
#include <QDebug>

SecurityControls::SecurityControls(QWidget *parent)
    : QWidget(parent),
    optionPanelLayout(new QVBoxLayout(this)),
    firewallCheckTimer(new QTimer(this)),
    lastKnownFirewallState(false)
{
    setLayout(optionPanelLayout);
    optionPanelLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QLabel *titleLabel = new QLabel("Security", this);
    titleLabel->setStyleSheet("background-color: transparent; color: white;");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(30);
    titleFont.setFamily("Arial");
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    firewallToggle = new ToggleButton(this);
    firewallToggle->setLabelText("Firewall     ");

    optionPanelLayout->addWidget(titleLabel);
    optionPanelLayout->addSpacing(40);
    optionPanelLayout->addWidget(firewallToggle);
    optionPanelLayout->addSpacing(40);
    displaySecurityDetails();
    optionPanelLayout->setContentsMargins(25, 25, 0, 0);

    checkFirewallState();
    connect(firewallToggle, &ToggleButton::toggled, this, &SecurityControls::handleFirewallToggle);
    connect(firewallCheckTimer, &QTimer::timeout, this, &SecurityControls::checkFirewallState);
    firewallCheckTimer->start(5000);
}

void SecurityControls::displaySecurityDetails()
{
    QStringList securityDetailsList = {
        "Firewall: {Enabled}",
        "Last Security Scan: 2024-07-30 14:35",
        "Operating System: Debian Linux 12",
        "Uptime: 12 days, 4 hours, 22 minutes"
    };

    for (const QString &detail : securityDetailsList)
    {
        QLabel *label = new QLabel(detail, this);
        label->setStyleSheet("background-color: transparent; color: white;");
        QFont font = label->font();
        font.setPointSize(16);
        font.setFamily("Arial");
        label->setFont(font);
        optionPanelLayout->addWidget(label);
    }
}

void SecurityControls::checkFirewallState()
{
    QProcess process;
    process.start("systemctl", QStringList() << "is-active" << "ufw");
    process.waitForFinished();
    bool isEnabled = (process.readAllStandardOutput().trimmed() == "active");

    if (isEnabled != lastKnownFirewallState) {
        lastKnownFirewallState = isEnabled;
        firewallToggle->setToggleState(isEnabled);
    }
}

void SecurityControls::handleFirewallToggle(bool enabled)
{
    if (enabled == lastKnownFirewallState) {
        return;
    }

    QProcess process;
    QString command = enabled ? "start" : "stop";

    process.start("pkexec", QStringList() << "systemctl" << command << "ufw");
    process.waitForFinished();

    if (process.exitCode() == 0) {
        qDebug() << "Firewall successfully" << (enabled ? "enabled" : "disabled");
        lastKnownFirewallState = enabled;
        checkFirewallState();
    } else {
        qDebug() << "Failed to change firewall state:" << process.readAllStandardError();
        firewallToggle->setToggleState(!enabled);  // Revert the toggle state
    }
}
