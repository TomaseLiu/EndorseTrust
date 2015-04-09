#include <iostream>

#include "User.h"
#include "Task.h"
#include "MFModel.h"
#include "experiment.h"
#include "generateData.h"

using namespace std;

int main()
{
    const char* userFilePath = "UserSkills.txt";
    const char* taskFilePath = "TaskSkills.txt";
    const char* endorseFilePath = "Endorsement.txt";

    int skillLen = 7;
    int userNum = 10;
    int taskNum = 20;
    float maxRating = 1;
    float tolerance = 1.0;

    int taskTimes = 2;
    int totalTimes = 1;
    int TopN = 10;

    vector<float> aveValueWith(taskTimes*taskNum, 0);
    vector<float> aveValueWithout(taskTimes*taskNum, 0);

    vector<float> aveRatingResultWithEndorse;
    vector<float> aveRatingResultWithoutEndorse;

for(int k = 0; k < totalTimes; k++){

    generateSkillData(skillLen, maxRating, userFilePath, userNum, false);
    generateSkillData(skillLen, maxRating, taskFilePath, taskNum, true);

    generateEndorseData(userFilePath, endorseFilePath, skillLen, tolerance, maxRating);

    cout << "generage data done" << endl;

    UserMap um;
    TaskMap tm;

    MFModel mf(30, 0.4, 0.4, 10, 0.1);
    //const char *filePath = UserSkillFilePath;
    //int skillLen = SKILL_LEN_DEFAULT;
    InitUsers(um, mf, userFilePath, skillLen);

    //const char* taskFilePath = TaskSkillFilePath;
    InitTasks(tm, mf, taskFilePath, skillLen);

    InitUserEndorse(um, mf, endorseFilePath, skillLen);

    //InitMFModel(mf, userFilePath, taskFilePath, skillLen);
    //mf.showVectors();
    //cout << mf.predictRating(1, 1) << endl;;
    //cout << (um.begin()->second)->endorse_degree.size() << endl;

    cout << "Initialization done" << endl;

//    int taskTimes = 3;
//    int totalTimes = 10;
//    int TopN = 3;


    for(int i = 0; i < taskTimes; i++){
        cout << "round " << i << " begin~" << endl;
        float aveRatingWithoutEndor = 0, aveRatingWithEndor = 0;
        int j = 0;
        for(map<int, Task*>::iterator it = tm.begin(); it != tm.end(); it++, j++){
            //cout << "task: " << it->first << endl;
            int TopNRand = rand()%TopN + 1;
            float aveTaskWith = oneTaskInWithoutEndorsement(it->second, um, tm, &mf, TopNRand, i);
            aveRatingWithoutEndor += aveTaskWith;
            float aveTaskWithout = oneTaskInWithEndorsement(it->second, um, tm, &mf, TopNRand, i);
            aveRatingWithEndor += aveTaskWithout;

            aveValueWith[i*taskNum + (j)] += aveTaskWith;
            aveValueWithout[i*taskNum + (j)] += aveTaskWithout;
            //cout << "task: " << it->first;
            //cout << " with Endorse: " << aveTaskWith;
            //cout << " without Endorse: " << aveTaskWithout << endl;

        }
        aveRatingWithoutEndor /= (tm.size() != 0 ? tm.size() : 1);
        aveRatingWithEndor /= (tm.size() != 0 ? tm.size() : 1);

        aveRatingResultWithEndorse.push_back(aveRatingWithEndor);
        aveRatingResultWithoutEndorse.push_back(aveRatingWithoutEndor);

        mf.updateVectorWithNTimes(um);
        //mf.showVectors();
    }
}
    ofstream fout("exResult.txt");
    int yes = 0;
    for(size_t i = 0; i < aveRatingResultWithEndorse.size(); i++){
        //float a = aveValueWith[i]/totalTimes;
        //float b = aveValueWithout[i]/totalTimes;
        float a = aveRatingResultWithEndorse[i];
        float b = aveRatingResultWithoutEndorse[i];
        int c = a>b;
        if(c)
            yes ++;
        c = i;
        fout << c << "\t";
        fout << a << "\t";
        fout << b << endl;
    }
    fout.close();
    cout << "total: " << taskTimes*taskNum << endl;
    cout << "yes: " << yes << endl;
/*
    for(size_t i = 0; i < aveRatingResultWithEndorse.size(); i++){
        cout << aveRatingResultWithEndorse[i] << " ";
        cout << aveRatingResultWithoutEndorse[i] << endl;
    }
*/

    return 0;
}
