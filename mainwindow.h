#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <string>
#include <about.h>
#include <csvreader.h>
#include <bibliometrics.h>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QString outputfolder;
    QStringList inputfiles;
};

#endif // MAINWINDOW_H
