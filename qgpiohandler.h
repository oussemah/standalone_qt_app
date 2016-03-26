#ifndef QGPIOHANDLER_H
#define QGPIOHANDLER_H

#include <QObject>
#include <QFileSystemWatcher>

class QGPIOHandler : public QObject
{
    Q_OBJECT

public:
    static const int DIRECTION_OUTPUT = 0;
    static const int DIRECTION_INPUT  = 1;

    static const int VALUE_HIGH = 1;
    static const int VALUE_LOW  = 0;

    explicit QGPIOHandler(QObject *parent = 0);
    ~QGPIOHandler(void);

    int setPinDirection(const int pin, const int direction);
    int setPinValue(const int pin, const int value);
    int readPinValue(const int pin);

    int reservePin(const int pin);
    int freePin(const int pin);

signals:
    void newPinValueReady(int pin);

public slots:
   void pinValueUpdated(const QString &path);

private:
    QFileSystemWatcher *m_watcher;
};

#endif // QGPIOHANDLER_H
