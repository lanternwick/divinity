#ifndef _IKEYBOARD_H
#define _IKEYBOARD_H

class IKeyboard
{
public:
	IKeyboard();
	~IKeyboard();

	BOOL Left();
	BOOL Right();
	BOOL Up();
	BOOL Down();
	BOOL PgUp();
	BOOL PgDn();
	BOOL Del();
	BOOL End();
	BOOL Shift();
};

IKeyboard::IKeyboard()
{

}

IKeyboard::~IKeyboard()
{

}

BOOL IKeyboard::Left()
{
	return (HIWORD(GetKeyState(VK_LEFT)) == 0xffff ? TRUE : FALSE);
}

BOOL IKeyboard::Right()
{
	return (HIWORD(GetKeyState(VK_RIGHT)) == 0xffff ? TRUE : FALSE);
}

BOOL IKeyboard::Up()
{
	return (HIWORD(GetKeyState(VK_UP)) == 0xffff ? TRUE : FALSE);
}

BOOL IKeyboard::Down()
{
	return (HIWORD(GetKeyState(VK_DOWN)) == 0xffff ? TRUE : FALSE);
}

BOOL IKeyboard::PgUp()
{
	return (HIWORD(GetKeyState(VK_PRIOR)) == 0xffff ? TRUE : FALSE);
}

BOOL IKeyboard::PgDn()
{
	return (HIWORD(GetKeyState(VK_NEXT)) == 0xffff ? TRUE : FALSE);
}

BOOL IKeyboard::Del()
{
	return (HIWORD(GetKeyState(VK_DELETE)) == 0xffff ? TRUE : FALSE);
}

BOOL IKeyboard::End()
{
	return (HIWORD(GetKeyState(VK_END)) == 0xffff ? TRUE : FALSE);
}

BOOL IKeyboard::Shift()
{
	return (HIWORD(GetKeyState(VK_SHIFT)) == 0xffff ? TRUE : FALSE);
}
#endif