#include "HomeScreen.h"
#include "NetworkControls.h"
#include "SecurityControls.h"
#include "Weather.h"
#include <QTime>
#include <QDate>
#include <QProcess>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QMouseEvent>
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <cmath>

NetworkControls *networkControls;
SecurityControls *securityControls;

// Constructor
HomeScreen::HomeScreen(QWidget *parent)
    : QMainWindow(parent),
    timer(new QTimer(this)),
    weatherTimer(new QTimer(this)),
    weather(new Weather(this)),
    currentAreaButton(nullptr),
    currentItemButton(nullptr),
    networkControls(new NetworkControls(this)),
    securityControls(new SecurityControls(this)),
    isDragging(false),
    dragThreshold(500),
    optionPanelAnimation(new QPropertyAnimation(this))
{
    // Create the central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Set the background color to a dark blue
    centralWidget->setStyleSheet("background-color: rgba(23,27,46,255);");

    // Add homescreen panels
    setUpTopPanel();
    setUpAreaPanel();
    setUpItemPanel();
    setUpWeatherPanel();
    setUpOptionPanel();

    // Connect the timer to update the time display
    connect(timer, &QTimer::timeout, this, &HomeScreen::updateTime);

    // Start the time to update every 1 second (1000ms)
    timer->start(1000);

    // Initial call to set the current time immediately
    updateTime();

    // Apply the layout geometry
    geometry();

    // Install the event filter for the homescreen
    this->installEventFilter(this);

    // Configure the option panel animation
    optionPanelAnimation->setTargetObject(optionPanel);
    optionPanelAnimation->setPropertyName("geometry");
    optionPanelAnimation->setEasingCurve(QEasingCurve::OutCubic);
    optionPanelAnimation->setDuration(300);

    connect(weather, &Weather::weatherDataUpdated, this, [this]() {
        double tempVal = weather->getTemperature().toDouble();
        double apparentVal = weather->getApparentTemperature().toDouble();
        double precipVal = weather->getPrecipitation().toDouble();
        double cloudVal = weather->getCloudCover().toDouble();
        double windVal = weather->getWindSpeed().toDouble();
        double windDir = weather->getWindDirection().toDouble();
        double snowVal = weather->getSnowfall().toDouble();
        bool day = (weather->getIsDay().toDouble() == 1.0);

        QString tempStr = QString::number(tempVal, 'f', 1) + "°C";
        QString apparentStr = "Feels Like: " + QString::number(apparentVal, 'f', 1) + "°C";

        QString conditionKey;
        if (snowVal >= 0.1) {
            conditionKey = "Snow";
        } else if (precipVal >= 0.1) {
            conditionKey = "Rain";
        } else if (precipVal < 0.1 && snowVal < 0.1 && windVal >= 8.0 && cloudVal < 20.0) {
            conditionKey = day ? "Windy-Day" : "Windy-Night";
        } else if (precipVal < 0.1 && snowVal < 0.1 && cloudVal >= 80.0) {
            conditionKey = "Overcast";
        } else if (precipVal < 0.1 && snowVal < 0.1 && cloudVal < 20.0) {
            conditionKey = day ? "Clear-Day" : "Clear-Night";
        } else {
            conditionKey = day ? "Few-Clouds-Day" : "Few-Clouds-Night";
        }

        updateWeatherPanel(conditionKey, tempStr, apparentStr, windVal, windDir);
    });
    weather->updateWeatherData();

    // Update weather info every minute
    connect(weatherTimer, &QTimer::timeout, weather, &Weather::updateWeatherData);
    weatherTimer->start(600000);

    // Display the homescreen
    this->show();
}

// Destructor
HomeScreen::~HomeScreen()
{
    // Cleanup
}

void HomeScreen::updateTime()
{
    // Get and format the current time
    QTime currentTime = QTime::currentTime();
    QString timeString = currentTime.toString("hh:mm");
    timeLabel->setText(timeString);

    // Get and format the current date
    QDate currentDate = QDate::currentDate();
    QString dateString = currentDate.toString("ddd, MMM dd");
    dateLabel->setText(dateString);
}

void HomeScreen::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // Reapply the layout geometry
    geometry();
}

