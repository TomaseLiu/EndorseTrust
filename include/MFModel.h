#ifndef MFMODEL_H
#define MFMODEL_H
#include "common.h"

class MFModel{
public:
    int userNum, itemNum;
    int latentLen;
    float fGama, fLambda;
    int iterTimes;
    float fInitVectorValue;

    mID2LatentVectors userVectors;
    mID2LatentVectors itemVectors;

    MFModel();
    MFModel(int latLen, float gama, float lambda, int itTimes, float initValue);

    ~MFModel(){}

    void addUser(int userID);

    void addItem(int itemID);

    //inline
    float predictRating(int userID, int itemID);

    void updateVectorsWithOneRating(int userID, int itemID, float rating);

    void updateVectorsWithOneUser(int userID, User* user);

    void updateVectorsWithOneTime(UserMap um);

    void updateVectorWithNTimes(UserMap um);

    void showVectors();
};


#endif // MFMODEL_H
