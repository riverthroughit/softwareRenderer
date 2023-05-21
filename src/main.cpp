#include "RendererApplication.h"
#include <QtWidgets/QApplication>
#include"render/Render.h"
#include<thread>
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    RendererApplication w(800,600);
    w.show();
    //��������
    Scene scene;
    //�������������Ⱦ��������Ⱦ
    Render renderer(800, 600, std::move(scene));
    //ע������Ҫ���ݶ���ָ��
    std::thread th(&Render::run, &renderer);
    //�����̷߳���
    th.detach();
    return a.exec();
}