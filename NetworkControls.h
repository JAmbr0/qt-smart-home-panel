#ifndef NETWORKCONTROLS_H
#define NETWORKCONTROLS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

class ToggleButton;

class NetworkControls : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkControls(QWidget *parent = nullptr);
    bool checkDependencies();

    void setActive(bool active);

private slots:
    void checkWifiState();
    void handleWifiToggle(bool enabled);

private:
    void displayNetworkDetails();
    void updateNetworkDisplay();
    QStringList getWifiDetails();

    QVBoxLayout *optionPanelLayout;
    ToggleButton *wifiToggle;
    QTimer *wifiCheckTimer;
    bool lastKnownWifiState;

    QTimer *displayUpdateTimer;
    QTimer *enableWifiTimer;
};

#endif // NETWORKCONTROLS_H
