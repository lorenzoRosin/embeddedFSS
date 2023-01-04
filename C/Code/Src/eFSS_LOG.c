/**
 * @file       eFSS_DB.c
 *
 * @brief      High level utils for fail safe storage
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS_DB.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_LOG_IsStatusStillCoherent(const t_eFSS_LOG_Ctx* p_ptCtx);
static bool_t eFSS_LOG_LoadIndex(const t_eFSS_LOG_Ctx* p_ptCtx);
static bool_t eFSS_LOG_LoadIndexFromCache(const t_eFSS_LOG_Ctx* p_ptCtx);
static bool_t eFSS_LOG_LoadIndexBySearch(const t_eFSS_LOG_Ctx* p_ptCtx);
static bool_t eFSS_LOG_VerifyVersion(const t_eFSS_LOG_Ctx* p_ptCtx);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_LOG_RES eFSS_LOG_InitCtx(t_eFSS_LOG_Ctx* const p_ptCtx, t_eFSS_TYPE_CbCtx* const p_ptCtxCb, uint32_t p_uPageToUse,
                               uint32_t p_uPageSize, uint8_t* p_puBuff, uint32_t p_uBuffL)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptCtxCb ) || ( NULL == p_puBuff )   )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( ( p_uPageToUse < 4u ) || ( p_uPageSize < EFSS_PAGEMETASIZE )|| ( p_uBuffL < ( p_uPageSize * 2u ) ) )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {
            p_ptCtx->bIsInitCtx = true;
            p_ptCtx->p_ptCtxCb = p_ptCtxCb;
            p_ptCtx->puBuff = p_puBuff;
            p_ptCtx->uBuffL = p_uBuffL;
            p_ptCtx->uPageSize = p_uPageSize;
            p_ptCtx->uTotalPage = p_uPageToUse;
            l_eRes = e_eFSS_UTILSHLPRV_RES_OK;
        }
	}


    return l_eRes;
}

e_eFSS_LOG_RES eFSS_LOG_IsInit(t_eFSS_LOG_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSS_LOG_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eCU_BUNSTF_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInitCtx;
        l_eRes = e_eCU_BUNSTF_RES_OK;
	}

	return l_eRes;
}



e_eFSS_LOG_RES eFSS_LOG_Format(t_eFSS_BLOB_Ctx* const p_ptCtx)
{
    if(cached)
    {
        /* erase the cache */
    }
    else
    {
        /* Erase first two index */
    }


}


/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_LOG_IsStatusStillCoherent(const t_eFSS_LOG_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check basic context validity */
	if( ( p_ptCtx->uBuffL <= 0u ) || ( NULL == p_ptCtx->puBuff ) )
	{
		l_eRes = false;
	}
	else
	{
        /* Check size validity */
        if( p_ptCtx->uFrameCtr > p_ptCtx->uBuffL )
        {
            l_eRes = false;
        }
        else
        {
            /* Check status coherence */
            if( ( e_eCU_BUNSTFPRV_SM_NEEDSOF == p_ptCtx->eSM ) && ( 0u != p_ptCtx->uFrameCtr ) )
            {
                l_eRes = false;
            }
            else
            {
                /* Check status coherence */
                if( ( e_eCU_BUNSTFPRV_SM_UNSTUFFEND == p_ptCtx->eSM ) && ( p_ptCtx->uFrameCtr <= 0u ) )
                {
                    l_eRes = false;
                }
                else
                {
                    l_eRes = true;
                }
            }

        }
	}

    return l_eRes;
}

static bool_t eFSS_LOG_LoadIndex(const t_eFSS_LOG_Ctx* p_ptCtx)
{
    if( true == p_ptCtx->bUseIndexingFlashCache )
    {
        /* Load index from cache */
        eFSS_LOG_LoadIndexFromCache();
    }
    else
    {
        /* verify if current index are ok, if not search them */
        eFSS_LOG_LoadIndexBySearch();
    }

    return;
}

static bool_t eFSS_LOG_LoadIndexFromCache(const t_eFSS_LOG_Ctx* p_ptCtx)
{
    /* Verify integrity */
    e_eFSS_Res l_eIntegrityRes;
    l_eIntegrityRes =  verifyAndRipristinateBkup(pageOrig);

    if( ( EFSS_RES_OK == l_eIntegrityRes ) || ( EFSS_RES_OK_BKP_RCVRD == l_eIntegrityRes ) )
    {
        /* Index page is OK, can retrive */
        newestpageindx = ;
        oldestPageinx = ;

        /* Verify coherence of index bla bla */
    }
    else
    {
        /* Some other error, not possible to find index */
    }

    return;
}

static bool_t eFSS_LOG_LoadIndexBySearch(const t_eFSS_LOG_Ctx* p_ptCtx)
{
    /* Verify integrity newest page */
    eFSS_UTILSHLPRV_IsValidPage(t_eFSS_TYPE_CbCtx* const p_ptCtx, const uint32_t p_uPageIdx,
                                                  uint8_t* const pageBuff, const uint32_t p_pageL, const uint32_t p_uReTry );

    if( true == valid )
    {
        /* Verify page type and subtype */
        if(valid type)
        {
            /* Ok */
        }
        else
        {
                /* Not valid type */
        }
    }
    else
    {
        /* Verify page type and subtype */
        if(valid type)
        {
            /* Ok */
        }
        else
        {
                /* Not valid type */
        }
    }

    if(notfound)
    {
        /* Read every page and search for type */

        for()
        {

        }

        if(found)
        {
            /* Update index */
        }

    }
}

static bool_t eFSS_LOG_VerifyVersion(const t_eFSS_LOG_Ctx* p_ptCtx)
{
    /* Verify log version */

}

e_eFSS_LOG_RES eFSS_LOG_AddLog(t_eFSS_LOG_Ctx* const p_ptCtx, uint8_t* p_puLogToSave, uint32_t p_uLogL)
{
    /* Verify pointer */

    /* Verify parameter */

    /* Verify if init */

    /* Verify if coherent */

    /* Load index */

    /* Read current write page */

    /* If page full go next */

    /* Update page and index */

    /* Return */
}















































































