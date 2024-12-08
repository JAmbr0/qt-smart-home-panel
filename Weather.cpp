#include "Weather.h"

#include <QStringList>
#include <QDebug>

// Constructor
Weather::Weather(QObject *parent)
    : QObject(parent),
    process(new QProcess(this))
{
    // Connect 'finished' signal to 'processFinished' slot
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processFinished(int, QProcess::ExitStatus)));

    // Connect error signal to read and log all error
    connect(process, &QProcess::readyReadStandardError, this, [this]()
    {
        QByteArray errData = process->readAllStandardError();
        if (!errData.isEmpty())
        {
            qDebug() << "Weather script stderr:" <<errData;
        }
    });
}

// Destructor
Weather::~Weather()
{
    if (process)
    {
        process->deleteLater();
    }
}

void Weather::updateWeatherData()
{
    // Virtual environment path
    QString pythonPath = "";

    // API script path
    QString scriptPath = "";

    // Start process
    process->start(pythonPath, QStringList() << scriptPath);
}

void Weather::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Process finished with exit code:" << exitCode << "and exit status:" << exitStatus;

    QString output = process->readAllStandardOutput();
    parseOutput(output);

    emit weatherDataUpdated();
}

// Split output into lines and process each line
void Weather::parseOutput(const QString &output)
{
    // Clear previous weather data
    temperature.clear();
    apparentTemperature.clear();
    precipitation.clear();
    cloudCover.clear();
    isDay.clear();
    windSpeed.clear();
    windDirection.clear();
    snowfall.clear();

    // Split into lines, ignoring empty lines
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    // Iterate though each line
    for (const QString &line : lines)
    {
        if (line.startsWith("Current temperature_2m")) {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) temperature = parts.last().trimmed();
        }
        else if (line.startsWith("Current apparent_temperature"))
        {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) apparentTemperature = parts.last().trimmed();
        }
        else if (line.startsWith("Current precipitation"))
        {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) precipitation = parts.last().trimmed();
        }
        else if (line.startsWith("Current cloud_cover"))
        {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) cloudCover = parts.last().trimmed();
        }
        else if (line.startsWith("Current is_day"))
        {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) isDay = parts.last().trimmed();
        }
        else if (line.startsWith("Current wind_speed_10m"))
        {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) windSpeed = parts.last().trimmed();
        }
        else if (line.startsWith("Current wind_direction_10m"))
        {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) windDirection = parts.last().trimmed();
        }
        else if (line.startsWith("Current snowfall"))
        {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) snowfall = parts.last().trimmed();
        }
    }
    if (temperature.isEmpty()) qDebug() << "No temperature found in output.";
    if (apparentTemperature.isEmpty()) qDebug() << "No apparent temperature found in output.";
    if (precipitation.isEmpty()) qDebug() << "No precipitation found in output.";
    if (cloudCover.isEmpty()) qDebug() << "No cloud cover found in output.";
    if (isDay.isEmpty()) qDebug() << "No is_day found in output.";
    if (windSpeed.isEmpty()) qDebug() << "No wind speed found in output.";
    if (windDirection.isEmpty()) qDebug() << "No wind direction found in output.";
    if (snowfall.isEmpty()) qDebug() << "No snowfall found in output.";
}

QString Weather::getTemperature() const { return temperature; }
QString Weather::getApparentTemperature() const { return apparentTemperature; }
QString Weather::getPrecipitation() const { return precipitation; }
QString Weather::getCloudCover() const { return cloudCover; }
QString Weather::getIsDay() const { return isDay; }
QString Weather::getWindSpeed() const { return windSpeed; }
QString Weather::getWindDirection() const { return windDirection; }
QString Weather::getSnowfall() const { return snowfall; }
