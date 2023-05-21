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
{//��renderer�й���֡ ���߽���ǰ���Ѿ�������֡ �Դ�ѭ��ʹ��
	static queue<QRgb*> frames;//�洢�Ѿ���Ⱦ�õ�֡	
	static mutex framesMutex;//��ֹ����̶߳�֡����ͬʱ����

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
		std::memset(frame, 200, sizeof(QRgb) * s);//����
		//for (int i = 0; i != s; ++i)
		//	frame[i] = qRgb(200, 200, 200);
		return frame;
	}

	static void addToFrames(QRgb* frame) {//��ǰ���ù���֡����֡������
		lock_guard<mutex> framesLock(framesMutex);
		frames.push(frame);
	}
};

