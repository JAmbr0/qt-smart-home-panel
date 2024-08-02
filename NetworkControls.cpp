#include "NetworkControls.h"
#include "ToggleButton.h"
#include <QStringList>

NetworkControls::NetworkControls(QWidget *parent)
    : QWidget(parent),
    optionPanelLayout(new QVBoxLayout(this))
{
    setLayout(optionPanelLayout);
    optionPanelLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QLabel *titleLabel = new QLabel("Network", this);
    titleLabel->setStyleSheet("background-color: transparent; color: white;");

    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(30); // Set title font size
    titleFont.setFamily("Arial"); // Set title font family
    titleFont.setBold(true); // Set title font to bold
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: white;");

    wifiToggle = new ToggleButton(this);
    wifiToggle->setLabelText("Wi-Fi     ");

    optionPanelLayout->addWidget(titleLabel);
    optionPanelLayout->addSpacing(40);
    optionPanelLayout->addWidget(wifiToggle);
    optionPanelLayout->addSpacing(40);

    optionPanelLayout->setContentsMargins(25, 25, 0, 0);
}

void NetworkControls::displayNetworkDetails()
{
    // Example Wi-Fi details
    QStringList wifiDetailsList = {
        "SSID: ExampleNetwork1",
        "BSSID: AA:BB:CC:DD:EE:FF",
        "Mode: Infrastructure",
        "Channel: 6",
        "Rate: 54 Mbit/s",
        "Signal: 70",
        "Device: wlan0",
        "",
        "SSID: ExampleNetwork2",
        "BSSID: 11:22:33:44:55:66",
        "Mode: Infrastructure",
        "Channel: 11",
        "Rate: 150 Mbit/s",
        "Signal: 80",
        "Device: wlan1"
    };

    // Add Wi-Fi details to the option panel
    for (const QString &detail : wifiDetailsList)
    {
        QLabel *label = new QLabel(detail, this);
        label->setStyleSheet("background-color: transparent; color: white;");

        // Set font properties
        QFont font = label->font();
        font.setPointSize(16);
        font.setFamily("Arial");
        font.setBold(false);
        label->setFont(font);

        optionPanelLayout->addWidget(label);
    }
}
