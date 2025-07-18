#ifndef INTERFACE_I_DISPOSABLE_H
#define INTERFACE_I_DISPOSABLE_H

#ifdef DEBUG
#include <iostream>
#include <map>
#include <string>

extern std::map<long long, std::string> names;
extern int _____k;
#define THIS_NAME names[reinterpret_cast<long long>(this)]
#define NEW_NAME std::to_string(_____k++)
#endif //DEBUG

void DisposeAll();

class IDisposable{
	public:
		IDisposable();
		IDisposable(bool add_to_disposable_list);
		void Dispose();
		void Dispose(bool remove_from_disposable_list);

	protected:
		virtual void OnDispose() {}

};

#endif //INTERFACE_I_DISPOSABLE_H