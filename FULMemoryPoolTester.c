/*
 * FULMemoryPoolTester.c
 *
 *  Created on: Oct 19, 2011
 *      Author: user
 */

#include "FULMemoryPool/MemoryPool.h"
#include <sys/time.h>

#ifndef DEFAULT_STRING_SIZE
#define DEFAULT_STRING_SIZE 16
#endif

#define TEST_MALLOC_TIMES 999
#define TEST_RETRY_TIMES 9999

int FULMemoryPoolTester()
{
	struct timeval startTime, endTime;
	unsigned long long costTime = 0ULL;

	char **pStrings = (char **)malloc(sizeof(char *) * TEST_MALLOC_TIMES);

	PrintLog("Now testing system default malloc/free");
	gettimeofday(&startTime, NULL);
	for (int i=0; i<TEST_RETRY_TIMES; ++i)
	{
		for (int j=0; j<TEST_MALLOC_TIMES; ++j)
		{
			pStrings[j] = (char *)malloc(sizeof(char) * DEFAULT_STRING_SIZE);
			*pStrings[j] = '\0';
			//GenerateRandStr(pStrings[j], DEFAULT_STRING_SIZE-1);
		}
		for (int j=0; j<TEST_MALLOC_TIMES; ++j)
		{
			free(pStrings[j]);
		}
	}
	gettimeofday(&endTime, NULL);
	costTime = 1000 * 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	printf("System default malloc/free tested, malloc and free %d strings for %d times, cost %llu us.\n",
			TEST_MALLOC_TIMES, TEST_RETRY_TIMES, costTime);

	PrintLog("Now testing memory pool Malloc/Free");
	gettimeofday(&startTime, NULL);
	MemoryPool_t *pPool = CreateMemoryPool(DEFAULT_STRING_SIZE);
	for (int i=0; i<TEST_RETRY_TIMES; ++i)
	{
		for (int j=0; j<TEST_MALLOC_TIMES; ++j)
		{
			pStrings[j] = (char *)Malloc(pPool);
			*pStrings[j] = '\0';
			//GenerateRandStr(pStrings[j], DEFAULT_STRING_SIZE-1);
		}
		for (int j=0; j<TEST_MALLOC_TIMES; ++j)
		{
			Free(pPool, pStrings[j]);
		}
	}
	gettimeofday(&endTime, NULL);
	DestroyMemoryPool(&pPool);
	costTime = 1000 * 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	printf("Memory pool Malloc/Free tested, malloc and free %d strings for %d times, cost %llu us.\n",
			TEST_MALLOC_TIMES, TEST_RETRY_TIMES, costTime);

	free(pStrings);
	return 0;
}
