#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Připojíme signál pro checkboxy (pro .txt, .xml, .jp2 a .md5 soubory a generování logu)
    connect(ui->checkBox_countTxtFiles, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_countTxtFiles_stateChanged);
    connect(ui->checkBox_countXmlFiles, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_countXmlFiles_stateChanged);
    connect(ui->checkBox_countJp2Files, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_countJp2Files_stateChanged);
    connect(ui->checkBox_countMd5Files, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_countMd5Files_stateChanged);
    connect(ui->checkBox_countDirs, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_countDirs_stateChanged);
    connect(ui->checkBox_generateLog, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_generateLog_stateChanged);  // Připojení signálu pro generování logu
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::countFiles(const QDir &dir, const QDate &startDate, const QDate &endDate, int &tifCount, int &txtCount, int &xmlCount, int &jp2Count, int &md5Count)
{
    // Procházej všechny soubory v aktuálním adresáři pro .tif
    QStringList tifFiles = dir.entryList(QStringList("*.tif"), QDir::Files);
    foreach (const QString &file, tifFiles) {
        QFileInfo fileInfo(dir.absoluteFilePath(file));
        QDate fileDate = fileInfo.lastModified().date();
        if (fileDate >= startDate && fileDate <= endDate) {
            tifCount++;
        }
    }

    // Procházej všechny soubory v aktuálním adresáři pro .txt, pokud je checkbox zaškrtnutý
    if (ui->checkBox_countTxtFiles->isChecked()) {
        QStringList txtFiles = dir.entryList(QStringList("*.txt"), QDir::Files);
        foreach (const QString &file, txtFiles) {
            QFileInfo fileInfo(dir.absoluteFilePath(file));
            QDate fileDate = fileInfo.lastModified().date();
            if (fileDate >= startDate && fileDate <= endDate) {
                txtCount++;
            }
        }
    }

    // Procházej všechny soubory v aktuálním adresáři pro .xml, pokud je checkbox zaškrtnutý
    if (ui->checkBox_countXmlFiles->isChecked()) {
        QStringList xmlFiles = dir.entryList(QStringList("*.xml"), QDir::Files);
        foreach (const QString &file, xmlFiles) {
            QFileInfo fileInfo(dir.absoluteFilePath(file));
            QDate fileDate = fileInfo.lastModified().date();
            if (fileDate >= startDate && fileDate <= endDate) {
                xmlCount++;
            }
        }
    }

    // Procházej všechny soubory v aktuálním adresáři pro .jp2, pokud je checkbox zaškrtnutý
    if (ui->checkBox_countJp2Files->isChecked()) {
        QStringList jp2Files = dir.entryList(QStringList("*.jp2"), QDir::Files);
        foreach (const QString &file, jp2Files) {
            QFileInfo fileInfo(dir.absoluteFilePath(file));
            QDate fileDate = fileInfo.lastModified().date();
            if (fileDate >= startDate && fileDate <= endDate) {
                jp2Count++;
            }
        }
    }

    // Procházej všechny soubory v aktuálním adresáři pro .md5, pokud je checkbox zaškrtnutý
    if (ui->checkBox_countMd5Files->isChecked()) {
        QStringList md5Files = dir.entryList(QStringList("*.md5"), QDir::Files);
        foreach (const QString &file, md5Files) {
            QFileInfo fileInfo(dir.absoluteFilePath(file));
            QDate fileDate = fileInfo.lastModified().date();
            if (fileDate >= startDate && fileDate <= endDate) {
                md5Count++;
            }
        }
    }

    // Rekurzivně procházej podadresáře
    QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &subdir, dirs) {
        QDir subDir = dir;
        subDir.cd(subdir);
        countFiles(subDir, startDate, endDate, tifCount, txtCount, xmlCount, jp2Count, md5Count);
    }
}

