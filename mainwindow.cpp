#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::countTifFiles(const QDir &dir, const QDate &startDate, const QDate &endDate, int &tifCount)
{
    // Procházej všechny soubory v aktuálním adresáři
    QStringList files = dir.entryList(QStringList("*.tif"), QDir::Files);
    foreach (const QString &file, files) {
        QFileInfo fileInfo(dir.absoluteFilePath(file));
        QDate fileDate = fileInfo.lastModified().date();
        if (fileDate >= startDate && fileDate <= endDate) {
            tifCount++;
        }
    }

    // Procházej rekurzivně všechny podadresáře
    QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &subdir, dirs) {
        QDir subDir = dir;
        subDir.cd(subdir);
        countTifFiles(subDir, startDate, endDate, tifCount);
    }
}

void MainWindow::on_toolButton_clicked()
{
    selectedDirectory = QFileDialog::getExistingDirectory(this, tr("Vybrat adresář"), QDir::homePath());
    ui->label_3->setText(selectedDirectory);
}


void MainWindow::on_pushButton_clicked()
{
    if (selectedDirectory.isEmpty()) {
        QMessageBox::warning(this, tr("Chybějící adresář"), tr("Nebyl vybrán žádný adresář."));
        return;
    }

    startDate = ui->dateEdit_start->date();
    endDate = ui->dateEdit_end->date();

    if (startDate > endDate) {
        QMessageBox::warning(this, tr("Neplatné datum"), tr("Datum konce je před datem začátku."));
        return;
    }

    QDir directory(selectedDirectory);
    int tifCount = 0;
    countTifFiles(directory, startDate, endDate, tifCount); // Zde zavolej metodu countTifFiles

    QMessageBox::information(this, tr("Počet souborů"), tr("Počet souborů s koncovkou '.tif' v zadaném období: %1").arg(tifCount));
}
