#ifndef __FONTLIB_SYSTEM_H
#define __FONTLIB_SYSTEM_H

#include "fontlib_manager.h"

void ADDFontLib(void);
PFontLib GetFontLib(char *name);
void SetDefaultFontLib(void);
int GetFontBitMap(int dwcode, PFontBitMap ptFontBitMap);

#endif	/*__FONTLIB_SYSTEM_H*/


