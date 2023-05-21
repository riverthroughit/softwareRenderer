#pragma once
#include<queue>
#include<qpainter.h>
#include<thread>
#include<mutex>
using std::mutex;
using std::lock_guard;
using std::queue;
using std::thread;
class FramesInRenderer
{//在renderer中构造帧 或者接收前端已经画过的帧 以此循环使用
	static queue<QRgb*> frames;//存储已经渲染好的帧	
	static mutex framesMutex;//防止多个线程对帧队列同时操作

public:
	static QRgb* generateFrames(int s) {
		lock_guard<mutex> framesLock(framesMutex);
		QRgb* frame = 0;
		if (!frames.empty()) {
			frame = frames.front();
			frames.pop();
		}
		else {
			frame = new QRgb[s];
		}
		std::memset(frame, 200, sizeof(QRgb) * s);//更快
		//for (int i = 0; i != s; ++i)
		//	frame[i] = qRgb(200, 200, 200);
		return frame;
	}

	static void addToFrames(QRgb* frame) {//将前端用过的帧传回帧队列中
		lock_guard<mutex> framesLock(framesMutex);
		frames.push(frame);
	}
};

