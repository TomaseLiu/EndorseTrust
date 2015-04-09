#ifndef COMMON_H
#define COMMON_H
#include <map>
#include <vector>
#include <iostream>
#include <math.h>
#include "conf.h"
#include <algorithm>
using namespace std;

#define float double


class LatentFactor;
class User;
class Task;
class MFModel;
typedef map<int, Task*> TaskMap;
typedef map<int, User*> UserMap;
typedef map<int, LatentFactor*> mID2LatentVectors;

struct LatentFactor{
    int ID;
    vector<float> latentVector;
    LatentFactor(int id): ID(id){}
    LatentFactor(int id, int n, float value){
        ID = id;
        for(int i = 0; i < n; i++){
            latentVector.push_back(value);
        }
    }
};

struct EdgeNode{
	int key;
	float value;

	EdgeNode(int x, float y): key(x), value(y){}

	//inline bool operator<(EdgeNode en);
};

struct EdgeNode3{
	int key;
	float value;
	int attach;

	EdgeNode3(int x, float y, int z): key(x), value(y), attach(z){}

	inline bool operator<(EdgeNode3 en3);
};

struct EdgeNode4{
	int key;
	float value;
	float attach1;
	float attach2;

	EdgeNode4(int x, float y, float z, float w): key(x), value(y), attach1(z), attach2(w){}

	inline bool operator<(EdgeNode4 en4);
};

#endif // COMMON_H
