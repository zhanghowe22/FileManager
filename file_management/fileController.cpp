#include "fileController.h"

CFileController::CFileController(CFileModel* model, CFileView* view, QObject* parent) :
    QObject(parent), model(model), view(view) {
    // 连接view的信号与槽
    connect(view, &CFileView::connectRequested, this, &CFileController::onConnectRequested);
    connect(view, &CFileView::refreshRequested, this, &CFileController::onRequestFileList);
    connect(view, &CFileView::downloadRequested, this, &CFileController::onDownloadFile);
    connect(view, &CFileView::deleteRequested, this, &CFileController::onDeleteFile);

    // 连接model的信号与槽
    connect(model, &CFileModel::connectionStatusChanged, this, &CFileController::onConnectionStatusChanged);
    connect(model, &CFileModel::fileListReceived, view, &CFileView::updateFileList);
    connect(model, &CFileModel::fileDownloaded, this, &CFileController::onFileDownloaded);
    connect(model, &CFileModel::fileDeleted, this, &CFileController::onFileDeleted);
    connect(model, &CFileModel::errorOccurred, view, &CFileView::showError);
}

void CFileController::onConnectRequested(const QString& ip, quint16 port) { model->connectToServer(ip, port); }

void CFileController::onConnectionStatusChanged(bool isConnected) {
    if (isConnected) {
        view->updateConnectionStatus("已连接 ", true);
    } else {
        view->updateConnectionStatus("未连接 ", false);
    }
}

void CFileController::onRequestFileList() { model->requestFileList(); }

void CFileController::onDownloadFile(const QString& fileName) { model->downloadFile(fileName); }

void CFileController::onDeleteFile(const QString& fileName) { model->deleteFile(fileName); }

void CFileController::onFileDownloaded(const QString& fileName, const QByteArray& data) {
    view->showDownloadProgress(fileName, 100);
    view->showError(QString("文件 %1 下载成功，大小: %2 字节").arg(fileName).arg(data.size()));
}

void CFileController::onFileDeleted(const QString& fileName) {
    view->updateFileList(QStringList());
    view->showError(QString("文件 %1 删除成功").arg(fileName));
}
