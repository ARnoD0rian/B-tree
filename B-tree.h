#include<iostream>
#include<string>
#include<map>
#include<list>
using namespace std;

map<string, list<unsigned long long>> personKeyList;


const int t = 2;
template <typename TValue>
class Node {
public:
	int num_key = 1;
	int key[2*t - 1];
	TValue value[2 * t - 1];
	bool leaf = true;
	Node<TValue>* c[2*t];
};

template <typename T>
class tree {
	Node<T>*  root = nullptr;
public:
	void search(int k) {
		b_tree_search(root, k);
	}
	void print() {
		printBTree(root, 0);
	}
	void insert(T v, int k) {
		if (root == nullptr) {
			create(v, k);
			return;
		}
		if (root->num_key == 2 * t - 1) {
			Node<T>* r = root;
			Node<T>* s = new Node<T>();
			root = s;
			s->num_key = 0;
			root->leaf = false;
			root->c[0] = r;
			split_child(root, r, 0);
			insert_nonfull(root, v, k);
		}
		else {
			insert_nonfull(root, v, k);
		}
	}
	void remove(int k){
		T value_index = b_tree_search(root, k);
		if (value_index == NULL) {
			return;
		}
		int index;
		Node<T>* element = root;
		Node<T>* parent = root;
		for (index = 0; index < element->num_key; ++index) {
			if (k == element->key[index]) {
				break;
			}
			if (k < element->key[index]) {
				parent = element;
				element = element->c[index];
				index = -1;
			}
			else {
				if (index == element->num_key - 1) {
					parent = element;
					element = element->c[index + 1];
					index = -1;
				}
			}
		}
		if (element->leaf) {
			if (element->num_key > t - 1) {
				remove_element(element, k);
			}
			else {
				remove_from_leaf(parent, element, k);
			}
		}
		else {
			remove_BTree(parent, element, k);
		}
	}
private:
	void remove_element(Node<T>* element, int k) {
		for (int i = 0; i < element->num_key; ++i) {
			if (k == element->key[i]) {
				for (int j = i; j < element->num_key - 1; ++j) {
					element->key[j] = element->key[j + 1];
					element->value[j] = element->value[j + 1];
				}
			}
		}
		element->num_key--;
	}
	void lconnect(Node<T>* parent, Node<T>* element, Node<T>* brother, int positionSon) {
		for (int i = 0; i < t - 1; ++i) {
			element->key[i + t] = brother->key[i];
			element->value[i + t] = brother->value[i];
			element->c[i + t] = brother->c[i];
		}
		delete brother;
		element->key[t - 1] = parent->key[positionSon];
		element->value[t - 1] = parent->value[positionSon];
		element->num_key = 2 * t - 1;
		print();
	}
	void rconnect(Node<T>* parent, Node<T>* element, Node<T>* brother, int positionSon) {
		for (int i = 0; i < t - 1; ++i) {
			element->key[i + t] = element->key[i];
			element->value[i + t] = element->value[i + t];
			element->c[i + t] = element->c[i + t];
			element->key[i] = brother->key[i];
			element->value[i] = brother->value[i];
			element->c[i] = brother->c[i];
		}
		delete brother;
		element->key[t - 1] = parent->key[positionSon];
		element->value[t - 1] = parent->value[positionSon];
		element->num_key = 2 * t - 1;
		remove(parent->key[positionSon]);
	}
	Node<T>* search_parent(Node<T>* child) {
		int index;
		int k = child->key[0];
		Node<T>* element = root;
		Node<T>* parent = root;
		for (index = 0; index < element->num_key; ++index) {
			if (k == element->key[index]) {
				break;
			}
			if (k < element->key[index]) {
				parent = element;
				element = element->c[index];
				index = -1;
			}
			else {
				if (index == element->num_key - 1) {
					parent = element;
					element = element->c[index + 1];
					index = -1;
				}
			}
		}
		return parent;
	}
	void merge_parent(Node<T>* element) {
		Node<T>* parent = search_parent(element);
		if ((parent->num_key == t - 1) && (parent != root)) {
			merge_parent(parent);
		}
		int positionSon;
		for (int i = 0; i < parent->num_key; ++i) {
			if (parent->c[i] == element) {
				positionSon = i;
				break;
			}
		}
		if (positionSon == 0) {
			if (parent->c[1]->num_key > t - 1) {
				element->key[t - 1] = parent->key[0];
				element->value[t - 1] = parent->value[0];
				element->c[t] = parent->c[1]->c[0];
				parent->key[0] = parent->c[1]->key[0];
				parent->value[0] = parent->c[1]->value[0];
				for (int i = 0; i < parent->c[1]->num_key - 1; ++i) {
					parent->c[1]->c[i] = parent->c[1]->c[i + 1];
					parent->c[1]->key[i] = parent->c[1]->key[i + 1];
					parent->c[1]->value[i] = parent->c[1]->value[i + 1];
				}
				parent->c[1]->c[parent->c[1]->num_key - 1] = parent->c[1]->c[parent->c[1]->num_key];
				parent->c[1]->num_key--;
			}
			else {
				lconnect(parent, element, parent->c[1], positionSon);
			}
		}
		else if (positionSon == parent->num_key) {
			if (parent->c[positionSon - 1]->num_key > t - 1) {
				for (int i = element->num_key; i > 0; --i) {
					element->key[i] = element->key[i - 1];
					element->value[i] = element->value[i - 1];
					element->c[i + 1] = element->c[i];
				}
				element->c[1] = element->c[0];
				element->key[0] = parent->key[positionSon - 1];
				element->value[0] = parent->value[positionSon - 1];
				element->c[0] = parent->c[positionSon - 1]->c[parent->c[positionSon - 1]->num_key];
				parent->key[positionSon - 1] = parent->c[positionSon - 1]->key[parent->c[positionSon - 1]->num_key - 1];
				parent->value[positionSon - 1] = parent->c[positionSon - 1]->value[parent->c[positionSon - 1]->num_key - 1];
				parent->c[positionSon - 1]->num_key--;
			}
			else {
				rconnect(parent, element, parent->c[positionSon - 1], positionSon);
			}

		}else{
			if ((parent->c[positionSon - 1]->num_key > t - 1) || (parent->c[positionSon - 1]->num_key > t - 1)) {
				if (parent->c[positionSon - 1]->num_key > t - 1) {
					for (int i = element->num_key; i > 0; --i) {
						element->key[i] = element->key[i - 1];
						element->value[i] = element->value[i - 1];
						element->c[i + 1] = element->c[i];
					}
					element->c[1] = element->c[0];
					element->key[0] = parent->key[positionSon - 1];
					element->value[0] = parent->value[positionSon - 1];
					element->c[0] = parent->c[positionSon - 1]->c[parent->c[positionSon - 1]->num_key];
						parent->key[positionSon - 1] = parent->c[positionSon - 1]->key[parent->c[positionSon - 1]->num_key - 1];
					parent->value[positionSon - 1] = parent->c[positionSon - 1]->value[parent->c[positionSon - 1]->num_key - 1];
					parent->c[positionSon - 1]->num_key--;
				}
				else {
					element->key[t - 1] = parent->key[positionSon];
					element->value[t - 1] = parent->value[positionSon];
					element->c[t] = parent->c[positionSon + 1]->c[0];
					parent->key[positionSon] = parent->c[positionSon + 1]->key[0];
					parent->value[positionSon] = parent->c[positionSon + 1]->value[0];
					for (int i = 0; i < parent->c[1]->num_key - 1; ++i) {
						parent->c[positionSon + 1]->c[i] = parent->c[positionSon + 1]->c[i + 1];
						parent->c[positionSon + 1]->key[i] = parent->c[positionSon + 1]->key[i + 1];
						parent->c[positionSon + 1]->value[i] = parent->c[positionSon + 1]->value[i + 1];
					}
					parent->c[positionSon + 1]->c[parent->c[positionSon + 1]->num_key - 1] = parent->c[positionSon + 1]->c[parent->c[positionSon + 1]->num_key];
					parent->c[positionSon + 1]->num_key--;
				}
			}
			else {
				rconnect(parent, element, parent->c[positionSon - 1], positionSon);
			}
		}
	}
		void remove_from_leaf(Node<T>*parent, Node<T>*element, int k) {
			if (element == root) {
				if (root->num_key == 1) {
					delete root;
					root = nullptr;
				}
				else {
					remove_element(root, k);
				}
				return;
			}
			int position, positionSon;
			for (int i = 0; i < element->num_key; ++i) {
				if (k == element->key[i]) {
					position = i;
					break;
				}
			}
			for (int i = 0; i <= parent->num_key; ++i) {
				if (parent->c[i] == element) {
					positionSon = i;
					break;
				}
			}
			if (positionSon == 0) {
				if (parent->c[1]->num_key > t - 1) {
					for (int i = position; i < element->num_key - 1; ++i) {
						element->key[i] = element->key[i + 1];
						element->value[i] = element->value[i + 1];
					}
					element->key[element->num_key - 1] = parent->key[0];
					element->value[element->num_key - 1] = parent->value[0];
					parent->key[0] = parent->c[1]->key[0];
					parent->value[0] = parent->c[1]->value[0];
					remove_element(parent->c[1], parent->c[1]->key[0]);
				}
				else {
					if (parent->num_key > t - 1) {
						lconnect(parent, element, parent->c[1], positionSon);
					}
					else {
						merge_parent(parent);
					}
					remove_element(element, k);
				}
			}
			else if (positionSon == parent->num_key) {
				if (parent->c[positionSon - 1]->num_key > t - 1) {
					for (int i = position; i > 0; --i) {
						element->key[i] = element->key[i - 1];
						element->value[i] = element->value[i - 1];
					}
					element->key[0] = parent->key[positionSon - 1];
					element->value[0] = parent->value[positionSon - 1];
					parent->key[positionSon - 1] = parent->c[positionSon - 1]->key[parent->c[positionSon - 1]->num_key - 1];
					parent->value[positionSon - 1] = parent->c[positionSon - 1]->value[parent->c[positionSon - 1]->num_key - 1];
					remove_element(parent->c[positionSon - 1], parent->c[1]->key[parent->c[positionSon - 1]->num_key - 1]);
				}
				else {
					if (parent->num_key > t - 1) {
						rconnect(parent, element, parent->c[1], positionSon);
					}
					else {
						merge_parent(parent);
					}
					remove_element(element, k);
				}
			}
			else {
				if ((parent->c[positionSon - 1]->num_key > t - 1) || (parent->c[positionSon + 1]->num_key > t - 1)) {
					if (parent->c[positionSon - 1]->num_key > t - 1) {
						for (int i = position; i > 0; --i) {
							element->key[i] = element->key[i - 1];
							element->value[i] = element->value[i - 1];
						}
						element->key[0] = parent->key[positionSon - 1];
						element->value[0] = parent->value[positionSon - 1];
						parent->key[positionSon - 1] = parent->c[positionSon - 1]->key[parent->c[positionSon - 1]->num_key - 1];
						parent->value[positionSon - 1] = parent->c[positionSon - 1]->value[parent->c[positionSon - 1]->num_key - 1];
						remove_element(parent->c[positionSon - 1], parent->c[1]->key[parent->c[positionSon - 1]->num_key - 1]);
					}
					else {
						for (int i = position; i < element->num_key - 1; ++i) {
							element->key[i] = element->key[i + 1];
							element->value[i] = element->value[i + 1];
						}
						element->key[element->num_key - 1] = parent->key[positionSon];
						element->value[element->num_key - 1] = parent->value[positionSon];
						parent->key[positionSon] = parent->c[positionSon + 1]->key[0];
						parent->value[positionSon] = parent->c[positionSon + 1]->value[0];
						remove_element(parent->c[positionSon + 1], parent->c[positionSon + 1]->key[0]);
					}
				}
				else {
					if (parent->num_key > t - 1) {
						rconnect(parent, element, parent->c[1], positionSon);
					}
					else {
						merge_parent(parent);
					}
					remove_element(element, k);
				}
			}
		}
		void remove_BTree(Node<T>*parent, Node<T>*element, int k) {
			int position;
			for (int i = 0; i < element->num_key; ++i) {
				if (element->key[i] == k) {
					position = i;
					break;
				}
			}
			Node<T>* new_parent = element;
			Node<T>* swapper = element->c[position];
			while (!swapper->leaf) {
				new_parent = swapper;
				swapper = swapper->c[swapper->num_key];
			}
			swap(element->key[position], swapper->key[swapper->num_key - 1]);
			swap(element->value[position], swapper->value[swapper->num_key - 1]);
			if (swapper->num_key > t - 1) {
				remove_element(swapper, k);
			}
			else {
				remove_from_leaf(new_parent, swapper, k);
			}

		}
		bool b_tree_search(Node<T>*element, int k) {
			int index = 0;
			while ((index < element->num_key) && (k > element->key[index])) {
				index++;
			}
			if (index <= element->num_key && k == element->key[index]) {
				//cout << element->key[index] << endl;
				return 1;
			}
			if (element->leaf) {
				cout << "no man" << endl;
				return 0;
			}
			else {
				//cout << element->key[index] <<  ' ';
				b_tree_search(element->c[index], k);
			}
		}

