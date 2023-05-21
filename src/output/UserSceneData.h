#pragma once
#include<utility>
#include<tuple>
#include<queue>
#include<mutex>
#include<string>
using std::queue;
using std::mutex;
using std::lock_guard;
using std::pair;
using std::string;
struct userChange {
	//用户改变的场景信息 如鼠标拖动改变相机位置 光照信息等
	
	//short为0表示鼠标信息当前无变化 
	// 1表示相机旋转事件 2表示相机靠近或远离事件
	// 3相机观察点移动事件 4光源移动事件 
	// 5表示导入新模型
	//short为1时 第二个pair为鼠标上次位置到这次位置在x y轴上分别移动的距离
	//short为2时 第二个pair的first为滚轮方向 second为滑动次数
	//short为3时 同1 
	//short为4时第二个pair为光源在x y轴上分别移动次数
	//short为5时 第二个pair不用管 file里存新模型的地址
	pair<short, pair<int, int>> data = { 0,{0,0}};
	string file;//模型文件地址



	userChange() = default;
	userChange(pair<short, pair<int, int>> d,const string& f = "") :
		data(d), file(f) {}
};
class UserSceneData
{
	static queue<userChange> changes;
	static mutex changesMutex;

public:
	static void pushChange(userChange change) {
		//此处加锁
		lock_guard<mutex> changesLock(changesMutex);
		changes.push(change);
	}

	static userChange getChange() {
		//此处加锁
		lock_guard<mutex> changesLock(changesMutex);
		if (changes.empty())
			return userChange();
		userChange topChange = changes.front();
		changes.pop();
		return topChange;
	}
};

