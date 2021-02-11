#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include "downloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    const QString requestTemplate = "https://yandex.ru/images/search?text=";
    QString request = "кот";
    QFile htmlFile;

    Downloader *loader;

private slots:
    void slotDone(const QUrl& url, const QByteArray&);
};

#endif // MAINWINDOW_H
