#pragma once
#include <Windows.h>

struct _TREE_NODE
{
	INT data;
	struct _TREE_NODE *left;
	struct _TREE_NODE *right;
	struct _TREE_NODE *parent;
};

typedef struct _TREE_NODE TREE_NODE;
typedef TREE_NODE *PTREE_NODE;
typedef PVOID *BINARY_TREE;
typedef VOID(*GET_DATA_CALLBACK)(INT);

BINARY_TREE
BTCreateBinTree(
	VOID);

BOOL
BTInsert(
	BINARY_TREE binTree,
	INT data);

BOOL
BTDelete(
	BINARY_TREE binTree,
	INT data);

BOOL
BTReplace(
	BINARY_TREE binTree,
	INT dataOld,
	INT dataNew);

VOID
BTFind(
	BINARY_TREE binTree,
	INT data,
	PBOOL res);

VOID
BTBypass(
	BINARY_TREE binTree,
	GET_DATA_CALLBACK callback);

VOID
BTDeleteBinTree(
	BINARY_TREE binTree);