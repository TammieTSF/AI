#include "Customer.h"


Customer::Customer()
{
}


Customer::~Customer()
{
}

void Customer::setToMale()
{
	this->gender = g_MALE;
}

void Customer::setToFemale()
{
	this->gender = g_FEMALE;
}

bool Customer::isMale()
{
	if (gender == g_MALE)
		return true;
	return false;
}

bool Customer::isFemale()
{
	if (gender == g_FEMALE)
		return true;
	return false;
}

void Customer::SetEnterState()
{
	this->state = s_ENTER;
}

void Customer::SetRequestState()
{
	this->state = s_REQUEST;
}

void Customer::SetLeaveState()
{
	this->state = s_LEAVE;
}

bool Customer::isEnter()
{
	if (state == s_ENTER)
		return true;
	return false;
}
bool Customer::isRequest()
{
	if (state == s_REQUEST)
		return true;
	return false;
}
bool Customer::isLeave()
{
	if (state == s_LEAVE)
		return true;
	return false;
}

int Customer::getState()
{
	return state;
}