void MainWindow::countMatchingDirs(const QDir &dir, const QString &prefix, int &dirCount)
{
    // Procházej všechny podadresáře
    QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &subdir, dirs) {
        if (subdir.startsWith(prefix, Qt::CaseInsensitive)) {
            dirCount++;
        }
    }

    // Rekurzivně procházej podadresáře
    foreach (const QString &subdir, dirs) {
        QDir subDir = dir;
        subDir.cd(subdir);
        countMatchingDirs(subDir, prefix, dirCount);
    }
}

QStringList MainWindow::getFilesOfType(const QDir &dir, const QString &fileType)
{
    QStringList files;

    // Získání souborů v aktuálním adresáři
    files += dir.entryList(QStringList(fileType), QDir::Files);

    // Rekurzivní prohledávání podadresářů
    QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &subDir, subDirs) {
        QDir subDirectory(dir.absoluteFilePath(subDir));
        files += getFilesOfType(subDirectory, fileType);  // Rekurzivní volání pro podadresáře
    }

    return files;
}

QStringList MainWindow::getDirectoriesOfPrefix(const QDir &dir, const QString &prefix)
{
    QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList matchingDirs;
    foreach (const QString &subdir, dirs) {
        if (subdir.startsWith(prefix, Qt::CaseInsensitive)) {
            matchingDirs.append(dir.absoluteFilePath(subdir));
        }
    }
    return matchingDirs;
}

void MainWindow::on_toolButton_clicked()
{
    selectedDirectory = QFileDialog::getExistingDirectory(this, tr("Vybrat adresář"), QDir::homePath());
    ui->label_3->setText(selectedDirectory);
}

void MainWindow::on_pushButton_clicked()
{
    if (selectedDirectory.isEmpty()) {
        QMessageBox::warning(this, tr("Chyba"), tr("Nebyl vybrán žádný adresář."));
        return;
    }

    startDate = ui->dateEdit_start->date();
    endDate = ui->dateEdit_end->date();

    if (startDate > endDate) {
        QMessageBox::warning(this, tr("Chyba"), tr("Datum konce je před datem začátku."));
        return;
    }

    // Načti text z QPlainTextEdit pro začátek názvu adresáře
    QString prefix = ui->plainTextEdit_prefix->toPlainText().trimmed();

    // Načti adresář a počítání souborů
    QDir directory(selectedDirectory);
    int tifCount = 0;
    int txtCount = 0;
    int xmlCount = 0;
    int jp2Count = 0;
    int md5Count = 0;
    countFiles(directory, startDate, endDate, tifCount, txtCount, xmlCount, jp2Count, md5Count);

    // Počítání adresářů, pokud je checkbox zaškrtnutý
    int dirCount = 0;
    if (ui->checkBox_countDirs->isChecked()) {
        countMatchingDirs(directory, prefix, dirCount);
    }

    QString message = tr("Počet souborů s koncovkou '.tif' v zadaném období: %1").arg(tifCount);
    if (ui->checkBox_countTxtFiles->isChecked()) {
        message += tr("\nPočet souborů s koncovkou '.txt' v zadaném období: %1").arg(txtCount);
    }
    if (ui->checkBox_countXmlFiles->isChecked()) {
        message += tr("\nPočet souborů s koncovkou '.xml' v zadaném období: %1").arg(xmlCount);
    }
    if (ui->checkBox_countJp2Files->isChecked()) {
        message += tr("\nPočet souborů s koncovkou '.jp2' v zadaném období: %1").arg(jp2Count);
    }
    if (ui->checkBox_countMd5Files->isChecked()) {
        message += tr("\nPočet souborů s koncovkou '.md5' v zadaném období: %1").arg(md5Count);
    }
    if (ui->checkBox_countDirs->isChecked()) {
        message += tr("\nPočet adresářů začínajících '%1': %2").arg(prefix).arg(dirCount);
    }

    QMessageBox::information(this, tr("Počet souborů a adresářů"), message);

    // Pokud je zaškrtnutý checkbox pro generování logu, vygenerujeme log soubor
    if (ui->checkBox_generateLog->isChecked()) {
        generateLog(tifCount, txtCount, xmlCount, jp2Count, md5Count, dirCount);
    }
}

