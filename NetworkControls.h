#ifndef NETWORKCONTROLS_H
#define NETWORKCONTROLS_H

#include "ToggleButton.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class NetworkControls : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkControls(QWidget *parent = nullptr);
    void displayNetworkDetails();

private:
    QVBoxLayout *optionPanelLayout;
    QLabel *optionPanelTitleLabel;

    ToggleButton *wifiToggle;

    QStringList wifiDetailsList;
};

#endif // NETWORKCONTROLS_H
