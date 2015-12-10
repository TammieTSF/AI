#include "Cashier.h"


Cashier::Cashier()
{
}


Cashier::~Cashier()
{
}

void Cashier::SetGreetState()
{
	this->state = s_GREET;
}

void Cashier::SetBuyState()
{
	this->state = s_BUY;
}

void Cashier::SetGreetFemale()
{
	this->greet = g_FEMALE;
}

void Cashier::SetGreetMale()
{
	this->greet = g_MALE;
}

bool Cashier::isGreet()
{
	if (state == s_GREET)
		return true;
	return false; 
}

bool Cashier::isBuy()
{
	if (state == s_BUY)
		return true;
	return false;
}

bool Cashier::isFemale()
{
	if (greet == g_FEMALE)
		return true;
	return false;
}

bool Cashier::isMale()
{
	if (greet == g_MALE)
		return true;
	return false;
}