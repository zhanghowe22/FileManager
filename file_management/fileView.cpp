#include "fileView.h"

#include <QMessageBox>

CFileView::CFileView(QWidget* parent) :
    QWidget(parent), ipInput(new QLineEdit(this)), portInput(new QLineEdit(this)),
    connectButton(new QPushButton("连接 ", this)), refreshButton(new QPushButton("刷新文件列表 ", this)),
    fileList(new QListWidget(this)), downloadButton(new QPushButton("下载文件 ", this)),
    deleteButton(new QPushButton("删除文件 ", this)), connectionStatusLabel(new QLabel("未连接 ", this)) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    // IP 和端口输入
    QHBoxLayout* connectionLayout = new QHBoxLayout();
    connectionLayout->addWidget(new QLabel("IP 地址: ", this));
    connectionLayout->addWidget(ipInput);
    connectionLayout->addWidget(new QLabel("端口: ", this));
    connectionLayout->addWidget(portInput);
    connectionLayout->addWidget(connectButton);
    layout->addLayout(connectionLayout);

    // 添加连接状态标签
    connectionStatusLabel->setStyleSheet("color: red; font-weight: bold;");
    layout->addWidget(connectionStatusLabel);

    // 文件操作部分
    layout->addWidget(refreshButton);
    layout->addWidget(fileList);
    layout->addWidget(downloadButton);
    layout->addWidget(deleteButton);

    setLayout(layout);

    // 默认端口
    portInput->setText("6666");

    // 信号与槽
    connect(connectButton, &QPushButton::clicked, this, [this]() {
        QString ip = ipInput->text();
        quint16 port = portInput->text().toUShort();
        emit connectRequested(ip, port);
    });

    connect(refreshButton, &QPushButton::clicked, this, &CFileView::refreshRequested);

    connect(downloadButton, &QPushButton::clicked, this, [this]() {
        if (fileList->currentItem()) {
            emit downloadRequested(fileList->currentItem()->text());
        } else {
            QMessageBox::warning(this, "警告", "请选择一个文件进行下载 ");
        }
    });

    connect(deleteButton, &QPushButton::clicked, this, [this]() {
        if (fileList->currentItem()) {
            emit deleteRequested(fileList->currentItem()->text());
        } else {
            QMessageBox::warning(this, "警告", "请选择一个文件进行删除 ");
        }
    });
}

void CFileView::updateFileList(const QStringList& files) {
    fileList->clear();
    fileList->addItems(files);
}

void CFileView::showDownloadProgress(const QString& fileName, int progress) {
    QMessageBox::information(this, "下载进度", QString("文件 %1 下载完成 %2%").arg(fileName).arg(progress));
}

void CFileView::showError(const QString& message) { QMessageBox::critical(this, "错误", message); }

void CFileView::updateConnectionStatus(const QString& status, bool isConnected) {
    connectionStatusLabel->setText(status);
    connectionStatusLabel->setStyleSheet(isConnected ? "color: green; font-weight: bold;" :
                                                       "color: red; font-weight: bold;");
}


