#include "RendererApplication.h"
#include <QtWidgets/QApplication>
#include"render/Render.h"
#include<thread>
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    RendererApplication w(800,600);
    w.show();
    //构建场景
    Scene scene;
    //将场景输入给渲染器进行渲染
    Render renderer(800, 600, std::move(scene));
    //注意这里要传递对象指针
    std::thread th(&Render::run, &renderer);
    //与主线程分离
    th.detach();
    return a.exec();
}