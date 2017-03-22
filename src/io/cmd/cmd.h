#pragma once
// #ifndef __CMD_H__
// #define __CMD_H__

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


namespace cmd{


template <class T>
struct arg_t;

template <class T>
using func_t = void (*)(arg_t<T>*);

template <class T>
struct entry_t{
	const char* cmd;
	func_t<T> func;
};

template <class T>
struct database_t{
	entry_t<T>* data;
	unsigned int size;
};

template <class T>
inline const entry_t<T>* find(const database_t<T>* db, const char* cmd){
	for (unsigned int i = 0; i < db->size; i++){
		if (strcmp(db->data[i].cmd, cmd) == 0)
			return &(db->data[i]);
	}

	return nullptr;
}

template <class T>
struct arg_t{
	static const unsigned int cmd_buffer_size = 80;
	static const unsigned int arg_buffer_size = 1024;


	arg_t(T* source, database_t<T>* database, std::istream& ins = std::cin, std::ostream& outs = std::cout, std::ostream& errs = std::cerr): src(source), db(database), in(ins), out(outs), err(errs){}

	T* src;
	std::istream& in;
	std::ostream& out;
	std::ostream& err;
	database_t<T>* db;

	char cmd_buffer[cmd_buffer_size];
	char arg_buffer[arg_buffer_size];
};

template <class T>
inline const entry_t<T>* process(arg_t<T>* parg){
	parg->in.width(arg_t<T>::cmd_buffer_size);
	parg->in >> parg->cmd_buffer;
	parg->in.getline(parg->arg_buffer, arg_t<T>::arg_buffer_size);

	const entry_t<T>* entry = find(parg->db, parg->cmd_buffer);

	if (entry != nullptr)
		entry->func(parg);

	return entry;
}

// template <class T>
// struct t{
// 	using arg_t = arg_t<T>;
// 	using func_t = func_t<T>;
// 	using entry_t = entry_t<T>;
// 	using database_t = database_t<T>;
// };


} // cmd


// #endif // __CMD_H__