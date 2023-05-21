#pragma once
#include<queue>
#include<qpainter.h>
#include<thread>
#include"FramesInRenderer.h"
using std::queue;
using std::thread;
class Frames
{//��renderer����ȡ���֡ ��app�н�֡���γ�����ʾ�ڽ�����
	static queue<QRgb*> frames;//�洢�Ѿ���Ⱦ�õ�֡	
	static mutex framesMutex;//��ֹ����̶߳�֡����ͬʱ����
public:
	static int getFramesSize() {
		lock_guard<mutex> framesLock(framesMutex);
		return frames.size();
	}

	//����ǰ֡ɾ�� �������ʱ�����Ѿ�Ϊ�� Ҫ����������� ��Ȼǰ����ͼ�ɻ� ����˸
	static bool deleteFrame(QRgb* frame) {
		lock_guard<mutex> framesLock(framesMutex);
		if (!frames.empty()) {
			if (frame)
				FramesInRenderer::addToFrames(frame);//���غ��֡����
			//delete[] frame;
			return true;
		}
		else {
			frames.push(frame);
			return false;
		}
			
	}
	
	//��ȡ��ͷ֡ ���������
	static QRgb* getFrontAndPop() {
		lock_guard<mutex> framesLock(framesMutex);
		QRgb* top = 0;
		if (!frames.empty()) {
			top = frames.front();
			frames.pop();
		}
		return top;				
	}

	static void addToFrames(QRgb* frame) {//�������ɵ�֡����֡������
		lock_guard<mutex> framesLock(framesMutex);
		frames.push(frame);
	}
};



