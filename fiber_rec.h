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

#include <scatterplot.h>
#include <algorithm>
#include <gsl/gsl_math.h>

class fiber_rec : public DefaultGUIModel {
	
	Q_OBJECT
	
	public:
		fiber_rec(void);
		virtual ~fiber_rec(void);
	
		void execute(void);
		void createGUI(DefaultGUIModel::variable_t *, int);
		void customizeGUI(void);
	
	protected:
		virtual void update(DefaultGUIModel::update_flags_t);
	
	private:
		double max_amp;
		double min_amp;
		double step;
		double pulse_width;
		double current_amp;
		double period;

		ScatterPlot *splot;

		void initStim(void);
	
	private slots:
};
