#ifndef GENERATEDATA_H_INCLUDED
#define GENERATEDATA_H_INCLUDED

#include <math.h>
#include <fstream>
#include <iostream>
#include "User.h"
#include "Task.h"

float similarity(vector<float> a, vector<float> b){
    float fSimilarity = 0;
    float total1 = 0, total2;
    for(size_t i = 0; i < a.size(); i++){
        fSimilarity += a[i] * b[i];
        total1 += a[i]*a[i];
        total2 += b[i]*b[i];
    }
    fSimilarity /= sqrt(total1)*sqrt(total2) == 0 ? 1 : sqrt(total1)*sqrt(total2);
    return fSimilarity;
}

float aveValue(vector<float> a){
    float ave = 0;
    size_t i;
    for(i = 0; i < a.size(); i++){
        ave += a[i];
    }
    ave /= (i==0 ? 1 : i);
    return ave;
}

void generateSkillData(int skillLength, float maxRating, const char* filePath, int userNum, bool normolize){
    ofstream fout(filePath);
    float maxSkillValue = (maxRating/skillLength);
    if(!normolize){
        for(int i = 1; i <= userNum; i++){
            fout << i;
            for(int j = 0; j < skillLength; j++){
                float skillValue = (float)rand()/RAND_MAX * maxRating;
                //cout << skillValue << endl;
                fout << "\t" << skillValue;
            }
            fout << endl;
        }
    }else{
        for(int i = 1; i <= userNum; i++){
            vector<float> vSkill(skillLength, 0);
            for(int j = 0; j < skillLength; j++){
                vSkill[j] = float(rand()%10);
            }
            fout << i;
            float ave = aveValue(vSkill);
            for(int j = 0; j < skillLength; j++){
                vSkill[j] /= ave*skillLength;
                fout << "\t" << vSkill[j];
            }
            fout << endl;
        }
    }
    fout.close();
}

void generateEndorseData(const char* userSkillFilePath, const char* endorsementFilePath, int skillLen, float fTolerance, float maxRating){
    ifstream fin(userSkillFilePath);
    map<int, vector<float> > mUserSkills;
    int userID;
    while(fin >> userID){
        vector<float> curSkills;
        mUserSkills[userID] = curSkills;
        for(int i = 0; i < skillLen; i++){
            float skillValue;
            fin >> skillValue;
            mUserSkills[userID].push_back(skillValue);
        }
    }
    fin.close();



    vector<EdgeNode4*> vEndorsement;

    map<int, vector<float> >::iterator itFirst;
    map<int, vector<float> >::iterator itSecond;
    for(itFirst = mUserSkills.begin(); itFirst != mUserSkills.end(); itFirst++){
        for(itSecond = mUserSkills.begin(); itSecond != mUserSkills.end(); itSecond++){
            if(itFirst == itSecond)
                continue;
            //float fSimilarity = similarity(itFirst->second, itSecond->second) * aveValue(itFirst->second);
            float fSimilarity = similarity(itFirst->second, itSecond->second) * aveValue(itFirst->second);
            float randValue = (float)rand()/RAND_MAX * maxRating;
            if(randValue < fSimilarity/1){
                int UserID = itFirst->first;
                int Endorser = itSecond->first;
                float initDegree = INIT_DEGREE;
                float tolerance = TOLERANCE;
                //float tolerance = fSimilarity * fTolerance;
                EdgeNode4 *newEndorsement = new EdgeNode4(UserID, Endorser, initDegree, tolerance);
                vEndorsement.push_back(newEndorsement);
            }
        }
    }


    ofstream fout(endorsementFilePath);
    for(size_t i = 0; i < vEndorsement.size(); i++){
        fout << vEndorsement[i]->key << "\t";
        fout << vEndorsement[i]->value << "\t";
        fout << vEndorsement[i]->attach1 << "\t";
        fout << vEndorsement[i]->attach2 << endl;
    }
    fout.close();

}
#endif // GENERATEDATA_H_INCLUDED
