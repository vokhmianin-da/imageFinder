#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QRegExp>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loader = new Downloader;

    connect(loader, SIGNAL(done(const QUrl&, const QByteArray&)),
            this, SLOT(slotDone(const QUrl&, const QByteArray&)));
    connect(loader, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(slotDownloadProgress(qint64, qint64)));
    connect(this, SIGNAL(reassignment()), this, SLOT(slotReassignment()));
    connect(this, SIGNAL(assignment()), this, SLOT(slotAssignment()));

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

void MainWindow::slotNewPicture(const QUrl &url, const QByteArray &ba)
{
    static int n = 0;
    pictFile.setFileName(url.path().section('/', -1));
    if(pictFile.open(QIODevice::WriteOnly))
    {
        pictFile.write(ba);
        pictFile.close();
    }
    switch(n){
    case 0: showPic(pictFile.fileName(), ui->label1); break;
    case 1: showPic(pictFile.fileName(), ui->label2); break;
    case 2: showPic(pictFile.fileName(), ui->label3); break;
    default: n = -1; break;
    }
        n++;
}

void MainWindow::on_pushButton_clicked()    //вывод картинок
{
    emit reassignment();
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
        QRegExp regex("<img class=\"[^\"]+\" alt=\"[^\"]*\"[^\"]+\"[^\"]+\" src=\"([^\"]+);n");
        int lastPos = 0;
        int index = 0;
        while( ( lastPos = regex.indexIn( temp, lastPos ) ) != -1 && index < 3) {
            lastPos += regex.matchedLength();
            list.append(regex.cap(1));
            index++;
        }
        QString str1 = "http:" + list[0];
        //pictFile.setFileName("pict1.jpg");
            loader->download(str1);
           // showPic(pictFile.fileName(), ui->label1);
            str1 = "http:" + list[1];
         //   pictFile.setFileName("pict2.jpg");
            loader->download(str1);
           // showPic(pictFile.fileName(), ui->label2);
            str1 = "http:" + list[2];
          //  pictFile.setFileName("pict3.jpg");
            loader->download(str1);
           // showPic(pictFile.fileName(), ui->label3);
    }
}

void MainWindow::slotDownloadProgress(qint64 received, qint64 total)
{
    //Странная хрень творится с размером
    if (total <=0){
        slotError();
        return;
    }

    ui->pbar->setValue(100 * received / total);
}

void MainWindow::slotError()
{
    QMessageBox::critical(this, "Error", "An error while download is occured");
}

void MainWindow::slotReassignment() //новое соединение сигналов и слотов
{
    disconnect(loader, SIGNAL(done(const QUrl&, const QByteArray&)),
            this, SLOT(slotDone(const QUrl&, const QByteArray&)));

    connect(loader, SIGNAL(done(const QUrl&, const QByteArray&)),
            this, SLOT(slotNewPicture(const QUrl&, const QByteArray&)));
}

void MainWindow::slotAssignment()
{
    disconnect(loader, SIGNAL(done(const QUrl&, const QByteArray&)),
            this, SLOT(slotNewPicture(const QUrl&, const QByteArray&)));
    connect(loader, SIGNAL(done(const QUrl&, const QByteArray&)),
            this, SLOT(slotDone(const QUrl&, const QByteArray&)));
}

void MainWindow::showPic(const QString &path, QLabel* label)
{
    QPixmap pix(path);
    //pix = pix.scaled(500, 400, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    label->setPixmap(pix);
    label->setFixedSize(pix.size());
    label->show();
}

void MainWindow::on_pushButtonSearch_clicked()  //установка имени запроса
{
    emit assignment();
    request = ui->requestName->text();

    QString temp = requestTemplate + request;

    loader->download(temp);
}
