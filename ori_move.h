#pragma once
#include <array>
//maybe change it to take uint and bitshift so not so many arguments
template<char length, char ori>
class ori_move
{
public:
	char len = length;

	ori_move() : _pos1(0), _pos2(3), _pos3(2), _pos4(1), _ori1(0), _ori2(0), _ori3(0), _ori4(0) {} //defaults to U corner
	ori_move(char pos1, char pos2, char pos3, char pos4, char ori1, char ori2, char ori3, char ori4) :
		_pos1(pos1), _pos2(pos2), _pos3(pos3), _pos4(pos4), _ori1(ori1), _ori2(ori2), _ori3(ori3), _ori4(ori4) {}
	void operator()(std::array<char, length>& ori_array) {
		char x;
		x = ori_array[_pos1];
		ori_array[_pos1] = (ori_array[_pos2] + _ori2) % ori;
		ori_array[_pos2] = (ori_array[_pos3] + _ori3) % ori;
		ori_array[_pos3] = (ori_array[_pos4] + _ori4) % ori;
		ori_array[_pos4] = (x + _ori1) % ori;
	}

private:
	char _pos1;
	char _pos2;
	char _pos3;
	char _pos4;
	char _ori1;
	char _ori2;
	char _ori3;
	char _ori4;
};