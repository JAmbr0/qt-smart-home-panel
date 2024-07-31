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
    void optionButtonClicked();

private:
    void geometry();
    void setUpTopPanel();
    void setUpAreaPanel();
    void setUpOptionPanel();
    void allDevicesButtons();
    void pcButtons();
    void adjustFontSizes();

    QWidget *centralWidget;
    QWidget *topPanel;
    QWidget *areaPanel;
    QWidget *optionPanel;

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

    QPushButton *currentAreaButton;
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
};

#endif // HOMESCREEN_H
