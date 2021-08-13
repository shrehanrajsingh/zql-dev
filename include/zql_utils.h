#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifdef _WIN32
#   ifdef ZQL_BUILD_DLL
#       define ZQL_API __declspec(dllexport)
#   else
#       define ZQL_API __declspec(dllimport)
#endif
#   else
#       define ZQL_API
#endif

typedef struct __zql_raw_type_token
{
    char _vKey[512];
    int _vKey_Len;
    char _vVal[1024];
    int _vVal_Len;

} zql_t;

typedef struct __zql_raw_meta_struct
{
    char *_dbName;

} zql_meta_t;

zql_meta_t _zql_m_conf;

zql_t *zql_hist_config;
int zql_hist_count = 0;

#ifdef __cplusplus
extern "C" {
#endif

zql_t ZQL_API ZQL_QueryCpy(zql_t _zQuery);
void ZQL_API ZQL_InitializeHeap(void);
void ZQL_API ZQL_RegisterDatabase(char *file_path);
void ZQL_API ZQL_ReallocHistConfig(void);
void ZQL_API ZQL_AddQueryToHistory(zql_t _zQuery);
void ZQL_API ZQL_ClearQueryHistory(void);
ZQL_API char *ZQL_EncodeKeyVal(char *_KeyVal);
ZQL_API char *ZQL_DecodeKeyVal(char *_KeyVal);
void ZQL_API ZQL_CommitHistory(void);
zql_t ZQL_API ZQL_CreateKey(char *_Key, char *_Val);
void ZQL_API ZQL_PrintQueryKey(zql_t _Data);
ZQL_API zql_t *ZQL_ReadAll(void);
ZQL_API zql_t *ZQL_Get(char *_Key);
int ZQL_API ZQL_QuerySetLength(zql_t *z_ref);
void ZQL_API ZQL_ClearDatabase(void);
void ZQL_API ZQL_DestroyHeap(void);
void ZQL_API ZQL_RemoveKey(char *_Key);
void ZQL_API ZQL_PrintQuerySet(zql_t *_zref);
void ZQL_API ZQL_RemoveFromHistory(char *_Key);

#ifdef __cplusplus
}
#endif