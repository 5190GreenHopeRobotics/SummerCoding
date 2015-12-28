/*
 * binarySearchTree.hh
 *
 *  Created on: Dec 28, 2015
 *      Author: sdai
 */

#ifndef BINARYSEARCHTREE_HH_
#define BINARYSEARCHTREE_HH_

#ifndef nullptr
#define nullptr 0
#endif

template<typename K, typename T>
struct binarySearchNode {
public:
	binarySearchNode();
	binarySearchNode(const K& key, const T& data);
	K key;
	T data;
	binarySearchNode<K, T>* left;
	binarySearchNode<K, T>* right;
	~binarySearchNode();
};

template<typename K, typename T>
class binarySearchTree {
public:
	binarySearchTree();
	binarySearchTree(const binarySearchTree<K, T>& src);
	binarySearchTree<K, T>& operator=(const binarySearchTree& src);
	bool doesContain(const K& key) const;
	void add(const K& key, const T& data);
	void remove(const K& key);
	T* find(const K& key) const;
	T& operator[](const K& key);
	template<typename F>
	void orderedTraverse(F& function);
	~binarySearchTree();
private:
	binarySearchNode<K, T>* root;
	binarySearchNode<K, T>* recurseFind(binarySearchNode<K, T>* root,
			const K& key) const;
	void recurseCopy(binarySearchNode<K, T>*& root);
	void recurseAdd(binarySearchNode<K, T>*& node,
			binarySearchNode<K, T>*& newNode);
	void recurseDelete(binarySearchNode<K, T>*& node, const K& key);
	template<typename F>
	void recurseTraverse(binarySearchNode<K, T>*& root, F& function);
};

template<typename K, typename T>
binarySearchNode<K, T>::binarySearchNode() {
	left = nullptr;
	right = nullptr;
}

template<typename K, typename T>
binarySearchNode<K, T>::binarySearchNode(const K& key, const T& data) :
		binarySearchNode() {
	this->key = key;
	this->data = data;
}

template<typename K, typename T>
binarySearchNode<K, T>::~binarySearchNode() {
	if (left != nullptr) {
		delete left;
	}
	if (right != nullptr) {
		delete right;
	}
}

template<typename K, typename T>
binarySearchTree<K, T>::binarySearchTree() {
	root = nullptr;
}

template<typename K, typename T>
binarySearchTree<K, T>::binarySearchTree(const binarySearchTree<K, T>& src) {
	recurseCopy(src.root);
}

template<typename K, typename T>
binarySearchTree<K, T>& binarySearchTree<K, T>::operator =(
		const binarySearchTree<K, T>& src) {
	if (root != nullptr) {
		delete root;
	}
	recurseCopy(src.root);
}

template<typename K, typename T>
binarySearchTree<K, T>::~binarySearchTree() {
	if (root != nullptr) {
		delete root;
	}
}

template<typename K, typename T>
void binarySearchTree<K, T>::recurseAdd(binarySearchNode<K, T>*& node,
		binarySearchNode<K, T>*& newNode) {
	if (node == nullptr) {
		node = newNode;
		return;
	}
	if (newNode->key > node->key) {
		recurseAdd(node->right, newNode);
	} else {
		recurseAdd(node->left, newNode);
	}
}

template<typename K, typename T>
binarySearchNode<K, T>* binarySearchTree<K, T>::recurseFind(
		binarySearchNode<K, T>* root, const K& key) const {
	if (root == nullptr) {
		return root;
	}
	if (root->key == key) {
		return root;
	}
	if (key > root->key) {
		return recurseFind(root->right, key);
	} else {
		return recurseFind(root->left, key);
	}
}

template<typename K, typename T>
void binarySearchTree<K, T>::recurseDelete(binarySearchNode<K, T>*& node,
		const K& key) {
	if (node == nullptr) {
		return;
	}
	if (node->key == key) {
		binarySearchNode<K, T>* temp;
		binarySearchNode<K, T>* replacement = nullptr;
		if (node->left == nullptr) {
			replacement = node->right;
		} else {
			replacement = node->left;
			binarySearchNode<K, T>* prev = nullptr;
			while (true) {
				if (replacement->right == nullptr) {
					if (prev != nullptr) {
						prev->right = nullptr;
					}
					replacement->right = node->right;
					break;
				}
				replacement = replacement->right;
			}
		}
		temp = node;
		node = replacement;
		temp->left = nullptr;
		temp->right = nullptr;
		delete temp;
	}

}
template<typename K, typename T>
void binarySearchTree<K, T>::recurseCopy(binarySearchNode<K, T>*& root) {
	if (root == nullptr) {
		return;
	}
	recurseCopy(root->left);
	this->add(root->key, root->data);
	recurseCopy(root->right);
}

template<typename K, typename T>
template<typename F>
void binarySearchTree<K, T>::recurseTraverse(binarySearchNode<K, T>*& root,
		F& function) {
	if (root == nullptr) {
		return;
	}
	recurseTraverse(root->left, function);
	function(root);
	recurseTraverse(root->right, function);
}

template<typename K, typename T>
bool binarySearchTree<K, T>::doesContain(const K& key) const {
	binarySearchNode<K, T>* result = recurseFind(root, key);
	if (result != nullptr) {
		return true;
	} else {
		return false;
	}
}
template<typename K, typename T>
void binarySearchTree<K, T>::add(const K& key, const T& data) {
	binarySearchNode<K, T>* newNode = new binarySearchNode<K, T>(key, data);
	recurseAdd(root, newNode);
}

template<typename K, typename T>
void binarySearchTree<K, T>::remove(const K& key) {
	recurseDelete(root, key);
}

template<typename K, typename T>
T* binarySearchTree<K, T>::find(const K& key) const {
	return &((recurseFind(root, key))->data);
}

template<typename K, typename T>
T& binarySearchTree<K, T>::operator [](const K& key) {
	binarySearchNode<K, T>* result = recurseFind(root, key);
	if (result != nullptr) {
		return result->data;
	} else {
		binarySearchNode<K, T> * newNode = new binarySearchNode<K, T>();
		newNode->key = key;
		recurseAdd(root, newNode);
		return newNode->data;
	}
}

template<typename K, typename T>
template<typename F>
void binarySearchTree<K, T>::orderedTraverse(F& function) {
	recurseTraverse(root, function);
}
#endif /* BINARYSEARCHTREE_HH_ */
