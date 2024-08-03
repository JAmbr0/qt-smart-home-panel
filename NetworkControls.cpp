#include "NetworkControls.h"
#include "ToggleButton.h"
#include <QStringList>
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>

NetworkControls::NetworkControls(QWidget *parent)
    : QWidget(parent),
    optionPanelLayout(new QVBoxLayout(this)),
    wifiCheckTimer(new QTimer(this)),
    lastKnownWifiState(false),
    displayUpdateTimer(new QTimer(this))
{
    setLayout(optionPanelLayout);
    optionPanelLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QLabel *titleLabel = new QLabel("Network", this);
    titleLabel->setStyleSheet("background-color: transparent; color: white;");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(30);
    titleFont.setFamily("Arial");
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    wifiToggle = new ToggleButton(this);
    wifiToggle->setLabelText("Wi-Fi     ");

    optionPanelLayout->addWidget(titleLabel);
    optionPanelLayout->addSpacing(40);
    optionPanelLayout->addWidget(wifiToggle);
    optionPanelLayout->addSpacing(60);
    displayNetworkDetails();
    optionPanelLayout->setContentsMargins(25, 25, 0, 0);

    connect(wifiToggle, &ToggleButton::toggled, this, &NetworkControls::handleWifiToggle);
    connect(wifiCheckTimer, &QTimer::timeout, this, &NetworkControls::checkWifiState);
    connect(displayUpdateTimer, &QTimer::timeout, this, &NetworkControls::updateNetworkDisplay);
}

void NetworkControls::setActive(bool active)
{
    if (active)
    {
        wifiCheckTimer->start(1000);
        displayUpdateTimer->start(1000);
        checkWifiState();
        updateNetworkDisplay();
    }
    else
    {
        wifiCheckTimer->stop();
        displayUpdateTimer->stop();
    }
}

void NetworkControls::displayNetworkDetails()
{
    static const QRegularExpression spaceSplitter("\\s{2,}");

    QProcess process;
    process.start("nmcli", QStringList() << "-f" << "ACTIVE,SSID,BSSID,MODE,CHAN,RATE,SIGNAL,DEVICE" << "device" << "wifi");
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    // Clear existing network details
    QLayoutItem *child;
    while ((child = optionPanelLayout->takeAt(3)) != nullptr) {
        delete child->widget();
        delete child;
    }

    bool activeConnectionFound = false;

    // Skip the header line and find the active connection
    for (int i = 1; i < lines.size(); ++i)
    {
        QString line = lines[i];
        QStringList fields = line.split(spaceSplitter, Qt::SkipEmptyParts);

        if (fields.size() >= 8 && fields[0] == "yes") {
            QStringList formattedDetails = {
                "SSID: " + fields[1],
                "BSSID: " + fields[2],
                "Mode: " + fields[3],
                "Channel: " + fields[4],
                "Rate: " + fields[5],
                "Signal: " + fields[6],
                "Device: " + fields[7],
                ""
            };

            for (const QString &formattedDetail : formattedDetails)
            {
                QLabel *label = new QLabel(formattedDetail, this);
                label->setStyleSheet("background-color: transparent; color: white;");
                QFont font = label->font();
                font.setPointSize(16);
                font.setFamily("Arial");
                label->setFont(font);
                label->setWordWrap(true);
                label->setFixedWidth(350);
                optionPanelLayout->addWidget(label);
            }
            activeConnectionFound = true;
            break; // Exit after displaying the active connection
        }
    }

    if (!activeConnectionFound) {
        QLabel *noConnectionLabel = new QLabel("No active Wi-Fi connection", this);
        noConnectionLabel->setStyleSheet("background-color: transparent; color: white;");
        QFont font = noConnectionLabel->font();
        font.setPointSize(16);
        font.setFamily("Arial");
        noConnectionLabel->setFont(font);
        optionPanelLayout->addWidget(noConnectionLabel);
    }
}

void NetworkControls::updateNetworkDisplay()
{
    qDebug() << "Display update timer triggered";

    // Clear existing network details
    QLayoutItem *child;
    while ((child = optionPanelLayout->takeAt(3)) != nullptr) {
        delete child->widget();
        delete child;
    }

    if (lastKnownWifiState) {
        displayNetworkDetails();
    } else {
        QLabel *noConnectionLabel = new QLabel("Wi-Fi is turned off", this);
        noConnectionLabel->setStyleSheet("background-color: transparent; color: white;");
        QFont font = noConnectionLabel->font();
        font.setPointSize(16);
        font.setFamily("Arial");
        noConnectionLabel->setFont(font);
        optionPanelLayout->addWidget(noConnectionLabel);
    }

    qDebug() << "Display update completed";
}

void NetworkControls::checkWifiState()
{
    qDebug() << "WiFi check timer triggered";

    QProcess process;
    process.start("nmcli", QStringList() << "-t" << "-f" << "WIFI" << "radio");
    process.waitForFinished();
    bool isEnabled = (process.readAllStandardOutput().trimmed() == "enabled");

    if (isEnabled != lastKnownWifiState) {
        lastKnownWifiState = isEnabled;
        wifiToggle->setToggleState(isEnabled);
        // updateNetworkDisplay();
    }

    qDebug() << "WiFi check completed";
}

void NetworkControls::handleWifiToggle(bool enabled)
{
    if (enabled == lastKnownWifiState) {
        return;
    }

    QProcess process;
    process.start("nmcli", QStringList() << "radio" << "wifi" << (enabled ? "on" : "off"));
    process.waitForFinished();

    if (process.exitCode() == 0) {
        lastKnownWifiState = enabled;

        if (enabled) {
            // Re-activate timers and update display when Wi-Fi is enabled
            setActive(true);
        } else {
            // Stop timers when Wi-Fi is disabled
            wifiCheckTimer->stop();
            displayUpdateTimer->stop();
            updateNetworkDisplay(); // Update display to show Wi-Fi is off
        }

        qDebug() << "Wi-Fi successfully" << (enabled ? "enabled" : "disabled");
    } else {
        qDebug() << "Failed to change Wi-Fi state:" << process.readAllStandardError();
        wifiToggle->setToggleState(!enabled);  // Revert the toggle state
    }
}