void HomeScreen::geometry()
{
    // Get the current dimensions of the central widget
    int windowWidth = centralWidget->width();
    int windowHeight = centralWidget->height();

    // Top panel (it's at the top)
    topPanel->setGeometry(0, 0, windowWidth, windowHeight*0.08);
    topPanel->setFixedHeight(100);

    // Center the title label within the top panel
    titleLabel->setGeometry((topPanel->width() - titleLabel->width()) / 2,
                            (topPanel->height() - titleLabel->height()) / 2,
                            titleLabel->width(),
                            titleLabel->height());

    // Position time label near left of top panel
    timeLabel->setGeometry(100,
                           (topPanel->height() - timeLabel->height()) / 2,
                           timeLabel->width(),
                           timeLabel->height());

    // Position the date label to the right of the time label
    dateLabel->setGeometry(200,
                           (topPanel->height() - dateLabel->height()) / 2,
                           dateLabel->width() + 20, // Sometimes date is too long for label? Gets cut off. Don't know why.
                           dateLabel->height());

    // Area panel (under the top panel)
    areaPanel->setGeometry(0, topPanel->height(), windowWidth, windowHeight*0.08);
    areaPanel->setFixedHeight(100);

    // Item panel (centered-ish)
    itemPanel->setGeometry(windowWidth / 2.5,
                             areaPanel->height() + topPanel->height(),
                             windowWidth,
                             windowHeight - areaPanel->height() - topPanel->height() - 50);

    // Weather panel (left of item panel)
    weatherPanel->setGeometry(50,
                           areaPanel->height() + topPanel->height() + 50,
                           windowWidth / 5,
                           windowHeight - areaPanel->height() - topPanel->height() - 150);

    // Option panel (almost covers screen, shows when an item is selected)
    optionPanel->setGeometry(0,
                             topPanel->height() + areaPanel->height() + 50,
                             windowWidth,
                             windowHeight - areaPanel->height() - topPanel->height() - 50);
}

