#pragma once


#include <functional>

class Command {
private:
	std::function<void()> executeFunctor = nullptr;
	std::function<void()> cancelFunctor  = nullptr;

public:
	Command() {}
	Command(std::function<void()> execute)
			: executeFunctor(execute) {}

	Command(std::function<void()> execute, std::function<void()> cancel)
			: executeFunctor(execute), cancelFunctor(cancel){}

	virtual void execute() {
		if (executeFunctor != nullptr)
			executeFunctor();
	}

	virtual void cancel() {
		if (cancelFunctor != nullptr)
			cancelFunctor();
	}
};
