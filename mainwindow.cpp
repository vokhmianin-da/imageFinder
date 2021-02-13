#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QRegExp>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loader = new Downloader;
    QString temp = requestTemplate + request;
    connect(loader, SIGNAL(done(const QUrl&, const QByteArray&)),
            this, SLOT(slotDone(const QUrl&, const QByteArray&)));
    loader->download(temp);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotDone(const QUrl &url, const QByteArray &ba)
{

    htmlFile.setFileName("htmlFile.txt");
    if(htmlFile.open(QIODevice::WriteOnly))
    {
        htmlFile.write(ba);
        htmlFile.close();
    }
}

void MainWindow::on_pushButton_clicked()    //вывод картинок
{
    QString temp;
    QTextStream stream(&htmlFile);
    QStringList list;
    if(htmlFile.open(QIODevice::ReadOnly))
    {
        temp = stream.readAll();
        htmlFile.close();
        // Проверить регулярку можно на сайте: https://regex101.com

//        <img class="related__thumb" alt="" data-error-handler="wizardThumbError:2" src="//im0-tub-ru.yandex.net/i?id=735d3bcb5413fde4f60e0dcd70b3fd2c&amp;n=11&amp;ref=rq">
//        QRegExp regex("(<img.*?src=\")([^\"]+)(\")"); //regex("<img([^(src)]+)src=\"([^(\")]+)\"")
        QRegExp regex("<img class=\"[^\"]+\" alt=\"[^\"]*\"[^\"]+\"[^\"]+\" src=\"([^\"]+)\"");
        int lastPos = 0;
        while( ( lastPos = regex.indexIn( temp, lastPos ) ) != -1 ) {
            lastPos += regex.matchedLength();
            list.append(regex.cap(1));
        }

    }
}

QString MainWindow::getString(QString& originalStr,const QString& frontStr, const QString& endStr, int begin)
{
    int indFrontStr;
    int indEndStr;
    QString temp;

    indFrontStr = originalStr.indexOf(frontStr, begin);  //начало искомой строки
    indEndStr = originalStr.indexOf(endStr, indFrontStr);   //конец искомой строки
    temp = originalStr.mid(indFrontStr + frontStr.size(), indEndStr - (indFrontStr + frontStr.size()));
    indFrontStr = temp.indexOf("src=\"");  //начало искомой строки
    indEndStr = temp.lastIndexOf("\"", indFrontStr);   //конец искомой строки
    temp = temp.mid(indFrontStr + frontStr.size(), indEndStr - (indFrontStr + frontStr.size()));
    return temp;

}

