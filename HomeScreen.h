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
    void adjustFontSizes();

    QWidget *centralWidget;
    QWidget *topPanel;
    QWidget *areaPanel;
    QWidget *optionPanel;

    QLabel *titleLabel;
    QLabel *timeLabel;
    QLabel *dateLabel;
    QTimer *timer;

    QHBoxLayout *areaPanelLayout;
    QGridLayout *optionPanelLayout;

    QPushButton *currentAreaButton;
    QPushButton *currentOptionButton;
};

#endif // HOMESCREEN_H
