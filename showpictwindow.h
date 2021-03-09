#ifndef SHOWPICTWINDOW_H
#define SHOWPICTWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class showPictWindow;
}

class showPictWindow : public QMainWindow   //класс для отображения картинок
{
    Q_OBJECT

public:
    explicit showPictWindow(QWidget *parent = nullptr);
    ~showPictWindow();

    /*Получение указателей на labels для вывода картинок*/
    QLabel* ptrLabel1();
    QLabel* ptrLabel2();
    QLabel* ptrLabel3();

private:
    Ui::showPictWindow *ui;
};

#endif // SHOWPICTWINDOW_H
