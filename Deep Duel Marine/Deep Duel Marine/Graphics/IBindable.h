#pragma once



class IBindable {
public:
	virtual ~IBindable(){}

	virtual void Bind() noexcept = 0;
	virtual void Bind(int bind_index) noexcept = 0;
};