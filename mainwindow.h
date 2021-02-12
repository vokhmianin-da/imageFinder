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
    QString getString(QString& originalStr,const QString& frontStr, const QString& endStr, int begin = 0);

private slots:
    void slotDone(const QUrl& url, const QByteArray&);
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
