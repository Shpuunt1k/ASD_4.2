#pragma once

#include "Spisok.h"
#include <iostream>
#include <Windows.h>
#include <exception>
#include <string>

#define RED 0
#define BLACK 1

#define underlineON "\033[4m"
#define underlineOFF "\033[0m"

using namespace std;

template <typename T0, typename T1>
class Map
{
public:
	Map();
	~Map();

	unsigned int GetSize()
	{
		return size;
	}

	void insert(T0 key, T1 value);
	void remove(T0 key);
	T1 find(T0 key);
	bool change(T0 key, T1 NewValue);
	void clear();

	void printTree();
	void printList();

	void get_keys(List<T0>& map)
	{
		map.clear();
		if (size == 0)
			return;
		keyFill(map, root);
	}
	void get_values(List<T1>& map)
	{
		map.clear();
		if (size == 0)
			return;
		valueFill(map, root);
	}

private:

	template <typename T0, typename T1>
	class Node
	{
	public:
		Node* parent;
		Node* left;
		Node* right;
		bool color;
		T0 key;
		T1 value;

		Node(T0 key = T0(), T1 value = T1(), Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr, bool color = RED)
		{
			this->key = key;
			this->value = value;
			this->parent = parent;
			this->left = left;
			this->right = right;
			this->color = color;
		}
	};

	void printTree(int index, int spaces, Node<T0, T1>* q);
	void printList(Node<T0, T1>* current);

	void insert(Node<T0, T1>* parent, T0 key, T1 value);
	void uncleCheck(Node<T0, T1>* node);

	void remove(Node<T0, T1>* node);
	void removeFIX(Node<T0, T1>* node, bool leafs);

	void clear(Node<T0, T1>* node);

	void keyFill(List<T0>& lst, Node<T0, T1>* current);
	void valueFill(List<T1>& lst, Node<T0, T1>* current);

	Node<T0, T1>* root;
	Node<T0, T1>* leaf;
	unsigned int size;
};

template <typename T0, typename T1>
Map<T0, T1>::Map()
{
	size = 0;
	root = nullptr;
	leaf = new Node<T0, T1>(NULL, NULL, nullptr, nullptr, nullptr, BLACK);
}

template <typename T0, typename T1>
Map<T0, T1>::~Map()
{
	clear();
}

template <typename T0, typename T1>
void Map<T0, T1>::insert(T0 key, T1 value)
{
	if (size == 0)
	{
		root = new Node<T0, T1>(key, value, nullptr, leaf, leaf, BLACK);
		size++;
		return;
	}
	insert(root, key, value);
}

template <typename T0, typename T1>
void Map<T0, T1>::printTree()
{
	int index = 0;
	int spaces = 0;

	if (index < size)
	{
		printTree(index, spaces + 4, root->right);
		cout << underlineON;
		cout << root->key;
		cout << underlineOFF << endl;
		cout << underlineON;
		cout << root->value;
		cout << underlineOFF << endl;
		printTree(index, spaces + 4, root->left);
		index++;
	}
}

template<typename T0, typename T1>
void Map<T0, T1>::printList()
{
	if (size == 0)
		return;
	printList(root->left);

	cout << root->key << " - " << root->value << endl;

	printList(root->right);
}

template <typename T0, typename T1>
void Map<T0, T1>::remove(T0 key)
{
	Node<T0, T1>* node = root;
	while (node != leaf)
	{
		if (node->key == key)
		{
			remove(node);
			node = root;
		}
		else if (node->key > key)
			node = node->left;
		else
			node = node->right;
	}
}

template <typename T0, typename T1>
T1 Map<T0, T1>::find(T0 key)
{
	if (size == 0)
		return NULL;
	Node<T0, T1>* current = root;
	while (current != leaf)
	{
		if (key == current->key)
			return current->value;
		else if (key < current->key)
			current = current->left;
		else
			current = current->right;
	}
	return NULL;
}

template<typename T0, typename T1>
bool Map<T0, T1>::change(T0 key, T1 NewValue)
{
	if (size == 0)
		return false;
	Node<T0, T1>* current = root;
	while (current != leaf)
	{
		if (key == current->key)
		{
			current->value = NewValue;
			return true;
		}

		else if (key < current->key)
			current = current->left;
		else
			current = current->right;
	}
	return false;
}

template <typename T0, typename T1>
void Map<T0, T1>::clear()
{
	if (size == 0)
		return;
	clear(root->left);
	clear(root->right);
	delete root;
	size--;
}

