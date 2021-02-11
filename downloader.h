#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);
    void download(const QUrl &url);

signals:
    void downloadProgress(qint64, qint64);
    void done(const QUrl& url, const QByteArray&);
    void error();

private slots:
    void slotFinished(QNetworkReply*);

private:
    QNetworkAccessManager *pnam;

};

