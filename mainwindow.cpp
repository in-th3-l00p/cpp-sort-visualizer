#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "visualizer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    visualizer = std::make_shared<Visualizer>();
    ui->centralWidget->layout()->addWidget(visualizer.get());
}

MainWindow::~MainWindow()
{
    delete ui;
}

