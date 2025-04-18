#pragma once



class IBindable {
public:
	virtual void Bind() = 0;
	virtual void Bind(int bind_index) = 0;
};