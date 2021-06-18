#pragma once

#include "Template/MapTemplate.h"
#include <string>


class NeuralNetwork final : public MapTemplate
{
public:
	NeuralNetwork();
	std::string getName() override { return "NeuralNetwork"; }
	void init() override;
	void initCallback() override;
	void update() override;

private:
	void enterCommand();
	bool _animate = false;
	bool _pause = false;
};
