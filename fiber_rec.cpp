/*
* This is a template implementation file for a user module derived from
* DefaultGUIModel with a custom GUI.
*/

#include <fiber_rec.h>
#include <main_window.h>
#include <iostream>
#include <QtGui>

extern "C" Plugin::Object *createRTXIPlugin(void){
	return new fiber_rec();
}

static DefaultGUIModel::variable_t vars[] = {
	{ "Pulse Width", "(ms)", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Max Amp", "Upper stimulus amplitude boundary value (V)", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
	{ "Min Amp", "Lower stimulus amplitude boundary value (V)", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
	{ "Amp Step", "Step size for incrememnting stimulus value (V)", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
	{ "Current Amp", "Current stimulus amplitude", DefaultGUIModel::STATE, },
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

fiber_rec::fiber_rec(void) : DefaultGUIModel("Fiber Recruitment Curve", ::vars, ::num_vars) {
	setWhatsThis("<p><b>fiber_rec:</b><br>QWhatsThis description.</p>");
	DefaultGUIModel::createGUI(vars, num_vars);
	customizeGUI();
	update( INIT ); 
	refresh(); 
	resizeMe();
}

fiber_rec::~fiber_rec(void) { }

void fiber_rec::execute(void) {
	return;
}

void fiber_rec::update(DefaultGUIModel::update_flags_t flag) {
	switch (flag) {
		case INIT:
			setParameter("Pulse Width", pulse_width);
			setParameter("Max Amp", max_amp);
			setParameter("Min Amp", min_amp);
			setParameter("Amp Step", step);
			setState("Current Amp", current_amp);
			period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
			break;
	
		case MODIFY:
			pulse_width = getParameter("Pulse Width").toDouble();
			max_amp = getParameter("Max Amp").toDouble();
			min_amp = getParameter("Min Amp").toDouble();
			step = getParameter("Amp Step").toDouble();
			break;

		case UNPAUSE:
			break;

		case PAUSE:
			output(0) = 0;
			break;

		case PERIOD:
			period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
			initStim();
			break;
	
		default:
			break;
	}
}

void fiber_rec::customizeGUI(void) {
	QGridLayout *customlayout = DefaultGUIModel::getLayout();
	
	QGroupBox *button_group = new QGroupBox;
	
	QPushButton *clearPlotButton = new QPushButton("Clear Plot");
	QHBoxLayout *button_layout = new QHBoxLayout;
	button_group->setLayout(button_layout);
	button_layout->addWidget(clearPlotButton);
	//QObject::connect(clearPlotButton, SIGNAL(clicked()), this, SLOT(clear_plot(void)));
	
	customlayout->addWidget(button_group, 0,0);
	setLayout(customlayout);
}

void fiber_rec::initStim(void)
{

}
