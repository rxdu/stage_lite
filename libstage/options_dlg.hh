#ifndef _OPTIONS_DLG_H_
#define _OPTIONS_DLG_H_

#include <FL/Fl_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include <string>
#include <vector>
#include <set>

#include "option.hh"

namespace Stg {

	class OptionsDlg : protected Fl_Window {
	public:
		enum event_t { NO_EVENT, CHANGE, CHANGE_ALL };
		
	private:
		std::vector<Option*> options;
		Option* changedItem;
		event_t status;
		Fl_Scroll* scroll;
		Fl_Button* button;
		void updateChecks();
		
		virtual int handle( int event );
		static void checkChanged( Fl_Widget* w, void* p );
		static void applyAllPress( Fl_Widget* w, void* p );
		
		// constants
		static const int vm = 2;
		const int hm;
		static const int btnH = 25;
		static const int boxH = 30;

	public:
		OptionsDlg( int x, int y, int w, int h );
		virtual ~OptionsDlg();
		void callback( Fl_Callback* cb, void* p ) { Fl_Window::callback( cb, p ); }
		void show() { Fl_Window::show(); }
		void hide() { Fl_Window::hide(); }
		
		void setOptions( const std::vector<Option*>& opts );
		void setOptions( const std::set<Option*, optComp>& opts );
		void clearOptions() { options.clear(); }
		const event_t event() const { return status; }
		Option* changed() { return changedItem; }
	};

}
	
#endif
