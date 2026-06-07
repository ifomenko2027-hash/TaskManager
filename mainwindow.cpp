#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->addButton, &QPushButton::clicked,
            this, &MainWindow::addTask);

    connect(ui->deleteButton, &QPushButton::clicked,
            this, &MainWindow::deleteTask);

    connect(ui->editButton, &QPushButton::clicked,
            this, &MainWindow::editTask);

    connect(ui->doneButton, &QPushButton::clicked,
            this, &MainWindow::completeTask);

    connect(ui->searchEdit, &QLineEdit::textChanged,
            this, &MainWindow::searchTask);

    loadTasks();
}

MainWindow::~MainWindow()
{
    saveTasks();
    delete ui;
}

void MainWindow::addTask()
{
    bool ok;

    QString text = QInputDialog::getText(
        this,
        "Новая задача",
        "Введите задачу:",
        QLineEdit::Normal,
        "",
        &ok);

    if(ok && !text.isEmpty())
    {
        ui->taskList->addItem(text);
        saveTasks();
    }
}

void MainWindow::deleteTask()
{
    delete ui->taskList->currentItem();
    saveTasks();
}

void MainWindow::editTask()
{
    QListWidgetItem *item = ui->taskList->currentItem();

    if(!item)
        return;

    bool ok;

    QString text = QInputDialog::getText(
        this,
        "Редактирование",
        "Изменить задачу:",
        QLineEdit::Normal,
        item->text(),
        &ok);

    if(ok && !text.isEmpty())
    {
        item->setText(text);
        saveTasks();
    }
}

void MainWindow::completeTask()
{
    QListWidgetItem *item = ui->taskList->currentItem();

    if(!item)
        return;

    item->setText("✓ " + item->text());

    saveTasks();
}

void MainWindow::searchTask(const QString &text)
{
    for(int i = 0; i < ui->taskList->count(); i++)
    {
        QListWidgetItem *item = ui->taskList->item(i);

        item->setHidden(
            !item->text().contains(text, Qt::CaseInsensitive));
    }
}

void MainWindow::saveTasks()
{
    QJsonArray array;

    for(int i = 0; i < ui->taskList->count(); i++)
    {
        array.append(ui->taskList->item(i)->text());
    }

    QFile file("tasks.json");

    if(file.open(QIODevice::WriteOnly))
    {
        file.write(
            QJsonDocument(array).toJson());
    }
}

void MainWindow::loadTasks()
{
    QFile file("tasks.json");

    if(!file.open(QIODevice::ReadOnly))
        return;

    QJsonDocument doc =
        QJsonDocument::fromJson(file.readAll());

    QJsonArray array = doc.array();

    for(auto value : array)
    {
        ui->taskList->addItem(
            value.toString());
    }
}