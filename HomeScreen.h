#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGridLayout>

class HomeScreen : public QMainWindow
{
    Q_OBJECT

public:
    HomeScreen(QWidget *parent = nullptr);
    ~HomeScreen();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateTime();
    void areaButtonClicked();
    void statusButtonClicked();
    void itemButtonClicked();

    void handleShutDown();
    void handleRestart();
    void handleSleep();
    void handleLock();
    void handleWiFiDetails();

private:
    void geometry();
    void setUpTopPanel();
    void setUpAreaPanel();
    void setUpitemPanel();
    void setUpWeatherPanel();
    void allDevicesButtons();
    void pcButtons();
    void updateWeatherPanel(const QString &condition, const QString &temperature, const QString &feelsliketemperature);
    void adjustFontSizes();

    QWidget *centralWidget;
    QWidget *topPanel;
    QWidget *areaPanel;
    QWidget *itemPanel;
    QWidget *weatherPanel;

    QLabel *titleLabel;
    QLabel *timeLabel;
    QLabel *dateLabel;
    QTimer *timer;

    QPushButton *allDevicesButton;
    QPushButton *pcButton;
    QPushButton *bedroomButton;
    QPushButton *homeButton;

    QPushButton* setUpButton(const QString &text, const QSize &size, const QString &style, int row, int col, QGridLayout *layout);

    QHBoxLayout *areaPanelLayout;
    QGridLayout *itemPanelLayout;
    QVBoxLayout *weatherPanelLayout;

    QPushButton *currentAreaButton;
    QPushButton *currentStatusButton;
    QPushButton *currentItemButton;

    QPushButton *getUpButton;
    QPushButton *leaveButton;
    QPushButton *atHomeButton;
    QPushButton *sleepButton;
    QPushButton *networkButton;
    QPushButton *lightsButton;
    QPushButton *thermostatButton;
    QPushButton *shutDownButton;
    QPushButton *restartButton;
    QPushButton *lockButton;
    QPushButton *securityButton;
    QPushButton *remoteButton;
    QPushButton *systemButton;

    QLabel *weatherConditionLabel;
    QLabel *weatherTemperatureLabel;
    QLabel *weatherFeelsLikeTemperatureLabel;
};

#endif // HOMESCREEN_H
