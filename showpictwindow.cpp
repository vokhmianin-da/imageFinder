#include "showpictwindow.h"
#include "ui_showpictwindow.h"

showPictWindow::showPictWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::showPictWindow)
{
    ui->setupUi(this);
}

showPictWindow::~showPictWindow()
{
    delete ui;
}

QLabel *showPictWindow::ptrLabel1()
{
    return ui->label1;
}

QLabel *showPictWindow::ptrLabel2()
{
    return ui->label2;
}

QLabel *showPictWindow::ptrLabel3()
{
    return ui->label3;
}
