#include "MFModel.h"
#include "User.h"
#include "conf.h"


MFModel::MFModel(){
    userNum = 0;
    itemNum = 0;
    latentLen = LATENT_LENGTH_DEFAULT;
    fGama = FGAMA_DEFAULT;
    fLambda = FLAMBDA_DEFAULT;
    iterTimes = ITERTIMES_DEFAULT;
    fInitVectorValue = INIT_VECTOR_VALUE_DEFAULT;
}
MFModel::MFModel(int latLen, float gama, float lambda, int itTimes, float initValue){
    userNum = 0;
    itemNum = 0;
    latentLen = latLen;
    fGama = gama;
    fLambda = lambda;
    iterTimes = itTimes;
    fInitVectorValue = initValue;
}


void MFModel::addUser(int userID){
    if(userVectors.count(userID) != 0)
        return;
    LatentFactor *newUser = new LatentFactor(userID, latentLen, fInitVectorValue);
    userVectors[userID] = newUser;
    userNum ++;
}
void MFModel::addItem(int itemID){
    if(itemVectors.count(itemID) != 0)
        return;
    LatentFactor *newItem = new LatentFactor(itemID, latentLen, fInitVectorValue);
    itemVectors[itemID] = newItem;
    itemNum ++;
}

float MFModel::predictRating(int userID, int itemID){
    if(userVectors.count(userID) == 0 || itemVectors.count(itemID) == 0){
        cout << "wrong userID or itemID" << endl;
        return 0;
    }
    float preRating = 0;
    vector<float> userVector(userVectors[userID]->latentVector);
    vector<float> itemVector(itemVectors[itemID]->latentVector);
    for(int i = 0; i < latentLen; i++){
        preRating += userVector[i] * itemVector[i];
        //cout << userVector[i] << " " << itemVector[i] << endl;
    }
    //cout << "predict " << userID << " " << itemID << " " << preRating << endl;
    return preRating;
}

void MFModel::updateVectorsWithOneRating(int userID, int itemID, float rating){
    addUser(userID);
    addItem(itemID);

    vector<float> &userFactors = userVectors[userID]->latentVector;
    vector<float> &itemFactors = itemVectors[itemID]->latentVector;

    float eui = rating - predictRating(userID, itemID);
    //cout << rating << " " << predictRating(userID, itemID) << " ";
    //cout << userID << itemID << " eui: " << eui << endl;//
    //for(int i = 0; i < latentLen; i++){
    for(int i = 0; i < latentLen; i++){
        //cout << "fGama " << fGama << " fLambda: " << fLambda << endl;

        userFactors[i] += fGama*(eui*itemFactors[i] - fLambda*userFactors[i]);
        //userVectors[userID]->latentVector[i] += fGama*(eui*itemVectors[itemID]->latentVector[i] - fLambda*userVectors[userID]->latentVector[i]);
        //cout << "userFactors: " << userFactors[i] << endl;

        itemFactors[i] += fGama*(eui*userFactors[i] - fLambda*itemFactors[i]);
        //itemVectors[itemID]->latentVector[i] += fGama*(eui*userVectors[userID]->latentVector[i] - fLambda*itemVectors[itemID]->latentVector[i]);
    }
    //cout << userID << " " << userFactors[0] << endl;
}

//inline
void MFModel::updateVectorsWithOneUser(int userID, User* user){
    vector<EdgeNode3*> ratings = user->task_rating;
    for(size_t i = 0; i < ratings.size(); i++){
        int curItemID = ratings[i]->key;
        int curRating = ratings[i]->value;
        updateVectorsWithOneRating(userID, curItemID, curRating);
    }
}

void MFModel::updateVectorsWithOneTime(UserMap um){
    map<int, User*>::iterator umIter ;
    for(umIter = um.begin(); umIter != um.end(); umIter++){
        int userID = umIter->first;
        User *user = umIter->second;
        updateVectorsWithOneUser(userID, user);
    }
}

void MFModel::updateVectorWithNTimes(UserMap um){
    for(int i = 0; i < iterTimes; i++){
        updateVectorsWithOneTime(um);
        fGama *= 0.9;
    }
}

void MFModel::showVectors(){
    map<int, LatentFactor*>::iterator itvUser = userVectors.begin();
    map<int, LatentFactor*>::iterator itvItem = itemVectors.begin();
    for(itvUser, itvItem; itvUser != userVectors.end() && itvItem != itemVectors.end(); itvUser++, itvItem++){
        vector<float> curUserVector = itvUser->second->latentVector;
        vector<float> curItemVector = itvItem->second->latentVector;
        cout << "user:" << itvUser->first;
        for(size_t i = 0; i < curUserVector.size(); i++){
            cout << " " << curUserVector[i];
        }
        cout << endl;

        cout << "item:" << itvItem->first;
        for(size_t i = 0; i < curItemVector.size(); i++){
            cout << " " << curItemVector[i];
        }
        cout << endl;
        cout << endl;
    }
}