e_eFSS_UTILSHLPRV_RES eFSS_UTILSHLPRV_GetMetaFromBuff(const uint8_t* pageBuff, const uint32_t p_pageL,
                                                      t_eFSS_TYPE_PageMeta* const pagePrm)
{
	/* Local variable */
	e_eFSS_UTILSHLPRV_RES l_eRes;
    uint32_t l_uComulIndx;
    uint32_t l_uTemp;

	/* Check pointer validity */
	if( ( NULL == pageBuff ) || ( NULL == pagePrm )  )
	{
		l_eRes = e_eFSS_UTILSHLPRV_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_pageL < EFSS_PAGEMETASIZE )
        {
            l_eRes = e_eFSS_UTILSHLPRV_RES_BADPARAM;
        }
        else
        {


            l_eRes = e_eFSS_UTILSHLPRV_RES_OK;
        }
	}

	return l_eRes;
}
































bool InOutFlashManager::init()
{
	/* verify if we have valid data in one of the two flash page, or if this is the first startup */
   /* integrity creator will set all daa to default value if crc error is ofund */
	bool intFlash = integrityCreatorData();

   /* Data in the flash is OK, not check i we have never version of the parameter */
   if(intFlash == true)
   {
      /* Data in the flash is OK, not check i we have never version of the parameter */
      intFlash = verifyParameterVersion();

      if(intFlash == true)
      {
         /* Parameter updated correctly*/
         intFlash = true;
      }
      else
      {
         /* Error uptading parameter version */
         intFlash = false;
      }
   }
   else
   {
      /* Impossible to create integrity in flash.. */
      intFlash = false;
   }

	/* now we are ready to go */
	return intFlash;
}

bool InOutFlashManager::saveTempProbeSettings(uint64_t tank_n_wire, uint64_t tank_use470ohm)
{
   bool success = false;
	if(integrityCreatorData() == false)
   {
      /* Problem with the data flash integrity */
      success = false;
   }
   else
   {
      DEVICE_INOUT_FLASH myStruct = *((DEVICE_INOUT_FLASH*)ADDRESS_IN_OUT_STORAGE_PAGE);

      /* Parameter check */
      if((tank_n_wire > 4) || (tank_n_wire < 2))
      {
         /* Invalid parameter */
        tank_n_wire = 3;
      }

      myStruct.tank_n_wire.parameter = tank_n_wire;
      myStruct.tank_use470ohm.parameter = tank_use470ohm;
      bool mainOk = saveData(&myStruct, MAIN_PAGE);
      bool backOk = saveData(&myStruct, BACK_UP_PAGE);
      success = mainOk&&backOk;
   }

   return success;
}


bool InOutFlashManager::getTempProbeSettings(uint64_t *tank_n_wire, uint64_t *tank_use470ohm)
{
   bool success = false;

   /* Check parameter */
   if( (tank_n_wire == NULL) || (tank_use470ohm == NULL ) )
   {
      SYSTEM_EXCEPTION();
   }

	if(integrityCreatorData() == false)
   {
      /* Problem with the data flash integrity */
      success = false;
   }
   else
   {
      *tank_use470ohm = ((DEVICE_INOUT_FLASH*)ADDRESS_IN_OUT_STORAGE_PAGE)->tank_use470ohm.parameter;
      *tank_n_wire = ((DEVICE_INOUT_FLASH*)ADDRESS_IN_OUT_STORAGE_PAGE)->tank_n_wire.parameter;
      success = true;
   }

   return success;
}



bool InOutFlashManager::saveDigitalOutLifeSpawn(uint64_t guarantyCycle, uint64_t guarantyYears)
{
   bool success = false;
	if(integrityCreatorData() == false)
   {
      /* Problem with the data flash integrity */
      success = false;
   }
   else
   {
      DEVICE_INOUT_FLASH myStruct = *((DEVICE_INOUT_FLASH*)ADDRESS_IN_OUT_STORAGE_PAGE);

      /* Parameter check */
      if(guarantyYears > 5)
      {
         /* Invalid parameter */
        guarantyYears = 5;
      }

      /* Parameter check */
      if(guarantyYears < 2)
      {
         /* Invalid parameter */
        guarantyYears = 2;
      }

      /* Parameter check */
      if(guarantyCycle > 1000000)
      {
         /* Invalid parameter */
        guarantyCycle = 1000000;
      }

      /* Parameter check */
      if(guarantyCycle < 100000)
      {
         /* Invalid parameter */
        guarantyCycle = 100000;
      }

      myStruct.guarantyCycle.parameter = guarantyCycle;
      myStruct.guarantyYears.parameter = guarantyYears;

      bool mainOk = saveData(&myStruct, MAIN_PAGE);
      bool backOk = saveData(&myStruct, BACK_UP_PAGE);
      success = mainOk&&backOk;
   }

   return success;
}


bool InOutFlashManager::getDigitalOutLifeSpawn(uint64_t *guarantyCycle, uint64_t *guarantyYears)
{
   bool success = false;

   /* Check parameter */
   if( (guarantyCycle == NULL) || (guarantyYears == NULL ) )
   {
      SYSTEM_EXCEPTION();
   }

	if(integrityCreatorData() == false)
   {
      /* Problem with the data flash integrity */
      success = false;
   }
   else
   {
      *guarantyCycle = ((DEVICE_INOUT_FLASH*)ADDRESS_IN_OUT_STORAGE_PAGE)->guarantyCycle.parameter;
      *guarantyYears = ((DEVICE_INOUT_FLASH*)ADDRESS_IN_OUT_STORAGE_PAGE)->guarantyYears.parameter;
      success = true;
   }

   return success;
}





