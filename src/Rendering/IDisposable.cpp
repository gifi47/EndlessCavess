#include "IDisposable.h"

#include <list>
#include <algorithm>

#ifdef DEBUG
#include <iostream>
#endif

static std::list<IDisposable*>* disposableList = new std::list<IDisposable*>(0);

void DisposeAll(){
	for (IDisposable* disposable : (*disposableList)){
		disposable->Dispose(false);
	}
	disposableList->clear();
}

// IDisposable
IDisposable::IDisposable(){
#ifdef DEBUG
	std::cout << "obj: " << this << " added to disposableList\n";
#endif
	disposableList->push_back(this);
}

IDisposable::IDisposable(bool add_to_disposable_list){
#ifdef DEBUG
	std::cout << "obj: " << this << " added to disposableList = " << add_to_disposable_list <<"\n";
#endif
	if (add_to_disposable_list) disposableList->push_back(this);
}

void IDisposable::Dispose(){
#ifdef DEBUG
	std::cout << "obj: " << this << " Dispose()\n";
#endif
	auto it = std::find(disposableList->begin(), disposableList->end(), this);
	if (it != disposableList->end()) {
		disposableList->erase(it);
	}
	OnDispose();
}

void IDisposable::Dispose(bool remove_from_disposable_list){
#ifdef DEBUG
	std::cout << "obj: " << this << " Dispose(" << remove_from_disposable_list << ")\n";
#endif
	if (remove_from_disposable_list){
		auto it = std::find(disposableList->begin(), disposableList->end(), this);
		if (it != disposableList->end())
			disposableList->erase(it);

	}

	OnDispose();
}