#include <QApplication>

#include "fileController.h"
#include "fileModel.h"
#include "fileView.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    // 创建MVC各组件
    CFileView view;
    //    CFileModel model;
    //    CFileController controller(&model, &view);

    // 窗口标题
    view.setWindowTitle("文件管理 ");
    view.resize(600, 400);

    view.show();

    // 连接服务器
    //    model.connectToServer("127.0.0.1", 6666);

    return a.exec();
}
