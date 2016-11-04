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

#include <QMdiArea>
#include <default_gui_model.h>

#include <qwt_plot_curve.h>
#include <scatterplot.h>
#include <algorithm>

class fiber_rec : public DefaultGUIModel {
	
	Q_OBJECT
	
	public:
		fiber_rec(void);
		virtual ~fiber_rec(void);
	
		void execute(void);
		void createGUI(DefaultGUIModel::variable_t *, int);
		void customizeGUI(void);

		enum mode_t {
			none=0, train=1, zap=2,
		};

	signals:
		void processData(void);
	
	protected:
		virtual void update(DefaultGUIModel::update_flags_t);
	
	private:
		mode_t mode;
		double max_plot_val;
		double max_plot_idx;
		double max_amp;
		double curr_avg_value;
		double min_amp;
		double step;
		double pulse_width;
		double period;
		double delay;
		double current_amp;
		double noise_floor;
		int num_pulses;
		int idx;
		void initStim(void);
		void trainNoise(void);
		ScatterPlot *splot;
		QwtPlotCurve *scurve;
		QStatusBar *statusBar;
		std::vector<double> stim;
		std::vector<double> voltage;
		std::vector<double> noise;
		QwtArray<double> plot_point;
		QwtArray<double> counter;
	
	private slots:
		void plotData(void);
		void clearData(void);
		void toggleTrainMode(void);
};
