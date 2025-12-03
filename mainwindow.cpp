#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QCloseEvent>  // Добавлено

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QMenu *fileMenu = menuBar()->addMenu("Файл");
    QMenu *productsMenu = menuBar()->addMenu("Товары");
    QMenu *helpMenu = menuBar()->addMenu("Справка");

    QAction *saveAction = fileMenu->addAction("Сохранить в файл");
    QAction *loadAction = fileMenu->addAction("Загрузить из файла");
    QAction *addAction = productsMenu->addAction("Добавить товар");
    QAction *editAction = productsMenu->addAction("Редактировать товар");
    QAction *deleteAction = productsMenu->addAction("Удалить товар");
    QAction *helpAction = helpMenu->addAction("О программе");

    connect(saveAction, &QAction::triggered, this, &MainWindow::saveToFile);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadFromFile);
    connect(addAction, &QAction::triggered, this, &MainWindow::addProduct);
    connect(editAction, &QAction::triggered, this, &MainWindow::editProduct);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteProduct);
    connect(helpAction, &QAction::triggered, this, &MainWindow::showHelp);

    addAction->setShortcut(QKeySequence("Ctrl+N"));
    editAction->setShortcut(QKeySequence("Ctrl+E"));
    deleteAction->setShortcut(QKeySequence::Delete);
    helpAction->setShortcut(QKeySequence("F1"));
    loadAction->setShortcut(QKeySequence::Open);
    saveAction->setShortcut(QKeySequence::Save);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addProduct()
{
    bool ok;
    QString product = QInputDialog::getText(this, "Добавить товар", "Название товара:", QLineEdit::Normal, "", &ok);
    if (!ok || product.isEmpty()) return;

    QString supplier = QInputDialog::getText(this, "Добавить товар", "Поставщик:", QLineEdit::Normal, "", &ok);
    if (!ok || supplier.isEmpty()) return;

    int received = QInputDialog::getInt(this, "Добавить товар", "Количество прибывшего товара:", 0, 0, 10000000, 1, &ok);
    if (!ok) return;

    int sold = QInputDialog::getInt(this, "Добавить товар", "Количество проданного товара:", 0, 0, received, 1, &ok);
    if (!ok) return;

    int price = QInputDialog::getInt(this, "Добавить товар", "Цена товара:", 0, 0, 2147483647, 1, &ok);
    if (!ok) return;

    int totalInStock = received - sold;
    int totalPrice = totalInStock * price;
    int revenue = sold * price;

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(product));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(supplier));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(received)));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(sold)));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(totalInStock)));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(price)));
    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(totalPrice)));
    ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(revenue)));

    markDataChanged();
}

void MainWindow::deleteProduct()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "Ошибка", "Выберите товар для удаления!");
        return;
    }

    QString productName = ui->tableWidget->item(currentRow, 0)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение удаления",
                                  "Вы уверены, что хотите удалить товар:\n\"" + productName + "\"?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        ui->tableWidget->removeRow(currentRow);
        markDataChanged();
        QMessageBox::information(this, "Успех!", "Данные удалены!");
    }
}

