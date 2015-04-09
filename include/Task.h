#ifndef TASK_H
#define TASK_H
#include "common.h"
class Task{
public:
	int id;
	vector<float> skills;
	size_t skillLen;
	vector<EdgeNode*> user_rating;

	Task(int taskID){
        id = taskID;
        skillLen = SKILL_LEN_DEFAULT;
        vector<float> initSkill(skillLen, SKILL_VALUE_DEFAULT);
        skills = initSkill;
	}
	Task(int taskID, int skLength){
        id = taskID;
        skillLen = skLength;
        vector<float> initSkill(skillLen, SKILL_VALUE_DEFAULT);
        skills = initSkill;
	}
    ~Task(){}

	void addUser(int userID, float rating);

	void updateSkills(vector<float> vSkl);

	float giveRatings(vector<float> vUserSkill);

	//inline
	float getAveRating();

	void clearRatings();

};

#endif // TASK_H
