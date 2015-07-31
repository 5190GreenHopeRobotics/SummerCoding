/*
 * vector.h
 *
 *  Created on: Jul 29, 2015
 *      Author: sdai
 */

#ifndef VECTOR_H_
#define VECTOR_H_
#ifndef nullptr
#define nullptr 0
#endif
namespace frc5190 {
	template<typename T>
	class vector {
	public:
		void append(const T& toAdd);
		void prepend(const T& toAdd);
		void insert(const T& in, const int index);
		void remove(const int index);
		T removeEnd();
		T removeFront();
		T& at(const int index);
		T& operator[](const int index);
		T& front();
		T& back();
		T* begin();
		T* end();
		void clear();
		int getSize() const;
		bool isEmpty() const;
		vector();
		vector(const vector<T>& src);
		vector<T>& operator=(const vector<T>& src);
		bool operator==(const vector<T>& src)const;
		~vector();
	protected:
		void reallocate();
		void cpVector(vector<T>& v);
		bool cmpData(const T* data) const;
		bool needReloc()const;
		T* data;
		int size;
		const int block;
		int capacity;
	};
}


using namespace frc5190;
template<typename T>
vector<T>::vector()
:block(5)
{
	data = new T[block];
	capacity=block;
	size = 0;
	for(int i=0;i<capacity;++i) {
		data[i] = nullptr;
	}
}

template<typename T>
vector<T>::vector(const vector<T>& src):
block(5)
{
	data = new T[src.capacity];
	capacity = src.capacity;
	size = src.size;
	cpVector(src);
}

template<typename T>
vector<T>::~vector() {
	clear();
}

template<typename T>
vector<T>& vector<T>::operator =(const vector<T>& src) {
	clear();
	capacity = src.capacity;
	size = src.size;
	data = new T[src.capacity];
	cpVector(src);
	return *this;
}

template<typename T>
bool vector<T>::operator ==(const vector<T>& src)const {
	if(src.size != size) {
		return false;
	}
	return cmpData(src.data);
}

template<typename T>
bool vector<T>::cmpData(const T* data) const {
	for(int i=0;i<size;++i) {
		if(this->data[i] != data[i]) {
			return false;
		}
	}
	return true;
}

template<typename T>
void vector<T>::cpVector(vector<T>& v) {
	for(int i=0;i<v.size;++i) {
		data[i] = v.data[i];
	}
	return;
}

template<typename T>
void vector<T>::reallocate() {
	T* temp = new T[capacity + block];
	for(int i=0;i<capacity+ temp;++i) {
		temp[i] = nullptr;
	}
	for(int i=0;i< size;++i) {
		temp[i] = data[i];
	}
	capacity+=block;
	delete[] data;
	data = temp;
	return;
}

template <typename T>
bool vector<T>::needReloc() const {
	return size == capacity;
}

template<typename T>
void vector<T>::append(const T& toAdd) {
	data[size] = toAdd;
	size+=1;
	if(needReloc()) {
		reallocate();
	}
	return;
}

template <typename T>
void vector<T>::prepend(const T& toAdd) {
	for(int i=0;i<size;++i) {
		data[i+1]=data[i];
	}
	data[0] = toAdd;
	size+=1;
	if(needReloc()) {
		reallocate();
	}
	return;
}

template <typename T>
void vector<T>::insert(const T& in,const int index) {
	if(index >= capacity) {
		return;
	}
	if(index >= size) {
		data[index] = in;
	} else {
		for(int i=index;i<size;++i) {
			data[i+1] = data[i];
		}
		data[index] = in;
	}
	size += 1;
	if(needReloc()) {
		reallocate();
	}
	return;
}

template <typename T>
void vector<T>::remove(const int index) {
	if(index == size-1) {
		removeEnd();
	}
	for(int i=index;i<size-1;++i) {
		data[i] = data[i+1];
	}
	size -=1;
	return;
}

template <typename T>
T vector<T>::removeEnd() {
	if(isEmpty()) {
		return T();
	} else {
		T deleted = data[size-1];
		data[size-1] = nullptr;
		size -=1;
		return deleted;
	}
}

template <typename T>
T vector<T>::removeFront() {
	if(isEmpty()) {
		return T();
	} else {
		T deleted = data[0];
		remove(0);
	}
}

template <typename T>
T& vector<T>::at(const int index) {
	return data[index];
}

template <typename T>
T& vector<T>::operator [](const int index) {
	return at(index);
}

template <typename T>
T& vector<T>::front() {
	return data[0];
}

template <typename T>
T& vector<T>::back() {
	return data[size-1];
}

template <typename T>
T* vector<T>::begin() {
	return &data[0];
}

template <typename T>
T* vector<T>::end() {
	return &data[size];
}

template <typename T>
void vector<T>::clear() {
	size = 0;
	capacity = block;
	delete[] data;
	data = new T[block];
}

template <typename T>
int vector<T>::getSize() const {
	return size;
}

template <typename T>
bool vector<T>::isEmpty()const {
	return size == 0;
}




#endif /* VECTOR_H_ */
