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
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
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
            /* Initialize internal status */
            l_uComulIndx = p_pageL - EFSS_PAGEMETASIZE;

            /* Init return data */
            pagePrm->uPageType = 0u;
            pagePrm->uPageSubType = 0u;
            pagePrm->uPageVersion = 0u;
            pagePrm->uPageByteFilled = 0u;
            pagePrm->uPagePresentElement = 0u;
            pagePrm->uPageSequentialN = 0u;
            pagePrm->uPageMagicNumber = 0u;
            pagePrm->uPageCrc = 0u;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageType |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageType |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageType |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageType |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageSubType |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageSubType |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageSubType |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageSubType |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageVersion |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageVersion |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageVersion |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageVersion |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageByteFilled |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageByteFilled |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageByteFilled |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageByteFilled |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPagePresentElement |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPagePresentElement |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPagePresentElement |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPagePresentElement |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageSequentialN |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageSequentialN |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageSequentialN |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageSequentialN |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageMagicNumber |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageMagicNumber |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageMagicNumber |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageMagicNumber |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

            /* Copy data Little endian */
            l_uTemp = (uint32_t) pageBuff[l_uComulIndx];
            pagePrm->uPageCrc |= ( l_uTemp & 0x000000FFu );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 8u  );
            pagePrm->uPageCrc |= ( l_uTemp & 0x0000FF00u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 16u  );
            pagePrm->uPageCrc |= ( l_uTemp & 0x00FF0000u );
            l_uComulIndx++;

            l_uTemp =  (uint32_t) ( ( (uint32_t) pageBuff[l_uComulIndx] ) << 24u  );
            pagePrm->uPageCrc |= ( l_uTemp & 0xFF000000u );
            l_uComulIndx++;

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


