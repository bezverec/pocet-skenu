#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qdatetime.h"
#include "qdir.h"
#include <QMainWindow>
#include <QCheckBox>
#include <QPlainTextEdit>

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
    void countFiles(const QDir &dir, const QDate &startDate, const QDate &endDate, int &tifCount, int &txtCount, int &xmlCount, int &jp2Count, int &md5Count);  // Přidání parametru pro MD5 soubory
    void countMatchingDirs(const QDir &dir, const QString &prefix, int &dirCount);  // Metoda pro počítání adresářů podle prefixu
    void generateLog(int tifCount, int txtCount, int xmlCount, int jp2Count, int md5Count, int dirCount);  // Metoda pro generování logu
    QStringList getFilesOfType(const QDir &dir, const QString &fileType);  // Metoda pro získání souborů podle přípony
    QStringList getDirectoriesOfPrefix(const QDir &dir, const QString &prefix);  // Metoda pro získání adresářů podle prefixu

private slots:
    void on_toolButton_clicked();
    void on_pushButton_clicked();
    void on_checkBox_countTxtFiles_stateChanged(int state);  // Slot pro .txt soubory
    void on_checkBox_countXmlFiles_stateChanged(int state);  // Slot pro .xml soubory
    void on_checkBox_countJp2Files_stateChanged(int state);  // Slot pro .jp2 soubory
    void on_checkBox_countMd5Files_stateChanged(int state);  // Slot pro .md5 soubory
    void on_checkBox_countDirs_stateChanged(int state);  // Slot pro .dirs soubory
    void on_checkBox_generateLog_stateChanged(int state);  // Slot pro generování logu

private:
    Ui::MainWindow *ui;
    QString selectedDirectory;
    QDate startDate;
    QDate endDate;
};

#endif // MAINWINDOW_H
