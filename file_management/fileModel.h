#ifndef CFILEMODEL_H
#define CFILEMODEL_H

#include <QByteArray>
#include <QObject>
#include <QStringList>
#include <QTcpSocket>

// 数据结构（小端模式）
#pragma pack(1)
struct file_t {
    uint32_t fileNameLen;
    char fileName[256];
};

struct data_t {
    uint32_t dataLen;
    uint8_t data[1024];
};

struct request_t {
    uint32_t magic;
    uint32_t length;
    uint32_t command;
    file_t file;
};

struct response_t {
    uint32_t magic;
    uint32_t length;
    uint32_t command;
    file_t file;
    data_t data;
};
#pragma pack()

class CFileModel : public QObject {
    Q_OBJECT
  public:
    explicit CFileModel(QObject* parent = nullptr);
    void connectToServer(const QString& host, quint16 port);
    void requestFileList();
    void downloadFile(const QString& fileName);
    void deleteFile(const QString& fileName);

  signals:
    void fileListReceived(const QStringList& files);
    void fileDownloaded(const QString& fileName, const QByteArray& data);
    void fileDeleted(const QString& fileName);
    void errorOccurred(const QString& message);

  private slots:
    void onReadyRead();
    void onDisconnected();

  private:
    QTcpSocket* socket;

    void sendRequest(uint32_t command, const QString& fileName = QString());
    void parseResponse(const QByteArray& data);
};

#endif  // CFILEMODEL_H
