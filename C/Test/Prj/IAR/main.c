#include "eCU_CRCTST.h"
#include "eCU_CRCDTST.h"
#include "eCU_CIRQTST.h"
#include "eCU_DPKTST.h"
#include "eCU_DUNPKTST.h"
#include "eCU_DPKDUNPKTST.h"
#include "eCU_BSTFTST.h"
#include "eCU_BUSTFTST.h"
#include "eCU_BSTFBUNSTFTST.h"

int main(void);


int main(void)
{
    /* Start testing */
    eCU_CRCTST_ExeTest();
    eCU_CRCDTST_ExeTest();
    eCU_CIRQTST_ExeTest();
    eCU_DPKTST_ExeTest();
    eCU_DUNPKTST_ExeTest();
	eCU_DPKDUNPKTST_ExeTest();
    eCU_BSTFTST_ExeTest();
    eCU_BUSTFTST_ExeTest();
    eCU_BSTFBUNSTFTST_ExeTest();

    return 0;
}
