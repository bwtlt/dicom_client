#include <QFileDialog>
#include <QScrollArea>
#include <QLabel>
#include <QString>
#include <QThread>
#include <QMessageBox>
#include <map>

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
    // clean UI: delete previous widgets
    QLayoutItem *child;
    while ((child = ui->verticalLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    QString path = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                "/home/debian/dicom_files",
                                                tr("DICOM Files (*.dcm)"));
    if (path.isEmpty())
        return;

    QThread* thread = new QThread;
    DicomReader* reader = new DicomReader;
    reader->setFilePath(path);
    reader->moveToThread(thread);

    connect(thread, &QThread::started, reader, &DicomReader::process);
    connect(reader, &DicomReader::finished, this, [=](std::map<QString, QString> metadata, QImage image) {
        for (const auto& [key, value] : metadata) {
            QLabel* label = new QLabel(key + ": " + value);
            ui->verticalLayout->addWidget(label);
        }
        if (!image.isNull()) {
            QLabel* imageLabel = new QLabel;
            imageLabel->setPixmap(QPixmap::fromImage(image).scaled(512, 512, Qt::KeepAspectRatio));

            QScrollArea* scrollArea = new QScrollArea;
            scrollArea->setWidget(imageLabel);
            scrollArea->setWidgetResizable(true);
            scrollArea->setAlignment(Qt::AlignCenter);

            ui->verticalLayout->addWidget(scrollArea);
        }
    });
    connect( reader, &DicomReader::finished, thread, &QThread::quit);
    connect( reader, &DicomReader::finished, reader, &DicomReader::deleteLater);
    connect( thread, &QThread::finished, thread, &QThread::deleteLater);

    connect(reader, &DicomReader::error, this, [=](QString message) {
        QMessageBox::critical(this, "DICOM error", message);
    });

    thread->start();
}
