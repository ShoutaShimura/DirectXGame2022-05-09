#pragma once

#include<functional>


/// <summary>
/// ŽžŒÀ”­“®
/// </summary>
class TimedCall
{
public:
	TimedCall(std::function<int(int)>f,uint32_t time);
	~TimedCall();

	void Update();

	bool IsFinished() { return completion;}

private:

	std::function<int(int)>f;

	uint32_t time_;

	bool completion = false;

};

