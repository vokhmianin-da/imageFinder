#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QLabel>
#include "downloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    const QString requestTemplate = "https://yandex.ru/images/search?text=";
    QString request = "кот";
    QFile htmlFile;
    QFile pictFile;

    Downloader *loader;
signals:
    void reassignment();

private slots:
    void slotDone(const QUrl& url, const QByteArray&);    
    void on_pushButton_clicked();
    void slotReassignment();
    void slotNewPicture(const QUrl& url, const QByteArray&);

public:
    explicit MainWindow(QWidget *parent = 0);
    void showPic(const QString &path, QLabel* label);
    ~MainWindow();
};

#endif // MAINWINDOW_H
