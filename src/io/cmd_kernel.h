#pragma once
// #ifndef __CMD_KERNEL_H__
// #define __CMD_KERNEL_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


// namespace {


template <class T>
struct cmd_kernel{

	using func_t = void (*)(cmd_kernel*);
	using str_t = std::string;

	struct cmd_t{
		str_t call;
		func_t func;
	};



	cmd_kernel(T* source = nullptr, std::istream& ins = std::cin, std::ostream& outs = std::cout, std::ostream& errs = std::cerr): src(source), in(ins), out(outs), err(errs){}


	void process(const cmd_t* cmd_list, int size){
		str_t cmd_buffer;
		in >> cmd_buffer;

		for (int i = 0; i < size; i++){
			if (cmd_buffer == cmd_list[i].call){
				cmd_list[i].func(this);
				return;
			}
		}

		err << "command \'" << cmd_buffer << "\' not known\n";
	}


	T* src;
	std::istream& in;
	std::ostream& out;
	std::ostream& err;
};


// }


// #endif // __CMD_KERNEL_H__