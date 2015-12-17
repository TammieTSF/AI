#include "Supplier.h"


Supplier::Supplier()
{
}


Supplier::~Supplier()
{
}


void Supplier::SetEnterState()
{
	this->state = s_ENTER;
}

void Supplier::SetSellGoodState()
{
	this->state = s_SELLGOOD;
}

void Supplier::SetLeaveState()
{
	this->state = s_LEAVE;
}

bool Supplier::isEnter()
{
	if (state == s_ENTER)
		return true;
	return false;
}
bool Supplier::isSelling()
{
	if (state == s_SELLGOOD)
		return true;
	return false;
}
bool Supplier::isLeave()
{
	if (state == s_LEAVE)
		return true;
	return false;
}


int Supplier::getState()
{
	return state;
}
std::string Supplier::getStateName(STATE state)
{
	if (state == s_ENTER)
	{
		return "Enter State";
	}
	else if (state == s_SELLGOOD)
	{
		return "Sell state";
	}
	else if (state == s_LEAVE)
	{
		return "Leave state";
	}

}
