#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/timer.hpp>
#include <iunoplugin.h>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <iunoplugincontroller.h>
#include "SDRunoPlugin_rttyForm.h"

// Forward reference
class SDRunoPlugin_rtty;

class SDRunoPlugin_rttyUi {
public:

		SDRunoPlugin_rttyUi (SDRunoPlugin_rtty& parent,
	                                 IUnoPluginController& controller);
		~SDRunoPlugin_rttyUi();

	void	HandleEvent		(const UnoEvent& evt);
	void	FormClosed		();

	void ShowUi			();

	int LoadX			();
	int LoadY			();

	void    set_rttyAfcon           (const std::string &);
	void    set_rttyReverse         (const std::string &);
	void    set_rttyShift           (const std::string &);
	void    set_rttyBaudrate        (const std::string &);
	void    set_rttyParity          (const std::string &);
	void    set_rttyMsb             (const std::string &);
	void    set_rttyNbits           (const std::string &);
	void    set_rttyStopbits        (const std::string &);

	void    set_rttyIF	        (int v);
	void    set_freqCorrection      (int v);
	void    set_rttyBaudRate        (int v);
	void    set_rttyGuess           (int v);

	void	set_rttyText		(const std::string &);

private:
	
	SDRunoPlugin_rtty & m_parent;
	std::thread m_thread;
	std::shared_ptr<SDRunoPlugin_rttyForm> m_form;

	bool m_started;
	std::mutex m_lock;
	IUnoPluginController & m_controller;
};
