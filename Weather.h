#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>
#include <QString>
#include <QProcess>

class Weather : public QObject
{
    Q_OBJECT

public:
    explicit Weather(QObject *parent = nullptr);
    ~Weather();

    // Initiate weather data update
    void updateWeatherData();

    // Getters for parsed weather data
    QString getTemperature() const;
    QString getApparentTemperature() const;
    QString getPrecipitation() const;
    QString getCloudCover() const;
    QString getIsDay() const;
    QString getWindSpeed() const;
    QString getWindDirection() const;
    QString getSnowfall() const;

signals:
    // Emit signal when weather data is successfully updated
    void weatherDataUpdated();

private slots:
    // Handle QProcess finishing
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    // Parse output from weather API
    void parseOutput(const QString &output);

    QString temperature;
    QString apparentTemperature;
    QString precipitation;
    QString cloudCover;
    QString isDay;
    QString windSpeed;
    QString windDirection;
    QString snowfall;

    QProcess *process;
};

#endif // WEATHER_H
