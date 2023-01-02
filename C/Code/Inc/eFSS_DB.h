/**
 * @file       eFSS_DB.h
 *
 * @brief      High level utils for fail safe storage
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSS_DB_H
#define EFSS_DB_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_UTILSLLPRV.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSS_DB_RES_OK = 0,
    e_eFSS_DB_RES_BADPARAM,
    e_eFSS_DB_RES_BADPOINTER,
    e_eFSS_DB_RES_CLBCKREPORTERROR,
    e_eFSS_DB_RES_NOTVALIDPAGE
}e_eFSS_DB_RES;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Erase a specified page
 *
 * @param[in]   p_ptCtx       - Erase function context
 * @param[in]   p_uPageIndx   - Page index we want to erase
 * @param[in]   p_uReTry      - Erase function pointer
 *
 * @return      e_eFSS_UTILSLLPRV_RES_OK                - Operation ended successfully
 *              e_eFSS_UTILSLLPRV_RES_BADPOINTER        - In case of bad pointer passed to the function
 *              e_eFSS_UTILSLLPRV_RES_CLBCKREPORTERROR  - Error reported from the callback
 */
e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_GetMetaFromBuff(const uint8_t* pageBuff, const uint32_t p_pageL,
                                                      t_eFSS_TYPE_PageMeta* const pagePrm);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_DB_H */












class InOutFlashManager
{
  public:
    InOutFlashManager();
    ~InOutFlashManager();

    bool init();

    bool saveTempProbeSettings(uint64_t tank_n_wire, uint64_t tank_use470ohm);
    bool getTempProbeSettings(uint64_t *tank_n_wire, uint64_t *tank_use470ohm);

    bool saveDigitalOutLifeSpawn(uint64_t guarantyCycle, uint64_t guarantyYears);
    bool getDigitalOutLifeSpawn(uint64_t *guarantyCycle, uint64_t *guarantyYears);

    bool saveAnalogInputCalibrationValue(ANALOG_INPUT_TYPE inputType, float calibValue);
    bool getAnalogInputCalibrationValue(ANALOG_INPUT_TYPE inputType,  float* calibValue);


private:

	bool isValidPageData(bool isMainPage);
	bool createBackUpPageData();
	bool regenerateMainPageData();
	bool integrityCreatorData();
   bool verifyParameterVersion();
	bool saveData(DEVICE_INOUT_FLASH *newDataToSave, bool isMainPage);
};
