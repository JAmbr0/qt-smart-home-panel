#include "HomeScreen.h"
#include <QTime>
#include <QDate>

HomeScreen::HomeScreen(QWidget *parent)
    : QMainWindow(parent), timer(new QTimer(this)), currentAreaButton(nullptr), currentOptionButton(nullptr)
{
    // Create the central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Set the background color to white
    centralWidget->setStyleSheet("background-color: rgba(23,27,46,255);");

    // Add panels
    setUpTopPanel();
    setUpAreaPanel();
    setUpOptionPanel();

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

    // Option panel
    optionPanel->setGeometry(windowWidth / 2.5,
                             areaPanel->height() + topPanel->height(),
                             windowWidth,
                             windowHeight - areaPanel->height() - topPanel->height() - 50);
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
    allDevicesButton = new QPushButton("All Devices");
    pcButton = new QPushButton("PC");
    bedroomButton = new QPushButton("Bedroom");
    homeButton = new QPushButton("Home");

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
    connect(allDevicesButton, &QPushButton::clicked, this, &HomeScreen::areaButtonClicked);
    connect(pcButton, &QPushButton::clicked, this, &HomeScreen::areaButtonClicked);
    connect(bedroomButton, &QPushButton::clicked, this, &HomeScreen::areaButtonClicked);
    connect(homeButton, &QPushButton::clicked, this, &HomeScreen::areaButtonClicked);

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
    currentAreaButton = allDevicesButton;
}

