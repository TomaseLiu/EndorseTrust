#ifndef EXPERIMENT_H_INCLUDED
#define EXPERIMENT_H_INCLUDED
#include <fstream>
#include "User.h"
#include "Task.h"
#include "MFModel.h"
#include <algorithm>
#include "generateData.h"
//#include "common.h"

using namespace std;

bool myEdgeNodeCompare(EdgeNode *i,EdgeNode *j) {
    return (i->value > j->value);
}

void InitUsers(UserMap &um, MFModel &mf, const char* skillFilePath, int skillLen){
    um.clear();

    ifstream finSkill(skillFilePath);
    if(!finSkill){
        cout << "userSkill file not exit" << endl;
        return;
    }
    int userID = 0;
    float value = 0;
    vector<float> curSkill;
    while(finSkill >> userID){
        curSkill.clear();
        for(int i = 0; i < skillLen; i++){
            finSkill >> value;
            curSkill.push_back(value);
        }
        if(um.count(userID) == 0){
            um[userID] = new User(userID, skillLen);
            um[userID]->updateSkills(curSkill);
            um[userID]->initReputationScore(aveValue(curSkill));
            //cout << um[userID]->reputationScore << endl;
        }
        mf.addUser(userID);
    }
    finSkill.close();
    return;
}
void InitUserEndorse(UserMap &um, MFModel &mf, const char* endorsementFileilePath, int skillLen){
    ifstream finEndorse(endorsementFileilePath);
    if(!finEndorse){
        cout << "file not exit" << endl;
        return;
    }
    int endorserID = 0, userID = 0;
    float initDegree = 0, tolerance = 0;
    string tmpStr;
    while(finEndorse >> userID >> endorserID >> initDegree >> tolerance){
    //while(getline(finEndorse, tmpStr)){
        if(um.count(userID) == 0)
            um[userID] = new User(userID, skillLen);
        if(um.count(endorserID) == 0)
            um[endorserID] = new User(endorserID);

        um[userID]->addEndorse(endorserID, initDegree, tolerance);
        mf.addUser(userID);
        mf.addUser(endorserID);
    }
    finEndorse.close();
    return;
}

void InitTasks(TaskMap &tm, MFModel &mf, const char *skillFilePath, int skillLen){
    tm.clear();

    ifstream finSkill(skillFilePath);
    if(!finSkill){
        cout << "taskSkill file not exit" << endl;
        return;
    }
    int taskID = 0;
    float value = 0;
    vector<float> curSkill;
    while(finSkill >> taskID){
        curSkill.clear();
        for(int i = 0; i < skillLen; i++){
            finSkill >> value;
            curSkill.push_back(value);
        }
        if(tm.count(taskID) == 0){
            tm[taskID] = new Task(taskID, skillLen);
            tm[taskID]->updateSkills(curSkill);
        }
        mf.addItem(taskID);
    }

    finSkill.close();
    return;
}

void InitMFModel(MFModel &mf, const char* userSkillFilePath, const char* itemSkillFilePath, int skillLen){
    ifstream finUser(userSkillFilePath);
    int userID;
    while(finUser >> userID){
        mf.addUser(userID);
        float fSkill;
        for(int i = 0; i < skillLen; i++){
            finUser >> fSkill;
        }
    }
    finUser.close();

    ifstream finItem(itemSkillFilePath);
    int itemID;
    while(finItem >> itemID){
        mf.addItem(itemID);
        float fSkill;
        for(int i = 0; i < skillLen; i++){
            finItem >> fSkill;
        }
    }
    finItem.close();
}


float oneTaskInWithEndorsement(Task *curTask, UserMap um, TaskMap tm, MFModel *mf, size_t topN, int timestamp){
    if(tm.count(curTask->id) == 0)
        tm[curTask->id] = curTask;
    //----------------get ToC----------------;
    map<int, User*>::iterator iterMap;
    vector<EdgeNode*> ToCList;
    for(iterMap = um.begin(); iterMap != um.end(); iterMap++){
        float toc = ((iterMap)->second)->getToC(curTask->id, um, mf);
        EdgeNode *newEdgeNode = new EdgeNode(iterMap->first, toc);
        ToCList.push_back(newEdgeNode);
    }
    sort(ToCList.begin(), ToCList.end(), myEdgeNodeCompare);


    //--------give Ratings to selected workers-------;
    float aveRatingOfThisTask = 0;
    size_t i;
    for(i = 0; i < ToCList.size() && i < topN; i++){
        User *curUser = um[ToCList[i]->key];
        vector<float> curSkill = curUser->skills;
        float curRating = curTask->giveRatings(curSkill);
        aveRatingOfThisTask += curRating;
        //cout << curTask->id << " give " << curUser->id << " rating: " << curRating << endl;
        //add new task to current user;
        curUser->addTask(curTask->id, curRating, timestamp);
        //update reputation score of current user;
        curUser->updateReputationScore();
        //update endorse degree list of current user;
        curUser->updateEndorseDegrees(tm, timestamp, ALPHA_WELL, ALPHA_ILL);
        //update the data of the current task;
        curTask->addUser(curUser->id, curRating);

        cout << "with: task: " << curTask->id << "  user: " << curUser->id << endl;

    }
    if(i!=0)
        aveRatingOfThisTask = aveRatingOfThisTask/i;
    return aveRatingOfThisTask;
}

float oneTaskInWithoutEndorsement(Task *curTask, UserMap um, TaskMap tm, MFModel *mf, size_t topN, int timestamp){
    if(tm.count(curTask->id) == 0)
        tm[curTask->id] = curTask;
    //----------------get ToC----------------;
    map<int, User*>::iterator iterMap;
    vector<EdgeNode*> ToCList;
    for(iterMap = um.begin(); iterMap != um.end(); iterMap++){
        float toc = ((iterMap)->second)->getReputationScore();
        EdgeNode *newEdgeNode = new EdgeNode(iterMap->first, toc);
        ToCList.push_back(newEdgeNode);
    }
    sort(ToCList.begin(), ToCList.end(), myEdgeNodeCompare);

    //--------give Ratings to selected workers-------;
    float aveRatingOfThisTask = 0;
    size_t i;
    for(i = 0; i < ToCList.size() && i < topN; i++){
        User *curUser = um[ToCList[i]->key];
        vector<float> curSkill = curUser->skills;
        float curRating = curTask->giveRatings(curSkill);
        aveRatingOfThisTask += curRating;
        //add new task to current user;
        curUser->addTask(curTask->id, curRating, timestamp);
        //update reputation score of current user;
        curUser->updateReputationScore();
        //update endorse degree list of current user;
        curUser->updateEndorseDegrees(tm, timestamp, ALPHA_WELL, ALPHA_ILL);
        //update the data of the current task;
        curTask->addUser(curUser->id, curRating);
        cout << "without: task: " << curTask->id << "  user: " << curUser->id << endl;
    }
    if(i!=0)
        aveRatingOfThisTask = aveRatingOfThisTask/i;
    return aveRatingOfThisTask;
}

//float getAveRatingOfAllTasks()
#endif // EXPERIMENT_H_INCLUDED
