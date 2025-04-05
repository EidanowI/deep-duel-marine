#pragma once



class IGUI {
public:
	virtual ~IGUI() noexcept {}

	virtual void Draw() noexcept = 0;
private:

};