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
	{ "Pulse Width", "(s)", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Max Amp", "Upper stimulus amplitude boundary value (V)", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
	{ "Min Amp", "Lower stimulus amplitude boundary value (V)", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
	{ "Amp Step", "Step size for incrememnting stimulus value (V)", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
	{ "Delay", "Delay (s) between stimuli", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Current Amp", "Current stimulus amp (V)", DefaultGUIModel::STATE | DefaultGUIModel::DOUBLE, },
	{ "Voltage", "Input signal", DefaultGUIModel::INPUT, },
	{ "Stimulus", "Stimulus output", DefaultGUIModel::OUTPUT, },
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

fiber_rec::fiber_rec(void) : DefaultGUIModel("Fiber Recruitment Curve", ::vars, ::num_vars)
{
	setWhatsThis("<p><b>fiber_rec:</b><br>QWhatsThis description.</p>");
	DefaultGUIModel::createGUI(vars, num_vars);
	customizeGUI();
	update( INIT ); 
	refresh(); 
	QTimer::singleShot(0, this, SLOT(resizeMe()));
}

fiber_rec::~fiber_rec(void)
{
}

void fiber_rec::execute(void)
{
	if (idx < stim.size())
		output(0) = current_amp = stim[idx++];
	else
	{
		pauseButton->setChecked(true);
	}
	return;
}

void fiber_rec::update(DefaultGUIModel::update_flags_t flag)
{
	switch (flag)
	{
		case INIT:
			max_amp = 1; // V
			min_amp = 0; // V
			step = 0.1; // V
			pulse_width = 0.2; // s
			delay = 1.0; // s
			current_amp = 0; // V
			idx = 0;
			setParameter("Pulse Width", pulse_width);
			setParameter("Max Amp", max_amp);
			setParameter("Min Amp", min_amp);
			setParameter("Amp Step", step);
			setParameter("Delay", delay);
			setState("Current Amp", current_amp);
			period = RT::System::getInstance()->getPeriod() * 1e-9; // s
			initStim();
			break;

		case MODIFY:
			pulse_width = getParameter("Pulse Width").toDouble();
			max_amp = getParameter("Max Amp").toDouble();
			min_amp = getParameter("Min Amp").toDouble();
			step = getParameter("Amp Step").toDouble();
			delay = getParameter("Delay").toDouble();
			initStim();
			break;

		case UNPAUSE:
			break;

		case PAUSE:
			output(0) = 0;
			idx = 0;
			current_amp = 0;
			break;

		case PERIOD:
			period = RT::System::getInstance()->getPeriod() * 1e-9; // s
			idx = 0;
			initStim();
			break;

		default:
			break;
	}
}

void fiber_rec::plotData(void)
{
}

void fiber_rec::customizeGUI(void)
{
	QGridLayout *customlayout = DefaultGUIModel::getLayout();

	// Initialize plot
	QGroupBox *scatterplotBox = new QGroupBox();
	QHBoxLayout *scatterplotBoxLayout = new QHBoxLayout;
	scatterplotBox->setLayout(scatterplotBoxLayout);
	splot = new ScatterPlot(this);
	splot->setFixedSize(450, 270);
	scatterplotBoxLayout->addWidget(splot);
	customlayout->addWidget(scatterplotBox, 0, 2, 2, 4);

	// Clear plot button
	QGroupBox *button_group = new QGroupBox;
	QPushButton *clearPlotButton = new QPushButton("Clear Plot and Data");
	QHBoxLayout *button_layout = new QHBoxLayout;
	button_group->setLayout(button_layout);
	button_layout->addWidget(clearPlotButton);
	QObject::connect(clearPlotButton, SIGNAL(clicked()), splot, SLOT(clear(void)));
	QObject::connect(clearPlotButton, SIGNAL(clicked()), this, SLOT(clearData(void)));
	QObject::connect(this, SIGNAL(newDataPoint(double,double)), splot, SLOT(appendPoint(double,double)));
	QObject::connect(this,SIGNAL(setPlotRange(double, double, double, double)),splot,SLOT(setAxes(double, double, double, double)));

	customlayout->addWidget(button_group, 0,0);
	setLayout(customlayout);
}

void fiber_rec::initStim(void)
{
	stim.clear();
	idx = 0;
	current_amp = 0;
	double amp = min_amp;
	num_pulses = (max_amp - min_amp)/step;
	for (int n = 0; n < num_pulses; n++)
	{
		amp += step;
		for (int i = 0; i < pulse_width / period; i++)
		{
			stim.push_back(amp);
		}
		for (int i = 0; i < ((delay - pulse_width) / period); i++)
		{
			stim.push_back(0);
		}
	}
}

void fiber_rec::clearData(void)
{
}
