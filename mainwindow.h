#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qdatetime.h"
#include "qdir.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public: 
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void countTifFiles(const QDir &dir, const QDate &startDate, const QDate &endDate, int &tifCount);

private slots:
    void on_toolButton_clicked();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString selectedDirectory;
    QDate startDate;
    QDate endDate;
};
#endif // MAINWINDOW_H