bool InOutFlashManager::saveAnalogInputCalibrationValue(ANALOG_INPUT_TYPE inputType, float calibValue)
{
   bool success = false;
	if(integrityCreatorData() == false)
   {
      /* Problem with the data flash integrity */
      success = false;
   }
   else
   {
         uint32_t posArray = 0;

         if(GET_POSITION_BY_ITEM(&posArray, ANALOG_PRESENT_INPUT, inputType))
         {
            DEVICE_INOUT_FLASH myStruct = *((DEVICE_INOUT_FLASH*)ADDRESS_IN_OUT_STORAGE_PAGE);

            /* Parameter check */
            if(calibValue <= -5.0)
            {
               /* Invalid parameter */
               calibValue = -5.0;
            }

            /* Parameter check */
            if(calibValue > 5.0)
            {
               /* Invalid parameter */
               calibValue = 5.0;
            }

            #if ANALOG_PRESENT_INPUT_SIZE
            myStruct.calibValAnalogInput[posArray].parameter = calibValue;
            #endif

            bool mainOk = saveData(&myStruct, MAIN_PAGE);
            bool backOk = saveData(&myStruct, BACK_UP_PAGE);
            success = mainOk&&backOk;
         }
         else
         {
            /* Searched output is not present in this configuration*/
            success = true;
         }
   }

   return success;
}


bool InOutFlashManager::getAnalogInputCalibrationValue(ANALOG_INPUT_TYPE inputType,  float* calibValue)
{
   bool success = false;

   /* Check parameter */
   if( calibValue == NULL )
   {
      SYSTEM_EXCEPTION();
   }

	if(integrityCreatorData() == false)
   {
      /* Problem with the data flash integrity */
      success = false;
   }
   else
   {
      uint32_t posArray = 0;

      if(GET_POSITION_BY_ITEM(&posArray, ANALOG_PRESENT_INPUT, inputType))
      {
         #if ANALOG_PRESENT_INPUT_SIZE
         *calibValue = ((DEVICE_INOUT_FLASH*)ADDRESS_IN_OUT_STORAGE_PAGE)->calibValAnalogInput[posArray].parameter;
         #endif
         success = true;
      }
      else
      {
         /* Searched output is not present in this configuration*/
         success = true;
      }
   }

   return success;
}



/* ------------------------------------------------------------------------------------------------------------------------ Private utils function, for internal usage */
/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

bool InOutFlashManager::integrityCreatorData()
{
   bool integritySuccess = false;
   if(isValidPageData(MAIN_PAGE) && isValidPageData(BACK_UP_PAGE))
   {
      /* Both page are OK, but are they equals?*/
      bool arePageEquals = false;

      /* check if data written is equal to the data received... */
      arePageEquals = UtilsFlashManager::isFlashPageEqualsToFlashPage((uint32_t)(ADDRESS_IN_OUT_STORAGE_PAGE), (uint32_t)(ADDRESS_BACKUP_IN_OUT_STORAGE_PAGE));

      if(arePageEquals)
      {
         /* all ok, no action is needed */
         integritySuccess =  true;
      }
      else
      {
         /* Page are different, copy main page in support page */
         integritySuccess = createBackUpPageData();
      }
	}
   else if(isValidPageData(MAIN_PAGE))
   {
		/* main page is OK create a backUp */
		integritySuccess = createBackUpPageData();
	}
   else if(isValidPageData(BACK_UP_PAGE))
   {
      /* main page is corrupted and back up page is ok, regenerate */
      integritySuccess = regenerateMainPageData();
	}
   else
   {
      /* no valid page is found... this is the fist power up, erase all page and init parameter */

		/* set defaultValue */
		DEVICE_INOUT_FLASH defaultValue;
      memset((uint8_t*)&defaultValue, sizeof(DEVICE_INOUT_FLASH), 0);

      /* Temp sensor settings */
      defaultValue.tank_n_wire.parameter = 3;
      defaultValue.tank_n_wire.parameterVersion = 1;

      defaultValue.tank_use470ohm.parameter = 1;
      defaultValue.tank_use470ohm.parameterVersion = 1;

      /* Digital Output life spawn */
      defaultValue.guarantyCycle.parameter = 100000;
      defaultValue.guarantyCycle.parameterVersion = 1;

      defaultValue.guarantyYears.parameter = 2;
      defaultValue.guarantyYears.parameterVersion = 1;


      /* Analog output counter */
      #if ANALOG_PRESENT_INPUT_SIZE
         for(uint32_t i=0; i<ANALOG_PRESENT_INPUT_SIZE ; i++)
         {
            defaultValue.calibValAnalogInput[i].parameter = 0;
            defaultValue.calibValAnalogInput[i].parameterVersion = 1;
         }
      #endif

      /* Not used counter */
      #if IN_OUT_FLASH_FREE_PARAMETER
         for(uint32_t i=0; i<IN_OUT_FLASH_FREE_PARAMETER ; i++)
         {
            defaultValue.inOutNotUsed[i].parameter = 0;
            defaultValue.inOutNotUsed[i].parameterVersion = 0;
         }
      #endif

      /* Padding */
      defaultValue.crc.magicNumber = UINT64_MAGIC_NUMBER;

      /* Crc will be calculated in another function */
      defaultValue.crc.crc = 0;

      /* Save data */
		bool mainOk = saveData(&defaultValue, MAIN_PAGE);
		bool backOk = saveData(&defaultValue, BACK_UP_PAGE);

      integritySuccess =  mainOk&&backOk;
	}

   return integritySuccess;
}

