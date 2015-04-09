#ifndef USER_H
#define USER_H
#include "common.h"
class User{
public:
	int id;
	float reputationScore;
	vector<float> skills;
	size_t skillLen;
	vector<EdgeNode3*> task_rating;
	vector<EdgeNode4*> endorse_degree;

	User(int userID){
        id = userID;
        reputationScore = RS_DEFAULT;
        skillLen = SKILL_LEN_DEFAULT;
        vector<float> initSkill(skillLen, SKILL_VALUE_DEFAULT);
        skills = initSkill;
	}
	User(int userID, int skLength){
        id = userID;
        reputationScore = RS_DEFAULT;
        skillLen = skLength;
        vector<float> initSkill(skillLen, SKILL_VALUE_DEFAULT);
        skills = initSkill;
	}

/*
    User(int userID, int skLength, float RS){
        id = userID;
        reputationScore = RS;
        skillLen = skLength;
        vector<float> initSkill(skillLen, SKILL_VALUE_DEFAULT);
        skills = initSkill;
    }
*/
	User(): id(0){}
	~User(){}

    void initReputationScore(float RS){
        reputationScore = RS;
    }

	void addTask(int taskID, float rating, int timestamp);

	void addEndorse(int endorseID, float initDegree, float tolerance);
    void addEndorse(int endorseID);

    void updateReputationScore();
	float getReputationScore();

	float getRi(TaskMap tm, int curTime, float alpha_well, float alpha_ill);

    float getEndorseDegree(size_t index);

    void updateEndorseDegree(size_t index, float ri);
    void updateEndorseDegrees(TaskMap tm, int curTime, float alpha_well, float alpha_ill);

	float getToC(int taskID, UserMap um, MFModel *mfModel);

	void updateSkills(vector<float> vSkl);
};


#endif // USER_H
