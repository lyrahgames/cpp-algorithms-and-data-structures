#include <io/cmd/ccmd.h>

struct ui_test{
	bool running;
	int count;
};

namespace ccmd_ui{
	void print(ccmd::arg_t* parg){
		ui_test *p = (ui_test*)parg->src;
		fprintf(parg->out, "count = %i\n", p->count);
	}

	void inc(ccmd::arg_t* parg){
		ui_test *p = (ui_test*)parg->src;
		p->count++;
	}

	void exit(ccmd::arg_t* parg){
		ui_test *p = (ui_test*)parg->src;
		p->running = false;
	}

	const ccmd::entry_t data[] = {
		"print", print,
		"inc", inc,
		"exit", exit
	};

	const ccmd::database_t database = {data, 3};

	void loop(ccmd::arg_t* parg){
		ui_test* p = (ui_test*)parg->src;
		while (p->running){
			if (!ccmd::process_line(parg)){
				fprintf(parg->out, "command \'%s\' not known\n", parg->cmd_buffer);
			}
		}
	}
}

int main(int argc, char const *argv[]){
	ui_test uit = {true, 0};
	ccmd::arg_t arg(&uit, &ccmd_ui::database);
	ccmd_ui::loop(&arg);
	return 0;
}