bool InOutFlashManager::verifyParameterVersion()
{
   /* Verify if saved parameter need to be updated based on their version */
   bool isUpdatedVersion = false;

   /* Verifity data integrity */
   isUpdatedVersion = integrityCreatorData();

   if(isUpdatedVersion == true)
   {
      /* Page are ok, check parameter */
      DEVICE_INOUT_FLASH *myStruct = NULL;

      myStruct = ((DEVICE_INOUT_FLASH*)ADDRESS_IN_OUT_STORAGE_PAGE);


      /* Copy data in RAM */
      DEVICE_INOUT_FLASH ramValue = *myStruct;

      /* Padding */
      ramValue.crc.magicNumber = UINT64_MAGIC_NUMBER;

      /* Crc will be calculated in another function */
      ramValue.crc.crc = 0;


      if(myStruct->tank_n_wire.parameterVersion == 1)
      {
         /* Updated */
      }
      else
      {
         /* New version found  */
         isUpdatedVersion = false;
         ramValue.tank_n_wire.parameter = 3;
         ramValue.tank_n_wire.parameterVersion = 1;
      }

      if(myStruct->tank_use470ohm.parameterVersion == 1)
      {
         /* Updated */
      }
      else
      {
         /* New version found  */
         isUpdatedVersion = false;
         ramValue.tank_use470ohm.parameter = 1;
         ramValue.tank_use470ohm.parameterVersion = 1;
      }

      if(myStruct->guarantyCycle.parameterVersion == 1)
      {
         /* Updated */
      }
      else
      {
         /* New version found  */
         isUpdatedVersion = false;
         ramValue.guarantyCycle.parameter = 100000;
         ramValue.guarantyCycle.parameterVersion = 1;
      }

      if(myStruct->guarantyYears.parameterVersion == 1)
      {
         /* Updated */
      }
      else
      {
         /* New version found  */
         isUpdatedVersion = false;
         ramValue.guarantyYears.parameter = 2;
         ramValue.guarantyYears.parameterVersion = 1;
      }

      /* Analog input calibration value */
      #if ANALOG_PRESENT_INPUT_SIZE
         for(uint32_t i=0; i<ANALOG_PRESENT_INPUT_SIZE ; i++)
         {
            if(myStruct->calibValAnalogInput[i].parameterVersion == 1)
            {
               /* Updated */
            }
            else
            {
               /* Need to update */
               isUpdatedVersion = false;
               ramValue.calibValAnalogInput[i].parameter = 0;
               ramValue.calibValAnalogInput[i].parameterVersion = 1;
            }
         }
      #endif

      /* Not used counter */
      #if IN_OUT_FLASH_FREE_PARAMETER
         for(uint32_t i=0; i<IN_OUT_FLASH_FREE_PARAMETER ; i++)
         {
            if(myStruct->inOutNotUsed[i].parameterVersion == 0)
            {
               /* Updated */
            }
            else
            {
               /* Need to update */
               isUpdatedVersion = false;
               ramValue.inOutNotUsed[i].parameter = 0;
               ramValue.inOutNotUsed[i].parameterVersion = 0;
            }
         }
      #endif

      if(isUpdatedVersion)
      {
         /* all ok, no action is needed */
         isUpdatedVersion =  true;
      }
      else
      {
         /* Page have different parameter version, save new parameter and create a backup */
         /* Save data */
         bool mainOk = saveData(&ramValue, MAIN_PAGE);
         bool backOk = saveData(&ramValue, BACK_UP_PAGE);

         isUpdatedVersion =  mainOk&&backOk;
      }
   }
   else
   {
      /* Impossible create integrity and so update parameter */
      isUpdatedVersion = false;
   }

   return isUpdatedVersion;
}

bool InOutFlashManager::isValidPageData(bool isMainPage)
{
   bool isPageValid = false;

   DEVICE_INOUT_FLASH *myStruct = NULL;

   if(isMainPage)
   {
      myStruct = ((DEVICE_INOUT_FLASH*)ADDRESS_IN_OUT_STORAGE_PAGE);
   }
   else
   {
      myStruct = ((DEVICE_INOUT_FLASH*)(ADDRESS_BACKUP_IN_OUT_STORAGE_PAGE));
   }

   uint64_t crcCalculated = UtilsFlashManager::getFlashParameterPageCrc(isMainPage?(uint32_t)ADDRESS_IN_OUT_STORAGE_PAGE:(uint32_t)ADDRESS_BACKUP_IN_OUT_STORAGE_PAGE);

	if(myStruct->crc.crc != crcCalculated)
   {
      /* Crc are different... */
		isPageValid = false;
   }
	else
   {
      /* Crc are valid, check padding magic number */
      if(myStruct->crc.magicNumber == UINT64_MAGIC_NUMBER)
      {
         /* Even magic number are ok*/
         isPageValid = true;
      }
      else
      {
         /* Magic number not OK */
         isPageValid = false;
      }
   }

   return isPageValid;
}

bool InOutFlashManager::createBackUpPageData()
{
	DEVICE_INOUT_FLASH *myStruct = ((DEVICE_INOUT_FLASH*)ADDRESS_IN_OUT_STORAGE_PAGE);
	return saveData(myStruct, BACK_UP_PAGE);
}


bool InOutFlashManager::regenerateMainPageData()
{
	DEVICE_INOUT_FLASH *myStruct = ((DEVICE_INOUT_FLASH*)ADDRESS_BACKUP_IN_OUT_STORAGE_PAGE);
	return saveData(myStruct, MAIN_PAGE);
}


bool InOutFlashManager::saveData(DEVICE_INOUT_FLASH *newDataToSave, bool isMainPage)
{
   bool isPageWritten = false;

   /* Create the union struct */
   DEVICE_INOUT_FLASH toSave = *newDataToSave;

   /* Set magic number */
   toSave.crc.magicNumber = UINT64_MAGIC_NUMBER;

   /* Calculate crc */
   uint64_t crcCalculated = UtilsFlashManager::calculateDataCrc((uint8_t*)&toSave, sizeof(DEVICE_INOUT_FLASH)-8);

   /* Set crc */
   toSave.crc.crc = crcCalculated;

   isPageWritten = UtilsFlashManager::saveFlashPage((uint8_t*)&toSave, sizeof(DEVICE_INOUT_FLASH), isMainPage?(uint32_t)ADDRESS_IN_OUT_STORAGE_PAGE:(uint32_t)ADDRESS_BACKUP_IN_OUT_STORAGE_PAGE );

   return isPageWritten;
}
















































































/* Includes ------------------------------------------------------------------*/
#include "CommonLogFlashManager.h"
#include "Sys_Exception.h"

/* Class Function */
CommonLogFlashManager::CommonLogFlashManager(KAPI_PARTITION_TYPE logPartition, uint32_t logPatitionSize, LogDescrFlashManager* logDescManager)
{
    m_logDescrp = logDescManager;
    m_logPartition = logPartition;
    m_logPatitionSize = logPatitionSize;
    m_logNumPage = (m_logPatitionSize/MINIMUM_PARTITION_SIZE_BYTE);
    m_SupportPage = NULL;
}

