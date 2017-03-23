#pragma once
// #ifndef __CCMD_H__
// #define __CCMD_H__

#include <stdio.h>
#include <string.h>


namespace ccmd{

typedef unsigned int uint;

struct arg_t;

typedef void (*func_t)(arg_t*);

struct entry_t{
	const char* cmd;
	const func_t func;
};

struct database_t{
	const entry_t* data;
	uint size;
};

inline const entry_t* find(const database_t* db, const char* cmd){
	for (uint i = 0; i < db->size; i++){
		if (strcmp(db->data[i].cmd, cmd) == 0){
			return &(db->data[i]);
		}
	}

	return nullptr;
}

struct arg_t{
	static const uint cmd_buffer_size = 64;
	static const uint arg_buffer_size = 1024;

	arg_t(void* source, const database_t* database, FILE* input = stdin, FILE* output = stdout, FILE* error = stderr): src(source), db(database), in(input), out(output), err(error){}

	void* src;
	const database_t* db;

	FILE* in;
	FILE* out;
	FILE* err;

	char cmd_buffer[cmd_buffer_size];
	char arg_buffer[arg_buffer_size];
};

inline int read_cmd(FILE* in, char* cmd_buffer, uint n){
	char *p = cmd_buffer;
	// get next graphical char or EOF
	while (!(isgraph(*p = fgetc(in)) || *p == EOF));
	// copy until non-graphical char
	while (isgraph(*(++p) = fgetc(in)));
	// put last byte back to stream
	ungetc(*p, in);
	// set last non-graphical char to zero
	*p= '\0';
	return (*(p-1) != EOF);
}

inline int read_arg(FILE* in, char* arg_buffer, uint n){
	fgets(arg_buffer, n, in);
}

inline const entry_t* process_line(arg_t* p){
	read_cmd(p->in, p->cmd_buffer, arg_t::cmd_buffer_size);
	read_arg(p->in, p->arg_buffer, arg_t::arg_buffer_size);

	const entry_t* entry = find(p->db, p->cmd_buffer);

	if (entry != nullptr)
		entry->func(p);

	return entry;
}

inline const entry_t* process(arg_t* p){
	read_cmd(p->in, p->cmd_buffer, arg_t::cmd_buffer_size);
	const entry_t* entry = find(p->db, p->cmd_buffer);

	if (entry != nullptr)
		entry->func(p);

	return entry;
}


} // ccmd


// #endif // __CCMD_H__