#include "zql_utils.h"

zql_t ZQL_API ZQL_QueryCpy(zql_t _zQuery)
{
    zql_t _zRes = (zql_t){
        ._vKey_Len = _zQuery._vKey_Len,
        ._vVal_Len = _zQuery._vVal_Len};

    strcpy(_zRes._vKey, _zQuery._vKey);
    strcpy(_zRes._vVal, _zQuery._vVal);
    return _zRes;
}

void ZQL_API ZQL_InitializeHeap(void)
{
    zql_hist_config = (zql_t *)malloc(sizeof(zql_t));
}

void ZQL_API ZQL_RegisterDatabase(char *file_path)
{
    FILE *_fptr = (FILE *)fopen(file_path, "r");
    assert(_fptr != NULL && "Database does not exist");

    fclose(_fptr);

    _zql_m_conf._dbName = strdup(file_path);
}

void ZQL_API ZQL_ReallocHistConfig(void)
{
    zql_hist_config = (zql_t *)realloc(zql_hist_config, (zql_hist_count + 1) * sizeof(zql_t));
}

void ZQL_API ZQL_AddQueryToHistory(zql_t _zQuery)
{
    if (zql_hist_count)
        ZQL_ReallocHistConfig();

    zql_hist_config[zql_hist_count++] = ZQL_QueryCpy(_zQuery);
}

void ZQL_API ZQL_ClearQueryHistory(void)
{
    free(zql_hist_config);
    zql_hist_config = (zql_t *)malloc(sizeof(zql_t));
    zql_hist_count = 0;
}

ZQL_API char *ZQL_EncodeKeyVal(char *_KeyVal)
{
    char *_res = (char *)malloc(strlen(_KeyVal) * sizeof(char));

    for (size_t i = 0; i < strlen(_KeyVal); i++)
        _res[i] = (char)_KeyVal[i] - 12;

    return strdup(_res);
}

ZQL_API char *ZQL_DecodeKeyVal(char *_KeyVal)
{
    char *_res = (char *)malloc(strlen(_KeyVal) * sizeof(char));

    for (size_t i = 0; i < strlen(_KeyVal); i++)
        _res[i] = (char)_KeyVal[i] + 12;

    return strdup(_res);
}

void ZQL_API ZQL_RemoveFromHistory(char *_Key)
{
    zql_t *_preserve_history = (zql_t *)malloc((zql_hist_count - 1) * sizeof(zql_t));
    for (size_t i = 0; i < zql_hist_count - 1; i++)
        if (strcmp(zql_hist_config[i]._vKey, _Key))
            _preserve_history[i] = ZQL_QueryCpy(zql_hist_config[i]);
    free(zql_hist_config);
    zql_hist_config = (zql_t *)malloc((zql_hist_count - 1) * sizeof(zql_t));
    for (size_t i = 0; i < zql_hist_count - 1; i++)
        zql_hist_config[i] = ZQL_QueryCpy(_preserve_history[i]);
    zql_hist_count--;
    free(_preserve_history);
    _preserve_history = NULL;
}

void ZQL_API ZQL_CommitHistory(void)
{
    FILE *_fptr = (FILE *)fopen(_zql_m_conf._dbName, "a");

    for (size_t i = 0; i < zql_hist_count; i++)
    {
        strcpy(zql_hist_config[i]._vKey, strdup(ZQL_EncodeKeyVal(zql_hist_config[i]._vKey)));
        strcpy(zql_hist_config[i]._vVal, strdup(ZQL_EncodeKeyVal(zql_hist_config[i]._vVal)));
        fwrite(&zql_hist_config[i], sizeof(zql_t), 1, _fptr);
    }

    ZQL_ClearQueryHistory();
    fclose(_fptr);
}

zql_t ZQL_API ZQL_CreateKey(char *_Key, char *_Val)
{
    zql_t _zconf = (zql_t){
        ._vKey_Len = strlen(_Key),
        ._vVal_Len = strlen(_Val)};
    strcpy(_zconf._vKey, _Key);
    strcpy(_zconf._vVal, _Val);
    return ZQL_QueryCpy(_zconf);
}

void ZQL_API ZQL_PrintQueryKey(zql_t _Data)
{
    printf("(%d) %s -> (%d) %s\n", _Data._vKey_Len, _Data._vKey, _Data._vVal_Len, _Data._vVal);
}

