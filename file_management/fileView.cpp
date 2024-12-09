#include "fileView.h"

CFileView::CFileView(QWidget* parent) : QWidget(parent) {
    // 初始化控件
    fileList = new QListWidget(this);
    downloadButton = new QPushButton("下载文件", this);
    deleteButton = new QPushButton("删除文件", this);
    refreshButton = new QPushButton("刷新列表", this);
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);
    statusLabel = new QLabel("状态：未连接 ", this);
    statusLabel->setFont(QFont("Microsoft YaHei", 10));

    // 布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    // 按钮布局
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(downloadButton);
    buttonLayout->addWidget(deleteButton);

    // 主界面布局
    mainLayout->addWidget(fileList);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(progressBar);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);

    // 信号与槽连接
    connect(refreshButton, &QPushButton::clicked, this, &CFileView::refreshRequested);
    connect(downloadButton, &QPushButton::clicked, this, [this]() {
        if (auto currentItem = fileList->currentItem()) {
            emit downloadRequested(currentItem->text());
        }
    });
    connect(deleteButton, &QPushButton::clicked, this, [this]() {
        if (auto currentItem = fileList->currentItem()) {
            emit deleteRequested(currentItem->text());
        }
    });
}

void CFileView::updateFileList(const QStringList& files) {
    fileList->clear();
    fileList->addItems(files);
    statusLabel->setText("状态: 文件列表已刷新 ");
}

void CFileView::showDownloadProgress(const QString& fileName, int progress) {
    progressBar->setValue(progress);
    if (progress == 100) {
        statusLabel->setText(QString("状态: 文件 %1 下载完成 ").arg(fileName));
    } else {
        statusLabel->setText(QString("状态: 文件 %1 下载中... (%2%)").arg(fileName).arg(progress));
    }
}

void CFileView::showError(const QString& message) { statusLabel->setText(QString("错误: %1").arg(message)); }