		void split_child(Node<T>*x, Node<T>*y, int index) {
			Node<T>* z = new Node<T>();
			y->num_key = t - 1;
			z->num_key = t - 1;
			for (int i = 0; i < t - 1; ++i) {
				z->key[i] = y->key[i + t];
				z->value[i] = y->value[i + t];
			}
			if (!y->leaf) {
				for (int i = 0; i < t; ++i) {
					z->c[i] = y->c[i + t];
				}
				z->leaf = false;
			}
			for (int i = x->num_key; i > index; --i) {
				x->c[i] = x->c[i - 1];
			}

			x->c[index + 1] = z;

			for (int i = x->num_key; i > t - 1; --i) {
				x->key[i] = x->key[i - 1];
			}
			x->key[index] = y->key[t - 1];
			x->value[index] = y->value[t - 1];
			x->num_key++;
		}
		void insert_nonfull(Node<T>*x, T v, int k) {
			int index = x->num_key - 1;
			if (x->leaf) {
				while ((index >= 0) && (k < x->key[index])) {
					x->key[index + 1] = x->key[index];
					index--;
				}
				x->key[index + 1] = k;
				x->value[index + 1] = v;
				x->num_key++;
			}
			else {
				while ((index >= 0) && ((k < x->key[index]) || (x->c[index] == NULL))) {
					index--;
				}
				index++;
				Node<T>* r = x->c[index];
				if (r->num_key == 2 * t - 1) {
					split_child(x, x->c[index], index);
					if (k > x->key[index]) {
						index++;
					}
				}
				insert_nonfull(x->c[index], v, k);
			}
		}
		void create(T v, int k) {
			Node<T>* s = new Node<T>();
			s->key[0] = k;
			s->value[0] = v;
			root = s;
		}
		void printSpaces(int n) {
			for (int i = 0; i < n; i++) {
				cout << " ";
			}
		}
		void printBTree(Node<T>*element, int depth) {
			if (element == NULL) {
				return;
			}

			printBTree(element->c[element->num_key], depth + 1);

			printSpaces(depth * 4);
			for (int i = 0; i < element->num_key; ++i) {
				cout << "[" << element->key[i] << "]";
			}
			cout << endl;

			for (int i = element->num_key - 1; i >= 0; --i) {
				printBTree(element->c[i], depth + 1);
			}
		}
};