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
    QString temp, temp1;
    QTextStream stream(&htmlFile);
    QStringList list;
    if(htmlFile.open(QIODevice::ReadOnly))
    {
        temp = stream.readAll();
        htmlFile.close();
        // Проверить регулярку можно на сайте: https://regex101.com
        QRegExp regex("<img.*src=\"([^\"]+)\""); //regex("<img([^(src)]+)src=\"([^(\")]+)\"")
        int lastPos = 0;
        while( ( lastPos = regex.indexIn( temp, lastPos ) ) != -1 ) {
            lastPos += regex.matchedLength();
            list.append(regex.cap(1));
        }
//       temp1 = getString(temp,"<img class=", ">");
//       list.append(temp1);
//       int x = temp.indexOf(temp1);

//       temp1 = getString(temp1,"<img class=", ">", x);
//       list.append(temp1);
//       x = temp.indexOf(temp1);

//       temp1 = getString(temp1,"<img class=", ">", x);
//       list.append(temp1);
//       x = temp.indexOf(temp1);

//       ui->label1->setText(list[0]);
        temp = "//im0-tub-ru.yandex.net/i?id=420ae0d7f3620a053d7030b8c23f9c78&amp;n=13";
        loader->download(temp);
        QPixmap pict("htmlFile.txt");
        pict = pict.scaled(500, 400, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->label1->setPixmap(pict);
        ui->label1->setFixedSize(pict.size());
        ui->label1->setPixmap(pict);

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

