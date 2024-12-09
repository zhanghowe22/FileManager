#include "fileController.h"

CFileController::CFileController(CFileModel* model, CFileView* view, QObject* parent) :
    QObject(parent), model(model), view(view) {
    // 连接view的信号与槽
    connect(view, &CFileView::refreshRequested, this, &CFileController::onRequestFileList);
    connect(view, &CFileView::downloadRequested, this, &CFileController::onDownloadFile);
    connect(view, &CFileView::deleteRequested, this, &CFileController::onDeleteFile);

    // 连接model的信号与槽
    connect(model, &CFileModel::fileListReceived, view, &CFileView::updateFileList);
    connect(model, &CFileModel::fileDownloaded, this, &CFileController::onFileDownloaded);
    connect(model, &CFileModel::fileDeleted, this, &CFileController::onFileDeleted);
    connect(model, &CFileModel::errorOccurred, view, &CFileView::showError);
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
