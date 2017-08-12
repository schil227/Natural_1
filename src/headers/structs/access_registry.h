/*
 * access_registry.h
 *
 *  Created on: Apr 13, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_ACCESS_REGISTRY_H_
#define SRC_HEADERS_STRUCTS_ACCESS_REGISTRY_H_

typedef struct{
	int fieldReadLock;
	int fieldWriteLock;

	int consoleReadLock;
	int consoleWriteLock;

	int hudReadLock;
	int hudWriteLock;

	int fieldInventoryReadLock;
	int fieldInventoryWriteLock;

	int individualGroupReadLock;
	int individualGroupWriteLock;

	int dialogReadLock;
	int dialogWriteLock;

	int lookReadLock;
	int lookWriteLock;

	int abilityCreationReadLock;
	int abilityCreationWriteLock;

	int worldMapReadLock;
	int worldMapWriteLock;
} lockAuthority;

#endif /* SRC_HEADERS_STRUCTS_ACCESS_REGISTRY_H_ */
