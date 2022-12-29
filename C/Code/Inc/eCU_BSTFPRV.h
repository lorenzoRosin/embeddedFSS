/**
 * @file       eCU_BSTFPRV.h
 *
 * @brief      Byte stuffer utils private definition
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef ECU_BSTFPRV_H
#define ECU_BSTFPRV_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eCU_BSTFPRV_SM_NEEDSOF = 0,
    e_eCU_BSTFPRV_SM_NEEDRAWDATA,
    e_eCU_BSTFPRV_SM_NEEDNEGATEPRECDATA,
	e_eCU_BSTFPRV_SM_NEEDEOF,
    e_eCU_BSTFPRV_SM_STUFFEND
}e_eCU_BSTFPRV_SM;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECU_BSTFPRV_H */