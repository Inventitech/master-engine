#ifndef FRAMECOUNTER_H
#define FRAMECOUNTER_H

class FrameCounter {
private:
	struct Fps {
		double startTime;
		long frames;
	} fps;
	double currentTime;
public:
	FrameCounter();
	FrameCounter(double);
	virtual ~FrameCounter();
	void countNewFrame(double);
};

#endif /* ARGUMENTPARSER_H_ */
