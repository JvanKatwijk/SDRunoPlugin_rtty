#include <sstream>
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/slider.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/timer.hpp>
#include <unoevent.h>
#include <stdlib.h>
#include <Windows.h>

#include "SDRunoPlugin_rtty.h"
#include "SDRunoPlugin_rttyUi.h"
#include "SDRunoPlugin_rttyForm.h"

// Ui constructor - load the Ui control into a thread
	SDRunoPlugin_rttyUi::
	                SDRunoPlugin_rttyUi (SDRunoPlugin_rtty& parent,
	                                     IUnoPluginController& controller) :
	m_parent(parent),
	m_form(nullptr),
	m_controller(controller) {
	m_thread = std::thread (&SDRunoPlugin_rttyUi::ShowUi, this);
}

// Ui destructor (the nana::API::exit_all();) is required if using Nana UI library
	SDRunoPlugin_rttyUi::~SDRunoPlugin_rttyUi() {	
	nana::API::exit_all();
	m_thread.join();	
}

// Show and execute the form
void	SDRunoPlugin_rttyUi::ShowUi() {	
	m_lock.lock();
	m_form = std::make_shared<SDRunoPlugin_rttyForm> (*this,
	                                                      m_controller);
	m_lock.unlock();

	m_form->Run();
}

// Load X from the ini file (if exists)
// TODO: Change Template to plugin name
int	SDRunoPlugin_rttyUi::LoadX () {
	std::string tmp;
	m_controller. GetConfigurationKey ("Template.X", tmp);
	if (tmp.empty()) {
	   return -1;
	}
	return stoi(tmp);
}

// Load Y from the ini file (if exists)
// TODO: Change Template to plugin name
int	SDRunoPlugin_rttyUi::LoadY () {
	std::string tmp;
	m_controller.GetConfigurationKey("Template.Y", tmp);
	if (tmp.empty()) {
	   return -1;
	}
	return stoi(tmp);
}

// Handle events from SDRuno
// TODO: code what to do when receiving relevant events
void SDRunoPlugin_rttyUi::HandleEvent (const UnoEvent& ev) {
	switch (ev. GetType ()) {
	   case UnoEvent::StreamingStarted:
	      break;

	   case UnoEvent::StreamingStopped:
	      break;

	   case UnoEvent::SavingWorkspace:
	      break;

	   case UnoEvent::ClosingDown:
	      FormClosed ();
	      break;

	   default:
	      break;
	}
}

// Required to make sure the plugin is correctly unloaded when closed
void	SDRunoPlugin_rttyUi::FormClosed () {
	m_controller.RequestUnload (&m_parent);
}

void	SDRunoPlugin_rttyUi::set_rttyAfcon	(const std::string &s) {
	m_parent. set_rttyAfcon (s);
}

void	SDRunoPlugin_rttyUi::set_rttyReverse	(const std::string &s) {
	m_parent. set_rttyReverse (s);
}

void	SDRunoPlugin_rttyUi::set_rttyShift	(const std::string &s) {
	m_parent. set_rttyShift (s);
}

void	SDRunoPlugin_rttyUi::set_rttyBaudrate	(const std::string &s) {
	m_parent. set_rttyBaudrate (s);
}

void	SDRunoPlugin_rttyUi::set_rttyParity	(const std::string &s) {
	m_parent. set_rttyParity (s);
}

void	SDRunoPlugin_rttyUi::set_rttyMsb	(const std::string &s) {
	m_parent. set_rttyMsb (s);
}

void	SDRunoPlugin_rttyUi::set_rttyNbits	(const std::string &s) {
	m_parent. set_rttyNbits (s);
}

void	SDRunoPlugin_rttyUi::set_rttyStopbits	(const std::string &s) {
	m_parent. set_rttyStopbits (s);
}

void	SDRunoPlugin_rttyUi::show_rttyIF	(int v) {
	std::lock_guard<std::mutex> l (m_lock);
	if (m_form != nullptr)
	   m_form -> show_rttyIF (v);
}

void	SDRunoPlugin_rttyUi::show_freqCorrection(int v) {
	std::lock_guard<std::mutex> l(m_lock);
	if (m_form != nullptr)
		m_form-> show_freqCorrection(v);
}

void	SDRunoPlugin_rttyUi::show_rttyBaudRate	(int v) {
	std::lock_guard<std::mutex> l (m_lock);
	if (m_form != nullptr)
	   m_form -> show_rttyBaudRate (v);
}

void	SDRunoPlugin_rttyUi::show_rttyGuess	(int v) {
	std::lock_guard<std::mutex> l (m_lock);
	if (m_form != nullptr)
	   m_form -> show_rttyGuess (v);
}

void	SDRunoPlugin_rttyUi::show_rttyText	(const std::string &s) {
	std::lock_guard<std::mutex> l (m_lock);
	if (m_form != nullptr)
	   m_form -> show_rttyText (s);
}

