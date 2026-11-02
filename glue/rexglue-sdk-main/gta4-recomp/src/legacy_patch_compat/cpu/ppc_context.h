#pragma once
// Maps the LibertyRecomp/patches macro names onto rexglue so those patch files build here unchanged.
#include "gta4_init.h"  // REX_LOAD_*/REX_STORE_* live in the generated config

#define PPC_FUNC(x) REX_FUNC(x)
#define PPC_FUNC_IMPL(x) REX_EXTERN(x)
#define PPC_LOAD_U16 REX_LOAD_U16
#define PPC_LOAD_U32 REX_LOAD_U32
#define PPC_STORE_U32 REX_STORE_U32
