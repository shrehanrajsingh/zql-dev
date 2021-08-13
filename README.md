# ZQL (Dev)
Minimal database management framework for C.
Stores content in key-value pairs.

> Makefile and GCC are required to build ZQL for any platform. (additional utilities might be required to build on non-windows platforms)

**Building on Windows**
* Clone:
	* ```git clone https://github.com/alpha-shrehan/zql-dev```
* Build:
	* ```make build```

Built DLL and ```zql.h``` file will be created in ```bin/```.

Using ZQL:
* Copy ```zql.dll``` and ```zql.h``` in your project directory (For operating systems other than windows, copy ```zql.c``` from ```src/```).
* ```#include "zql.h"``` in your project. (```#include "zql.c"``` for non-windows OS)
* For Windows, build with ```-L<path/to/zql.dll> -lzql```

Examples
* Store some values in it.
```
#include "zql.h"
#include "zql.c"

int main(int argc, const char *argv[])
{	
	ZQL_InitializeHeap();
	ZQL_RegisterDatabase("test.db"); /* make sure test.db exists */
	
	zql_t  _query_set[]  =  {
		ZQL_CreateKey("name", "john"),
		ZQL_CreateKey("age", "15"),
		ZQL_CreateKey("d.o.b", "1/1/1970")
	};
	for (int i = 0; i < 3; i++)
		ZQL_AddQueryToHistory(_query_set[i]); /* Add to history */

	ZQL_CommitHistory(); /* Add history to database */
	
	ZQL_DestroyHeap();
	return 0;
}
```
* Read values
```
#include "zql.h"
#include "zql.c"

int main(int argc, const char *argv[])
{	
	ZQL_InitializeHeap();
	ZQL_RegisterDatabase("test.db");
	
	zql_t *get_name = ZQL_Get("name");
	ZQL_PrintQueryKey(get_name[0]);
	/**
	 * Will print:
	 * (4) name -> (4) john
	 */
	
	// Read everything
	zql_t *get_everything = ZQL_ReadAll();
	ZQL_PrintQuerySet(get_everything);
	/**
	 * Will print:
	 * (4) name -> (4) john     
	 * (3) age -> (2) 15        
	 * (5) d.o.b -> (8) 1/1/1970
	 */
	
	// free memory
	free(get_name);
	free(get_everything);
	
	ZQL_DestroyHeap();
	return 0;
}
```

Other utilities:
* ```void ZQL_API ZQL_RemoveKey(char *_Key)``` 
	* Remove a particular key from database.
	* ```_Key``` The key to remove.
* ```int ZQL_API ZQL_QuerySetLength(zql_t *z_ref)```
	* Get number of values from a query-set.
	* ```z_ref``` Query set.
* ```void ZQL_API ZQL_ClearQueryHistory(void)```
	* Clear entire history.
* ```void ZQL_API ZQL_ClearDatabase(void)```
	* Clear database.
