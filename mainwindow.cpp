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

    connect(loader, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(slotDownloadProgress(qint64, qint64)));
    connect(this, SIGNAL(reassignment()), this, SLOT(slotReassignment()));
    connect(this, SIGNAL(assignment()), this, SLOT(slotAssignment()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotDone(const QUrl &url, const QByteArray &ba)    //скачивание html-разметки
{
    Q_UNUSED(url);
    if(htmlFile.open())
    {
        htmlFile.write(ba);
        htmlFile.close();
    }
    ui->pbar->setValue(0); //сброс шкалы загрузки
}

void MainWindow::slotNewPicture(const QUrl &url, const QByteArray &ba)  //скачивание картинки
{
    Q_UNUSED(url);
    static int n = 0;
    switch(n){
    case 0:
        if(pictFile[n].open())
        {
            pictFile[n].write(ba);
            pictFile[n].close();
        }
        showPic(pictFile[n].fileName(), ui->label1);
        break;
    case 1:
        if(pictFile[n].open())
        {
            pictFile[n].write(ba);
            pictFile[n].close();
        }
        showPic(pictFile[n].fileName(), ui->label2);
        break;
    case 2:
        if(pictFile[n].open())
        {
            pictFile[n].write(ba);
            pictFile[n].close();
        }
        showPic(pictFile[n].fileName(), ui->label3);
        break;
    default: n = -1; break;
    }
    n++;
    ui->pbar->setValue(0); //сброс шкалы загрузки
}

void MainWindow::on_pushButton_clicked()    //разбор файла с запросом, выделение адресов картинок и их вывод
{
    if(htmlFile.fileName().isEmpty())
    {
        ui->errorLabel->setText("Введите имя запроса");
        return;
    }
    emit reassignment();
    QString temp;
    QTextStream stream(&htmlFile);
    QStringList list;
    if(htmlFile.open())
    {
        temp = stream.readAll();
        htmlFile.close();

        QRegExp regex("<img class=\"[^\"]+\" alt=\"[^\"]*\"[^\"]+\"[^\"]+\" src=\"([^\"]+);n");
        int lastPos = 0;
        int index = 0;
        while( ( lastPos = regex.indexIn( temp, lastPos ) ) != -1 && index < 3) {
            lastPos += regex.matchedLength();
            list.append(regex.cap(1));
            index++;
        }
        QString str1 = "http:" + list[0];
        loader->download(str1);
        str1 = "http:" + list[1];
        loader->download(str1);
        str1 = "http:" + list[2];
        loader->download(str1);
    }
}

void MainWindow::slotDownloadProgress(qint64 received, qint64 total)
{
    if (total <=0){
        //slotError();
        return;
    }

    ui->pbar->setValue(100 * received / total);
}

void MainWindow::slotError()
{
    QMessageBox::critical(this, "Error", "An error while download is occured");
}

void MainWindow::slotReassignment() //новое соединение сигналов и слотов (для загрузки картинок)
{
    disconnect(loader, SIGNAL(done(const QUrl&, const QByteArray&)),
               this, SLOT(slotDone(const QUrl&, const QByteArray&)));

    connect(loader, SIGNAL(done(const QUrl&, const QByteArray&)),
            this, SLOT(slotNewPicture(const QUrl&, const QByteArray&)));
}

void MainWindow::slotAssignment()   //новое соединение сигналов и слотов (для загрузки html-разметки запроса)
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
    if(ui->requestName->text().isEmpty())
    {
        ui->errorLabel->setText("Введите имя запроса");
        return;
    }
    else
    {
       ui->errorLabel->setText("Имя запроса установлено");;    //имя установлено
    }
    emit assignment();
    request = ui->requestName->text();

    QString temp = requestTemplate + request;

    loader->download(temp);
}