ZQL_API zql_t *ZQL_ReadAll(void)
{
    zql_t *_res = (zql_t *)malloc(sizeof(zql_t));
    int rcount = 0;

    FILE *_fptr = (FILE *)fopen(_zql_m_conf._dbName, "r");
    zql_t needle;

    while (fread(&needle, sizeof(zql_t), 1, _fptr))
    {
        if (rcount)
            _res = (zql_t *)realloc(_res, (rcount + 1) * sizeof(zql_t));

        strcpy(needle._vKey, strdup(ZQL_DecodeKeyVal(needle._vKey)));
        strcpy(needle._vVal, strdup(ZQL_DecodeKeyVal(needle._vVal)));

        needle._vKey[needle._vKey_Len] = '\0';
        needle._vVal[needle._vVal_Len] = '\0';
        _res[rcount++] = ZQL_QueryCpy(needle);
    }

    if (rcount)
        _res = (zql_t *)realloc(_res, (rcount + 1) * sizeof(zql_t));
    _res[rcount++] = (zql_t){
        ._vKey_Len = -1,
        ._vVal_Len = -1};

    fclose(_fptr);
    return _res;
}

ZQL_API zql_t *ZQL_Get(char *_Key)
{
    FILE *_fptr = (FILE *)fopen(_zql_m_conf._dbName, "r");

    zql_t *_zres = (zql_t *)malloc(sizeof(zql_t));
    int zc = 0;

    zql_t needle;

    while (fread(&needle, sizeof(zql_t), 1, _fptr))
    {
        strcpy(needle._vKey, strdup(ZQL_DecodeKeyVal(needle._vKey)));
        strcpy(needle._vVal, strdup(ZQL_DecodeKeyVal(needle._vVal)));

        needle._vKey[needle._vKey_Len] = '\0';
        needle._vVal[needle._vVal_Len] = '\0';

        if (!strcmp(needle._vKey, _Key))
        {
            if (zc)
                _zres = (zql_t *)realloc(_zres, (zc + 1) * sizeof(zql_t));

            _zres[zc++] = ZQL_QueryCpy(needle);
        }
    }

    if (!zc)
        return NULL;

    if (zc)
        _zres = (zql_t *)realloc(_zres, (zc + 1) * sizeof(zql_t));

    _zres[zc++] = (zql_t){
        ._vKey_Len = -1,
        ._vVal_Len = -1};

    return _zres;
}

/* last token must be -1, -1. Last token is excluded */
int ZQL_API ZQL_QuerySetLength(zql_t *z_ref)
{
    int count = 0;

    for (size_t i = 0;; i++)
    {
        if (z_ref[i]._vKey_Len == -1)
            break;
        count++;
    }
    return count;
}

void ZQL_API ZQL_ClearDatabase(void)
{
    FILE *_fptr = (FILE *)fopen(_zql_m_conf._dbName, "w");
    fclose(_fptr);
}

void ZQL_API ZQL_DestroyHeap(void)
{
    free(zql_hist_config);
    zql_hist_config = NULL;
    _zql_m_conf._dbName = "";
}

void ZQL_API ZQL_RemoveKey(char *_Key)
{
    zql_t *key_recs = ZQL_ReadAll();
    int key_len = ZQL_QuerySetLength(key_recs);

    FILE *_fptr = (FILE *)fopen(_zql_m_conf._dbName, "w");

    for (size_t i = 0; i < key_len; i++)
    {
        if (!strcmp(key_recs[i]._vKey, _Key))
            continue;
        zql_t ned = ZQL_QueryCpy(key_recs[i]);
        strcpy(ned._vKey, strdup(ZQL_EncodeKeyVal(ned._vKey)));
        strcpy(ned._vVal, strdup(ZQL_EncodeKeyVal(ned._vVal)));
        fwrite(&ned, sizeof(zql_t), 1, _fptr);
    }

    free(key_recs);
    key_recs = NULL;
    fclose(_fptr);
}

void ZQL_API ZQL_PrintQuerySet(zql_t *_zref)
{
    int zlen = ZQL_QuerySetLength(_zref);
    for (size_t i = 0; i < zlen; i++)
        ZQL_PrintQueryKey(_zref[i]);
}