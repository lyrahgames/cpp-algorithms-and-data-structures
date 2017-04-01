#include <iostream>
#include <util.h>


#include <unordered_map>
#include <glut_app/glut_app.h>


int main(int argc, char** argv){
	using namespace std;

	// using umap = unordered_map<char,int>;

	// umap m;

	// m.emplace('a', 1);
	// m.emplace('g', 20);
	// m.emplace('b', 2);
	// m.emplace('c', 3);
	// // m.emplace('b', 5);
	// // m.emplace('b', 10);
	// m.emplace('e', 23);
	// m.emplace('f', 22);
	// m.emplace('h', 12);
	// m.emplace('i', 21);
	// m.emplace('j', 28);


	// for (auto it = m.begin(); it != m.end(); it++){
	// 	cout << it->first << "\t" << it->second << "\n";
	// }
	// cout << "\n";


	// cout << m['a'] << m['b'] << m['c'] << endl;

	// wavefront_obj::obj_t obj;
	// obj.v_data.push_back(wavefront_obj::vertex(1,2,3));
	// wavefront_obj::load_file(&obj, "../tmp/test.txt");
	// cout << obj << endl;
	// cout << obj.v_data.size() << endl;

	vecf2 v(1,2);
	float f = 1.5f;

	// cout << v << endl << 2.0f*v << endl << f*v << endl << v*f << endl << v*(1-f) << endl;

	glut_app::init(argc, argv);
	glut_app::exec();

	return 0;
}