#include "RendererApplication.h"

RendererApplication::RendererApplication(int iw, int ih, QWidget* parent)
    : QMainWindow(parent),imageWidth(iw), imageHeight(ih), timerID(startTimer(2, Qt::PreciseTimer)),timerID2(startTimer(1000, Qt::PreciseTimer))
{
    ui.setupUi(this);
    
    connect(
        ui.actionopen_new_model, SIGNAL(triggered()),
        this, SLOT(openNewMod())
    );

    connect(
        ui.actionClipping, SIGNAL(triggered()),
        this, SLOT(chooseClipping())
    );

    connect(
        ui.actionShadow_mapping, SIGNAL(triggered()),
        this, SLOT(chooseShadowMapping())
    );

    connect(
        ui.bilinear, SIGNAL(triggered()),
        this, SLOT(chooseBilinear())
    );

    connect(
        ui.actionMSAA, SIGNAL(triggered()),
        this, SLOT(chooseMSAA())
    );

    connect(
        ui.actionSSAA, SIGNAL(triggered()),
        this, SLOT(chooseSSAA())
    );

    connect(
        ui.actionflat_shading, SIGNAL(triggered()),
        this, SLOT(chooseFlat())
    );

    connect(
        ui.actionGourand_shading, SIGNAL(triggered()),
        this, SLOT(chooseGourand())
    );

    connect(
        ui.actionPhong_shading, SIGNAL(triggered()),
        this, SLOT(choosePhong())
    );

    connect(
        ui.actionMesh, SIGNAL(triggered()),
        this, SLOT(chooseMesh())
    );
}

RendererApplication::~RendererApplication()
{}

void RendererApplication::beginDraw()
{
    //while
}


void RendererApplication::timerEvent(QTimerEvent* event) {
    if (event->timerId() == timerID) {//绘图定时器响应
        update();
    }
    else {//更新fps计时器响应
        updateFPS();
    } 
}


void RendererApplication::paintEvent(QPaintEvent*)
{
    QRgb* image = Frames::getFrontAndPop();
    QPainter(this).
        drawImage(0, 45, QImage((uchar*)image, imageWidth, imageHeight, QImage::Format_RGB32));
    //成功删除一帧 就代表已经成功生成下一帧 故fps加一
    if (Frames::deleteFrame(image))
        ++fps;
}

void RendererApplication::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        mouseLeftPressed = true;
    else
        mouseLeftPressed = false;
}

void RendererApplication::mouseMoveEvent(QMouseEvent* event)
{
    setCursor(Qt::ClosedHandCursor);//变为拳头状
    if (++mouseMoveTimes != 10)//降低该事件调用频率 否则change太多
        return;
    else
        mouseMoveTimes = 0;
    int nowX = event->x(), nowY = imageHeight - event->y();
    if (nowX >= imageWidth || nowX < 0 ||
        nowY >= imageHeight || nowY < 0) {//超出图片区域
        lastX = -1;
        return;
    }

    if (lastX == -1) {//第一次调用(或刚从外界移动到图片内)
        lastX = nowX, lastY = nowY;
        return;
    }
    
    if (mouseLeftPressed)//左键旋转相机
        UserSceneData::pushChange(userChange({ 1, { nowX - lastX,nowY - lastY } }));
    else//右键移动相机观测点
        UserSceneData::pushChange(userChange({ 3, { nowX - lastX,nowY - lastY } }));
    lastX = nowX, lastY = nowY;
}

void RendererApplication::mouseReleaseEvent(QMouseEvent* event) {
    lastX = -1, lastY = -1;//可能为拖拽图片结束
    setCursor(Qt::ArrowCursor);//变回箭头状
}

void RendererApplication::wheelEvent(QWheelEvent* event){
    int wheelDis = event->delta();
    UserSceneData::pushChange(userChange({ 2, { wheelDis > 0,wheelDis } }));
}
int a = 0;
void RendererApplication::keyPressEvent(QKeyEvent* event) {
    ++a;
    userChange change({ 4,{} });
    switch (event->key())
    {
    case Qt::Key_Up:
        change.data.second = { 0,1 };
        break;
    case Qt::Key_Down:
        change.data.second = { 0,-1 };
        break;
    case Qt::Key_Left:
        change.data.second = { -1,0 };
        break;
    case Qt::Key_Right:
        change.data.second = { 1,0 };
        break;
    default:
        UserSceneData::pushChange(userChange());
        return;
    }
    UserSceneData::pushChange(change);
}