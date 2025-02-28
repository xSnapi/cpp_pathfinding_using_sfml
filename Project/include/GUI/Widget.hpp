#pragma once

class Layer;

class Widget {
public:
	Widget() = default;
	virtual ~Widget() = default;

	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual bool Activated() const {
		return false;
	}
};
