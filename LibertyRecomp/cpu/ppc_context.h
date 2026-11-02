#pragma once
// RexGlue's PPC types are authoritative. graine SDK 0.7.5 renamed the generated
// project header from `gta4_config.h` → `gta4_init.h` and the image/code-size
// defines from `PPC_*` → `REX_*`. Keep the old PPC_* aliases so Liberty's
// kernel/memory.cpp + main.cpp don't need wholesale renames.
#include "../../glue/rexglue-sdk-main/gta4-recomp/generated/gta4_init.h"
#ifndef PPC_IMAGE_BASE
  #define PPC_IMAGE_BASE REX_IMAGE_BASE
  #define PPC_IMAGE_SIZE REX_IMAGE_SIZE
  #define PPC_CODE_BASE  REX_CODE_BASE
  #define PPC_CODE_SIZE  REX_CODE_SIZE
#endif
#include <rex/ppc/context.h>
#include <rex/ppc/function.h>

// Remaining PPC_* -> REX_* aliases (per rexglue migration_scan.cpp rename table).
#ifndef PPC_FUNC
  #define PPC_FUNC(x) REX_FUNC(x)
#endif
#ifndef PPC_FUNC_IMPL
  #define PPC_FUNC_IMPL(x) REX_EXTERN(x)
#endif
#ifndef PPC_MEMORY_SIZE
  #define PPC_MEMORY_SIZE REX_MEMORY_SIZE
#endif
#ifndef PPC_LOOKUP_FUNC
  #define PPC_LOOKUP_FUNC(x, y) REX_LOOKUP_FUNC(x, y)
#endif
#ifndef PPC_CALL_INDIRECT_FUNC
  #define PPC_CALL_INDIRECT_FUNC(x) REX_CALL_INDIRECT_FUNC(x)
#endif

// Full PPC_* -> REX_* alias set (object-like so both plain and function-like uses forward).
#ifndef PPC_HOOK
  #define PPC_HOOK REX_HOOK
#endif
#ifndef PPC_STUB
  #define PPC_STUB REX_STUB
#endif
#ifndef PPC_STUB_LOG
  #define PPC_STUB_LOG REX_STUB_LOG
#endif
#ifndef PPC_STUB_RETURN
  #define PPC_STUB_RETURN REX_STUB_RETURN
#endif
#ifndef PPC_FUNC
  #define PPC_FUNC REX_FUNC
#endif
#ifndef PPC_WEAK_FUNC
  #define PPC_WEAK_FUNC REX_WEAK_FUNC
#endif
#ifndef PPC_FUNC_IMPL
  #define PPC_FUNC_IMPL REX_EXTERN
#endif
#ifndef PPC_EXTERN_IMPORT
  #define PPC_EXTERN_IMPORT REX_EXTERN
#endif
#ifndef PPC_JOIN
  #define PPC_JOIN REX_JOIN
#endif
#ifndef PPC_XSTRINGIFY
  #define PPC_XSTRINGIFY REX_XSTRINGIFY
#endif
#ifndef PPC_STRINGIFY
  #define PPC_STRINGIFY REX_STRINGIFY
#endif
#ifndef PPC_FUNC_PROLOGUE
  #define PPC_FUNC_PROLOGUE REX_FUNC_PROLOGUE
#endif
#ifndef PPC_CALL_FUNC
  #define PPC_CALL_FUNC REX_CALL_FUNC
#endif
#ifndef PPC_LOOKUP_FUNC
  #define PPC_LOOKUP_FUNC REX_LOOKUP_FUNC
#endif
#ifndef PPC_CALL_INDIRECT_FUNC
  #define PPC_CALL_INDIRECT_FUNC REX_CALL_INDIRECT_FUNC
#endif
#ifndef PPC_SET_FLUSH_MODE
  #define PPC_SET_FLUSH_MODE REX_SET_FLUSH_MODE
#endif
#ifndef PPC_UNIMPLEMENTED
  #define PPC_UNIMPLEMENTED REX_UNIMPLEMENTED
#endif
#ifndef PPC_QUERY_TIMEBASE
  #define PPC_QUERY_TIMEBASE REX_QUERY_TIMEBASE
#endif
#ifndef PPC_CHECK_GLOBAL_LOCK
  #define PPC_CHECK_GLOBAL_LOCK REX_CHECK_GLOBAL_LOCK
#endif
#ifndef PPC_ENTER_GLOBAL_LOCK
  #define PPC_ENTER_GLOBAL_LOCK REX_ENTER_GLOBAL_LOCK
#endif
#ifndef PPC_LEAVE_GLOBAL_LOCK
  #define PPC_LEAVE_GLOBAL_LOCK REX_LEAVE_GLOBAL_LOCK
#endif
#ifndef PPC_PHYS_HOST_OFFSET
  #define PPC_PHYS_HOST_OFFSET REX_PHYS_HOST_OFFSET
#endif
#ifndef PPC_RAW_ADDR
  #define PPC_RAW_ADDR REX_RAW_ADDR
#endif
#ifndef PPC_LOAD_U8
  #define PPC_LOAD_U8 REX_LOAD_U8
#endif
#ifndef PPC_LOAD_U16
  #define PPC_LOAD_U16 REX_LOAD_U16
#endif
#ifndef PPC_LOAD_U32
  #define PPC_LOAD_U32 REX_LOAD_U32
#endif
#ifndef PPC_LOAD_U64
  #define PPC_LOAD_U64 REX_LOAD_U64
#endif
#ifndef PPC_LOAD_STRING
  #define PPC_LOAD_STRING REX_LOAD_STRING
#endif
#ifndef PPC_STORE_U8
  #define PPC_STORE_U8 REX_STORE_U8
#endif
#ifndef PPC_STORE_U16
  #define PPC_STORE_U16 REX_STORE_U16
#endif
#ifndef PPC_STORE_U32
  #define PPC_STORE_U32 REX_STORE_U32
#endif
#ifndef PPC_STORE_U64
  #define PPC_STORE_U64 REX_STORE_U64
#endif
#ifndef PPC_MEMORY_SIZE
  #define PPC_MEMORY_SIZE REX_MEMORY_SIZE
#endif

// SDK v0.7.4: Thread-local PPC context accessed via rex::runtime::current_ppc_context()
#include <rex/system/thread_state.h>

inline PPCContext* GetPPCContext()
{
    return rex::runtime::current_ppc_context();
}

inline void SetPPCContext(PPCContext& /* ctx */)
{
    // In 0.7.4, the context is per-thread via ThreadState::Bind().
    // SetPPCContext is a no-op — the context is managed by the runtime.
}
