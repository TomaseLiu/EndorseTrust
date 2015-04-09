#ifndef CONF_H_INCLUDED
#define CONF_H_INCLUDED


/*--------------
MFModel Default
--------------*/
#define LATENT_LENGTH_DEFAULT       64
#define FGAMA_DEFAULT               0.4
#define FLAMBDA_DEFAULT             0.4
#define ITERTIMES_DEFAULT           10
#define INIT_VECTOR_VALUE_DEFAULT   0.25
#define DEGREE_DEFAULT              0.5
#define INIT_DEGREE_DEFAULT         0.5
#define TOLERANCE_DEFAULT           0.5

/*--------
MFModel Configuration
---------*/
#define LATENT_LENGTH       64
#define FGAMA               0.4
#define FLAMBDA             0.4
#define ITERTIMES           10
#define INIT_VECTOR_VALUE   0.25
#define DEGREE              0.5
#define INIT_DEGREE         0.5
#define TOLERANCE           0.5

#define SKILL_LENGTH_DEFAULT    10
#define RS_DEFAULT              1.0

/*-----------------
    FilePathConf
-----------------*/

#define ALPHA_WELL  0.7
#define ALPHA_ILL   0.8

#define EndorsementFilePath     "endorsement.txt"
#define TaskSkillFilePath       "taskSkill.txt"
#define UserSkillFilePath       "userSkill.txt"

/*------------
Class Default Configuration
------------*/
#define SKILL_LEN_DEFAULT       10
#define SKILL_VALUE_DEFAULT     0.5

/*
size_t  TaskTimes    =   0;
size_t  SkillLength  =   5;
char*   sEndorsementFilePath    =   "";
char*   sTaskSkillFilePath      =   "";
char*   sUserSkillFilePath      =   "";
*/







#endif // CONF_H_INCLUDED
