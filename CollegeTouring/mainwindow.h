#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <databaseviewform.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_Campus_Database_triggered();

private:
    Ui::MainWindow *ui;
    databaseViewForm* dbView;
};
#endif // MAINWINDOW_H
