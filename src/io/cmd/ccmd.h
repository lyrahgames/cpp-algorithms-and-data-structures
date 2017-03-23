#pragma once
// #ifndef __CCMD_H__
// #define __CCMD_H__

#include <stdio.h>
#include <string.h>


namespace ccmd{


struct arg_t;

typedef void (*func_t)(arg_t*);

struct entry_t{
	const char* cmd;
	const func_t func;
};

struct database_t{
	entry_t* data;
	unsigned int size;
};

inline const entry_t* find(const database_t* db, const char* cmd){
	for (unsigned int i = 0; i < db->size; i++){
		if (strcmp(db->data[i].cmd, cmd) == 0){
			return &(db->data[i]);
		}
	}

	return nullptr;
}

struct arg_t{
	static const unsigned int cmd_buffer_size = 64;
	static const unsigned int arg_buffer_size = 1024;

	arg_t(void* source, database_t* database, FILE* input = stdin, FILE* output = stdout, FILE* error = stderr): src(source), db(database), in(input), out(output), err(error){}

	void* src;
	database_t* db;

	FILE* in;
	FILE* out;
	FILE* err;

	char cmd_buffer[cmd_buffer_size];
	char arg_buffer[arg_buffer_size];
};

inline const entry_t* process_line(arg_t* p){


	const entry_t<T>* entry = find(p->db, p->cmd_buffer);

	if (entry != nullptr)
		entry->func(p);

	return entry;
}


} // ccmd


// #endif // __CCMD_H__