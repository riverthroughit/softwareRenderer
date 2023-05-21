#pragma once
#include<queue>
#include<qpainter.h>
#include<thread>
#include"FramesInRenderer.h"
using std::queue;
using std::thread;
class Frames
{//从renderer处获取添加帧 在app中将帧依次出队显示在界面上
	static queue<QRgb*> frames;//存储已经渲染好的帧	
	static mutex framesMutex;//防止多个线程对帧队列同时操作
public:
	static int getFramesSize() {
		lock_guard<mutex> framesLock(framesMutex);
		return frames.size();
	}

	//将当前帧删除 但如果此时队列已经为空 要将其重新入队 不然前端无图可画 会闪烁
	static bool deleteFrame(QRgb* frame) {
		lock_guard<mutex> framesLock(framesMutex);
		if (!frames.empty()) {
			if (frame)
				FramesInRenderer::addToFrames(frame);//传回后端帧队列
			//delete[] frame;
			return true;
		}
		else {
			frames.push(frame);
			return false;
		}
			
	}
	
	//获取队头帧 并将其出队
	static QRgb* getFrontAndPop() {
		lock_guard<mutex> framesLock(framesMutex);
		QRgb* top = 0;
		if (!frames.empty()) {
			top = frames.front();
			frames.pop();
		}
		return top;				
	}

	static void addToFrames(QRgb* frame) {//将新生成的帧传入帧队列中
		lock_guard<mutex> framesLock(framesMutex);
		frames.push(frame);
	}
};



