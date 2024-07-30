#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>

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
    void buttonClicked();

private:
    void geometry();
    void setUpTopPanel();
    void setUpAreaPanel();
    void adjustFontSizes();

    QWidget *centralWidget;
    QWidget *topPanel;
    QWidget *areaPanel;

    QLabel *titleLabel;
    QLabel *timeLabel;
    QLabel *dateLabel;
    QTimer *timer;

    QHBoxLayout *areaPanelLayout;

    QPushButton *currentButton;
};

#endif // HOMESCREEN_H
