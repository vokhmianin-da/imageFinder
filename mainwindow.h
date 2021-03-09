#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTemporaryFile>
#include <QLabel>
#include "downloader.h"
#include "showpictwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    const QString requestTemplate = "https://yandex.ru/images/search?text=";
    QString request;
    QTemporaryFile htmlFile;
    QTemporaryFile pictFile[3];

    showPictWindow *viewer; //окно для отображения картинок

    Downloader *loader;
signals:
    void reassignment();
    void assignment();

private slots:
    void slotDone(const QUrl& url, const QByteArray&);    
    void on_pushButton_clicked();
    void slotReassignment();
    void slotAssignment();
    void slotNewPicture(const QUrl& url, const QByteArray&);
    void slotDownloadProgress(qint64, qint64);
    void slotError();

    void on_pushButtonSearch_clicked();

public:
    explicit MainWindow(QWidget *parent = 0);
    void showPic(const QString &path, QLabel* label);
    ~MainWindow();
};

#endif // MAINWINDOW_H
