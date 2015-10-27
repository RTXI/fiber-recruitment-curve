/*
	 Copyright (C) 2015 Georgia Institute of Technology

	 This program is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.

	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
  QTimer::singleShot(0, this, SLOT(resizeMe()));
}

fiber_rec::~fiber_rec(void) { }

void fiber_rec::execute(void) {
	return;
}

void fiber_rec::update(DefaultGUIModel::update_flags_t flag) {
	switch (flag) {
		case INIT:
			initStim();
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
	
	// Initialize plots
	QGroupBox *scatterplotBox = new QGroupBox();
	QHBoxLayout *scatterplotBoxLayout = new QHBoxLayout;
	scatterplotBox->setLayout(scatterplotBoxLayout);
	splot = new ScatterPlot(this);
	splot->setFixedSize(450, 270);
	scatterplotBoxLayout->addWidget(splot);
	customlayout->addWidget(scatterplotBox, 0, 2, 2, 4);

	QGroupBox *button_group = new QGroupBox;
	QPushButton *clearPlotButton = new QPushButton("Clear Plot");
	QHBoxLayout *button_layout = new QHBoxLayout;
	button_group->setLayout(button_layout);
	button_layout->addWidget(clearPlotButton);
	QObject::connect(clearPlotButton, SIGNAL(clicked()), splot, SLOT(clear(void)));

	customlayout->addWidget(button_group, 0,0);
	setLayout(customlayout);
}

void fiber_rec::initStim(void)
{
	max_amp = 5;
	min_amp = 0;
	step = 0.1;
	pulse_width = 0.2;
	current_amp = 0.0;
}
