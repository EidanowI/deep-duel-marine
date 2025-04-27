#pragma once



class IScene {
public:
	virtual ~IScene() noexcept {}

	virtual void Render() noexcept = 0;
};