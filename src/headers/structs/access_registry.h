/*
 * access_registry.h
 *
 *  Created on: Apr 13, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_ACCESS_REGISTRY_H_
#define SRC_HEADERS_STRUCTS_ACCESS_REGISTRY_H_

typedef struct{
	volatile int fieldReadLock;
	volatile int fieldWriteLock;

	volatile int consoleReadLock;
	volatile int consoleWriteLock;

	volatile int hudReadLock;
	volatile int hudWriteLock;

	volatile int fieldInventoryReadLock;
	volatile int fieldInventoryWriteLock;

	volatile int individualGroupReadLock;
	volatile int individualGroupWriteLock;

	volatile int dialogReadLock;
	volatile int dialogWriteLock;

	volatile int lookReadLock;
	volatile int lookWriteLock;
} lockAuthority;

#endif /* SRC_HEADERS_STRUCTS_ACCESS_REGISTRY_H_ */
