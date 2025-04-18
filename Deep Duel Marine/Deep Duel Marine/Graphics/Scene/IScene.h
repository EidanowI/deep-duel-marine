#pragma once



class IScene {
public:
	virtual ~IScene(){}
	virtual void Render() = 0;
};