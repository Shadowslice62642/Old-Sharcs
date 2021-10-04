#pragma once
#include <array>

template<char length>
class perm_move
{
public:
	char len = length;

	perm_move()
		: _pos1(0), _pos2(3), _pos3(2), _pos4(1) {} //defaults to U;
	perm_move(char pos1, char pos2, char pos3, char pos4)
		: _pos1(pos1), _pos2(pos2), _pos3(pos3), _pos4(pos4) {}
	void operator() (std::array<char, length>& perm_array) {
		char x;
		x = perm_array[_pos1];
		perm_array[_pos1] = perm_array[_pos2];
		perm_array[_pos2] = perm_array[_pos3];
		perm_array[_pos3] = perm_array[_pos4];
		perm_array[_pos4] = x;
	}

private:
	char _pos1;
	char _pos2;
	char _pos3;
	char _pos4;
};