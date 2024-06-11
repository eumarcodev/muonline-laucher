#ifndef MUONLINELAUNCHER_H
#define MUONLINELAUNCHER_H


#include "resource.h"
#include "framework.h"

void InitializeGDIPlus();
void ShutdownGDIPlus();

extern ULONG_PTR gdiplusToken;

#endif