CommonLogFlashManager::~CommonLogFlashManager()
{

}


bool CommonLogFlashManager::init(uint8_t *supportPage, uint8_t *additionalSupportPage)
{
    bool intFlash = false;

    if( m_logDescrp == NULL )
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
    }

    if( supportPage == NULL )
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
    }

    if( additionalSupportPage == NULL )
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
    }

	m_SupportPage = supportPage;

    /* log page, need to retrive from memory the starting point of the current LOG */
    /* Init it */
    intFlash = m_logDescrp->init(additionalSupportPage);

	if(intFlash == true)
	{
        /* Check if a new version of log are present */
        bool updatedVersion = true;
        if( true == verifyCurrentLogVersion() )
        {
            /* Log version is updated, nothing to do  */
            updatedVersion = true;
        }
        else
        {
            /* New log version, clear old log */
        	if(true == clearLogPage(0))
        	{
        		/* First page cleaned */
        		/* CLean descriptor */
                if( true == m_logDescrp->setnewestOldestLogPageLocation( 0, 0 ) )
                {
                    /* Log descriptor OK */
                	updatedVersion = integrityCreatorCurrentLogData();
                }
                else
                {
                    /* Error */
                    ERROR_LOG("Error setting new old page location");
                    updatedVersion = false;
                }
        	}
        	else
        	{
                /* Error */
                ERROR_LOG("Error cleaning first log page");
                updatedVersion = false;
        	}
        }

        if(true == updatedVersion)
        {
            /* verify if we have valid data in one of the two flash page, or if this is the first time in this area */
            /* integrity creator will set all data to default value if crc error is found */
        	intFlash = integrityCreatorCurrentLogData();
            if(true == intFlash)
            {
            	/* All ok */
            }
            else
            {
                /* Impossible get current page */
            	ERROR_LOG("Impossible to create integrity in log page");
                intFlash = false;
            }
        }
        else
        {
            ERROR_LOG("error updating log..");
            /* error updating log... */
        }
    }
    else
    {
		/* Impossible to create integrity in flash.. */
    	ERROR_LOG("Impossible to create integrity in log desriptor");
		intFlash = false;
    }

	/* now we are ready to go */
	return intFlash;
}


bool CommonLogFlashManager::clearAllLogLight()
{
	bool cleared = true;
    if( m_logDescrp == NULL )
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
    }

    /* Clean only first page location */

    /* First align the descriptor. In case of power loss we dont corrupt data in the index page */
    if( true == clearLogPage( 0 ) )
    {
        if( true == m_logDescrp->setnewestOldestLogPageLocation( 0, 0 ) )
        {
            /* Log descriptor OK */
            cleared = integrityCreatorCurrentLogData();
        }
        else
        {
            /* Error */
            ERROR_LOG("Error clearing firt page location");
            cleared = false;
        }
    }
    else
    {
        /* Error */
        ERROR_LOG("Error setting new page location");
        cleared = false;
    }

    return cleared;
}

bool CommonLogFlashManager::eraseAllLogpartition()
{
	bool formatted = false;

	/* First erase data page */
	formatted = eraseLogPagePartition(0);

	if(true == formatted)
	{
		/* Now format page descriptor */
		formatted = m_logDescrp->eraseLogPageLocationPartition();

		if(true == formatted)
		{
			/* Can finish */
			formatted = eraseLogPagePartition(1);
		}
		else
		{
			ERROR_LOG("Impossible reset partition ");
		}
	}
	else
	{
		/* Failed */
		ERROR_LOG("Impossible reset partition ");
	}

	return formatted;
}

bool CommonLogFlashManager::clearLogPage(uint32_t logIndex)
{
	bool cleared = false;
    if( m_logDescrp == NULL )
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
    }

    /* Support init */
    DEVICE_GENERICLOG_FLASH *supportData = (DEVICE_GENERICLOG_FLASH*)m_SupportPage;
    memset((uint8_t*)supportData, 0, sizeof(DEVICE_GENERICLOG_FLASH));

    /* Set all to default value  */
    #if GENERICLOG_FLASH_FREE_PARAMETER
        for(uint32_t i=0; i<GENERICLOG_FLASH_FREE_PARAMETER; i++)
        {
        	supportData->notUsedGenericLogPARAM[i].parameter  = 0;
        	supportData->notUsedGenericLogPARAM[i].parameterVersion = 0;
        }
    #endif

    /* Padding */
    supportData->crc.magicNumber = UINT64_MAGIC_NUMBER;

    /* Crc will be calculated in another function */
    supportData->crc.crc = 0;

    /* Save data */
    cleared = saveSinglePageLogData(supportData, logIndex);

    return cleared;
}

bool CommonLogFlashManager::eraseLogPagePartition(uint32_t logIndex)
{
	bool partitionResetted = false;

	WARNING_LOG("ERASING LOG PAGE DATA PARTITION");

	if(UtilsFlashManager::eraseFlashArea(m_logPartition, MINIMUM_PARTITION_SIZE_BYTE*logIndex, MINIMUM_PARTITION_SIZE_BYTE) == true)
	{
		/* Page resetted */
		partitionResetted = true;
	}
	else
	{
		ERROR_LOG("Impossible reset partition ");
		partitionResetted = false;
	}

	return partitionResetted;
}


bool CommonLogFlashManager::getCurrentLogPageIndex(uint32_t *logIndex)
{
    bool returned = false;
    if( (m_logDescrp == NULL) || (logIndex == NULL) )
    {
          ERROR_LOG("Bad param");
          SYSTEM_EXCEPTION();
          returned = false;
    }

    if(m_logDescrp->getnewestLogPageLocation(logIndex))
    {
        returned = true;
    }
    else
    {
        /* Error with descr page */
        returned = false;
    }

    return returned;
}