template <typename T0, typename T1>
void Map<T0, T1>::printTree(int index, int spaces, Node<T0, T1>* q)
{
	if (index < size)
	{
		if (q != leaf)
			printTree(index, spaces + 4, q->right);
		for (int i = 0; i < spaces; i++)
			cout << ' ';
		if (q->color == BLACK)
		{
			cout << underlineON;
			if (q == leaf)
				cout << "NIL";
			else
			{
				cout << q->key;
				cout << underlineOFF << endl;
				for (int i = 0; i < spaces; i++)
					cout << ' ';
				cout << underlineON;
				cout << q->value;
			}
			cout << underlineOFF << endl;
		}
		else
		{
			cout << q->key << endl;
			for (int i = 0; i < spaces; i++)
				cout << ' ';
			cout << q->value << endl;
		}

		if (q != leaf)
			printTree(index, spaces + 4, q->left);
		if (q != leaf)
			index++;
	}
}

template<typename T0, typename T1>
void Map<T0, T1>::printList(Node<T0, T1>* current)
{
	if (current == leaf)
		return;
	printList(current->left);

	cout << current->key << " - " << current->value << endl;

	printList(current->right);
}

template <typename T0, typename T1>
void Map<T0, T1>::insert(Node<T0, T1>* parent, T0 key, T1 value)
{
	if (key == parent->key)
		throw exception("attempt to add an existant key into map");
	else if (key < parent->key)
	{
		if (parent->left != leaf)
			insert(parent->left, key, value);
		else
		{
			parent->left = new Node<T0, T1>(key, value, parent, leaf, leaf);
			size++;
			if (parent->color == RED) // parent is red
				uncleCheck(parent->left);
		}
	}
	else
	{
		if (parent->right != leaf)
			insert(parent->right, key, value);
		else
		{
			parent->right = new Node<T0, T1>(key, value, parent, leaf, leaf);
			size++;
			if (parent->color == RED) // parent is red
				uncleCheck(parent->right);
		}
	}
}

template <typename T0, typename T1>
void Map<T0, T1>::uncleCheck(Node<T0, T1>* node)
{
	Node<T0, T1>* parent = node->parent;
	Node<T0, T1>* grandparent = parent->parent;

	if (grandparent->left == parent) // uncle is right
	{
		Node<T0, T1>* uncle = grandparent->right;

		if (uncle->color == RED) // uncle is right red
		{
			parent->color = BLACK;
			uncle->color = BLACK;
			if (grandparent == root)
				return;
			grandparent->color = RED;
			if (grandparent->parent->color == RED) // grand-grandparent is red, so we check his uncle now
				uncleCheck(grandparent);
		}
		else // uncle is right black
		{
			if (parent->right == node) // " < " scenario, additional rotation needed
			{
				parent->right = node->left;
				parent->right->parent = parent;
				parent->parent = node;
				node->parent = grandparent;
				node->left = parent;
				grandparent->left = node;
				// now there is a " / " instead of " < "

				// finally, we swap node and parent links for further " / " rotation (because they both were swapped before)
				node = parent;
				parent = parent->parent;
			}

			grandparent->left = parent->right;
			if (grandparent->left != leaf)
				grandparent->left->parent = grandparent;
			grandparent->left->parent = grandparent;
			parent->right = grandparent;

			parent->parent = grandparent->parent;

			if (grandparent != root) // grand-grandparent exists
			{
				if (grandparent->parent->left == grandparent) // grandparent is left
					grandparent->parent->left = parent;
				else // grandparent is right
					grandparent->parent->right = parent;
			}
			else // grand-grandparent don't exist => grandparent is a root => parent is root now
				root = parent;

			grandparent->parent = parent;

			parent->color = BLACK;
			grandparent->color = RED;
		}
	}
	else // uncle is left
	{
		Node<T0, T1>* uncle = grandparent->left;

		if (uncle->color == RED) // uncle is left red
		{
			parent->color = BLACK;
			uncle->color = BLACK;
			if (grandparent == root)
				return;
			grandparent->color = RED;
			if (grandparent->parent->color == RED) // grand-grandparent is red, so we check his uncle now
				uncleCheck(grandparent);
		}
		else // uncle is left black
		{
			if (parent->left == node) // " > " scenario, additional rotation needed
			{
				parent->left = node->right;
				parent->left->parent = parent;
				parent->parent = node;
				node->parent = grandparent;
				node->right = parent;
				grandparent->right = node;
				// now there is a " \ " instead of " > "

				node = parent;
				parent = parent->parent;
			}

			Node<T0, T1>* sibling = parent->left;

			grandparent->right = parent->left;
			if (grandparent->right != leaf)
				grandparent->right->parent = grandparent;
			parent->left = grandparent;

			parent->parent = grandparent->parent;

			if (grandparent != root) // grand-grandparent exists
			{
				if (grandparent->parent->right == grandparent) // grandparent is right
				{
					grandparent->parent->right = parent;
				}
				else // grandparent is left
					grandparent->parent->left = parent;
			}
			else // grand-grandparent don't exist => grandparent is a root => parent is root now
				root = parent;

			grandparent->parent = parent;

			parent->color = BLACK;
			grandparent->color = RED;
		}
	}
}

