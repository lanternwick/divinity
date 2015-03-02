#ifndef _FIXED32_H
#define _FIXED32_H

class fixed32
{
public:
	WORD i;
	WORD d;
};

fixed32 fixed32::operator + (fixed32 lhs, fixed32 rhs)
{

