﻿#include "filesetting.h"

#define SS(SET_FUNCTION) CSimpleIniA ini;\
static char ss[DEFPATHSIZE];\
memset(ss, sizeof(ss), 0);\
snprintf(ss, sizeof(ss), s, id);\
ini.SetUnicode(true);\
if(ini.LoadFile(INITFILE) < 0)\
printf("New File!\n");\
ini.SET_FUNCTION(ss, k, n);\
ini.SaveFile(INITFILE);

#define SG(GET_FUNCTION,RETURN_TYPE) CSimpleIniA ini;\
static char ss[DEFPATHSIZE];\
memset(ss, sizeof(ss), 0);\
snprintf(ss, sizeof(ss), s, id);\
ini.SetUnicode(true);\
if (ini.LoadFile(INITFILE) < 0)\
return RETURN_TYPE(n);\
return RETURN_TYPE(ini.GET_FUNCTION(ss, k, n));


//using SI_Error = int;
//constexpr int SI_OK = 0;        //!< No error
//constexpr int SI_UPDATED = 1;   //!< An existing value was updated
//constexpr int SI_INSERTED = 2;  //!< A new value was inserted
//// note: test for any error with (retval < 0)
//constexpr int SI_FAIL = -1;     //!< Generic failure
//constexpr int SI_NOMEM = -2;    //!< Out of memory error
//constexpr int SI_FILE = -3;     //!< File error (see errno for detail error)


void FileSetting::SetValue(int id, const char* s, const char* k, const char* wn){
    std::string str = wn;
    std::string target = " ";
    std::string replacement = "~SPACE~";

    size_t pos = 0;
    while ((pos = str.find(target, pos)) != std::string::npos) {
        str.replace(pos, target.length(), replacement);
        pos += replacement.length();
    }

    const char* n = str.c_str();
    SS(SetValue);
}
std::string FileSetting::GetValue(int id, const char* s, const char* k, const char* n){
    CSimpleIniA ini;
    static char ss[DEFPATHSIZE];
    memset(ss, sizeof(ss), 0);
    snprintf(ss, sizeof(ss), s, id);
    ini.SetUnicode(true);
    if (ini.LoadFile(INITFILE) < 0)
        return std::string(n);
    std::string str = ini.GetValue(ss, k, n);
    std::string target = "~SPACE~";
    std::string replacement = " ";
    size_t pos = 0;
    while ((pos = str.find(target, pos)) != std::string::npos) {
        str.replace(pos, target.length(), replacement);
        pos += replacement.length();
    }
    return str;
}
void FileSetting::SetLongValue(int id, const char* s, const char* k, int n){
    SS(SetLongValue)
}
int FileSetting::GetLongValue(int id, const char* s, const char* k, int n){
    SG(GetLongValue,int)
}
void FileSetting::SetDoubleValue(int id, const char* s, const char* k, double n){
    SS(SetDoubleValue)
}
double FileSetting::GetDoubleValue(int id, const char* s, const char* k, double n){
    SG(GetDoubleValue,double)
}
void FileSetting::SetBoolValue(int id, const char* s, const char* k, bool n){
    SS(SetBoolValue)
}
bool FileSetting::GetBoolValue(int id, const char* s, const char* k, bool n){
    SG(GetBoolValue, bool)
}

void FileSetting::ClearFile()
{
    if (std::remove(INITFILE) == 0) {
        std::printf("File %s deleted successfully.\n", INITFILE);
    }
    else {
        perror("Error deleting file");
    }
}

