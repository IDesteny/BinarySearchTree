#include "BinarySearchTree.h"

PTREE_NODE
_search(
	PTREE_NODE root,
	INT data)
{
	return root ? root->data == data ? root : _search(data < root->data ? root->left : root->right, data) : NULL;
}

PTREE_NODE
_min(
	PTREE_NODE root)
{
	return root ? root->left ? _min(root->left) : root : NULL;
}

PTREE_NODE
_max(
	PTREE_NODE root)
{
	return root ? root->right ? _max(root->right) : root : NULL;
}

VOID
_remove(
	PTREE_NODE root)
{
	if (root)
	{
		_remove(root->left);
		_remove(root->right);
		free(root);
	}
}

VOID
_bypass(
	PTREE_NODE node,
	GET_DATA_CALLBACK callback)
{
	if (node)
	{
		_bypass(node->right, callback);
		callback(node->data);
		_bypass(node->left, callback);
	}
}

BINARY_TREE
BTCreateBinTree(
	VOID)
{
	return calloc(1, sizeof(PTREE_NODE));
}

BOOL
BTInsert(
	BINARY_TREE binTree,
	INT data)
{
	if (!*binTree)
	{
		*binTree = calloc(1, sizeof(TREE_NODE));
		if (!*binTree)
			return EXIT_FAILURE;

		(*(PTREE_NODE *)binTree)->data = data;
		return EXIT_SUCCESS;
	}

	PTREE_NODE iter1 = *binTree, iter2 = *binTree;
	while (iter1)
	{
		iter2 = iter1;
		if (data == iter1->data)
			return EXIT_SUCCESS;

		iter1 = data < iter1->data ? iter1->left : iter1->right;
	}

	PTREE_NODE newNode = calloc(1, sizeof(TREE_NODE));
	if (!newNode)
		return EXIT_FAILURE;

	newNode->data = data;
	newNode->parent = iter2;

	if (data < iter2->data)
		iter2->left = newNode;
	else
		iter2->right = newNode;

	return EXIT_SUCCESS;
}

VOID
BTFind(
	BINARY_TREE binTree,
	INT data,
	PBOOL res)
{
	*res = !!_search(*binTree, data);
}

BOOL
BTDelete(
	BINARY_TREE binTree,
	INT data)
{
	PTREE_NODE target = _search(*binTree, data);
	if (!target)
		return EXIT_FAILURE;

	if (target == *binTree)
	{
		if (target->right && target->left)
		{
			target->right->parent = _max(target->left);
			target->right->parent->right = target->right;
			target->left->parent = NULL;
			*binTree = target->left;
		}
		else if (target->right && !target->left)
		{
			target->right->parent = NULL;
			*binTree = target->right;
		}
		else if (!target->right && target->left)
		{
			target->left->parent = NULL;
			*binTree = target->left;
		}
		else
		{
			*binTree = NULL;
		}
	}
	else
	{
		if (target->parent->right == target)
		{
			if (target->left && target->right)
			{
				target->right->parent = _max(target->left);
				target->right->parent->right = target->right;
				target->left->parent = target->parent;
				target->parent->right = target->left;
			}
			else if (target->left && !target->right)
			{
				target->left->parent = target->parent;
				target->parent->right = target->left;
			}
			else if (!target->left && target->right)
			{
				target->right->parent = target->parent;
				target->parent->right = target->right;
			}
			else
			{
				target->parent->right = NULL;
			}
		}
		else
		{
			if (target->left && target->right)
			{
				target->left->parent = _min(target->right);
				target->left->parent->left = target->left;
				target->right->parent = target->parent;
				target->parent->left = target->right;
			}
			else if (target->left && !target->right)
			{
				target->left->parent = target->parent;
				target->parent->left = target->left;
			}
			else if (!target->left && target->right)
			{
				target->right->parent = target->parent;
				target->parent->left = target->right;
			}
			else
			{
				target->parent->left = NULL;
			}
		}
	}

	free(target);
	return EXIT_SUCCESS;
}

VOID
BTDeleteBinTree(
	BINARY_TREE binTree)
{
	_remove(*binTree);
	*binTree = NULL;
}

BOOL
BTReplace(
	BINARY_TREE binTree,
	INT dataOld,
	INT dataNew)
{
	return
		BTDelete(binTree, dataOld) ||
		BTInsert(binTree, dataNew);
}

VOID
BTBypass(
	BINARY_TREE binTree,
	GET_DATA_CALLBACK callback)
{
	_bypass(*binTree, callback);
}