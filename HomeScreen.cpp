#include "HomeScreen.h"
#include <QTime>
#include <QDate>

HomeScreen::HomeScreen(QWidget *parent)
    : QMainWindow(parent), timer(new QTimer(this)), currentButton(nullptr)
{
    // Create the central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Set the background color to white
    centralWidget->setStyleSheet("background-color: rgba(23,27,46,255);");

    // Add panels
    setUpTopPanel();
    setUpAreaPanel();

    // Connect the timer to the updateTime slot
    connect(timer, &QTimer::timeout, this, &HomeScreen::updateTime);

    // Start the time to update every 1000ms
    timer->start();

    // Initial call to set the current time immediately
    updateTime();

    geometry();

    this->show();
}

HomeScreen::~HomeScreen()
{
    // Cleanup
}

void HomeScreen::updateTime()
{
    QTime currentTime = QTime::currentTime();
    QString timeString = currentTime.toString("hh:mm");
    timeLabel->setText(timeString);

    QDate currentDate = QDate::currentDate();
    QString dateString = currentDate.toString("ddd, MMM dd");
    dateLabel->setText(dateString);
}

void HomeScreen::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    geometry();
}

void HomeScreen::geometry()
{
    int windowWidth = centralWidget->width();
    int windowHeight = centralWidget->height();

    // Top panel
    topPanel->setGeometry(0, 0, windowWidth, windowHeight*0.08);
    topPanel->setFixedHeight(100);

    titleLabel->setGeometry((topPanel->width() - titleLabel->width()) / 2,
                            (topPanel->height() - titleLabel->height()) / 2,
                            titleLabel->width(),
                            titleLabel->height());

    timeLabel->setGeometry(100,
                           (topPanel->height() - timeLabel->height()) / 2,
                           timeLabel->width(),
                           timeLabel->height());

    dateLabel->setGeometry(200,
                           (topPanel->height() - dateLabel->height()) / 2,
                           dateLabel->width(),
                           dateLabel->height());

    // Area panel
    areaPanel->setGeometry(0, topPanel->height(), windowWidth, windowHeight*0.08);
    areaPanel->setFixedHeight(100);
}

void HomeScreen::setUpTopPanel()
{
    topPanel = new QWidget(centralWidget);
    topPanel->setStyleSheet("background-color: transparent;");

    // Add top panel title
    titleLabel = new QLabel("Control Hub", topPanel);
    titleLabel->setStyleSheet("background-color: transparent; color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Set title font size
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setFamily("Arial");
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->adjustSize();

    // Add top panel time
    timeLabel = new QLabel("00:00", topPanel);
    timeLabel->setStyleSheet("background-color: transparent; color: white;");
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->adjustSize();

    // Set time font size
    QFont timeFont = timeLabel->font();
    timeFont.setPointSize(20);
    timeFont.setFamily("Arial");
    timeFont.setBold(true);
    timeLabel->setFont(timeFont);
    timeLabel->adjustSize();

    // Add top panel time
    dateLabel = new QLabel("Sun, Jan 01", topPanel);
    dateLabel->setStyleSheet("background-color: transparent; color: white;");
    dateLabel->setAlignment(Qt::AlignCenter);
    dateLabel->adjustSize();

    // Set date font size
    QFont dateFont = dateLabel->font();
    dateFont.setPointSize(20);
    dateFont.setFamily("Arial");
    dateLabel->setFont(dateFont);
    dateLabel->adjustSize();
}

void HomeScreen::setUpAreaPanel()
{
    areaPanel = new QWidget(centralWidget);
    areaPanel->setStyleSheet("background-color: transparent;");

    // Create a horizontal layout
    areaPanelLayout = new QHBoxLayout(areaPanel);

    // Create the buttons
    QPushButton *allDevicesButton = new QPushButton("All Devices");
    QPushButton *pcButton = new QPushButton("PC");
    QPushButton *bedroomButton = new QPushButton("Bedroom");
    QPushButton *homeButton = new QPushButton("Home");

    QFont buttonFont;
    buttonFont.setPointSize(12);
    buttonFont.setFamily("Arial");
    buttonFont.setBold(true);

    QString buttonStyle = QString("background-color: transparent;"
                                  "color: white;"
                                  "border-radius: 30px;");

    QString selectedButtonStyle = QString("background-color: rgba(58,94,171,255);"
                                  "color: white;"
                                  "border-radius: 30px;");

    QSize minimumButtonSize(160, 60);
    allDevicesButton->setMinimumSize(minimumButtonSize);
    pcButton->setMinimumSize(minimumButtonSize);
    bedroomButton->setMinimumSize(minimumButtonSize);
    homeButton->setMinimumSize(minimumButtonSize);

    allDevicesButton->setStyleSheet(selectedButtonStyle);
    allDevicesButton->setFont(buttonFont);
    pcButton->setStyleSheet(buttonStyle);
    pcButton->setFont(buttonFont);
    bedroomButton->setStyleSheet(buttonStyle);
    bedroomButton->setFont(buttonFont);
    homeButton->setStyleSheet(buttonStyle);
    homeButton->setFont(buttonFont);

    // Connect the buttons to the slot
    connect(allDevicesButton, &QPushButton::clicked, this, &HomeScreen::buttonClicked);
    connect(pcButton, &QPushButton::clicked, this, &HomeScreen::buttonClicked);
    connect(bedroomButton, &QPushButton::clicked, this, &HomeScreen::buttonClicked);
    connect(homeButton, &QPushButton::clicked, this, &HomeScreen::buttonClicked);

    // Add a spacer item to the left of the buttons
    QSpacerItem *leftSpacer = new QSpacerItem(50, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
    areaPanelLayout->addItem(leftSpacer);

    // Add the buttons to the horizontal layout
    areaPanelLayout->addWidget(allDevicesButton);
    areaPanelLayout->addSpacing(10);
    areaPanelLayout->addWidget(pcButton);
    areaPanelLayout->addSpacing(10);
    areaPanelLayout->addWidget(bedroomButton);
    areaPanelLayout->addSpacing(10);
    areaPanelLayout->addWidget(homeButton);

    // Set the horizontal layout for the area panel
    areaPanel->setLayout(areaPanelLayout);
    areaPanelLayout->setAlignment(Qt::AlignLeft);

    // Set the all devices button to be selected initially
    currentButton = allDevicesButton;
}

void HomeScreen::buttonClicked()
{
    // Get the button that was clicked
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());

    // Reset the style of any previously selected button
    if (currentButton)
    {
        currentButton->setStyleSheet("background-color: transparent;"
                                  "color: white;"
                                  "border-radius: 30px;");
    }

    // Set the stle of the clicked button to indicate its selection
    clickedButton->setStyleSheet("background-color: rgba(58,94,171,255);"
                                  "color: white;"
                                  "border-radius: 30px;");

    // Update the current button
    currentButton = clickedButton;
}











































