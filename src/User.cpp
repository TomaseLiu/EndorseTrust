#include "User.h"
#include "Task.h"
#include "MFModel.h"


void User::addTask(int taskID, float rating, int timestamp){
    EdgeNode3 *newTask = new EdgeNode3(taskID, rating, timestamp);
    task_rating.push_back(newTask);
    return;
}

void User::addEndorse(int endorseID, float initDegree, float tolerance){
    EdgeNode4 *newEndorse = new EdgeNode4(endorseID, DEGREE_DEFAULT, initDegree, tolerance);
    endorse_degree.push_back(newEndorse);
    return;
}

void User::addEndorse(int endorseID){
    EdgeNode4 *newEndorse = new EdgeNode4(endorseID, DEGREE_DEFAULT, INIT_DEGREE_DEFAULT, TOLERANCE_DEFAULT);
    endorse_degree.push_back(newEndorse);
    return;
}

void User::updateReputationScore(){
	float rs = 0;
	size_t i;
	for(i = 0; i < task_rating.size(); i++){
        float curRating = task_rating[i]->value;
		rs += curRating;
	}
	if(i != 0)
		rs = rs/i;
	reputationScore = rs * log(i+1);
	//reputationScore = rs;
}

float User::getReputationScore(){
    return reputationScore;
}

float User::getRi(TaskMap tm, int curTime, float alpha_well, float alpha_ill){
	int time = 0;
	float Ri = 0;
	for(size_t i = 0; i < task_rating.size(); i++){
		int taskID = task_rating[i]->key;
		if(tm.count(taskID) != 0){
			Task* curTask = tm[taskID];
			float aveRating = curTask->getAveRating();
			float curRating = task_rating[i]->value;
			time = curTime - task_rating[i]->attach;
			float alpha = curRating-aveRating > 0 ? alpha_well : alpha_ill;
            Ri += pow(alpha, time)*(curRating - aveRating);
		}
	}
	return Ri;
}

void User::updateEndorseDegree(size_t index, float Ri){
    float initDegree = endorse_degree[index]->attach1;
    float tolerance = endorse_degree[index]->attach2;
    float A = (0 > initDegree-tolerance) ? 0:initDegree-tolerance;
    float B = (1 < initDegree+tolerance) ? 1:initDegree+tolerance;

    float curDegree = A + (B-A)/(1+exp(-1*Ri));

    endorse_degree[index]->value = curDegree;
}

void User::updateEndorseDegrees(TaskMap tm, int curTime, float alpha_well, float alpha_ill){
    for(size_t i = 0; i < endorse_degree.size(); i++){
        float Ri = getRi(tm, curTime, alpha_well, alpha_ill);
        updateEndorseDegree(i, Ri);
    }
}

float User::getEndorseDegree(size_t index){
	if(index >= endorse_degree.size()){
		cout << "out of range" << endl;
		return 0;
	}
    return endorse_degree[index]->value;
}

float User::getToC(int taskID,  UserMap um, MFModel *mfModel){
    float RS = getReputationScore();
    float ToC = RS;
    float tmpValue = 0;
    for(size_t i = 0; i < endorse_degree.size(); i++){
        //cout << "userID: " << id << endl;
        int curEndorUserID = endorse_degree[i]->key;
        if(um.count(curEndorUserID) != 0){
            User *curEndorUser = um[curEndorUserID];
            float endorPreRating = mfModel->predictRating(curEndorUser->id, taskID);
            float EndorseDegree = getEndorseDegree(i);
            float RS = curEndorUser->getReputationScore();
            ToC += RS * EndorseDegree * endorPreRating;
            /*
            cout << "endorser: " << curEndorUserID << "   ";
            cout << "enRS: " << RS;
            cout << ";   " << "enDegree: " << EndorseDegree;
            cout << ";   " << "enPreRating: " << endorPreRating;
            cout << endl;
            */
        }
    }
    if(ToC != RS)
        cout << "userID: " << id <<  "  RS: " << RS << "  " << "endorValue: " << ToC - RS << endl;
    //cout << endl << endl;
    return ToC;
}

void User::updateSkills(vector<float> vSkl){
    if(vSkl.size() != skillLen){
        cout << "wrong length of skill vector: " << vSkl.size() << " " << skillLen << endl;
        return;
    }
	skills = vSkl;
}
