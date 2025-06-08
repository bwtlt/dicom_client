#include <QFileDialog>

#include "dicomreader.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::onOpenFileClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenFileClicked()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/debian/dicom_files", tr("DICOM Files (*.dcm)"));
    if (!fileName.isEmpty()) {
        //open file
    }
}
