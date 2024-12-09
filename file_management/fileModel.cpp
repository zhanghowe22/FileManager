#include "fileModel.h"

#include <QDataStream>
#include <QDebug>
#include <QHostAddress>

CFileModel::CFileModel(QObject* parent) : QObject(parent), socket(new QTcpSocket(this)) {
    connect(socket, &QTcpSocket::readyRead, this, &CFileModel::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &CFileModel::onDisconnected);
}

void CFileModel::connectToServer(const QString& host, quint16 port) {
    // 通过线程连接到服务器，避免阻塞主线程
    QThread* thread = new QThread();
    connect(thread, &QThread::started, this, [this, host, port]() { onConnectToServer(host, port); });
    moveToThread(thread);
    thread->start();
}

void CFileModel::onConnectToServer(const QString& host, quint16 port) {
    socket->connectToHost(host, port);
    if (!socket->waitForConnected(3000)) {
        emit errorOccurred("连接服务器失败: " + socket->errorString());
    } else {
        qDebug() << "成功连接到服务器";
    }
}

void CFileModel::requestFileList() { sendRequest(0xAAAAAAAA); }

void CFileModel::downloadFile(const QString& fileName) { sendRequest(0xBBBBBBBB, fileName); }

void CFileModel::deleteFile(const QString& fileName) { sendRequest(0xCCCCCCCC, fileName); }

void CFileModel::sendRequest(uint32_t command, const QString& fileName) {
    QByteArray requestData;
    QDataStream stream(&requestData, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    request_t request;
    request.magic = 0xAABBCCDD;
    request.command = command;
    request.file.fileNameLen = fileName.size();

    if (!fileName.isEmpty()) {
        strncpy(request.file.fileName, fileName.toUtf8().constData(), 256);
    }

    request.length = sizeof(request_t) - sizeof(file_t) + request.file.fileNameLen;
    stream.writeRawData(reinterpret_cast<char*>(&request), request.length);

    socket->write(requestData);
    if (!socket->waitForBytesWritten(3000)) {
        emit errorOccurred("请求发送失败: " + socket->errorString());
    }
}

void CFileModel::onReadyRead() {
    QByteArray data = socket->readAll();
    parseResponse(data);
}

void CFileModel::onDisconnected() { emit errorOccurred("与服务器断开连接"); }

void CFileModel::parseResponse(const QByteArray& data) {
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    response_t response;
    stream.readRawData(reinterpret_cast<char*>(&response), sizeof(response_t));

    if (response.magic != 0xAABBCCDD) {
        emit errorOccurred("无效的响应数据 ");
        return;
    }

    switch (response.command) {
        case 0xAAAAAAAA: {  // 文件列表
            QStringList files;
            files.append(QString::fromUtf8(response.file.fileName));
            emit fileListReceived(files);
            break;
        }
        case 0xBBBBBBBB:  // 文件下载
            emit fileDownloaded(QString::fromUtf8(response.file.fileName),
                                QByteArray(reinterpret_cast<char*>(response.data.data), response.data.dataLen));
            break;
        case 0xCCCCCCCC:  // 文件删除
            emit fileDeleted(QString::fromUtf8(response.file.fileName));
            break;
        default: emit errorOccurred("未知命令"); break;
    }
}
