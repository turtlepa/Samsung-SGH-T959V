/** 
* Samsung Handset Platform
* Copyright (c) 2007 Samsung Electronics, Inc.
* All rights reserved. 
*/
/*
*  <descr>
*  @author 
*/

#ifndef _WMLSINTERFACE_H
#define _WMLSINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes */
#include <Wmls.h>

/*#ifndef WMLSRETCODE
typedef int	WMLSRETCODE;
#endif - Already defined in Wmls.h*/

/*	This table provides a map for the name and the corresponding url.
	This is required in cases where we have a common name across different url's
*/
typedef struct nameURLMap
{
	unsigned int uiNameIndex ; 
	unsigned int uiURLIndex ;
	struct nameURLMap *pstNext ;
} NAME_URL_MAP ;


/* Generic Bytecode buff structure */
typedef struct bcInst
{
	unsigned char	*pcInstBuffer ; /* Contains the bytecode instructions in UTF8. No '\0' appended */
	unsigned int	uiBuffLen ;	/* Bytecode Inst length */
} BC_INST ;

/* yalhandle structure */
typedef struct stYALHnd
{
	MCHAR			*pmInputBuff ;	/* Input buffer */
	unsigned int	uiBuffLen ;		/* Input buffer length */
	void			*pvParserHnd ;
	void			*pvLexerHnd ;
	BC_INST			*pstBCInstBuff ; /* Contains the bytecode buffer (char *) and buff len for the given input */
	void			*pvConstantPoolHnd ; /* Handle to the constant pool list */
	void			*pvPragmaPoolHnd ; /* Handle to the pragma pool list */
	void			*pvFunctionPoolHnd ; /* Handle to the function pool list */
	void			*pvVarPoolHnd ; /* Handle to the arguments pool list */

	unsigned char	ucNumFormalVarCnt ;
	unsigned char	ucNumLocalVarCnt ;	
	unsigned short	ukCharSetInfo ; /* Character set info for the Constant pool */

	BOOL			bExternFound ; /* Set to FALSE at initialization and is TRUE 
									when we encounter 'extern' keyword. 
									If no extern keyword present in the compilation unit, 
									compilation error is returned */

	BOOL			bIsFormalArg ;
	char			bIsOpAssignExpr ;
	char			bIsLibCall ;
	BOOL			bFuntionHasReturn ;
	unsigned char	ucFor ;
	unsigned char	ucWhile ;
	int				rCode ;
	MCHAR			*pFuncName ;
	NAME_URL_MAP	*pNameUrlmap ;
	int				iPragmaCntinCompUnit ;
	char			cExprState ;
	//int				iOffsetCnt ;
	int				Incrlen ;

} YALHND ;

typedef struct yalInitParam
{
	void *vInitElt ;
	
} YAL_INIT_PARAM ;

int wmlsyyparse(void *) ;
/* Error handling macros */
#define YAL_ERR_FUNCTION_ARG_MISMATCH	-100
#define YAL_ERR_INVALID_LIB_CALL		-101
#define YAL_ERR_MISSING_RPAREN			-102
#define YAL_ERR_ID_DECLARED				-103
#define YAL_ERR_INVALID_COMP_UNIT		-104
#define YAL_ERR_MISSING_ID				-105
#define YAL_ERR_UNDECL_NAME				-106
#define YAL_ERR_MULTIPLE_ACCESS			-107
#define YAL_ERR_ILLEGAL_BREAK			-108
#define YAL_ERR_ILLEGAL_CONTINUE		-109
#define YAL_ERR_INVALID_BRACE			-110

#define YAL_GET_ERR_CODE(YALHnd)		(((YALHND *)YALHnd)->rCode)
/* YAL hnd get elt macros */
#define YAL_GET_VAR_POOL_HND(YalHnd)	((YALHND *)YalHnd)->pvVarPoolHnd
#define YAL_GET_CONST_POOL_HND(YalHnd)	((YALHND *)YalHnd)->pvConstantPoolHnd
#define YAL_GET_FUNC_POOL_HND(YalHnd)	((YALHND *)YalHnd)->pvFunctionPoolHnd
#define YAL_GET_PRAGMA_POOL_HND(YalHnd)	((YALHND *)YalHnd)->pvPragmaPoolHnd

#define YAL_GET_BCNODE_HND(vBCHnd)

#define BC_IS_CONST_ES(vConstPoolHnd, ConstIndex)	wmlsIsConstES(vConstPoolHnd, ConstIndex)
#define YAL_IS_CONST_ES(vConstPoolHnd, ConstIndex)	BC_IS_CONST_ES(vConstPoolHnd, ConstIndex)

/* Memory Allocation and free Macros */

#define WMLS_ALLOC_INST_BUFF(vBCHnd, len)	((unsigned char *)WMLS_MEM_ALLOC(NULL, len, sizeof(unsigned char)))
#define WMLS_FREE_INST_BUFF(vBCHnd, ptr)	(WMLS_MEM_FREE(NULL, ptr))
#define YAL_SET_INCR_LEN(YalHnd, len)		(((YALHND *)YalHnd) ->Incrlen += len)
#define YAL_RESET_INCR_LEN(YalHnd)			(((YALHND *)YalHnd) ->Incrlen = 0)

#define WMLS_ALLOC_BC_INST(vBCHnd)	((BC_INST *)WMLS_MEM_ALLOC(NULL, 1, sizeof(BC_INST)))
#define WMLS_FREE_BC_INST(vBCHnd, pInst)	if (pInst){if (((BC_INST*)pInst)->pcInstBuffer)	{ \
												WMLS_MEM_FREE(NULL, ((BC_INST*)pInst)->pcInstBuffer) ; \
												((BC_INST*)pInst)->pcInstBuffer = NULL ; \
												((BC_INST*)pInst)->uiBuffLen = 0 ; \
												} \
												WMLS_MEM_FREE (NULL, pInst) ;}
												
#define WMLS_GET_BYTECODE(YalHnd)		((NULL != ((YALHND*)YalHnd)->pstBCInstBuff) ? ((YALHND*)YalHnd)->pstBCInstBuff->pcInstBuffer : NULL) 
#define WMLS_GET_BYTECODE_LEN(YalHnd)	((NULL != ((YALHND*)YalHnd)->pstBCInstBuff) ? ((YALHND*)YalHnd)->pstBCInstBuff->uiBuffLen : -1)
/* Local function declarations */
/* Local Function definitions */
/* Global Function definitions */

/*
 * Initializes the yal context
 * @param pstYALInitparam[in]:Init params reqd for initialization
 * @param ppvYALHnd[out]	 :YAL handle.
*/

WMLSRETCODE
wmlsYALInit (YAL_INIT_PARAM *pstYALInitparam, void **ppvYALHnd) ;

/*
 * Deinits the yal context
 * @param ppvYALHnd[in]:YAL handle.
*/

void
wmlsYALDeinit (void *pvYALHnd) ;

/*
 * Compiles a given input buffer and generates the bytecode. The generated bytecode is stored in the YAL context.
 * @param ppvYALHnd[in]:YAL handle.
 * @param pwInputBuff[in]:Input buffer
 * @param uiInpuBuffLen[in]:Input buffer length
*/
WMLSRETCODE
wmlsYALParse (void *pvYALHnd, MCHAR *pmInputBuff, unsigned int uiInpuBuffLen) ;

void
wmlsYALReset (void *pvYALHnd) ;


#ifdef __cplusplus
} //extern "C"
#endif /* __cplusplus */

#endif // _WMLSINTERFACE_H
