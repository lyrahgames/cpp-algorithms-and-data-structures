#include <io/cmd/cmd.h>

namespace cmd_ui{
	using cmd = cmd::typespace<ui_test>;

	void print(cmd::arg_t* parg){
		ui_test *p = (ui_test*)parg->src;
		parg->out << "count = " << p->count << "\n";
	}

	void inc(cmd::arg_t* parg){
		ui_test *p = (ui_test*)parg->src;
		p->count++;
	}

	void exit(cmd::arg_t* parg){
		ui_test *p = (ui_test*)parg->src;
		p->running = false;
	}

	void help(cmd::arg_t* parg){
		for (int i = 0; i < parg->db->size; i++){
			parg->out << parg->db->data[i].cmd << "\t";
		}
		parg->out << "\n";
	}

	const cmd::entry_t data[] = {
		"print", print,
		"inc", inc,
		"exit", exit,
		"help", help
	};

	const cmd::database_t database = {data, 4};

	void loop(cmd::arg_t* parg){
		ui_test* p = (ui_test*)parg->src;
		while (p->running){
			if (!cmd::process(parg)){
				parg->out << "command \'" << parg->cmd_buffer << "\' not known\n";
			}
		}
	}
}

int main(int argc, char const *argv[]){
	ui_test uit = {true, 0};
	cmd_ui::cmd::arg_t _arg(&uit, &cmd_ui::database);
	cmd_ui::loop(&_arg);
	return 0;
}