bool CommonLogFlashManager::getOldestLogPageIndex(uint32_t *logIndex)
{
    bool returned = false;
    if( (m_logDescrp == NULL) || (logIndex == NULL) )
    {
          ERROR_LOG("Bad param");
          SYSTEM_EXCEPTION();
          returned = false;
    }

    if(m_logDescrp->getoldestLogPageLocation(logIndex))
    {
        returned = true;
    }
    else
    {
        /* Error with descr page */
        returned = false;
    }

    return returned;
}

bool CommonLogFlashManager::getNumberOfLogPageValorized(uint32_t *num)
{
    bool returned = false;
    if( (m_logDescrp == NULL) || (num == NULL) )
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
        returned = false;
    }

    uint32_t newEstIndex = 0;
    uint32_t oldEstIndex = 0;
    if(m_logDescrp->getnewestLogPageLocation(&newEstIndex))
    {
        if(m_logDescrp->getoldestLogPageLocation(&oldEstIndex))
        {
            returned = true;

            if(newEstIndex == oldEstIndex)
            {
                /* Only current Log page */
                *num = 1;
            }
            else if(newEstIndex > oldEstIndex)
            {
                *num = newEstIndex - oldEstIndex +1;
            }
            else
            {
                *num = ( m_logNumPage ) - (oldEstIndex -  newEstIndex -1) ;
            }
        }
        else
        {
            /* Error with descr page */
        	ERROR_LOG("Failed load oldest log page");
            returned = false;
        }
    }
    else
    {
        /* Error with descr page */
    	ERROR_LOG("Failed load newest log page");
        returned = false;
    }

    return returned;
}



bool CommonLogFlashManager::getLogVersion(uint32_t *logVersion)
{
    bool returned = false;
    if( (m_logDescrp == NULL) || (logVersion == NULL) )
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
        returned = false;
    }

    if(m_logDescrp->getFwLogVersion(logVersion))
    {
    	/* Log version getted */
    	returned = true;
    }
    else
    {
        /* Error with descr page */
        returned = false;
    }

    return returned;
}


bool CommonLogFlashManager::getNumOfTotalLogPage(uint32_t *maxLogPages)
{
    bool returned = false;
    if( (m_logDescrp == NULL) || (maxLogPages == NULL) )
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
        returned = false;
    }

    *maxLogPages = m_logNumPage;
    returned = true;

    return returned;
}

/* Increase page counter desriptor, back up it, erase newt log page, and back up it */
bool CommonLogFlashManager::currentPageFullGoNext()
{
    bool returned = false;
    if(m_logDescrp == NULL)
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
        returned = false;
    }


    /* Current page is full, increase */
    uint32_t currentLogIndex = 0;
    uint32_t oldestLogIndex = 0;

    uint32_t currentLogIndexNext = 0;
    uint32_t currentLogBckupIndexNext = 0;
    uint32_t oldestLogIndexNext = 0;
    if( true ==  CommonLogFlashManager::getCurrentLogPageIndex(&currentLogIndex) )
    {
        if( true ==  CommonLogFlashManager::getOldestLogPageIndex(&oldestLogIndex) )
        {
        	/* Remember to survive to power outage! */
        	/* First, if needed, increase oldest page location, format it, format the full backup page, and after increase current page location */
            currentLogIndexNext = getNextIndex(currentLogIndex);
            currentLogBckupIndexNext = getNextIndex(currentLogIndexNext);
            oldestLogIndexNext = getNextIndex(oldestLogIndex);

            if(currentLogIndex == oldestLogIndex)
            {
            	/* No other log page present, first page to be full */

            }
            else if(currentLogBckupIndexNext == oldestLogIndex)
            {
            	/* Circular queue is full, need to increase the oldest value */
            	m_logDescrp->setoldestLogPageLocation(oldestLogIndexNext);

            }
            else
            {
            	/* Some free page present, no need to increase oldest log position */

            }

            /* Format future backup page and future data page */
            clearLogPage(currentLogBckupIndexNext);
            clearLogPage(currentLogIndexNext);

            /* now that all page are cleaared we can increase log descriptor */
            m_logDescrp->setnewestLogPageLocation(currentLogIndexNext);
            returned = true;

        }
        else
        {
        	/* Impossible get oldest index! */
        	ERROR_LOG("Impossible get oldest index");
        	returned = false;
        }
    }
    else
    {
    	/* Impossible get current index! */
    	ERROR_LOG("Impossible get current index");
    	returned = false;
    }


    return returned;
}


/* Save log page and back up it */
bool CommonLogFlashManager::saveCurrentLogPageAndBackUp(uint8_t* logPage)
{
    bool returned = false;
    if( (m_logDescrp == NULL) || (logPage == NULL) )
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
        returned = false;
    }

    /* First, load current index */
    uint32_t logIndex = 0;
    if( true == getCurrentLogPageIndex(&logIndex))
    {
        if(true == saveSinglePageLogData((DEVICE_GENERICLOG_FLASH*) logPage, logIndex) )
        {
        	/* Page saved, create backup!  */
        	returned = createBackUpPageDataOfIndex(logIndex);
        }
        else
        {
        	/* Error saving page */
        	ERROR_LOG("Error saving pages");
            returned = false;
        }
    }
    else
    {
    	/* Error loading current index */
    	ERROR_LOG("Error loading current index");
    	returned = false;
    }

    return returned;
}


/* Load a log page checking CRC */
bool CommonLogFlashManager::loadALogPage(uint32_t logPageIndex, uint8_t* logPage)
{
    bool returned = false;
    if( (m_logDescrp == NULL) || (logPage == NULL) )
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
        returned = false;
    }

    /* First, is valid? */
    if( true == isValidPageData(logPageIndex))
    {
    	/* Valid page! Load it */
        returned = UtilsFlashManager::getFlashArea(m_logPartition, MINIMUM_PARTITION_SIZE_BYTE*logPageIndex, logPage, sizeof(DEVICE_GENERICLOG_FLASH));

        if(returned == true)
        {
        	/* Page loaded check CRC */
        	returned = true;
        }
        else
        {
        	ERROR_LOG("Error loading log page");
        	returned = false;
        }
    }
    else
    {
    	/* Not a valid page... return empty page */
    	returned = false;
    }


    return returned;
}