void MainWindow::editProduct()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "Ошибка", "Выберите товар для редактирования!");
        return;
    }

    QString currentProduct = ui->tableWidget->item(currentRow, 0)->text();
    QString currentSupplier = ui->tableWidget->item(currentRow, 1)->text();
    int currentReceived = ui->tableWidget->item(currentRow, 2)->text().toInt();
    int currentSold = ui->tableWidget->item(currentRow, 3)->text().toInt();
    int currentPrice = ui->tableWidget->item(currentRow, 5)->text().toInt();

    bool ok;

    QString product = QInputDialog::getText(this, "Редактировать товар", "Название товара:", QLineEdit::Normal, currentProduct, &ok);
    if (!ok || product.isEmpty()) return;

    QString supplier = QInputDialog::getText(this, "Редактировать товар", "Поставщик:", QLineEdit::Normal, currentSupplier, &ok);
    if (!ok || supplier.isEmpty()) return;

    int received = QInputDialog::getInt(this, "Редактировать товар", "Количество прибывшего товара:", currentReceived, 0, 100000, 1, &ok);
    if (!ok) return;

    int sold = QInputDialog::getInt(this, "Редактировать товар", "Количество проданного товара:", currentSold, 0, received, 1, &ok);
    if (!ok) return;

    int price = QInputDialog::getInt(this, "Редактировать товар", "Цена товара:", currentPrice, 0, 2147483647, 1, &ok);
    if (!ok) return;

    int totalInStock = received - sold;
    int totalPrice = totalInStock * price;
    int revenue = sold * price;

    ui->tableWidget->item(currentRow, 0)->setText(product);
    ui->tableWidget->item(currentRow, 1)->setText(supplier);
    ui->tableWidget->item(currentRow, 2)->setText(QString::number(received));
    ui->tableWidget->item(currentRow, 3)->setText(QString::number(sold));
    ui->tableWidget->item(currentRow, 4)->setText(QString::number(totalInStock));
    ui->tableWidget->item(currentRow, 5)->setText(QString::number(price));
    ui->tableWidget->item(currentRow, 6)->setText(QString::number(totalPrice));
    ui->tableWidget->item(currentRow, 7)->setText(QString::number(revenue));

    markDataChanged();
}

void MainWindow::showHelp()
{
    QMessageBox::about(this, "О программе",
                       "Данная программа представляет собой приложение для складского учета\n"
                       "С помощью нее вы можеет:\n"
                       "1) Добавить товар и всю информацию о нем(Название, поставщика, количество поступившего/проданного товара и цену, все остальное высчитватся автоматичски)\n"
                       "2) Редактирование данных о товаре\n"
                       "3) Удаления товара из списка\n\n"
                       "Функции будут пополняться");
}

void MainWindow::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "Текстовые файлы (*.txt)");

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи");
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
        out << ui->tableWidget->horizontalHeaderItem(col)->text();
        if (col < ui->tableWidget->columnCount() - 1) {
            out << "\t";
        }
    }
    out << "\n";

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            if (item) {
                out << item->text();
            }
            if (col < ui->tableWidget->columnCount() - 1) {
                out << "\t";
            }
        }
        out << "\n";
    }

    file.close();
    markDataSaved();
    QMessageBox::information(this, "Успех", "Данные успешно сохранены в файл");
}

void MainWindow::loadFromFile()
{
    if (dataChanged) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Несохраненные изменения","У вас есть несохраненные изменения.\n""Сохранить перед загрузкой нового файла?",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (reply == QMessageBox::Save) {
            saveToFile();
            if (dataChanged) return;
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }

    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Текстовые файлы (*.txt)");

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для чтения");
        return;
    }

    ui->tableWidget->setRowCount(0);

    QTextStream in(&file);
    in.setCodec("UTF-8");

    int lineNumber = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (lineNumber == 0) {
            lineNumber++;
            continue;
        }

        if (!line.isEmpty()) {
            QStringList fields = line.split("\t");

            if (fields.size() >= 8) {
                int row = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(row);

                for (int col = 0; col < qMin(fields.size(), ui->tableWidget->columnCount()); ++col) {
                    ui->tableWidget->setItem(row, col, new QTableWidgetItem(fields[col]));
                }
            }
        }

        lineNumber++;
    }

    file.close();
    markDataSaved();
    QMessageBox::information(this, "Успех", "Данные успешно загружены из файла");
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (dataChanged) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Несохраненные изменения","У вас есть несохраненные изменения.\n""Сохранить перед выходом?",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (reply == QMessageBox::Save) {
            saveToFile();
            if (dataChanged) {
                event->ignore();
                return;
            }
            event->accept();
        } else if (reply == QMessageBox::Discard) {
            event->accept();
        } else { // Cancel
            event->ignore();
        }
    } else {
        event->accept();
    }
}

void MainWindow::markDataChanged()
{
    dataChanged = true;
}

void MainWindow::markDataSaved()
{
    dataChanged = false;
}
