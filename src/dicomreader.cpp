#include "dicomreader.h"

#include <QDebug>
#include <string>

#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>

DicomReader::DicomReader(QObject* parent)
    : QObject(parent)
{
}

void DicomReader::setFilePath(const QString &path) {
    filePath = path;
}

void DicomReader::process() {
    std::map<QString, QString> result;

    DcmFileFormat file;
    OFCondition status = file.loadFile(filePath.toStdString().c_str());
    if (!status.good()) {
        result["Error"] = QString::fromStdString(status.text());
        emit error("Could not open DICOM file: " + QString::fromStdString(status.text()));
        return;
    }

    DcmDataset* dataset = file.getDataset();
    OFString patientName, studyDate, modality;

    if (dataset->findAndGetOFString(DCM_PatientName, patientName).good())
        result["Patient Name"] = QString::fromStdString(patientName.c_str());
    if (dataset->findAndGetOFString(DCM_StudyDate, studyDate).good())
        result["Study Date"] = QString::fromStdString(studyDate.c_str());
    if (dataset->findAndGetOFString(DCM_Modality, modality).good())
        result["Modality"] = QString::fromStdString(modality.c_str());

    DicomImage img(filePath.toStdString().c_str());
    if (img.getStatus() != EIS_Normal) {
        emit error("Invalid image");
        return;
    }

    if (!img.isMonochrome()) {
        emit error("RGB images not handled");
        return;
    }

    img.setMinMaxWindow();

    const int width = img.getWidth();
    const int height = img.getHeight();

    QImage qimg(width, height, QImage::Format_Grayscale8);
    img.getOutputData(qimg.bits(), width * height, 8);

    emit finished(result, qimg);
}
