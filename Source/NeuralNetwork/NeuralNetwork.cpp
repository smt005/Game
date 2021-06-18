
#include "NeuralNetwork/NeuralNetwork.h"
#include "nn2x2.h"
#include "Core.h"
#include <iostream>

NeuralNetwork::NeuralNetwork() {
}

void NeuralNetwork::init() {
	MapTemplate::init();
}

void NeuralNetwork::initCallback() {
	MapTemplate::initCallback();
}

void NeuralNetwork::update() {
	enterCommand();
}

void NeuralNetwork::enterCommand() {
	auto separateText = [](const auto& text, std::vector<std::string>& words, const std::string& separator) {
		words.clear();
		std::string textStr(text);
		bool breakState = false;
		size_t posBegin = 0;
		size_t posEnd = 0;

		while (!breakState) {
			posEnd = textStr.find(separator, posBegin);

			if (posEnd == textStr.npos) {
				posEnd = textStr.length();
				breakState = true;
			}

			words.emplace_back(textStr.substr(posBegin, posEnd - posBegin));

			++posEnd;
			posBegin = posEnd;
		}
	};

	if (_animate) {
		return;
	}

	//...
	std::cout << "Enter command..." << std::endl;

	std::string command;
	std::cin >> command;

	std::vector<std::string> words;
	if (!command.empty()) {
		separateText(command, words, ",");
	}

	if (words.empty()) return;

	if (words[0] == "exit" || words[0][0] == 'e') {
		Engine::Core::close();
	}
	else if (words[0] == "all" || words[0][0] == 'a') {
		neuralNetwork::init();
		neuralNetwork::query();
		neuralNetwork::train(1000);
		neuralNetwork::query();
	}
	else if (words[0] == "animate") {
		_animate = true;
	}
	else if (words[0] == "init" || words[0][0] == 'i') {
		neuralNetwork::init();
	}
	else if (words[0] == "weight" || words[0][0] == 'w') {
		neuralNetwork::infoWeights();
	}
	else if (words[0] == "query" || words[0][0] == 'q') {
		neuralNetwork::query();
	}
	else if (words[0] == "train" || words[0] == "t") {
		int count = 1;
		if (words.size() > 1) {
			count = (int)std::atof(words[1].c_str());
		}
		neuralNetwork::train(count);
	}
}