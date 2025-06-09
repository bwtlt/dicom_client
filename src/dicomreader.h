#pragma once

#include <QObject>
#include <QString>
#include <QImage>
#include <map>

class DicomReader : public QObject {
    Q_OBJECT
public:
    explicit DicomReader(QObject* parent = nullptr);
    ~DicomReader() = default;

    void setFilePath(const QString& path);

signals:
    void finished(std::map<QString, QString> metadata, QImage image);
    void error(QString message);

public slots:
    void process();

private:
    QString filePath;
};
