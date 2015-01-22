//

#ifndef H_CONTAINERS
#define H_CONTAINERS

#include "string.h"

// Hash Table
template <typename Type> class HashSet {
private:
	struct ItemT { char *id; Type Obj; };
	LinearBuffer *MemoryBuffer;
	vector<int> ItemsArr;
	//
public:
	//
	vector<string> Hashes;
	ItemT	***Items;
	int		  *Last;
	int		   M, Size;
	//
	
	inline int Hash(char *str) {
		//
		int h = 0;
		//
		for (int i = 0; i < strlen(str); i++)
			h = (h*128+str[i]) % M;
		//
		return h % M;
	}
	//
	inline void Insert (char *str, Type Obj) {
		//
		int idx = Hash(str);
		Hashes.push_back(str);
		//
		if (Last[idx] == 0) {
			Items[idx] = (ItemT**) malloc (sizeof(ItemT*));
		} else {
			Items[idx] = (ItemT**) realloc (Items[idx], sizeof(ItemT*)*(Last[idx]+1));
		}
		//
		Items[idx][Last[idx]] = (ItemT*) malloc (sizeof(ItemT));
		Items[idx][Last[idx]]->id  = str;
		Items[idx][Last[idx]]->Obj = Obj;
		//
		ItemsArr.push_back(idx);
		Last[idx]++; Size++;
		//
	}
	//
	inline Type GetData(char *str) {
		//
		int idx = Hash(str);
		//
		for (int i = Last[idx]-1; i >= 0; i--)
			if (!strcmp(str, Items[idx][i]->id))
				return Items[idx][i]->Obj;
		//
		LOG_ERROR("Can't find %s element in HashSet\n", str);
		Terminate(EXIT_FAILURE);
	}
	//
	inline Type operator[](char *idx) {
		return GetData(idx);
	}
	//
	HashSet (const int MemPageSize) {
		//
		M = MemPageSize-1; Size = 0;
		//
		Items	= (ItemT***) malloc (sizeof(ItemT*)*MemPageSize);
		Last	=     (int*) malloc (sizeof(int)   *MemPageSize);
		//
		memset (Last, 0, sizeof(int)*MemPageSize);
	}
	
	//
	~HashSet() {
		//
		for (int i = 0; i < ItemsArr.size(); i++) {
			for (int j = 0; j < Last[ItemsArr[i]]; j++) {
				//
				if (Items[ItemsArr[i]] != 0 && Items[ItemsArr[i]][j] != 0) {
					free (Items[ItemsArr[i]][j]);
					Items[ItemsArr[i]][j] = 0;
				}
			}

			//
			if (Items[ItemsArr[i]] != 0) {
				free (Items[ItemsArr[i]]);
				Items[ItemsArr[i]] = 0;
			}
		}
		//
		free (Items);
		free (Last);
	}
	//
};

template <class T>
class nvector : public vector<T> {
public:
	T nextToBack() {
		typename vector<T>::iterator s = vector<T>::end()-1;

		if (s == vector<T>::begin()) return *s;
		else return *(s-1);
	}
};

class Node {
protected:
	Node *_next;
	Node *_prev;

public:
	Node() : _next(this), _prev(this) {}
	virtual ~Node() {}

	inline Node *next() { return _next; }
	inline Node *prev() { return _prev; }

	inline Node *insert (Node* b) {
		Node *c = _next;
		b->_next = c;
		b->_prev = this;
		_next = b;
		c->_prev = b;

		return b;
	}

	inline Node *remove() {
		_prev->_next = _next;
		_next->_prev = _prev;
		_next = _prev = this;

		return this;
	}

	inline void Splice (Node* b) {
		Node *a = this;
		Node *an = a->_next;
		Node *bn = b->_next;

		a ->_next = bn; b ->_next = an;
		an->_prev = b;  bn->_prev = a;
	}
};

template <class T> class List;
template <class T> class ListNode : public Node {
public:
	T _val;
	ListNode(T val) : _val(val) {}
	friend class List<T>;
};

template <class T> class List {
private:
	ListNode<T> *header;
	ListNode<T> *win;
	int _length;

public:
	List() : _length(0) {
		header = new ListNode<T>(0);
		win = header;
	}

	~List() {
		while (length() > 0) {
			first();
			remove();
		}

		delete header;
	}

	inline T insert(T val) {
		win->insert(new ListNode<T>(val));
		++_length;
		return val;
	}

	inline T append(T val) {
		header->prev()->insert(new ListNode<T>(val));
		++_length;
		return val;
	}

	inline List *append(List *l) {
		ListNode<T> *a = (ListNode<T>*) header->prev();
		a->splice(l->header);
		_length += l->_length();
		l->header->remove();
		l->_length = 0;
		l->win = header;

		return this;
	}

	inline T prepend(T val) {
		header->insert (new ListNode<T>(val));
		++_length;
		return val;
	}

	inline T remove() {
		if (win == header) return 0;
		void *val = win->_val;
		win = (ListNode<T>*) win->prev();
		delete (ListNode<T>*) win->next()->remove();
		--_length;

		return (T)val;
	}

	inline void val(T v) {
		if (win != header)
			win->_val = v;
	}

	inline T val() {
		return win->_val;
	}

	inline T next() {
		win = (ListNode<T>*) win->next();
		return win->_val;
	}

	inline T prev() {
		win = (ListNode<T>*) win->prev();
		return win->_val;
	}

	inline T first() {
		win = (ListNode<T>*) header->next();
		return win->_val;
	}

	inline T last() {
		win = (ListNode<T>*) header->prev();
		return win->_val;
	}

	inline int length() {
		return _length;
	}

	inline bool isFirst() {
		return win == header->next() && _length > 0;
	}

	inline bool isLast() {
		return win == header->prev() && _length > 0;
	}

	inline bool isHead() {
		return win == header;
	}
};

template <class T> class Stack {
private:
	List<T> *s;

public:	
	Stack() : s (new List<T>) {}
	~Stack() { delete s; }
	//
	inline void push(T v) {
		s->append(v);
	}

	inline T pop() {
		s->first();
		return s->remove();
	}

	inline bool empty() {
		return s->length() == 0;
	}

	inline int size() {
		return s->length();
	}

	inline T top() {
		return s->first();
	}

	inline T nextToTop() {
		s->first();
		return s->next();
	}

	inline T bottom() {
		return s->last();
	}
};

//#include "Containers.cpp"
#endif
