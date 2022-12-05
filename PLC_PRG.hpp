#ifndef PLC_PRG_HPP
#define PLC_PRG_HPP

enum Step: int {
	Init,
	Step0,
	Step7,
	Step4,
	Step8,
	Step9,
	Step10,
	Step11,
};

class PLC_PRG {
public:
	void autoCycle();
	void outputAnalysis();

	bool trans0;
	bool trans1;
	bool trans2;
	bool trans4;
	bool trans5;
	bool trans6;
	bool trans7;
	bool trans8;
	bool trans9;
	bool trans10;
	bool out0;
	bool trans12;
	bool trans13;
	bool trans11;
	bool trans15;
	bool trans14;
	bool trans16;
	bool trans17;
	bool out2;
private:
	Step step;
	unsigned long elapsedMin = 0;
	void changeStep(Step step);
};

#endif