void MainWindow::generateLog(int tifCount, int txtCount, int xmlCount, int jp2Count, int md5Count, int dirCount)
{
    // Získání aktuálního data pro log soubor
    QString dateStr = QDate::currentDate().toString("yyyy-MM-dd");
    QFile logFile(selectedDirectory + "/log_" + dateStr + ".txt");  // Log soubor bude mít název s datem
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Chyba"), tr("Nepodařilo se otevřít log soubor pro zápis."));
        return;
    }

    QTextStream out(&logFile);

    // Zápis souhrnných údajů
    out << "Log soubor - Počet souborů a adresářů\n\n";
    out << "Počet souborů s koncovkou '.tif': " << tifCount << "\n";
    out << "Počet souborů s koncovkou '.txt': " << txtCount << "\n";
    out << "Počet souborů s koncovkou '.xml': " << xmlCount << "\n";
    out << "Počet souborů s koncovkou '.jp2': " << jp2Count << "\n";
    out << "Počet souborů s koncovkou '.md5': " << md5Count << "\n";
    out << "Počet adresářů začínajících: " << dirCount << "\n";

    // Výpis všech souborů
    out << "\nSeznam souborů s příponou .tif:\n";
    QStringList tifFiles = getFilesOfType(QDir(selectedDirectory), "*.tif");
    foreach (const QString &file, tifFiles) {
        out << file << "\n";
    }

    if (ui->checkBox_countTxtFiles->isChecked()) {
        out << "\nSeznam souborů s příponou .txt:\n";
        QStringList txtFiles = getFilesOfType(QDir(selectedDirectory), "*.txt");
        foreach (const QString &file, txtFiles) {
            out << file << "\n";
        }
    }

    if (ui->checkBox_countXmlFiles->isChecked()) {
        out << "\nSeznam souborů s příponou .xml:\n";
        QStringList xmlFiles = getFilesOfType(QDir(selectedDirectory), "*.xml");
        foreach (const QString &file, xmlFiles) {
            out << file << "\n";
        }
    }

    if (ui->checkBox_countJp2Files->isChecked()) {
        out << "\nSeznam souborů s příponou .jp2:\n";
        QStringList jp2Files = getFilesOfType(QDir(selectedDirectory), "*.jp2");
        foreach (const QString &file, jp2Files) {
            out << file << "\n";
        }
    }

    if (ui->checkBox_countMd5Files->isChecked()) {
        out << "\nSeznam souborů s příponou .md5:\n";
        QStringList md5Files = getFilesOfType(QDir(selectedDirectory), "*.md5");
        foreach (const QString &file, md5Files) {
            out << file << "\n";
        }
    }

    // Výpis všech adresářů začínajících daným prefixem
    out << "\nSeznam adresářů začínajících '" << ui->plainTextEdit_prefix->toPlainText() << "'\n";
    QStringList dirs = getDirectoriesOfPrefix(QDir(selectedDirectory), ui->plainTextEdit_prefix->toPlainText());
    foreach (const QString &dir, dirs) {
        out << dir << "\n";
    }

    // Zavření souboru po zápisu
    logFile.close();
    QMessageBox::information(this, tr("Log soubor"), tr("Log soubor byl úspěšně vytvořen."));
}

// Slot pro .txt soubory
void MainWindow::on_checkBox_countTxtFiles_stateChanged(int state) {}

// Slot pro .xml soubory
void MainWindow::on_checkBox_countXmlFiles_stateChanged(int state) {}

// Slot pro .jp2 soubory
void MainWindow::on_checkBox_countJp2Files_stateChanged(int state) {}

// Slot pro .md5 soubory
void MainWindow::on_checkBox_countMd5Files_stateChanged(int state) {}

// Slot pro adresáře
void MainWindow::on_checkBox_countDirs_stateChanged(int state) {}

// Slot pro log
void MainWindow::on_checkBox_generateLog_stateChanged(int state) {}
