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
    connect(this, SIGNAL(reassignment()), this, SLOT(slotReassignment()));
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

void MainWindow::slotNewPicture(const QUrl &url, const QByteArray &ba)
{
    htmlFile.setFileName("pict.jpg");
    if(htmlFile.open(QIODevice::WriteOnly))
    {
        htmlFile.write(ba);
        htmlFile.close();
    }
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
        QRegExp regex("<img class=\"[^\"]+\" alt=\"[^\"]*\"[^\"]+\"[^\"]+\" src=\"([^\"]+)\"");
        int lastPos = 0;
        int index = 0;
        while( ( lastPos = regex.indexIn( temp, lastPos ) ) != -1 && index < 3) {
            lastPos += regex.matchedLength();
            list.append(regex.cap(1));
            index++;
        }        
            loader->download(list[0]);
            showPic(htmlFile.fileName(), ui->label1);
            loader->download(list[1]);
            showPic(htmlFile.fileName(), ui->label2);
            loader->download(list[2]);
            showPic(htmlFile.fileName(), ui->label3);
    }
}

void MainWindow::slotReassignment() //новое соединение сигналов и слотов
{
    disconnect(loader, SIGNAL(done(const QUrl&, const QByteArray&)),
            this, SLOT(slotDone(const QUrl&, const QByteArray&)));

    connect(loader, SIGNAL(done(const QUrl&, const QByteArray&)),
            this, SLOT(slotNewPicture(const QUrl&, const QByteArray&)));
}

void MainWindow::showPic(const QString &path, QLabel* label)
{
    QPixmap pix(path);
    pix = pix.scaled(500, 400, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    label->setPixmap(pix);
    label->setFixedSize(pix.size());
    label->show();
}
