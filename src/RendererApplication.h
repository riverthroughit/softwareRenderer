#pragma once

#include <QtWidgets/QMainWindow>
#include<Windows.h>
#include <QPainter>
#include<QMouseEvent>
#include<qfiledialog.h>
#include "ui_RendererApplication.h"
#include"output/FuncCodes.h"
#include"output/Frames.h"
#include"output/UserSceneData.h"

class RendererApplication : public QMainWindow
{
    Q_OBJECT

public:
    RendererApplication(int iw = 1920, int ih = 1080, QWidget* parent = nullptr);
    ~RendererApplication();
    //void drawFrame(QRgb*);
    void beginDraw();
private:
    Ui::RendererApplicationClass ui;
    int imageWidth, imageHeight;
    int timerID;//绘图定时器id
    int timerID2;//fps定时器id
    //定时器每激活一次(10ms) time加一 time到100时即1秒
    //成功删除一帧 就代表已经成功生成下一帧 fps加一
    short time = 0, fps = 1;
    int lastX = -1, lastY = -1;//上次拖动鼠标的位置
    short mouseMoveTimes = 0;
    bool mouseLeftPressed = 1;

    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent* event);
    void updateFPS() {
        QString s = QString("%1 %2%3").arg("renderer").arg("FPS:").arg(fps);
        setWindowTitle(s);
        fps = 1;
    }

    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    //void closeEvent(QCloseEvent* event);

    //同一类菜单下的有些选项不可同时选择 例如不可同时msaa和ssaa 线框模式和blinnphong模型
    void removeSSAAUI() {
        ui.actionSSAA->setChecked(false);
    }

    void removeMSAAUI() {
        ui.actionMSAA->setChecked(false);
    }

    void removeMeshUI() {
        ui.actionMesh->setChecked(false);
    }

    void removeFlatUI() {
        ui.actionflat_shading->setChecked(false);
    }

    void removeGourandUI() {
        ui.actionGourand_shading->setChecked(false);
    }

    void removePhongUI() {
        ui.actionPhong_shading->setChecked(false);
    }


private slots:

    void openNewMod() {
        QString file = QFileDialog::getOpenFileName(this, tr("choose an obj file"), "./", tr("Image Files(*.obj *.OBJ)"));
        if (!file.isEmpty())
            UserSceneData::pushChange(userChange({ 5, {0,0} }, file.toStdString()));
    }

    void chooseSSAA() {
        FuncCodes::negativeCode(SSAA);
        FuncCodes::removeCode(MSAA);
        FuncCodes::pushCode();
        removeMSAAUI();
    }

    void chooseMSAA() {
        FuncCodes::negativeCode(MSAA);
        FuncCodes::removeCode(SSAA);
        FuncCodes::pushCode();
        removeSSAAUI();
    }

    void chooseFlat() {
        FuncCodes::setCode(FLAT);
        FuncCodes::removeCode(GOURAND);
        FuncCodes::removeCode(PHONG);
        FuncCodes::removeCode(MESH);
        FuncCodes::pushCode();
        removeGourandUI();
        removePhongUI();
        removeMeshUI();
    }

    void chooseGourand() {
        FuncCodes::setCode(GOURAND);
        FuncCodes::removeCode(FLAT);
        FuncCodes::removeCode(PHONG);
        FuncCodes::removeCode(MESH);
        FuncCodes::pushCode();
        removeFlatUI();
        removePhongUI();
        removeMeshUI();
    }

    void choosePhong() {
        FuncCodes::setCode(PHONG);
        FuncCodes::removeCode(GOURAND);
        FuncCodes::removeCode(FLAT);
        FuncCodes::removeCode(MESH);
        FuncCodes::pushCode();
        removeFlatUI();
        removeGourandUI();
        removeMeshUI();
    }

    void chooseMesh() {
        FuncCodes::setCode(MESH);
        FuncCodes::removeCode(GOURAND);
        FuncCodes::removeCode(FLAT);
        FuncCodes::removeCode(PHONG);
        FuncCodes::pushCode();
        removeFlatUI();
        removeGourandUI();
        removePhongUI();
    }

    void chooseClipping() {
        FuncCodes::negativeCode(ClIP);
        FuncCodes::pushCode();
    }

    void chooseBilinear() {
        FuncCodes::negativeCode(BILINEAR);
        FuncCodes::pushCode();
    }

    void chooseShadowMapping() {
        FuncCodes::negativeCode(SHADOWMAP);
        FuncCodes::pushCode();
    }
};