template <typename T0, typename T1>
void Map<T0, T1>::remove(Node<T0, T1>* node)
{
	if (node->left == leaf && node->right == leaf) // case 1: node doesn't have any non-leaf kids
	{
		if (node == root) // node is root
		{
			root = nullptr;
			delete node;
			size--;
		}
		else if (node->color == RED)
		{
			if (node->parent->left == node) // node is left
				node->parent->left = leaf;
			else // node is right
				node->parent->right = leaf;
			delete node;
			size--;
		}
		else
			removeFIX(node, true);
	}
	else if ((node->left != leaf) && (node->right == leaf)) // case 2.1: one child (left) isn't a leaf
	{
		if (node == root) // node is root
		{
			root = node->left;
			node->left->parent = nullptr;
			node->left->color = BLACK;
			delete node;
			size--;
		}
		else if (node->color == RED)
		{
			if (node->parent->left == node) // node is left
			{
				node->parent->left = node->left;
				node->left->parent = node->parent;
			}
			else // node is right
			{
				node->parent->right = node->left;
				node->left->parent = node->parent;
			}
			delete node;
			size--;
		}
		else
			removeFIX(node, true);
	}
	else if ((node->right != leaf) && (node->left == leaf)) // case 2.2: one child (right) isn't a leaf
	{
		if (node == root) // node is root
		{
			root = node->right;
			node->right->parent = nullptr;
			node->right->color = BLACK;
			delete node;
			size--;
		}
		else if (node->color == RED)
		{
			if (node->parent->left == node) // node is left
			{
				node->parent->left = node->right;
				node->right->parent = node->parent;
			}
			else // node is right
			{
				node->parent->right = node->right;
				node->right->parent = node->parent;
			}
			delete node;
			size--;
		}
		else
			removeFIX(node, true);
	}
	else // case 3: both childs aren't leafs
	{
		Node<T0, T1>* current = node->right;
		while (current->left != leaf)
			current = current->left;
		node->key = current->key;
		node->value = current->value;
		remove(current);
	}
}