/* Load the current log page checking CRC, and backup */
bool CommonLogFlashManager::loadCurrentLogPage(uint8_t* logPage)
{
    bool returned = false;
    if( (m_logDescrp == NULL) || (logPage == NULL) )
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
        returned = false;
    }

    /* First, load current index */
    uint32_t logIndex = 0;
    if( true == getCurrentLogPageIndex(&logIndex))
    {
    	/* can now verify integrity and backup copy  */
    	if(true == integrityCreatorCurrentLogData())
    	{
    		/* all ok */
            returned = UtilsFlashManager::getFlashArea(m_logPartition, MINIMUM_PARTITION_SIZE_BYTE*logIndex, logPage, sizeof(DEVICE_GENERICLOG_FLASH));

            if(returned == true)
            {
            	/* Page loaded check CRC */
            	returned = true;
            }
            else
            {
            	ERROR_LOG("Error loading log page");
            	returned = false;
            }
    	}
    	else
    	{
        	ERROR_LOG("Error loading current page and create backup version");
        	returned = false;
    	}
    }
    else
    {
    	/* Error loading current index */
    	ERROR_LOG("Error loading current index");
    	returned = false;
    }


    return returned;
}


/* ---------------------------------------------------------------------------------------------------------------------------- Private function */
uint32_t CommonLogFlashManager::getNextIndex(uint32_t currIndex)
{
	/* Support functiom, log buffer are circular */
	uint32_t nextIndex = currIndex;

	if(currIndex >= (m_logNumPage-1) )
	{
        /* Circular queue */
		nextIndex = 0;
	}
	else
	{
		nextIndex = currIndex + 1 ;
	}
    return nextIndex;
}


uint32_t CommonLogFlashManager::getPrevIndex(uint32_t currIndex)
{
	/* Support functiom, log buffer are circular */
	uint32_t prevIndex = currIndex;

	if(currIndex <= 0 )
	{
        /* Circular queue */
		prevIndex = m_logNumPage-1;
	}
	else
	{
		prevIndex = currIndex - 1 ;
	}
    return prevIndex;
}


bool CommonLogFlashManager::saveSinglePageLogData(DEVICE_GENERICLOG_FLASH* devInfoToSave, uint32_t logPageIndex)
{
   bool isPageWritten = false;

   /* Calculate crc */
   uint64_t crcCalculated = UtilsFlashManager::calculateDataCrc((uint8_t*)devInfoToSave, sizeof(DEVICE_GENERICLOG_FLASH)-sizeof(PAGE_CRC));

   /* Set crc and magic number */
   devInfoToSave->crc.crc = crcCalculated;
   devInfoToSave->crc.magicNumber = UINT64_MAGIC_NUMBER;

   isPageWritten = UtilsFlashManager::saveFlashArea(m_logPartition, MINIMUM_PARTITION_SIZE_BYTE*logPageIndex, (uint8_t*)devInfoToSave, sizeof(DEVICE_GENERICLOG_FLASH));

   return isPageWritten;
}




bool CommonLogFlashManager::isValidPageData(uint32_t logPageIndex)
{
   bool isPageValid = false;

   uint64_t calculatedCrc = 0;

   /* Calculate CRC */
   calculatedCrc = UtilsFlashManager::getFlashParameterAreaCrc(m_logPartition, MINIMUM_PARTITION_SIZE_BYTE*logPageIndex, sizeof(DEVICE_GENERICLOG_FLASH));

   /* Get stored CRC */
   bool readed = false;

   /* Support init */
   DEVICE_GENERICLOG_FLASH *supportData = (DEVICE_GENERICLOG_FLASH*)m_SupportPage;
   memset((uint8_t*)supportData, 0, sizeof(DEVICE_GENERICLOG_FLASH));

   readed = UtilsFlashManager::getFlashArea(m_logPartition, MINIMUM_PARTITION_SIZE_BYTE*logPageIndex, (uint8_t*)supportData, sizeof(DEVICE_GENERICLOG_FLASH));


   if(readed == true)
   {
	   if(supportData->crc.crc != calculatedCrc)
	   {
	        /* Crc are different... */
		    ERROR_LOG("Different crc in ErrorLog page");
			isPageValid = false;
	   }
		else
	   {
	      /* Crc are valid, check padding magic number */
	      if(supportData->crc.magicNumber == UINT64_MAGIC_NUMBER)
	      {
	         /* Even magic number are ok*/
	         isPageValid = true;
	      }
	      else
	      {
	         /* Magic number not OK */
	    	 ERROR_LOG("Different magic number in ErrorLog page");
	         isPageValid = false;
	      }
	   }
   }
   else
   {
	   ERROR_LOG("Error reading ErrorLog partition");
	   isPageValid = false;
   }
   return isPageValid;
}




bool CommonLogFlashManager::createBackUpPageDataOfIndex(uint32_t logPageIndex)
{
	bool created = false;
	bool readed = false;
	uint32_t backupindex = getNextIndex(logPageIndex);

	/* Support init */
	DEVICE_GENERICLOG_FLASH *supportData = (DEVICE_GENERICLOG_FLASH*)m_SupportPage;
	memset((uint8_t*)supportData, 0, sizeof(DEVICE_GENERICLOG_FLASH));

    readed = UtilsFlashManager::getFlashArea(m_logPartition, MINIMUM_PARTITION_SIZE_BYTE*logPageIndex, (uint8_t*)supportData, sizeof(DEVICE_GENERICLOG_FLASH));

    if(readed == true)
    {
    	bool writed = false;
    	writed = UtilsFlashManager::saveFlashArea(m_logPartition, MINIMUM_PARTITION_SIZE_BYTE*backupindex,  (uint8_t*)supportData, sizeof(DEVICE_GENERICLOG_FLASH));

    	if(writed == true)
    	{
    		created = true;
    	}
    	else
    	{
        	ERROR_LOG("Error writing GenericLog partition");
        	created = false;
    	}
    }
    else
    {
    	ERROR_LOG("Error reading GenericLog partition");
    	created = false;
    }

	return created;
}




