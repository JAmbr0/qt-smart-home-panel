#include "SecurityControls.h"
#include "ToggleButton.h"
#include <QStringList>
#include <QProcess>
#include <QRegularExpression>
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
    QStringList securityDetailsList;
    // List network connections
    QProcess ssProcess;
    ssProcess.start("sh", QStringList() << "-c" << "ss -tuln");
    ssProcess.waitForFinished();
    QString connections = ssProcess.readAllStandardOutput().trimmed();
    QStringList connectionLines = connections.split('\n', Qt::SkipEmptyParts);
    securityDetailsList.append("Network connections:");
    static const QRegularExpression spaceSplit("\\s+");
    for (int i = 1; i < connectionLines.size(); ++i) { // Skip header
        QStringList parts = connectionLines[i].split(spaceSplit, Qt::SkipEmptyParts);
        if (parts.size() >= 6) {
            QString netid = parts[0];
            QString state = parts[1];
            QString localAddress = parts[4];

            // Extract port number from localAddress
            int colonIndex = localAddress.lastIndexOf(':');
            QString port = colonIndex != -1 ? localAddress.mid(colonIndex + 1) : "N/A";

            securityDetailsList.append(QString("Port: %1, Netid: %2, State: %3")
                                           .arg(port, netid, state));
        }
    }

    // Check for available system updates
    QProcess updatesProcess;
    updatesProcess.start("sh", QStringList() << "-c" << "apt -s upgrade | grep 'newly installed' | awk '{print $1}'");
    updatesProcess.waitForFinished();
    QString updates = updatesProcess.readAllStandardOutput().trimmed();
    securityDetailsList.append(QString("Available updates: %1").arg(updates == "" ? "0" : updates));

    // Display last system login
    QProcess lastLoginProcess;
    lastLoginProcess.start("sh", QStringList() << "-c" << "last -1 -R | head -1");
    lastLoginProcess.waitForFinished();
    QString lastLogin = lastLoginProcess.readAllStandardOutput().trimmed();
    securityDetailsList.append(QString("Last login: %1").arg(lastLogin));

    // Clear existing labels
    QLayoutItem *child;
    while ((child = optionPanelLayout->takeAt(3)) != nullptr) {
        delete child->widget();
        delete child;
    }

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