void HomeScreen::setUpTopPanel()
{
    // Create the top panel widget
    topPanel = new QWidget(centralWidget);
    topPanel->setStyleSheet("background-color: transparent;");

    // Create the title label
    titleLabel = new QLabel("Control Panel", topPanel); // Change title
    titleLabel->setStyleSheet("background-color: transparent; color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Font for the title
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setFamily("Arial");
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->adjustSize();

    // Create the time label
    timeLabel = new QLabel("00:00", topPanel);
    timeLabel->setStyleSheet("background-color: transparent; color: white;");
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->adjustSize();

    // Font for the time
    QFont timeFont = timeLabel->font();
    timeFont.setPointSize(20);
    timeFont.setFamily("Arial");
    timeFont.setBold(true);
    timeLabel->setFont(timeFont);
    timeLabel->adjustSize();

    // Create the date label
    dateLabel = new QLabel("Sun, Jan 01", topPanel);
    dateLabel->setStyleSheet("background-color: transparent; color: white;");
    dateLabel->setAlignment(Qt::AlignCenter);
    dateLabel->adjustSize();

    // Font for the date
    QFont dateFont = dateLabel->font();
    dateFont.setPointSize(20);
    dateFont.setFamily("Arial");
    dateLabel->setFont(dateFont);
    dateLabel->setAlignment(Qt::AlignLeft);
    dateLabel->adjustSize();
}

void HomeScreen::setUpAreaPanel()
{
    // Create the area panel widget
    areaPanel = new QWidget(centralWidget);
    areaPanel->setStyleSheet("background-color: transparent;");

    // Create a horizontal layout
    areaPanelLayout = new QHBoxLayout(areaPanel);

    // Create the area buttons (rooms/categories)
    allDevicesButton = new QPushButton("All Devices");
    pcButton = new QPushButton("PC");
    bedroomButton = new QPushButton("Bedroom");
    homeButton = new QPushButton("Home");

    // Font for the area buttons
    QFont areaButtonFont;
    areaButtonFont.setPointSize(12);
    areaButtonFont.setFamily("Arial");
    areaButtonFont.setBold(true);

    // Default style for area buttons
    QString areaButtonStyle = QString("background-color: transparent;"
                                  "color: white;"
                                  "border-radius: 30px;");

    // Style for selected button (adds a blue background)
    QString selectedAreaButtonStyle = QString("background-color: rgba(58,94,171,255);"
                                          "color: white;"
                                          "border-radius: 30px;");

    // Minimum size for the buttons
    QSize minimumButtonSize(160, 60);
    allDevicesButton->setMinimumSize(minimumButtonSize);
    pcButton->setMinimumSize(minimumButtonSize);
    bedroomButton->setMinimumSize(minimumButtonSize);
    homeButton->setMinimumSize(minimumButtonSize);

    // Apply all styles and fonts to the buttons
    allDevicesButton->setStyleSheet(selectedAreaButtonStyle);
    allDevicesButton->setFont(areaButtonFont);
    pcButton->setStyleSheet(areaButtonStyle);
    pcButton->setFont(areaButtonFont);
    bedroomButton->setStyleSheet(areaButtonStyle);
    bedroomButton->setFont(areaButtonFont);
    homeButton->setStyleSheet(areaButtonStyle);
    homeButton->setFont(areaButtonFont);

    // Connect the buttons to the click handler
    connect(allDevicesButton, &QPushButton::clicked, this, &HomeScreen::areaButtonClicked);
    connect(pcButton, &QPushButton::clicked, this, &HomeScreen::areaButtonClicked);
    connect(bedroomButton, &QPushButton::clicked, this, &HomeScreen::areaButtonClicked);
    connect(homeButton, &QPushButton::clicked, this, &HomeScreen::areaButtonClicked);

    // Add a small spacer item to the left of the buttons to keep away from window edge
    QSpacerItem *leftSpacer = new QSpacerItem(50, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
    areaPanelLayout->addItem(leftSpacer);

    // Add the buttons to the horizontal layout with a small spacing between each button
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

    // Reset the style of the previously selected button
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
    QLayoutItem *child;
    while ((child = itemPanelLayout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    // Reset the current item button when changing options (prevents crash)
    currentItemButton = nullptr;

    // Update the item panel based on the selected area button
    if (clickedAreaButton == allDevicesButton)
    {
        allDevicesButtons();
    }
    else if (clickedAreaButton == pcButton)
    {
        pcButtons();
    }
}

static QPixmap recolorIcon(const QPixmap &original)
{
    QImage img = original.toImage().convertToFormat(QImage::Format_ARGB32);
    QPixmap colored(img.size());
    colored.fill(Qt::transparent);

    QPainter p(&colored);
    p.drawImage(0, 0, img);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(img.rect(), Qt::white);
    p.end();

    return colored;
}

void HomeScreen::setUpWeatherPanel()
{
    weatherPanel = new QWidget(centralWidget);
    weatherPanel->setStyleSheet("background-color: rgba(27,33,52,200);"
                                "border-radius: 100;");

    QFont weatherTemperatureFont;
    weatherTemperatureFont.setPointSize(60);
    weatherTemperatureFont.setFamily("Arial");
    weatherTemperatureFont.setBold(true);

    QFont weatherApparentTemperatureFont;
    weatherApparentTemperatureFont.setPointSize(20);
    weatherApparentTemperatureFont.setFamily("Arial");

    QFont weatherWindFont;
    weatherWindFont.setPointSize(20);
    weatherWindFont.setFamily("Arial");

    weatherPanelLayout = new QVBoxLayout(weatherPanel);

    weatherIconLabel = new QLabel(weatherPanel);
    weatherIconLabel->setAlignment(Qt::AlignCenter);
    weatherIconLabel->setStyleSheet("background: transparent;");
    QPixmap defaultIcon("");
    weatherIconLabel->setPixmap(recolorIcon(defaultIcon).scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    weatherTemperatureLabel = new QLabel("N/A", weatherPanel);
    weatherTemperatureLabel->setFont(weatherTemperatureFont);
    weatherTemperatureLabel->setStyleSheet("background: transparent; color: white;");
    weatherTemperatureLabel->setAlignment(Qt::AlignCenter);

    weatherApparentTemperatureLabel = new QLabel("N/A", weatherPanel);
    weatherApparentTemperatureLabel->setFont(weatherApparentTemperatureFont);
    weatherApparentTemperatureLabel->setStyleSheet("background: transparent; color: white;");
    weatherApparentTemperatureLabel->setAlignment(Qt::AlignCenter);

    weatherWindLabel = new QLabel("N/A", weatherPanel);
    weatherWindLabel->setFont(weatherWindFont);
    weatherWindLabel->setStyleSheet("background: transparent; color: white;");
    weatherWindLabel->setAlignment(Qt::AlignCenter);

    weatherPanelLayout->addWidget(weatherIconLabel);
    weatherPanelLayout->addSpacing(30);
    weatherPanelLayout->addWidget(weatherTemperatureLabel);
    weatherPanelLayout->addSpacing(10);
    weatherPanelLayout->addWidget(weatherApparentTemperatureLabel);
    weatherPanelLayout->addSpacing(10);
    weatherPanelLayout->addWidget(weatherWindLabel);
    weatherPanelLayout->setContentsMargins(0, 0, 0, 0);
    weatherPanel->setLayout(weatherPanelLayout);
    weatherPanelLayout->setAlignment(Qt::AlignCenter);
}

void HomeScreen::updateWeatherPanel(const QString &condition, const QString &temperature, const QString &apparentTemperature, double windSpeedVal, double windDirVal)
{
    weatherTemperatureLabel->setText(temperature);
    weatherApparentTemperatureLabel->setText(apparentTemperature);

    QString iconPath = "";

    if (condition == "Clear-Day") {
        iconPath += "weather-clear-symbolic.symbolic.png";
    } else if (condition == "Clear-Night") {
        iconPath += "weather-clear-night-symbolic.symbolic.png";
    } else if (condition == "Overcast") {
        iconPath += "weather-overcast-symbolic.symbolic.png";
    } else if (condition == "Rain") {
        iconPath += "weather-showers-symbolic.symbolic.png";
    } else if (condition == "Few-Clouds-Day") {
        iconPath += "weather-few-clouds-symbolic.symbolic.png";
    } else if (condition == "Few-Clouds-Night") {
        iconPath += "weather-few-clouds-night-symbolic.symbolic.png";
    } else if (condition == "Snow") {
        iconPath += "weather-snow-symbolic.symbolic.png";
    } else if (condition == "Windy-Day" || condition == "Windy-Night") {
        iconPath += "weather-windy-symbolic.symbolic.png";
    } else {
        iconPath += "weather-severe-alert-symbolic.symbolic.png";
    }

    QPixmap icon(iconPath);
    if (!icon.isNull()) {
        QPixmap whiteIcon = recolorIcon(icon).scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        weatherIconLabel->setPixmap(whiteIcon);
    } else {
        qDebug() << "Icon not found at:" << iconPath;
    }

    double dir = fmod((windDirVal + 22.5), 360);
    int index = (int)(dir / 45.0);
    QStringList directions = {"N","NE","E","SE","S","SW","W","NW"};
    QString directionStr = directions.at(index % 8);

    QString windStr = "Wind: " + QString::number(windSpeedVal, 'f', 0) + " m/s" + directionStr;
    weatherWindLabel->setText(windStr);
}

void HomeScreen::setUpOptionPanel()
{
    // Create the option panel widget
    optionPanel = new QWidget(centralWidget);
    optionPanel->setStyleSheet("background-color: rgba(5,10,30,255);"
                               "border-top-left-radius: 60px;"
                               "border-top-right-radius: 60px;");

    // Create a vertical layout
    optionPanelLayout = new QVBoxLayout(optionPanel);
    optionPanel->setLayout(optionPanelLayout);

    // Install an event filter
    qApp->installEventFilter(this);

    // Hide the option panel until needed
    optionPanel->hide();
}

void HomeScreen::clearOptionPanelLayout()
{
    // Loop through all items in the option panel layout
    QLayoutItem *child;
    while ((child = optionPanelLayout->takeAt(0)) != nullptr)
    {
        // Remove widget from its parent and schedule for deletion
        if (child->widget())
        {
            child->widget()->setParent(nullptr);
            child->widget()->deleteLater();
        }
        // Delete the layout item itself
        delete child;
    }
}

void HomeScreen::setUpItemPanel()
{
    // Create the item panel widget
    itemPanel = new QWidget(centralWidget);
    itemPanel->setStyleSheet("background-color: transparent;");

    // Create a grid layout
    itemPanelLayout = new QGridLayout(itemPanel);
    itemPanelLayout->setSpacing(10);

    // Set up the "all devices" buttons
    allDevicesButtons();

    // Set the grid layout for the item panel
    itemPanel->setLayout(itemPanelLayout);
    itemPanelLayout->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
}

QPushButton* HomeScreen::setUpItemButton(const QString &text, const QSize &size, const QString &style, int row, int col, QGridLayout *layout)
{
    // Create a new button with the given text
    QPushButton *itemButton = new QPushButton(text);
    itemButton->setFixedSize(size);
    itemButton->setStyleSheet(style);

    // Font for item buttons
    QFont itemButtonFont;
    itemButtonFont.setPointSize(16);
    itemButtonFont.setFamily("Arial");
    itemButtonFont.setBold(true);
    itemButton->setFont(itemButtonFont);

    // Add the button to the given layout at the given position
    layout->addWidget(itemButton, row, col);
    return itemButton;
}

void HomeScreen::allDevicesButtons()
{
    // Create a widget to hold the secondary grid layout for the small buttons
    QWidget *smallItemButtonsWidget = new QWidget;

    // Define buttons sizes
    QSize smallItemButtonSize(170, 170);
    QSize largeItemButtonSize(350, 350);

    // Default style for item buttons
    QString itemButtonStyle = QString("background-color: rgba(27,33,52,200);"
                                  "color: white;"
                                  "border-radius: 5px;");

    // Style for selected button (adds a blue background)
    QString selectedItemButtonStyle = QString("background-color: rgba(58,94,171,255);"
                                          "color: white;"
                                          "border-radius: 5px;");

    // Create a secondary grid layout for the small buttons
    QGridLayout *smallItemButtonLayout = new QGridLayout;

    // Create the small buttons
    getUpButton = setUpItemButton("Get Up", smallItemButtonSize, itemButtonStyle, 0, 0, smallItemButtonLayout);
    leaveButton = setUpItemButton("Leave", smallItemButtonSize, itemButtonStyle, 0, 1, smallItemButtonLayout);
    atHomeButton = setUpItemButton("Home", smallItemButtonSize, selectedItemButtonStyle, 1, 0, smallItemButtonLayout);
    toSleepButton = setUpItemButton("Sleep", smallItemButtonSize, itemButtonStyle, 1, 1, smallItemButtonLayout);

    // Create the large buttons
    networkButton = setUpItemButton("Network", largeItemButtonSize, itemButtonStyle, 0, 1, itemPanelLayout);
    lightsButton = setUpItemButton("Lights", largeItemButtonSize, itemButtonStyle, 1, 0, itemPanelLayout);
    thermostatButton = setUpItemButton("Thermostat", largeItemButtonSize, itemButtonStyle, 1, 1, itemPanelLayout);

    // Connect the small buttons to their click handler
    connect(getUpButton, &QPushButton::clicked, this, &HomeScreen::statusButtonClicked);
    connect(leaveButton, &QPushButton::clicked, this, &HomeScreen::statusButtonClicked);
    connect(atHomeButton, &QPushButton::clicked, this, &HomeScreen::statusButtonClicked);
    connect(toSleepButton, &QPushButton::clicked, this, &HomeScreen::statusButtonClicked);

    // Connect the large buttons to their click handler
    connect(networkButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);
    connect(lightsButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);
    connect(thermostatButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);

    // Set up the for the small buttons
    smallItemButtonLayout->setSpacing(10);
    smallItemButtonLayout->setContentsMargins(0, 0, 0, 0);
    smallItemButtonsWidget->setLayout(smallItemButtonLayout);

    // Add the small buttons widget to the main item panel layout
    itemPanelLayout->addWidget(smallItemButtonsWidget, 0, 0);

    // Set the initial status button as "At Home"
    currentStatusButton = atHomeButton;
}

void HomeScreen::pcButtons()
{
    // Create a widget to hold the secondary grid layout for the small buttons
    QWidget *smallItemButtonsWidget = new QWidget;

    // Define buttons sizes
    QSize smallItemButtonSize(170, 170);
    QSize largeItemButtonSize(350, 350);

    // Default style for item buttons
    QString itemButtonStyle = QString("background-color: rgba(27,33,52,200);"
                                  "color: white;"
                                  "border-radius: 5px;");

    // Create a secondary grid layout for small buttons
    QGridLayout *smallItemButtonsLayout = new QGridLayout;

    // Create the small buttons
    shutDownButton = setUpItemButton("Shut Down", smallItemButtonSize, itemButtonStyle, 0, 0, smallItemButtonsLayout);
    restartButton = setUpItemButton("Restart", smallItemButtonSize, itemButtonStyle, 0, 1, smallItemButtonsLayout);
    sleepButton = setUpItemButton("Sleep", smallItemButtonSize, itemButtonStyle, 1, 0, smallItemButtonsLayout);
    lockButton = setUpItemButton("Lock", smallItemButtonSize, itemButtonStyle, 1, 1, smallItemButtonsLayout);

    // Connect the small buttons to their click handler
    connect(shutDownButton, &QPushButton::clicked, this, &HomeScreen::handleShutDown);
    connect(restartButton, &QPushButton::clicked, this, &HomeScreen::handleRestart);
    connect(sleepButton, &QPushButton::clicked, this, &HomeScreen::handleSleep);
    connect(lockButton, &QPushButton::clicked, this, &HomeScreen::handleLock);

    // Create the large buttons
    networkButton = setUpItemButton("WI-FI", largeItemButtonSize, itemButtonStyle, 0, 1, itemPanelLayout);
    lightsButton = setUpItemButton("LEDs", largeItemButtonSize, itemButtonStyle, 1, 0, itemPanelLayout);
    securityButton = setUpItemButton("Security", largeItemButtonSize, itemButtonStyle, 1, 1, itemPanelLayout);
    remoteButton = setUpItemButton("Remote", largeItemButtonSize, itemButtonStyle, 0, 2, itemPanelLayout);
    systemButton = setUpItemButton("System", largeItemButtonSize, itemButtonStyle, 1, 2, itemPanelLayout);

    // Connect the large buttons to their click handler
    connect(networkButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);
    connect(lightsButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);
    connect(securityButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);
    connect(remoteButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);
    connect(systemButton, &QPushButton::clicked, this, &HomeScreen::itemButtonClicked);

    // Set up the for the small buttons
    smallItemButtonsLayout->setSpacing(10);
    smallItemButtonsLayout->setContentsMargins(0, 0, 0, 0);
    smallItemButtonsWidget->setLayout(smallItemButtonsLayout);

    // Add the buttons to the grid layout
    itemPanelLayout->addWidget(smallItemButtonsWidget, 0, 0);
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

void HomeScreen::statusButtonClicked()
{
    // Get the button that was clicked
    QPushButton *clickedStatusButton = qobject_cast<QPushButton*>(sender());

    // Reset the style of the previously selected button
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
}

void HomeScreen::itemButtonClicked()
{
    // Get the button that was clicked
    QPushButton *clickedItemButton = qobject_cast<QPushButton*>(sender());
    if (!clickedItemButton) return;

    // Update button styles
    if (currentItemButton && currentItemButton != clickedItemButton)
    {
        currentItemButton->setStyleSheet("background-color: rgba(27,33,52,200); color: white; border-radius: 5px;");
    }
    clickedItemButton->setStyleSheet("background-color: rgba(58,94,171,255); color: white; border-radius: 5px;");
    currentItemButton = clickedItemButton;

    // Clear and prepare the option panel
    clearOptionPanelLayout();

    // Determine which control to show
    QWidget *controlWidget = nullptr;
    if (clickedItemButton == networkButton)
    {
        controlWidget = new NetworkControls(this);
        static_cast<NetworkControls*>(controlWidget)->setActive(true);
    }
    else if (clickedItemButton == securityButton)
    {
        controlWidget = new SecurityControls(this);
    }

    // If a control widget was created, show it in the option panel
    if (controlWidget)
    {
        optionPanelLayout->addWidget(controlWidget);

        // Position and animate the panel
        QRect startGeometry(optionPanel->x(), centralWidget->height(),
                            optionPanel->width(), optionPanel->height());
        QRect endGeometry(optionPanel->x(), centralWidget->height() - optionPanel->height(),
                          optionPanel->width(), optionPanel->height());

        optionPanel->setGeometry(startGeometry);
        optionPanel->show();
        animatePanel(endGeometry);
    }
    else
    {
        // If no control widget was created, hide the panel
        optionPanel->hide();

        // Deactivate any existing NetworkControls
        NetworkControls *existingNetworkControls = optionPanel->findChild<NetworkControls*>();
        if (existingNetworkControls)
        {
            existingNetworkControls->setActive(false);
        }
    }
}

bool HomeScreen::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this && event->type() == QEvent::MouseButtonPress)
    {
        // Swipe the option panel down if the user clicks outside of it
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (optionPanel->isVisible() && !optionPanel->geometry().contains(mouseEvent->pos()))
        {
            swipePanelDown();
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void HomeScreen::swipePanelDown()
{
    if (optionPanel->isVisible())
    {
        // Calculate the end position for ther panel animation (just off-screen)
        QRect endGeometry(optionPanel->x(), height(),
                          optionPanel->width(), optionPanel->height());
        // Start the option panel animation
        animatePanel(endGeometry);
    }

    // Deactivate any existing NetworkControls
    NetworkControls *existingNetworkControls = optionPanel->findChild<NetworkControls*>();
    if (existingNetworkControls)
    {
        existingNetworkControls->setActive(false);
    }
}

void HomeScreen::mousePressEvent(QMouseEvent *event)
{
    if (optionPanel->isVisible())
    {
        // Define the drag area
        QRect dragArea(optionPanel->x(), optionPanel->y(), optionPanel->width(), 80);

        // Start dragging if the mouse press is within the drag area
        if (dragArea.contains(event->pos()))
        {
            isDragging = true;
            dragStartPosition = event->pos();
        }
    }
    QMainWindow::mousePressEvent(event);
}

void HomeScreen::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging)
    {
        int deltaY = event->pos().y() - dragStartPosition.y();
        if (deltaY >= 0) // Only allow dragging downwards
        {
            // Move the option panel based on the drag distance
            QRect geometry = optionPanel->geometry();
            geometry.moveTop(qMin(height() - optionPanel->height() + deltaY, height()));
            optionPanel->setGeometry(geometry);
        }
    }
    QMainWindow::mouseMoveEvent(event);
}

void HomeScreen::mouseReleaseEvent(QMouseEvent *event)
{
    if (isDragging)
    {
        isDragging = false;
        int deltaY = event->pos().y() - dragStartPosition.y();

        QRect endGeometry;
        if (deltaY > dragThreshold)
        {
            // Close the panel if dragged far enough and released
            endGeometry = QRect(optionPanel->x(), height(),
                                optionPanel->width(), optionPanel->height());

            // Deactivate any existing NetworkControls
            NetworkControls *existingNetworkControls = optionPanel->findChild<NetworkControls*>();
            if (existingNetworkControls)
            {
                existingNetworkControls->setActive(false);
            }
        }
        else
        {
            // Return to original position if not dragged and released far enough
            endGeometry = QRect(optionPanel->x(), height() - optionPanel->height(),
                                optionPanel->width(), optionPanel->height());
        }

        animatePanel(endGeometry);
    }
    QMainWindow::mouseReleaseEvent(event);
}

void HomeScreen::animatePanel(const QRect &endValue)
{
    // Set up the option panel animation
    optionPanelAnimation->setStartValue(optionPanel->geometry());
    optionPanelAnimation->setEndValue(endValue);
    optionPanelAnimation->start();

    // If panel is closing, hide it when the animation finishes
    if (endValue.y() >= height())
    {
        connect(optionPanelAnimation, &QPropertyAnimation::finished, this, [this]() {
            optionPanel->hide();
        }, Qt::SingleShotConnection);
    }
}
