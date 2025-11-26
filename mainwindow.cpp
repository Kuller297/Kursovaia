#include "mainwindow.h"
#include <QTableWidget>
#include <QMenuBar>
#include <QMenu>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , table(nullptr)
{
    setupUI();
    setupTable();
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);

    table = new QTableWidget(centralWidget);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(table);

    setCentralWidget(centralWidget);

    QMenu *productsMenu = menuBar()->addMenu("Товары");
    QMenu *helpMenu = menuBar()->addMenu("Справка");

    QAction *addAction = productsMenu->addAction("Добавить товар");
    QAction *editAction = productsMenu->addAction("Редактировать товар");
    QAction *deleteAction = productsMenu->addAction("Удалить товар");
    QAction *helpAction = helpMenu->addAction("О программе");

    connect(addAction, &QAction::triggered, this, &MainWindow::addProduct);
    connect(editAction, &QAction::triggered, this, &MainWindow::editProduct);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteProduct);
    connect(helpAction, &QAction::triggered, this, &MainWindow::help);

    resize(1280, 720);
}

void MainWindow::setupTable()
{
    table->setColumnCount(8);

    QStringList headers;
    headers << "Товар" << "Поставщик" << "Прибыло на склад, шт" << "Продано, шт"<< "Всего на складе, шт" << "Цена, р" << "Общая цена товара на складе, р" << "Выручка с продажи, р";
    table->setHorizontalHeaderLabels(headers);

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    table->resizeColumnsToContents();

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setRowCount(0);
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

    int row = table->rowCount();
    table->insertRow(row);

    table->setItem(row, 0, new QTableWidgetItem(product));
    table->setItem(row, 1, new QTableWidgetItem(supplier));
    table->setItem(row, 2, new QTableWidgetItem(QString::number(received)));
    table->setItem(row, 3, new QTableWidgetItem(QString::number(sold)));
    table->setItem(row, 4, new QTableWidgetItem(QString::number(totalInStock)));
    table->setItem(row, 5, new QTableWidgetItem(QString::number(price)));
    table->setItem(row, 6, new QTableWidgetItem(QString::number(totalPrice)));
    table->setItem(row, 7, new QTableWidgetItem(QString::number(revenue)));
}

void MainWindow::deleteProduct()
{
    int currentRow = table->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "Ошибка", "Выберите товар для удаления!");
        return;
    }

    table->removeRow(currentRow);
}

void MainWindow::editProduct()
{
    int currentRow = table->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "Ошибка", "Выберите товар для редактирования!");
        return;
    }

    QString currentProduct = table->item(currentRow, 0)->text();
    QString currentSupplier = table->item(currentRow, 1)->text();
    int currentReceived = table->item(currentRow, 2)->text().toInt();
    int currentSold = table->item(currentRow, 3)->text().toInt();
    int currentPrice = table->item(currentRow, 5)->text().toInt();

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

    table->item(currentRow, 0)->setText(product);
    table->item(currentRow, 1)->setText(supplier);
    table->item(currentRow, 2)->setText(QString::number(received));
    table->item(currentRow, 3)->setText(QString::number(sold));
    table->item(currentRow, 4)->setText(QString::number(totalInStock));
    table->item(currentRow, 5)->setText(QString::number(price));
    table->item(currentRow, 6)->setText(QString::number(totalPrice));
    table->item(currentRow, 7)->setText(QString::number(revenue));
}

void MainWindow::help()
{
    QMessageBox::about(this, "О программе",
        "Данная программа представляет собой приложение для складского учета\n"
                       "С помощью нее вы можеет:\n"
                       "1) Добавить товар и всю информацию о нем(Название, поставщика, количество поступившего/проданного товара и цену, все остальное высчитватся автоматичски)\n"
                       "2) Редактирование данных о товаре\n"
                       "3) Удаления товара из списка\n\n"
                       "Функции будут пополняться");
}
