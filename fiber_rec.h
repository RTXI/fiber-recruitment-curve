/*
* This is a template header file for a user modules derived from
* DefaultGUIModel with a custom GUI.
*/

#include <QMdiArea>
#include <default_gui_model.h>

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

		void initStim(void);
	
	private slots:
		void clear_plot(void);
};
