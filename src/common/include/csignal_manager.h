#ifndef CSIGNALMANAGER_H
#define CSIGNALMANAGER_H

#include <QObject>

class CSignalManager : public QObject
{
    Q_OBJECT

public:
    static CSignalManager *GetCSignalManager()
    {
        static CSignalManager signal_manager_;
        return &signal_manager_;
    }

private:
    explicit CSignalManager() {}
    ~CSignalManager() {}

    CSignalManager(const CSignalManager &) = delete;
    CSignalManager &operator=(const CSignalManager &) = delete;

signals:
    void SigLoadFinished();
    void SigParseFinished(const QString &type, double timestamp);
    void SigSliderMoved(int timestamp);
    void SigUpdateDataBufSize();
    void SigSyncData(double timestamp);
};

#endif // CSIGNALMANAGER_H