template <typename T0, typename T1>
void Map<T0, T1>::removeFIX(Node<T0, T1>* node, bool leafs)
{
	// Node there can be only RED an non-root, because of remove(Node<T0, T1>* node) construction

	Node<T0, T1>* parent = node->parent;
	bool nodeLeft; // will be used only in worst case. where node's childs are leafs;

	if (leafs == true)
	{
		if (node->left != leaf) // case 0.1.1: one of node's childs (left) aren't leaf (if so, child can obly be RED)
		{
			node->left->parent = parent;
			node->left->color = BLACK;
			if (node == parent->left) // node is left
				parent->left = node->left;
			else // node is right
				parent->right = node->left;
			delete node;
			size--;
			return;
		}
		else if (node->right != leaf) // case 0.1.2: one of node's childs (right) aren't leaf (if so, child can obly be RED)
		{
			node->right->parent = parent;
			node->right->color = BLACK;
			if (node == parent->left) // node is left
				parent->left = node->right;
			else // node is right
				parent->right = node->right;
			delete node;
			size--;
			return;
		}
		else // case 0.2: both childs are leafs;
		{
			if (node == parent->left) // node is left
			{
				nodeLeft = true;
				parent->left = leaf;
			}
			else // node is right
			{
				nodeLeft = false;
				parent->right = leaf;
			}
			delete node;
			size--;
		}
	}

	Node<T0, T1>* grandparent = parent->parent;
	Node<T0, T1>* sibling;
	if (leafs == true)
	{
		if (nodeLeft)
			sibling = parent->right;
		else
			sibling = parent->left;
	}
	else
	{
		if (node == parent->left)
		{
			nodeLeft = true;
			sibling = parent->right;
		}

		else
		{
			nodeLeft = false;
			sibling = parent->left;
		}
	}

	Node<T0, T1>* SL = sibling->left;
	Node<T0, T1>* SR = sibling->right;

	if (sibling->color == RED) // case 1, not final, continues into case 3, 4 or 5
	{
		parent->parent = sibling;
		SL->parent = parent;

		if (root == parent)
		{
			root = sibling;
			sibling->parent = nullptr;
		}
		else
		{
			if (grandparent->left == parent) // parent is left
				grandparent->left = sibling;
			else // parent is right
				grandparent->right = sibling;
			sibling->parent = grandparent;
		}

		if (nodeLeft == true)
		{
			parent->right = SL;
			sibling->left = parent;
		}
		else
		{
			parent->left = SL;
			sibling->right = parent;
		}

		parent->color = RED;
		sibling->color = BLACK;

		sibling = SL; // because this case isn't final, wer are renaming sone of the nodes
		SL = sibling->left;
		SR = sibling->right;
	}

	if (parent->color == BLACK && sibling->color == BLACK && SL->color == BLACK && SR->color == BLACK) // case 2, not final, requires checking parent as node
	{
		sibling->color = RED;
		removeFIX(parent, false);
		return;
	}

	if (sibling->color == BLACK && SL->color == BLACK && SR->color == BLACK && parent->color == RED) // case 3, final
	{
		parent->color = BLACK;
		sibling->color = RED;
		return;
	}

	if (nodeLeft == true) // left node scenarios
	{
		if (SL->color == RED && SR->color == BLACK) // case 4, not final, continues into case 5
		{
			SL->parent = parent;
			parent->right = SL;
			SL->right->parent = sibling;
			sibling->left = SL->right;
			SL->right = sibling;
			sibling->parent = SL;
			SL->color = BLACK;
			sibling->color = RED;
			sibling = SL;
			SL = SL->left;
			SR = sibling;
		}

		if (SR->color == RED) // case 5, final
		{
			sibling->color = parent->color;
			parent->color = BLACK;
			SR->color = BLACK;
			parent->right = SL;
			SL->parent = parent;
			sibling->left = parent;
			parent->parent = sibling;
			if (root == parent)
			{
				root = sibling;
				sibling->parent = nullptr;
			}
			else
			{
				sibling->parent = grandparent;
				if (grandparent->left == parent) // parent is left
					grandparent->left = sibling;
				else // parent is right
					grandparent->right = sibling;
			}
		}
	}
	else // right node case
	{
		if (SR->color == RED && SL->color == BLACK) // case 4, not final, continues into case 5
		{
			SR->parent = parent;
			parent->left = SR;
			SR->left->parent = sibling;
			sibling->right = SR->left;
			SR->left = sibling;
			sibling->parent = SR;
			SR->color = BLACK;
			sibling->color = RED;
			sibling = SR;
			SL = sibling;
			SR = SR->right;
		}

		if (SL->color == RED) // case 5, final
		{
			sibling->color = parent->color;
			parent->color = BLACK;
			SL->color = BLACK;
			parent->left = SR;
			SR->parent = parent;
			sibling->right = parent;
			parent->parent = sibling;
			if (root == parent)
			{
				root = sibling;
				sibling->parent = nullptr;
			}
			else
			{
				sibling->parent = grandparent;
				if (grandparent->left == parent) // parent is left
					grandparent->left = sibling;
				else // parent is right
					grandparent->right = sibling;
			}
		}
	}

	// earlier leaf was able to gain unwelcome changes. It's more easy to just write down extra 4 lines than many if-elses throughout code
	leaf->color = BLACK;
	leaf->parent = nullptr;
	leaf->left = nullptr;
	leaf->right = nullptr;
}

template <typename T0, typename T1>
void Map<T0, T1>::clear(Node<T0, T1>* node)
{
	if (node == leaf)
		return;
	clear(node->left);
	clear(node->right);
	delete node;
	size--;
}

template <typename T0, typename T1>
void Map<T0, T1>::keyFill(List<T0>& lst, Node<T0, T1>* current)
{
	if (current == leaf)
		return;
	keyFill(lst, current->left);
	lst.push_back(current->key);
	keyFill(lst, current->right);
}

template <typename T0, typename T1>
void Map<T0, T1>::valueFill(List<T1>& lst, Node<T0, T1>* current)
{
	if (current == leaf)
		return;
	valueFill(lst, current->left);
	lst.push_back(current->value);
	valueFill(lst, current->right);
}