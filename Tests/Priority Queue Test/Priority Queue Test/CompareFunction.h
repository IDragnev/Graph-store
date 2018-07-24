#ifndef _COMP_FUN_H_INCLUDED__
#define _COMP_FUN_H_INCLUDED__

class CompareFunction
{
public:
	bool operator()(int lhs, int rhs) { return lhs < rhs; }
};

#endif //_COMP_FUN_H_INCLUDED__