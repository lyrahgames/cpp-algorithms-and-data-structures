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
struct arg;

template <class T>
using func = void (*)(arg<T>*);

template <class T>
struct entry{
	const char* cmd;
	const func<T> f;
};

template <class T>
struct database{
	const entry<T>* data;
	unsigned int size;
};

template <class T>
inline const entry<T>* find(const database<T>* db, const char* cmd){
	for (unsigned int i = 0; i < db->size; i++){
		if (strcmp(db->data[i].cmd, cmd) == 0)
			return &(db->data[i]);
	}

	return nullptr;
}

template <class T>
struct arg{
	static const unsigned int cmd_buffer_size = 80;
	static const unsigned int arg_buffer_size = 1024;


	arg(T* source, const database<T>* database, std::istream& ins = std::cin, std::ostream& outs = std::cout, std::ostream& errs = std::cerr): src(source), db(database), in(ins), out(outs), err(errs){}

	T* src;
	std::istream& in;
	std::ostream& out;
	std::ostream& err;
	const database<T>* db;

	char cmd_buffer[cmd_buffer_size];
	char arg_buffer[arg_buffer_size];
};

template <class T>
inline const entry<T>* process(arg<T>* parg){
	parg->in.width(arg<T>::cmd_buffer_size);
	parg->in >> parg->cmd_buffer;

	const entry<T>* entry = find(parg->db, parg->cmd_buffer);

	if (entry != nullptr)
		entry->f(parg);

	return entry;
}

template <class T>
inline const entry<T>* process_line(arg<T>* parg){
	parg->in.width(arg<T>::cmd_buffer_size);
	parg->in >> parg->cmd_buffer;
	parg->in.getline(parg->arg_buffer, arg<T>::arg_buffer_size);

	const entry<T>* entry = find(parg->db, parg->cmd_buffer);

	if (entry != nullptr)
		entry->f(parg);

	return entry;
}

template <class T>
struct typespace{
	using arg_t = arg<T>;
	using func_t = func<T>;
	using entry_t = entry<T>;
	using database_t = database<T>;

	static const entry_t* process(arg_t* parg){return cmd::process(parg);}
	static const entry_t* process_line(arg_t* parg){return cmd::process_line(parg);}
};


} // cmd


// #endif // __CMD_H__