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

void autoCycle() {
	if(step == Init && (trans0)) changeStep(Step0);
	if(step == Step0 && (trans4)) changeStep(Step4);
	if(step == Init && (trans7)) changeStep(Step7);
	if(step == Step7 && (trans9)) changeStep(Step4);
	if(step == Step4 && (trans10)) changeStep(Step8);
	if(step == Step8 && (trans11)) changeStep(Step0);
	if(step == Step4 && (trans12)) changeStep(Step9);
	if(step == Step9 && (trans13)) changeStep(Step7);
	if(step == Step4 && (trans14)) changeStep(Step10);
	if(step == Step10 && (trans15)) changeStep(Init);
	if(step == Step4 && (trans16)) changeStep(Step11);
	if(step == Step11 && (trans17)) changeStep(Init);
}
