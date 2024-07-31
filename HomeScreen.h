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
    void optionButtonClicked();

    void handleShutDown();
    void handleRestart();
    void handleSleep();
    void handleLock();
    void handleWiFiDetails();

private:
    void geometry();
    void setUpTopPanel();
    void setUpAreaPanel();
    void setUpOptionPanel();
    void setUpInfoPanel();
    void allDevicesButtons();
    void pcButtons();
    void updateInfoPanel(const QString &title, const QString &info);
    void adjustFontSizes();

    QWidget *centralWidget;
    QWidget *topPanel;
    QWidget *areaPanel;
    QWidget *optionPanel;
    QWidget *infoPanel;

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
    QGridLayout *optionPanelLayout;
    QVBoxLayout *infoPanelLayout;

    QPushButton *currentAreaButton;
    QPushButton *currentStatusButton;
    QPushButton *currentOptionButton;

    // Member variables for buttons
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

    QLabel *infoTitleLabel;
    QLabel *infoLabel;
};

#endif // HOMESCREEN_H
