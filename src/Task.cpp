#include "Task.h"

float nextGaussian(float a,float b)  {
    float v0, v1, v2, s;
    float nextNextGaussian;     //int haveNextNextGaussian=0;
    do {
        do {
            v1 = 2 * (((float)rand()/RAND_MAX))-1;   // between -1.0 and 1.0
            v2 = 2 * (((float)rand()/RAND_MAX))-1;   // between -1.0 and 1.0
            s = v1 * v1 + v2 * v2;
        } while (s >= 1 || s == 0);
        float multiplier = float(sqrt(-2*log(s)/s));
        nextNextGaussian = v2 * multiplier;    //haveNextNextGaussian = 1;
        v0=v1 * multiplier;                      //均值为0，方差为1的标准正态分布
    }while(s >= 1 || s == 0);
    return v0*b+a;
}


void Task::addUser(int userID, float rating){
	EdgeNode *usr = new EdgeNode(userID, rating);
	user_rating.push_back(usr);
}

void Task::updateSkills(vector<float> vSkl){
	skills = vSkl;
}

float Task::giveRatings(vector<float> vUserSkill){
	if(vUserSkill.size() != skills.size()){
		cout << "The dimention of user_skill vector and task_skill vector are not matched!" << endl;
		return 0;
	}

	float rating = 0;
	for(size_t i = 0; i < skills.size(); i++){
		rating += skills[i] * vUserSkill[i];
	}
	rating = nextGaussian(rating, 0.0005);

	return rating;
}

float Task::getAveRating(){
	float aveRating = 0, sumRating = 0;
	for(size_t i = 0; i < user_rating.size(); i++){
        sumRating += user_rating[i]->value;
	}
	if(user_rating.size() != 0)
        aveRating = sumRating/user_rating.size();
    return aveRating;
}

void Task::clearRatings(){
    user_rating.clear();
}
