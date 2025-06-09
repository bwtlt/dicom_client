#include <QtTest>
#include "src/dicomreader.h"

class DicomReaderTest : public QObject {
    Q_OBJECT

private slots:
    void test_invalid_file_emits_error();
    void test_valid_file_emits_finished();
};

void DicomReaderTest::test_invalid_file_emits_error()
{
    DicomReader reader;
    QSignalSpy spyError(&reader, &DicomReader::error);
    QVERIFY(spyError.isValid());

    reader.setFilePath("not_a_real_file.dcm");
    reader.process();

    QCOMPARE(spyError.count(), 1);

    QString message = spyError.takeFirst().at(0).toString();
    QVERIFY(message.contains("Could not open DICOM"));
}

void DicomReaderTest::test_valid_file_emits_finished()
{
    QString testFile = QFINDTESTDATA("data/sample.dcm");
    QVERIFY2(!testFile.isEmpty(), "sample.dcm not found");

    DicomReader reader;
    QSignalSpy spyFinished(&reader, &DicomReader::finished);
    QVERIFY(spyFinished.isValid());

    reader.setFilePath(testFile);
    reader.process();

    QCOMPARE(spyFinished.count(), 1);

    const QVariantList args = spyFinished.takeFirst();
    auto metadata = args.at(0).value<std::map<QString, QString>>();
    QImage image = qvariant_cast<QImage>(args.at(1));

    QVERIFY(metadata.find(QString("Patient Name")) != metadata.end());
    QVERIFY(!metadata["Patient Name"].isEmpty());
    QVERIFY2(metadata["Patient Name"] == "0159^Patient", metadata["Patient Name"].toStdString().c_str());
    QVERIFY(!image.isNull());
    QVERIFY(image.width() > 0 && image.height() > 0);
}

QTEST_MAIN(DicomReaderTest)
#include "dicomreadertest.moc"
