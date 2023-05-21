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
	//�û��ı�ĳ�����Ϣ ������϶��ı����λ�� ������Ϣ��
	
	//shortΪ0��ʾ�����Ϣ��ǰ�ޱ仯 
	// 1��ʾ�����ת�¼� 2��ʾ���������Զ���¼�
	// 3����۲���ƶ��¼� 4��Դ�ƶ��¼� 
	// 5��ʾ������ģ��
	//shortΪ1ʱ �ڶ���pairΪ����ϴ�λ�õ����λ����x y���Ϸֱ��ƶ��ľ���
	//shortΪ2ʱ �ڶ���pair��firstΪ���ַ��� secondΪ��������
	//shortΪ3ʱ ͬ1 
	//shortΪ4ʱ�ڶ���pairΪ��Դ��x y���Ϸֱ��ƶ�����
	//shortΪ5ʱ �ڶ���pair���ù� file�����ģ�͵ĵ�ַ
	pair<short, pair<int, int>> data = { 0,{0,0}};
	string file;//ģ���ļ���ַ



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
		//�˴�����
		lock_guard<mutex> changesLock(changesMutex);
		changes.push(change);
	}

	static userChange getChange() {
		//�˴�����
		lock_guard<mutex> changesLock(changesMutex);
		if (changes.empty())
			return userChange();
		userChange topChange = changes.front();
		changes.pop();
		return topChange;
	}
};

