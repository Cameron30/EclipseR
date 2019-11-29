/*
 * Tree.h
 *
 *  Created on: Dec 4, 2017
 *      Author: camer
 */

#ifndef AVLTREE_H_
#define AVLTREE_H_
#include <iostream>
#include <string>
#include "Eclipse.h"
#define pow2(n) (1 << (n))

//avlTree code taken from:
//http://www.sanfoundry.com/cpp-program-implement-avl-trees/
//heavily edited/renamed to fit my needs

//minValueNode and deleteItem taken from:
//http://www.geeksforgeeks.org/avl-tree-set-2-deletion/
//slightly edited/renamed to fit variables and data


/*
 * avlNode Declaration
 */
struct avlNode
{
	Eclipse data;
	struct avlNode *left;
	struct avlNode *right;
	int position;
	std::string info;
}*root;

/*
 * Class Declaration
 */
template <class T> class avlTree
{
public:

	//counter of items
	int numItems;

	avlTree()
{
		numItems = 0;
		root = NULL;
}


	/*
	 * Height of AVL Tree
	 */
	int height(avlNode* temp)
	{
		int h = 0;
		if (temp != NULL)
		{
			int l_height = height(temp->left);
			int r_height = height(temp->right);
			int max_height;
			if (l_height > r_height) {
				max_height = l_height;

			}else {
				max_height = r_height;
			}

			h = max_height + 1;
		}
		return h;
	}

	/*
	 * Height Difference
	 */
	int diff(avlNode *temp)
	{
		int l_height = height (temp->left);
		int r_height = height (temp->right);
		int b_factor= l_height - r_height;
		return b_factor;
	}

	/*
	 * Right- Right Rotation
	 */
	avlNode *rr_rotation(avlNode *parent)
	{
		avlNode *temp;
		temp = parent->right;
		parent->right = temp->left;
		temp->left = parent;
		return temp;
	}
	/*
	 * Left- Left Rotation
	 */
	avlNode *ll_rotation(avlNode *parent)
	{
		avlNode *temp;
		temp = parent->left;
		parent->left = temp->right;
		temp->right = parent;
		return temp;
	}

	/*
	 * Left - Right Rotation
	 */
	avlNode *lr_rotation(avlNode *parent)
	{
		avlNode *temp;
		temp = parent->left;
		parent->left = rr_rotation (temp);
		return ll_rotation (parent);
	}

	/*
	 * Right- Left Rotation
	 */
	avlNode *rl_rotation(avlNode *parent)
	{
		avlNode *temp;
		temp = parent->right;
		parent->right = ll_rotation (temp);
		return rr_rotation (parent);
	}

	/*
	 * Balancing AVL Tree
	 */
	avlNode *balance(avlNode *temp)
	{
		int bal_factor = diff (temp);
		if (bal_factor > 1)
		{
			if (diff (temp->left) > 0)
				temp = ll_rotation (temp);
			else
				temp = lr_rotation (temp);
		}
		else if (bal_factor < -1)
		{
			if (diff (temp->right) > 0)
				temp = rl_rotation (temp);
			else
				temp = rr_rotation (temp);
		}
		return temp;
	}

	/*
	 * Insert Element into the tree
	 */
	avlNode *insert(avlNode *root, T value, int position, string info)
	{
		if (root == NULL)
		{
			//set node information
			root = new avlNode;
			root->data = value;
			root->left = NULL;
			root->right = NULL;
			root->position = position;
			root->info = info;
			return root;
		}
		else if (position < root->position)
		{
			root->left = insert(root->left, value, position, info);
			root = balance (root);
		}
		else if (position >= root->position)
		{
			root->right = insert(root->right, value, position, info);
			root = balance (root);
		}
		return root;

		//add to counter
		numItems += 1;
	}

	/* Given a non-empty binary search tree, return the
	   node with minimum key value found in that tree.
	   Note that the entire tree does not need to be
	   searched. */
	struct avlNode * minValueNode(avlNode* node)
	{
	    avlNode* current = node;

	    /* loop down to find the leftmost leaf */
	    while (current->left != NULL)
	        current = current->left;

	    return current;
	}

	// Recursive function to delete a node with given key
	// from subtree with given root. It returns root of
	// the modified subtree.
	avlNode *deleteItem(avlNode* root, int key)
	{
	    // STEP 1: PERFORM STANDARD BST DELETE
	    if (root == NULL)
	        return root;

	    // If the key to be deleted is smaller than the
	    // root's key, then it lies in left subtree
	    if (key < root->position)
	        root->left = deleteItem(root->left, key);

	    // If the key to be deleted is greater than the
	    // root's key, then it lies in right subtree
	    else if( key > root->position )
	        root->right = deleteItem(root->right, key);

	    // if key is same as root's key, then This is
	    // the node to be deleted
	    else
	    {
	        // node with only one child or no child
	        if( (root->left == NULL) || (root->right == NULL) )
	        {
	            avlNode *temp = root->left ? root->left : root->right;

	            // No child case
	            if (temp == NULL)
	            {
	                temp = root;
	                root = NULL;
	            }
	            else // One child case
	             *root = *temp; // Copy the contents of
	                            // the non-empty child
	            free(temp);
	        }
	        else
	        {
	            // node with two children: Get the inorder
	            // successor (smallest in the right subtree)
	            avlNode* temp = minValueNode(root->right);

	            // Copy the inorder successor's data to this node
	            root = temp;

	            // Delete the inorder successor
	            root->right = deleteItem(root->right, temp->position);
	        }
	    }

	    //subtract one from total if successful
	    numItems -= 1;
	    return root;
	}

	/*
	 * Display AVL Tree
	 */
	void display(avlNode *ptr, int level)
	{
		int i;
		if (ptr!=NULL)
		{
			display(ptr->right, level + 1);
			printf("\n");
			if (ptr == root)
				std::cout<<"Root -> ";
			for (i = 0; i < level && ptr != root; i++)
				std::cout<<"        ";
			std::cout<<ptr->data;
			display(ptr->left, level + 1);
		}
	}

	/*
	 * Inorder Traversal of AVL Tree
	 */
	void inorder(avlNode *tree)
	{
		if (tree == NULL)
			return;
		inorder (tree->left);
		std::cout<<tree->info;;
		inorder (tree->right);
	}
	/*
	 * Preorder Traversal of AVL Tree
	 */
	void preorder(avlNode *tree)
	{
		if (tree == NULL)
			return;
		std::cout<<tree->info<<"  ";
		preorder (tree->left);
		preorder (tree->right);

	}

	/*
	 * Postorder Traversal of AVL Tree
	 */
	void postorder(avlNode *tree)
	{
		if (tree == NULL)
			return;
		postorder ( tree ->left );
		postorder ( tree ->right );
		std::cout<<tree->info<<"  ";
	}
};

#endif /* AVLTREE_H_ */