void HomeScreen::areaButtonClicked()
{
    // Get the button that was clicked
    QPushButton *clickedAreaButton = qobject_cast<QPushButton*>(sender());

    // Reset the style of any previously selected button
    if (currentAreaButton)
    {
        currentAreaButton->setStyleSheet("background-color: transparent;"
                                         "color: white;"
                                         "border-radius: 30px;");
    }

    // Set the stle of the clicked button to indicate its selection
    clickedAreaButton->setStyleSheet("background-color: rgba(58,94,171,255);"
                                     "color: white;"
                                     "border-radius: 30px;");

    // Update the current button
    currentAreaButton = clickedAreaButton;

    // Clear the current option panel layout before showing the new one
    // TODO: understand this
    QLayoutItem *child;
    while ((child = optionPanelLayout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    // Update the option panel based on the selected area button
    if (clickedAreaButton == allDevicesButton)
    {
        allDevicesButtons();
    }
    else if (clickedAreaButton == pcButton)
    {
        pcButtons();
    }
}

void HomeScreen::setUpOptionPanel()
{
    optionPanel = new QWidget(centralWidget);
    optionPanel->setStyleSheet("background-color: transparent;");

    // Create a grid layout
    optionPanelLayout = new QGridLayout(optionPanel);

    optionPanelLayout->setSpacing(10);

    // Call the function to set up the all devices buttons
    allDevicesButtons();

    // Set the grid layout for the option panel
    optionPanel->setLayout(optionPanelLayout);
    optionPanelLayout->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
}

QPushButton* HomeScreen::setUpButton(const QString &text, const QSize &size, const QString &style, int row, int col, QGridLayout *layout)
{
    QPushButton *button = new QPushButton(text);
    button->setFixedSize(size);
    button->setStyleSheet(style);

    QFont buttonFont;
    buttonFont.setPointSize(16);
    buttonFont.setFamily("Arial");
    buttonFont.setBold(true);
    button->setFont(buttonFont);

    layout->addWidget(button, row, col);

    return button;
}

void HomeScreen::allDevicesButtons()
{
    // Create a widget to hold the secondary grid layout
    QWidget *smallButtonsWidget = new QWidget;

    QSize smallButtonSize(220, 220);
    QSize largeButtonSize(450, 450);

    QString buttonStyle = QString("background-color: rgba(27,33,52,200);"
                                  "color: white;"
                                  "border-radius: 5px;");

    QString selectedButtonStyle = QString("background-color: rgba(58,94,171,255);"
                                          "color: white;"
                                          "border-radius: 5px;");

    // Create a secondary grid layout for smaller buttons
    QGridLayout *smallButtonsLayout = new QGridLayout;

    // Create the small buttons
    getUpButton = setUpButton("Get Up", smallButtonSize, buttonStyle, 0, 0, smallButtonsLayout);
    leaveButton = setUpButton("Leave", smallButtonSize, buttonStyle, 0, 1, smallButtonsLayout);
    atHomeButton = setUpButton("Home", smallButtonSize, selectedButtonStyle, 1, 0, smallButtonsLayout);
    sleepButton = setUpButton("Sleep", smallButtonSize, buttonStyle, 1, 1, smallButtonsLayout);

    // Create the large buttons
    networkButton = setUpButton("Network", largeButtonSize, buttonStyle, 0, 1, optionPanelLayout);
    lightsButton = setUpButton("Lights", largeButtonSize, buttonStyle, 1, 0, optionPanelLayout);
    thermostatButton = setUpButton("Thermostat", largeButtonSize, buttonStyle, 1, 1, optionPanelLayout);

    // Connect the buttons to the slot
    connect(getUpButton, &QPushButton::clicked, this, &HomeScreen::optionButtonClicked);
    connect(leaveButton, &QPushButton::clicked, this, &HomeScreen::optionButtonClicked);
    connect(atHomeButton, &QPushButton::clicked, this, &HomeScreen::optionButtonClicked);
    connect(sleepButton, &QPushButton::clicked, this, &HomeScreen::optionButtonClicked);

    smallButtonsLayout->setSpacing(10);
    smallButtonsLayout->setContentsMargins(0, 0, 0, 0);
    smallButtonsWidget->setLayout(smallButtonsLayout);

    optionPanelLayout->addWidget(smallButtonsWidget, 0, 0);

    currentOptionButton = atHomeButton;
}

void HomeScreen::pcButtons()
{
    // Create a widget to hold the secondary grid layout
    QWidget *smallButtonsWidget = new QWidget;

    QSize smallButtonSize(220, 220);
    QSize largeButtonSize(450, 450);

    QString buttonStyle = QString("background-color: rgba(27,33,52,200);"
                                  "color: white;"
                                  "border-radius: 5px;");

    // Create a secondary grid layout for smaller buttons
    QGridLayout *smallButtonsLayout = new QGridLayout;

    // Create the small buttons
    shutDownButton = setUpButton("Shut Down", smallButtonSize, buttonStyle, 0, 0, smallButtonsLayout);
    restartButton = setUpButton("Restart", smallButtonSize, buttonStyle, 0, 1, smallButtonsLayout);
    sleepButton = setUpButton("Sleep", smallButtonSize, buttonStyle, 1, 0, smallButtonsLayout);
    lockButton = setUpButton("Lock", smallButtonSize, buttonStyle, 1, 1, smallButtonsLayout);

    // Create the large buttons
    networkButton = setUpButton("WI-FI", largeButtonSize, buttonStyle, 0, 1, optionPanelLayout);
    lightsButton = setUpButton("LEDs", largeButtonSize, buttonStyle, 1, 0, optionPanelLayout);
    securityButton = setUpButton("Security", largeButtonSize, buttonStyle, 1, 1, optionPanelLayout);
    remoteButton = setUpButton("Remote", largeButtonSize, buttonStyle, 0, 2, optionPanelLayout);
    systemButton = setUpButton("System", largeButtonSize, buttonStyle, 1, 2, optionPanelLayout);

    smallButtonsLayout->setSpacing(10);
    smallButtonsLayout->setContentsMargins(0, 0, 0, 0);
    smallButtonsWidget->setLayout(smallButtonsLayout);

    // Add the buttons to the grid layout
    optionPanelLayout->addWidget(smallButtonsWidget, 0, 0);
}

void HomeScreen::optionButtonClicked()
{
    // Get the button that was clicked
    QPushButton *clickedOptionButton = qobject_cast<QPushButton*>(sender());

    // Reset the style of any previously selected button
    if (currentOptionButton)
    {
        currentOptionButton->setStyleSheet("background-color: rgba(27,33,52,200);"
                                           "color: white;"
                                           "border-radius: 5px;");
    }

    // Set the stle of the clicked button to indicate its selection
    clickedOptionButton->setStyleSheet("background-color: rgba(58,94,171,255);"
                                       "color: white;"
                                       "border-radius: 5px;");

    // Update the current button
    currentOptionButton = clickedOptionButton;
}
