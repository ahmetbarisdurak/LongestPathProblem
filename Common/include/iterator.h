// StaticVectorProject.h : Include file for standard system include files,
// or project specific include files.

#pragma once

template <typename T>
class Iterator {
public:
	virtual bool HasNext() = 0;
	virtual T Next() = 0;
	virtual bool HasPrev() = 0;
	virtual T Prev() = 0;
};
// TODO: Reference additional headers your program requires here.
