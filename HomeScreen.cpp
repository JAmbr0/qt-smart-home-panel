#include "HomeScreen.h"
#include <QTime>
#include <QDate>
#include <QProcess>
#include <QMessageBox>
#include <QNetworkInterface>

HomeScreen::HomeScreen(QWidget *parent)
    : QMainWindow(parent), timer(new QTimer(this)), currentAreaButton(nullptr), currentItemButton(nullptr)
{
    // Create the central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Set the background color to white
    centralWidget->setStyleSheet("background-color: rgba(23,27,46,255);");

    // Add panels
    setUpTopPanel();
    setUpAreaPanel();
    setUpitemPanel();
    setUpWeatherPanel();

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
    itemPanel->setGeometry(windowWidth / 2.5,
                             areaPanel->height() + topPanel->height(),
                             windowWidth,
                             windowHeight - areaPanel->height() - topPanel->height() - 50);

    // Weather panel
    weatherPanel->setGeometry(50,
                           areaPanel->height() + topPanel->height() + 50,
                           windowWidth / 5,
                           windowHeight - areaPanel->height() - topPanel->height() - 150);
}

void HomeScreen::setUpTopPanel()
{
    topPanel = new QWidget(centralWidget);
    topPanel->setStyleSheet("background-color: transparent;");

    // Add top panel title
    titleLabel = new QLabel("Control Panel", topPanel);
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

void HomeScreen::setUpWeatherPanel()
{
    weatherPanel = new QWidget(centralWidget);
    weatherPanel->setStyleSheet("background-color: rgba(27,33,52,200);"
                             "border-radius: 100;");

    QFont weatherConditionFont;
    weatherConditionFont.setPointSize(30);
    weatherConditionFont.setFamily("Arial");
    weatherConditionFont.setBold(true);

    QFont weatherTemperatureFont;
    weatherTemperatureFont.setPointSize(60);
    weatherTemperatureFont.setFamily("Arial");
    weatherTemperatureFont.setBold(true);

    QFont weatherFeelsLikeTemperatureFont;
    weatherFeelsLikeTemperatureFont.setPointSize(20);
    weatherFeelsLikeTemperatureFont.setFamily("Arial");
    weatherFeelsLikeTemperatureFont.setBold(false);

    weatherPanelLayout = new QVBoxLayout(weatherPanel);

    weatherConditionLabel = new QLabel("Clear", weatherPanel);
    weatherConditionLabel->setFont(weatherConditionFont);
    weatherConditionLabel->setStyleSheet("background-color: transparent; color: white;");

    weatherTemperatureLabel = new QLabel("18°C", weatherPanel);
    weatherTemperatureLabel->setFont(weatherTemperatureFont);
    weatherTemperatureLabel->setStyleSheet("background-color: transparent; color: white;");

    weatherFeelsLikeTemperatureLabel = new QLabel("Feels like: 19°C", weatherPanel);
    weatherFeelsLikeTemperatureLabel->setFont(weatherFeelsLikeTemperatureFont);
    weatherFeelsLikeTemperatureLabel->setStyleSheet("background-color: transparent; color: white;");

    weatherPanelLayout->addWidget(weatherConditionLabel);
    weatherPanelLayout->addSpacing(30);
    weatherPanelLayout->addWidget(weatherTemperatureLabel);
    weatherPanelLayout->addSpacing(30);
    weatherPanelLayout->addWidget(weatherFeelsLikeTemperatureLabel);
    weatherPanelLayout->setContentsMargins(0, 0, 0, 0);

    weatherPanel->setLayout(weatherPanelLayout);
    weatherPanelLayout->setAlignment(Qt::AlignCenter);
}

void HomeScreen::updateWeatherPanel(const QString &condition, const QString &temperature, const QString &feelsliketemperature)
{
    weatherConditionLabel->setText(condition);
    weatherTemperatureLabel->setText(temperature);
    weatherFeelsLikeTemperatureLabel->setText(feelsliketemperature);
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
    while ((child = itemPanelLayout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    // Reset the current option button when changing options (prevents crash)
    currentItemButton = nullptr;

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

void HomeScreen::setUpitemPanel()
{
    itemPanel = new QWidget(centralWidget);
    itemPanel->setStyleSheet("background-color: transparent;");

    // Create a grid layout
    itemPanelLayout = new QGridLayout(itemPanel);

    itemPanelLayout->setSpacing(10);

    // Call the function to set up the all devices buttons
    allDevicesButtons();

    // Set the grid layout for the option panel
    itemPanel->setLayout(itemPanelLayout);
    itemPanelLayout->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
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
    networkButton = setUpButton("Network", largeButtonSize, buttonStyle, 0, 1, itemPanelLayout);
    lightsButton = setUpButton("Lights", largeButtonSize, buttonStyle, 1, 0, itemPanelLayout);
    thermostatButton = setUpButton("Thermostat", largeButtonSize, buttonStyle, 1, 1, itemPanelLayout);

    // Connect the small buttons to their slot
    connect(getUpButton, &QPushButton::clicked, this, &HomeScreen::statusButtonClicked);
    connect(leaveButton, &QPushButton::clicked, this, &HomeScreen::statusButtonClicked);
    connect(atHomeButton, &QPushButton::clicked, this, &HomeScreen::statusButtonClicked);
    connect(sleepButton, &QPushButton::clicked, this, &HomeScreen::statusButtonClicked);

    // Connect the large buttons to their slot
    connect(networkButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);
    connect(lightsButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);
    connect(thermostatButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);

    smallButtonsLayout->setSpacing(10);
    smallButtonsLayout->setContentsMargins(0, 0, 0, 0);
    smallButtonsWidget->setLayout(smallButtonsLayout);

    itemPanelLayout->addWidget(smallButtonsWidget, 0, 0);

    currentStatusButton = atHomeButton;
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

    // Connect the small buttons to their respective slots
    connect(shutDownButton, &QPushButton::clicked, this, &HomeScreen::handleShutDown);
    connect(restartButton, &QPushButton::clicked, this, &HomeScreen::handleRestart);
    connect(sleepButton, &QPushButton::clicked, this, &HomeScreen::handleSleep);
    connect(lockButton, &QPushButton::clicked, this, &HomeScreen::handleLock);

    // Create the large buttons
    networkButton = setUpButton("WI-FI", largeButtonSize, buttonStyle, 0, 1, itemPanelLayout);
    lightsButton = setUpButton("LEDs", largeButtonSize, buttonStyle, 1, 0, itemPanelLayout);
    securityButton = setUpButton("Security", largeButtonSize, buttonStyle, 1, 1, itemPanelLayout);
    remoteButton = setUpButton("Remote", largeButtonSize, buttonStyle, 0, 2, itemPanelLayout);
    systemButton = setUpButton("System", largeButtonSize, buttonStyle, 1, 2, itemPanelLayout);

    connect(networkButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);
    connect(lightsButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);
    connect(securityButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);
    connect(remoteButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);
    connect(systemButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);

    smallButtonsLayout->setSpacing(10);
    smallButtonsLayout->setContentsMargins(0, 0, 0, 0);
    smallButtonsWidget->setLayout(smallButtonsLayout);

    // Add the buttons to the grid layout
    itemPanelLayout->addWidget(smallButtonsWidget, 0, 0);
}

void HomeScreen::handleShutDown()
{
    QProcess::startDetached("systemctl", QStringList() << "poweroff");
}

void HomeScreen::handleRestart()
{
    QProcess::startDetached("systemctl", QStringList() << "reboot");
}

void HomeScreen::handleSleep()
{
    QProcess::startDetached("systemctl", QStringList() << "suspend");}

void HomeScreen::handleLock()
{
    QProcess::startDetached("xfce4-session-logout", QStringList() << "--logout");
}

void HomeScreen::handleWiFiDetails()
{
    // // A list of example Wi-Fi details
    // QStringList wifiDetailsList = {
    //     "SSID: ExampleNetwork1",
    //     "BSSID: AA:BB:CC:DD:EE:FF",
    //     "Mode: Infrastructure",
    //     "Channel: 6",
    //     "Rate: 54 Mbit/s",
    //     "Signal: 70",
    //     "Device: wlan0",
    //     "",
    //     "SSID: ExampleNetwork2",
    //     "BSSID: 11:22:33:44:55:66",
    //     "Mode: Infrastructure",
    //     "Channel: 11",
    //     "Rate: 150 Mbit/s",
    //     "Signal: 80",
    //     "Device: wlan1"
    // };

    // // Join the list into a single string
    // QString wifiDetails = wifiDetailsList.join("\n");

    // // Display Wi-Fi details in a message box
    // QString title = "Network";
}

void HomeScreen::statusButtonClicked()
{
    // Get the button that was clicked
    QPushButton *clickedStatusButton = qobject_cast<QPushButton*>(sender());

    // Reset the style of any previously selected button
    if (currentStatusButton)
    {
        currentStatusButton->setStyleSheet("background-color: rgba(27,33,52,200);"
                                           "color: white;"
                                           "border-radius: 5px;");
    }

    // Set the style of the clicked button to indicate its selection
    clickedStatusButton->setStyleSheet("background-color: rgba(58,94,171,255);"
                                       "color: white;"
                                       "border-radius: 5px;");

    // Update the current button
    currentStatusButton = clickedStatusButton;

    // // Display info based on the clicked button
    // if (clickedStatusButton == getUpButton) {
    //     updateWeatherPanel("Get Up", "Get up and start your day.", false, "");
    // } else if (clickedStatusButton == leaveButton) {
    //     updateWeatherPanel("Leave", "You are leaving the house.", false, "");
    // } else if (clickedStatusButton == atHomeButton) {
    //     updateWeatherPanel("Home", "You are at home.",false, "");
    // } else if (clickedStatusButton == sleepButton) {
    //     updateWeatherPanel("Sleep", "Time to go to sleep.", true, "Alarm:");
    // }
}

void HomeScreen::itemButtonClicked()
{
    // Get the button that was clicked
    QPushButton *clickedItemButton = qobject_cast<QPushButton*>(sender());

    // Check if the clicked button is different from the current option button
    if (currentItemButton && currentItemButton != clickedItemButton)
    {
        // Reset the style of the previously selected button
        currentItemButton->setStyleSheet("background-color: rgba(27,33,52,200);"
                                           "color: white;"
                                           "border-radius: 5px;");

        // Reset the info panel when the option button changes
        // QString title = "Information";
        // QString info = "Select an option to see details.";
        // updateWeatherPanel(title, info, false, "");
    }

    // Set the style of the clicked button to indicate its selection
    clickedItemButton->setStyleSheet("background-color: rgba(58,94,171,255);"
                                       "color: white;"
                                       "border-radius: 5px;");

    // Update the current button
    currentItemButton = clickedItemButton;

    // // Display options based on the clicked button
    // if (clickedItemButton == networkButton) {
    //     handleWiFiDetails();
    // } else if (clickedItemButton == lightsButton) {
    //     updateWeatherPanel("Lights", "Control your home lighting system.", false, "");
    // } else if (clickedItemButton == thermostatButton) {
    //     updateWeatherPanel("Thermostat", "Adjust the temperature settings.", false, "");
    // } else if (clickedItemButton == securityButton) {
    //     updateWeatherPanel("Security", "Manage your home security settings.", false, "");
    // } else if (clickedItemButton == remoteButton) {
    //     updateWeatherPanel("Remote", "Access and control remote devices.", false, "");
    // } else if (clickedItemButton == systemButton) {
    //     updateWeatherPanel("System", "View and manage system settings.", false, "");
    // }
}
