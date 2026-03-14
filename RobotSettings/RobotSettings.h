#pragma once

#include "Vector2D.h"
#include<tuple>

using ChronoDuration = std::chrono::duration<float>;
using SettingsGroup = std::tuple<bool, MappedSpace, ChronoDuration>;  // <-- STEP 1

/*
* EXAMPLE:
*
* You want to add a new setting of type FLOAT which is going to represent the total |DISTANCE TRAVELLED BY THE ROBOT| :
*
* ===============================================================
* STEP 1 : Add a new FLOAT TYPE parameter to the SETTINGS GROUP :
* ===============================================================
* 
* using SettingsGroup = std::tuple<bool, MappedSpace, ChronoDuration, float>;
*
* =====================================================================================
* STEP 2 : Add an intuitive name to the SETTINGS COLLECTION ( the enum ESettingsType ):
* =====================================================================================
* 
* enum ESettingsType : int8_t {
*    USE_INIT,
*    MAPPED_SPACE,
*    DELAY_STEP,
*    TOTAL_DISTANCE
* };
*
* ======================================================================
* STEP 3 : You ADD the VARIABLE to the "Settings declaration section:" :
* ======================================================================
*
* private:
*   float TotalDistanceTravelled = 0.f;
*
* ====================================================================================
* STEP 4 (THE LAST STEP) : You add it to the strict SettingsGroup, called "Settings" :
* ====================================================================================
*
* SettingsGroup Settings = { bUseInit_EVENT, mappedSpace, Delay_Step, TotalDistanceTravelled };
* 
*/

enum ESettingsType : int8_t { // <-- STEP 2
    USE_INIT,
    MAPPED_SPACE,
    DELAY_STEP
};

struct R_Settings
{
private:

    //Settings declaration section: // <-- STEP 3

    bool bUseInit_EVENT{ true };

    MappedSpace mappedSpace = {
        Vector2D<double>(0.f, 1000.f),
        Vector2D<double>(0.f, 1000.f)
    };

    std::chrono::duration<float> Delay_Step = 0.0s;

    //==============================

public:

    SettingsGroup Settings = { bUseInit_EVENT, mappedSpace, Delay_Step }; // <-- STEP 4

};
