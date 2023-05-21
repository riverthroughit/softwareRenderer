#pragma once
#define FLAT 0
#define GOURAND 1
#define PHONG 2
#define MESH 3
#define ClIP 4
#define BILINEAR 5
#define SHADOWMAP 6
#define MSAA 7
#define SSAA 8
//SSAA MSAA SHADOWMAP BACKfACE ClIP MESH PHONG GOURAND FLAT
//8    7    6		  5		   4	3	 2	   1	   0
 
#include<queue>
#include<mutex>
using std::queue;
using std::mutex;
using std::lock_guard;

class FuncCodes {
	static queue<unsigned short> codes;//存储用户界面选择的功能code 会逐条发往renderer中渲染
	static unsigned short code;//当前选择的功能
	static mutex codesMutex;
public:
	static void setCode(const unsigned short& choicedCode) {
		code |= (1 << choicedCode);
	}

	static void negativeCode(const unsigned short& choicedCode) {
		code ^= (1 << choicedCode);
	}

	static void removeCode(const unsigned short& removedCode) {
		code &= ~(1 << removedCode);
	}

	static void pushCode() {
		lock_guard<mutex> codesLock(codesMutex);
		codes.push(code);
	}

	static unsigned short getCode() {
		lock_guard<mutex> codesLock(codesMutex);
		if (codes.empty())
			return 0;		
		unsigned short topCode = codes.front();
		codes.pop();
		return topCode;
	}
	//curCode码中是否包含funcCode对应的功能码
	static bool withFunc(const unsigned short& curCode,const unsigned short& funcCode) {
		return curCode & (1 << funcCode);
	}

	static bool hasFLAT(const unsigned short& curCode) {
		return withFunc(curCode, FLAT);
	}

	static bool hasGOURAND(const unsigned short& curCode) {
		return withFunc(curCode, GOURAND);
	}

	static bool hasPHONG(const unsigned short& curCode) {
		return withFunc(curCode, PHONG);
	}

	static bool hasMESH(const unsigned short& curCode) {
		return withFunc(curCode, MESH);
	}

	static bool hasClIP(const unsigned short& curCode) {
		return withFunc(curCode, ClIP);
	}

	static bool hasSHADOWMAP(const unsigned short& curCode) {
		return withFunc(curCode, SHADOWMAP);
	}

	static bool hasMSAA(const unsigned short& curCode) {
		return withFunc(curCode, MSAA);
	}

	static bool hasSSAA(const unsigned short& curCode) {
		return withFunc(curCode, SSAA);
	}

	static bool hasBILINEAR(const unsigned short& curCode) {
		return withFunc(curCode, BILINEAR);
	}

};
