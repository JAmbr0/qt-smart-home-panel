#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include "NetworkControls.h"
#include "SecurityControls.h"

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QPropertyAnimation>

class HomeScreen : public QMainWindow
{
    Q_OBJECT

public:
    HomeScreen(QWidget *parent = nullptr);
    ~HomeScreen();

protected:
    // Event handlers
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    // Slot to update the time display
    void updateTime();

    // Button click handlers
    void areaButtonClicked();
    void statusButtonClicked();
    void itemButtonClicked();

    // PC control handlers
    void handleShutDown();
    void handleRestart();
    void handleSleep();
    void handleLock();

private:
    // Setup methods
    void geometry();
    void setUpTopPanel();
    void setUpAreaPanel();
    void setUpItemPanel();
    void setUpWeatherPanel();
    void setUpOptionPanel();
    void clearOptionPanelLayout();
    void allDevicesButtons();
    void pcButtons();

    // Helper methods
    void updateWeatherPanel(const QString &condition, const QString &temperature, const QString &feelsliketemperature);
    void adjustFontSizes();
    QPushButton* setUpItemButton(const QString &text, const QSize &size, const QString &style, int row, int col, QGridLayout *layout);
    void animatePanel(const QRect &endValue);
    void swipePanelDown();

    // Main widgets
    QWidget *centralWidget;
    QWidget *topPanel;
    QWidget *areaPanel;
    QWidget *itemPanel;
    QWidget *weatherPanel;
    QWidget *optionPanel;

    // Top panel labels
    QLabel *titleLabel;
    QLabel *timeLabel;
    QLabel *dateLabel;

    // Timers
    QTimer *timer;

    // Area buttons
    QPushButton *allDevicesButton;
    QPushButton *pcButton;
    QPushButton *bedroomButton;
    QPushButton *homeButton;

    // Layouts
    QHBoxLayout *areaPanelLayout;
    QGridLayout *itemPanelLayout;
    QVBoxLayout *weatherPanelLayout;
    QVBoxLayout *optionPanelLayout;

    // Current selected buttons
    QPushButton *currentAreaButton;
    QPushButton *currentStatusButton;
    QPushButton *currentItemButton;

    // Status buttons
    QPushButton *getUpButton;
    QPushButton *leaveButton;
    QPushButton *atHomeButton;
    QPushButton *toSleepButton;

    // Item buttons
    QPushButton *networkButton;
    QPushButton *lightsButton;
    QPushButton *thermostatButton;
    QPushButton *securityButton;
    QPushButton *remoteButton;
    QPushButton *systemButton;

    // PC power buttons
    QPushButton *shutDownButton;
    QPushButton *restartButton;
    QPushButton *lockButton;
    QPushButton *sleepButton;

    // Weather labels
    QLabel *weatherConditionLabel;
    QLabel *weatherTemperatureLabel;
    QLabel *weatherFeelsLikeTemperatureLabel;

    // Control widgets
    NetworkControls *networkControls;
    SecurityControls *securityControls;

    // Drag variables
    bool isDragging;
    QPoint dragStartPosition;
    int dragThreshold;

    // Animations
    QPropertyAnimation *optionPanelAnimation;
};

#endif // HOMESCREEN_H