bool CommonLogFlashManager::regenerateMainPageDataOfIndex(uint32_t logPageIndex)
{
	bool created = false;
	bool readed = false;
	uint32_t mainindex = getPrevIndex(logPageIndex);

	/* Support init */
	DEVICE_GENERICLOG_FLASH *supportData = (DEVICE_GENERICLOG_FLASH*)m_SupportPage;
	memset((uint8_t*)supportData, 0, sizeof(DEVICE_GENERICLOG_FLASH));

    readed = UtilsFlashManager::getFlashArea(m_logPartition, MINIMUM_PARTITION_SIZE_BYTE*logPageIndex, (uint8_t*)supportData, sizeof(DEVICE_GENERICLOG_FLASH));

    if(readed == true)
    {
    	bool writed = false;
    	writed = UtilsFlashManager::saveFlashArea(m_logPartition, MINIMUM_PARTITION_SIZE_BYTE*mainindex,  (uint8_t*)supportData, sizeof(DEVICE_GENERICLOG_FLASH));

    	if(writed == true)
    	{
    		created = true;
    	}
    	else
    	{
        	ERROR_LOG("Error writing GenericLog partition");
        	created = false;
    	}
    }
    else
    {
    	ERROR_LOG("Error reading GenericLog partition");
    	created = false;
    }

	return created;
}



bool CommonLogFlashManager::integrityCreatorCurrentLogData()
{
	bool integritySuccess = false;

	/* First, get current index page */
	uint32_t currentLogIndex = 0;
	if( true == getCurrentLogPageIndex(&currentLogIndex))
	{
	    uint32_t logCurrPageIndex = currentLogIndex;
	    uint32_t logbckupPageIndex = getNextIndex(currentLogIndex);

		if(isValidPageData(logCurrPageIndex) && isValidPageData(logbckupPageIndex))
		{
		      /* Both page are OK, but are they equals?*/
		      bool arePageEquals = false;

		      /* check if data written is equal to the data received... */
		      arePageEquals = UtilsFlashManager::isFlashAreaEqualsToFlashArea(m_logPartition, MINIMUM_PARTITION_SIZE_BYTE*logCurrPageIndex, m_logPartition, MINIMUM_PARTITION_SIZE_BYTE*logbckupPageIndex, sizeof(DEVICE_GENERICLOG_FLASH));

		      if(arePageEquals)
		      {
		         /* all ok, no action is needed */
		         integritySuccess =  true;
		      }
		      else
		      {
		         /* Page are different, copy main page in support page */
		    	 ERROR_LOG("GenericLog page are ok but they are different from main to bckp");
		         integritySuccess = createBackUpPageDataOfIndex(logCurrPageIndex);
		      }
		}
		else if(isValidPageData(logCurrPageIndex))
		{
				/* main page is OK create a backUp */
				ERROR_LOG("createBackUpPageDataOfIndex");
				integritySuccess = createBackUpPageDataOfIndex(logCurrPageIndex);
		}
		else if(isValidPageData(logbckupPageIndex))
		{
		      /* main page is corrupted and back up page is ok, regenerate */
			  ERROR_LOG("regenerateMainPageDataOfIndex");
		      integritySuccess = regenerateMainPageDataOfIndex(logbckupPageIndex);
		}
		else
		{
		    /* no valid page is found... this is the fist time we write in this area, init parameter */
			WARNING_LOG("No valid page found, regenerate main and bkp pwrd page of the LOG General");

			/* Support init */
			DEVICE_GENERICLOG_FLASH *supportData = (DEVICE_GENERICLOG_FLASH*)m_SupportPage;
			memset((uint8_t*)supportData, 0, sizeof(DEVICE_GENERICLOG_FLASH));


	        /* Not used counter */
	        #if GENERICLOG_FLASH_FREE_PARAMETER
	            /* Set all to default value, name and note  */
	            for(uint32_t i=0; i<GENERICLOG_FLASH_FREE_PARAMETER; i++)
	            {
	            	supportData->notUsedGenericLogPARAM[i].parameter  = 0;
	            	supportData->notUsedGenericLogPARAM[i].parameterVersion = 0;
	            }
	        #endif

		    /* Padding */
	        supportData->crc.magicNumber = UINT64_MAGIC_NUMBER;

		    /* Crc will be calculated in another function */
	        supportData->crc.crc = 0;

		    /* Save data */
		    bool mainOk = saveSinglePageLogData(supportData, logCurrPageIndex);
		    bool backOk = saveSinglePageLogData(supportData, logbckupPageIndex);

		    integritySuccess =  mainOk&&backOk;
		}
	}
	else
	{
		/* Error getting current page */
		ERROR_LOG("Error getting current page ");
		integritySuccess = false;
	}


	return integritySuccess;
}


bool CommonLogFlashManager::verifyCurrentLogVersion()
{
	/* Single log will not have a parameter version. The version is Stored only in the log descriptor. When version in LOG descriptor changes
     * we erase all log */
	bool isUpdatedVersion = false;

    if( m_logDescrp == NULL )
    {
        ERROR_LOG("Bad param");
        SYSTEM_EXCEPTION();
        isUpdatedVersion = false;
    }

    uint32_t previousStoredLogVersion = 0;
    uint32_t currentPresentLogVersion = 0;
    if( true == m_logDescrp->getStoredLogVersion(&previousStoredLogVersion) )
    {
    	if( true == m_logDescrp->getFwLogVersion(&currentPresentLogVersion) )
    	{
            /* Are they equals? */
            if( currentPresentLogVersion == previousStoredLogVersion )
            {
                /* Same log version */
                 isUpdatedVersion = true;
            }
            else
            {
                /* New log version */
                 isUpdatedVersion = false;
            }
    	}
    	else
    	{
            /* Error reading costant */
            ERROR_LOG("Impossible read curret log version ");
            isUpdatedVersion = false;
    	}

    }
    else
    {
        /* Error reading flash */
        ERROR_LOG("Impossible read descriptor LOG version ");
        isUpdatedVersion = false;
    }

	return isUpdatedVersion;
}


